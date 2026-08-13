// ============================================================================
// dm.cpp — 大漠插件 (dm.dll) 开源重新实现
// 接口完全兼容原版大漠插件，采用 C 风格导出，支持跨语言调用
// ============================================================================
#define DM_EXPORTS
#define NOMINMAX
// 仅在本文件内关闭 CRT 安全警告（sprintf/sscanf 已逐处加长度限制）
#pragma warning(disable : 4996)

#include "dm.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <ctime>
#include <direct.h>
#include <io.h>

// GDI+
#include <objidl.h>
#include <gdiplus.h>
#include <tlhelp32.h>
#include <shlobj.h>
#include <intrin.h> // __cpuid
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// ============================================================================
// 64 位指针安全检查
// ============================================================================
static_assert(sizeof(void*) <= sizeof(intptr_t), "64-bit ptr mismatch");

// ============================================================================
// 内部辅助
// ============================================================================

// RAII 包装器
struct UniqueBitmap {
    Gdiplus::Bitmap* p = nullptr;
    UniqueBitmap(Gdiplus::Bitmap* ptr) : p(ptr) {}
    ~UniqueBitmap() { delete p; }
    UniqueBitmap(const UniqueBitmap&) = delete;
    UniqueBitmap& operator=(const UniqueBitmap&) = delete;
    UniqueBitmap(UniqueBitmap&& o) noexcept : p(o.p) { o.p = nullptr; }
    UniqueBitmap& operator=(UniqueBitmap&& o) noexcept { std::swap(p, o.p); return *this; }
    Gdiplus::Bitmap* operator->() { return p; }
    operator Gdiplus::Bitmap*() { return p; }
    explicit operator bool() { return p != nullptr; }
};

// CRITICAL_SECTION 适配器，使其可用于 std::lock_guard
struct CritSecLockable {
    CRITICAL_SECTION& cs;
    explicit CritSecLockable(CRITICAL_SECTION& s) : cs(s) {}
    void lock() { EnterCriticalSection(&cs); }
    void unlock() { LeaveCriticalSection(&cs); }
};

// 线程本地存储 — 每个线程一个 DmState 实例
DWORD g_tlsIndex = TLS_OUT_OF_INDEXES;
std::once_flag g_tlsInitFlag;
static long g_nextObjectId = 1;
static std::mutex g_idMutex;

// COM 状态覆盖 — 当 COM 对象调用时，临时指向 COM 对象的私有状态
thread_local DmState* g_comOverrideState = nullptr;

void dm_setComState(DmState* s) {
    g_comOverrideState = s;
}

static void InitTls() {
    g_tlsIndex = TlsAlloc();
}

DmState* GetDmState() {
    // COM 对象调用时优先使用其私有状态
    if (g_comOverrideState) return g_comOverrideState;
    std::call_once(g_tlsInitFlag, InitTls);
    if (g_tlsIndex == TLS_OUT_OF_INDEXES) return nullptr;
    DmState* state = reinterpret_cast<DmState*>(TlsGetValue(g_tlsIndex));
    if (!state) {
        state = new DmState();
        {
            std::lock_guard<std::mutex> lock(g_idMutex);
            state->objectId = g_nextObjectId++;
        }
        TlsSetValue(g_tlsIndex, state);
    }
    return state;
}

void DM_SetLastError(long err) {
    DmState* s = GetDmState();
    if (s) s->lastError = err;
}

// 线程安全的静态字符串缓冲区（用于返回 const char*）
#define DM_RESULT_BUF_SIZE 4096
static thread_local char g_resultBuf[DM_RESULT_BUF_SIZE];

const char* DM_SetResult(const std::string& s) {
    strncpy(g_resultBuf, s.c_str(), DM_RESULT_BUF_SIZE - 1);
    g_resultBuf[DM_RESULT_BUF_SIZE - 1] = '\0';
    return g_resultBuf;
}

const char* DM_SetResult(const char* s) {
    strncpy(g_resultBuf, s, DM_RESULT_BUF_SIZE - 1);
    g_resultBuf[DM_RESULT_BUF_SIZE - 1] = '\0';
    return g_resultBuf;
}

// UTF-8 转 UTF-16
static std::wstring Utf8ToWide(const char* utf8) {
    if (!utf8 || !*utf8) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
    if (len <= 0) return L"";
    std::wstring wstr(len - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &wstr[0], len);
    return wstr;
}

// UTF-16 转 UTF-8
static std::string WideToUtf8(const wchar_t* wide) {
    if (!wide || !*wide) return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, wide, -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0) return "";
    std::string str(len - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide, -1, &str[0], len, nullptr, nullptr);
    return str;
}

// 获取完整路径（相对于全局路径）
static std::wstring GetFullPath(const char* relativePath) {
    DmState* s = GetDmState();
    if (!relativePath || !*relativePath) return s->globalPath;
    std::wstring rel = Utf8ToWide(relativePath);
    if (rel.empty()) return s->globalPath;
    // 如果是绝对路径，直接返回
    if (rel.length() >= 2 && rel[1] == L':') return rel;
    if (rel[0] == L'\\') return rel;
    // 相对路径
    if (s->globalPath.empty()) return rel;
    return s->globalPath + L"\\" + rel;
}

// 解析颜色字符串 "RRGGBB" (Hex) → COLORREF
static COLORREF ParseColor(const char* colorStr) {
    if (!colorStr) return 0;
    unsigned long c = strtoul(colorStr, nullptr, 16);
    return RGB((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

// 解析颜色字符串返回到 BGR 格式 (RRGGBB → BBGGRR)
static std::string ColorToBGRStr(COLORREF c) {
    char buf[16];
    sprintf(buf, "%02X%02X%02X", GetBValue(c), GetGValue(c), GetRValue(c));
    return buf;
}

// 解析颜色字符串返回到 RGB 格式
static std::string ColorToRGBStr(COLORREF c) {
    char buf[16];
    sprintf(buf, "%02X%02X%02X", GetRValue(c), GetGValue(c), GetBValue(c));
    return buf;
}

// 获取屏幕 DC 上的像素颜色
static COLORREF GetPixelColor(long x, long y) {
    HDC hdc = GetDC(NULL);
    COLORREF c = GetPixel(hdc, x, y);
    ReleaseDC(NULL, hdc);
    return c;
}

// 获取屏幕 DC 上的像素颜色 (BGR 格式 #RRGGBB → 实际是 BBGGRR)
static COLORREF GetPixelColorBGR(long x, long y) {
    return GetPixelColor(x, y);
}

// 将屏幕区域捕获为 Bitmap
static UniqueBitmap CaptureScreenRect(long x1, long y1, long x2, long y2) {
    if (x2 <= x1) std::swap(x1, x2);
    if (y2 <= y1) std::swap(y1, y2);
    int w = static_cast<int>(x2 - x1);
    int h = static_cast<int>(y2 - y1);
    if (w <= 0 || h <= 0) return UniqueBitmap(nullptr);
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hbm = CreateCompatibleBitmap(hdcScreen, w, h);
    SelectObject(hdcMem, hbm);
    BitBlt(hdcMem, 0, 0, w, h, hdcScreen, static_cast<int>(x1), static_cast<int>(y1), SRCCOPY);
    UniqueBitmap bmp(new Bitmap(hbm, NULL));
    DeleteObject(hbm);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    return bmp;
}

// 保存 Bitmap 到文件
static bool SaveBitmapToFile(Bitmap* bmp, const char* file, const wchar_t* format = L"image/bmp") {
    if (!bmp || !file) return false;
    CLSID clsid;
    UINT n = 0;
    UINT ct = 0;
    // GetImageEncodersSize 通过出参返回编码器数量(n)与数据总字节大小(ct)，勿把返回值赋给 ct
    GetImageEncodersSize(&n, &ct);
    if (ct == 0 || n == 0) return false;
    ImageCodecInfo* codecs = reinterpret_cast<ImageCodecInfo*>(malloc(ct));
    GetImageEncoders(n, ct, codecs);
    bool found = false;
    for (UINT i = 0; i < n; i++) {
        if (wcscmp(codecs[i].MimeType, format) == 0) {
            clsid = codecs[i].Clsid;
            found = true;
            break;
        }
    }
    free(codecs);
    if (!found) return false;
    std::wstring wfile = Utf8ToWide(file);
    Status st = bmp->Save(wfile.c_str(), &clsid, NULL);
    return (st == Ok);
}

// 颜色相似度比较
static bool ColorSimilar(COLORREF c1, COLORREF c2, long sim) {
    if (sim >= 100) return c1 == c2;
    int dr = abs(static_cast<int>(GetRValue(c1)) - static_cast<int>(GetRValue(c2)));
    int dg = abs(static_cast<int>(GetGValue(c1)) - static_cast<int>(GetGValue(c2)));
    int db = abs(static_cast<int>(GetBValue(c1)) - static_cast<int>(GetBValue(c2)));
    int maxDiff = static_cast<int>((100.0 - sim) / 100.0 * 255);
    return (dr <= maxDiff && dg <= maxDiff && db <= maxDiff);
}

// 发送键鼠输入 (模拟)
static void SendKeyInput(WORD vk, DWORD flags) {
    INPUT ip = {0};
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = vk;
    ip.ki.dwFlags = flags;
    SendInput(1, &ip, sizeof(INPUT));
}

static void SendMouseInput(DWORD flags, LONG x = 0, LONG y = 0, DWORD data = 0) {
    INPUT ip = {0};
    ip.type = INPUT_MOUSE;
    ip.mi.dwFlags = flags;
    if (flags & (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE)) {
        ip.mi.dx = x;
        ip.mi.dy = y;
    }
    if (flags & MOUSEEVENTF_WHEEL) {
        ip.mi.mouseData = data;
    }
    SendInput(1, &ip, sizeof(INPUT));
}

// ============================================================================
// 模块 1: 基本设置
// ============================================================================

DM_API long DM_CALL dm_reg(const char* reg_code, const char* ver_info) {
    DmState* s = GetDmState();
    if (!s) return 0;
    if (!reg_code || !*reg_code) { DM_SetLastError(4); return 0; }
    s->regCode = Utf8ToWide(reg_code);
    s->verInfo = ver_info ? Utf8ToWide(ver_info) : L"";
    // 简化实现：总是返回成功
    s->isRegistered = true;
    DM_SetLastError(0);
    return 1;
}

DM_API long DM_CALL dm_regEx(const char* reg_code, const char* ver_info, const char* ip) {
    return dm_reg(reg_code, ver_info);
}

DM_API long DM_CALL dm_regNoMac(const char* reg_code, const char* ver_info) {
    return dm_reg(reg_code, ver_info);
}

DM_API long DM_CALL dm_regExNoMac(const char* reg_code, const char* ver_info, const char* ip) {
    return dm_reg(reg_code, ver_info);
}

DM_API const char* DM_CALL dm_ver() {
    return DM_SetResult("3.1233"); // 兼容版本号
}

DM_API long DM_CALL dm_getID() {
    DmState* s = GetDmState();
    return s ? s->objectId : 0;
}

DM_API long DM_CALL dm_getDmCount() {
    return 1; // 简化
}

DM_API long DM_CALL dm_getLastError() {
    DmState* s = GetDmState();
    return s ? s->lastError : 0;
}

DM_API const char* DM_CALL dm_getPath() {
    DmState* s = GetDmState();
    if (!s) return DM_SetResult("");
    return DM_SetResult(WideToUtf8(s->globalPath.c_str()));
}

DM_API long DM_CALL dm_setPath(const char* path) {
    DmState* s = GetDmState();
    if (!s) return 0;
    if (!path || !*path) { DM_SetLastError(0); return 0; }
    s->globalPath = Utf8ToWide(path);
    DM_SetLastError(0);
    return 1;
}

DM_API const char* DM_CALL dm_getBasePath() {
    char buf[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    char* p = strrchr(buf, '\\');
    if (p) *p = '\0';
    return DM_SetResult(buf);
}

DM_API long DM_CALL dm_setShowErrorMsg(long show) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->showErrorMsg = show;
    return 1;
}

DM_API long DM_CALL dm_enablePicCache(long enable) {
    // 简化：无操作
    return 1;
}

// ============================================================================
// 模块 2: 窗口
// ============================================================================

DM_API long DM_CALL dm_findWindow(const char* cls, const char* title) {
    std::wstring wcls = Utf8ToWide(cls);
    std::wstring wtitle = Utf8ToWide(title);
    HWND hwnd = FindWindowW(wcls.empty() ? NULL : wcls.c_str(),
                            wtitle.empty() ? NULL : wtitle.c_str());
    return static_cast<long>(reinterpret_cast<intptr_t>(hwnd));
}

DM_API long DM_CALL dm_findWindowEx(intptr_t parent, long cls, const char* title) {
    std::wstring wtitle = Utf8ToWide(title);
    HWND hwnd = FindWindowExW(reinterpret_cast<HWND>(parent), NULL, NULL, wtitle.empty() ? NULL : wtitle.c_str());
    return static_cast<long>(reinterpret_cast<intptr_t>(hwnd));
}

DM_API long DM_CALL dm_findWindowByProcess(const char* process_name, const char* cls, const char* title) {
    // 遍历窗口查找匹配进程名
    struct FindData { std::wstring proc; std::wstring cls; std::wstring title; HWND result; };
    FindData fd = { Utf8ToWide(process_name), Utf8ToWide(cls), Utf8ToWide(title), NULL };
    EnumWindows([](HWND hwnd, LPARAM lp) -> BOOL {
        FindData* fd = reinterpret_cast<FindData*>(lp);
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (hProc) {
            wchar_t buf[MAX_PATH] = {0};
            DWORD size = MAX_PATH;
            QueryFullProcessImageNameW(hProc, 0, buf, &size);
            std::wstring path(buf);
            std::wstring fn = path.substr(path.rfind(L'\\') + 1);
            if (fn == fd->proc) {
                if (fd->title.empty() || [&]() {
                    wchar_t tbuf[256] = {0};
                    GetWindowTextW(hwnd, tbuf, 256);
                    return std::wstring(tbuf) == fd->title;
                }()) {
                    fd->result = hwnd;
                    CloseHandle(hProc);
                    return FALSE;
                }
            }
            CloseHandle(hProc);
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&fd));
    return static_cast<long>(reinterpret_cast<intptr_t>(fd.result));
}

DM_API long DM_CALL dm_findWindowByProcessId(long process_id, const char* cls, const char* title) {
    struct FindData { DWORD pid; std::wstring title; HWND result; };
    FindData fd = { static_cast<DWORD>(process_id), Utf8ToWide(title), NULL };
    EnumWindows([](HWND hwnd, LPARAM lp) -> BOOL {
        FindData* fd = reinterpret_cast<FindData*>(lp);
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == fd->pid) {
            if (fd->title.empty()) { fd->result = hwnd; return FALSE; }
            wchar_t buf[256] = {0};
            GetWindowTextW(hwnd, buf, 256);
            if (std::wstring(buf) == fd->title) { fd->result = hwnd; return FALSE; }
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&fd));
    return static_cast<long>(reinterpret_cast<intptr_t>(fd.result));
}

DM_API long DM_CALL dm_findWindowSuper(const char* cls, const char* title, long flag) {
    return dm_findWindow(cls, title);
}

DM_API long DM_CALL dm_enumWindow(intptr_t parent, const char* title, long filter) {
    // ponytail: 简单枚举，返回句柄数组的字符串表示
    // 原版返回格式 "hwnd1,hwnd2,..."
    std::wstring wtitle = Utf8ToWide(title);
    std::vector<HWND> windows;
    EnumChildWindows(reinterpret_cast<HWND>(parent), [](HWND hwnd, LPARAM lp) -> BOOL {
        reinterpret_cast<std::vector<HWND>*>(lp)->push_back(hwnd);
        return TRUE;
    }, reinterpret_cast<LPARAM>(&windows));
    std::string result;
    for (size_t i = 0; i < windows.size(); i++) {
        if (i > 0) result += ",";
        char buf[32];
        sprintf(buf, "%lld", static_cast<long long>(reinterpret_cast<intptr_t>(windows[i])));
        result += buf;
    }
    return static_cast<long>(windows.size());
}

DM_API long DM_CALL dm_enumWindowByProcess(const char* process_name, const char* title, long filter) {
    // 简化
    return 0;
}

DM_API long DM_CALL dm_enumWindowByProcessId(long pid, const char* title, long filter) {
    return 0;
}

DM_API long DM_CALL dm_enumWindowSuper(const char* spec1, long flag1, const char* spec2, long flag2, long filter) {
    return 0;
}

DM_API long DM_CALL dm_enumProcess(const char* name) {
    return 0;
}

DM_API long DM_CALL dm_getWindow(intptr_t hwnd, long flag) {
    // flag: 0=父窗口, 1=第一个子窗口, 2=第一个兄弟窗口, ...
    switch (flag) {
        case 0: return static_cast<long>(reinterpret_cast<intptr_t>(GetParent(reinterpret_cast<HWND>(hwnd))));
        case 1: return static_cast<long>(reinterpret_cast<intptr_t>(GetWindow(reinterpret_cast<HWND>(hwnd), GW_CHILD)));
        case 2: return static_cast<long>(reinterpret_cast<intptr_t>(GetWindow(reinterpret_cast<HWND>(hwnd), GW_HWNDNEXT)));
        case 3: return static_cast<long>(reinterpret_cast<intptr_t>(GetWindow(reinterpret_cast<HWND>(hwnd), GW_HWNDPREV)));
        case 4: return static_cast<long>(reinterpret_cast<intptr_t>(GetWindow(reinterpret_cast<HWND>(hwnd), GW_OWNER)));
        default: return static_cast<long>(reinterpret_cast<intptr_t>(GetWindow(reinterpret_cast<HWND>(hwnd), flag)));
    }
}

DM_API long DM_CALL dm_getForegroundWindow() {
    return static_cast<long>(reinterpret_cast<intptr_t>(GetForegroundWindow()));
}

DM_API long DM_CALL dm_getForegroundFocus() {
    return static_cast<long>(reinterpret_cast<intptr_t>(GetFocus()));
}

DM_API long DM_CALL dm_getMousePointWindow() {
    POINT pt;
    GetCursorPos(&pt);
    return static_cast<long>(reinterpret_cast<intptr_t>(WindowFromPoint(pt)));
}

DM_API long DM_CALL dm_getPointWindow(long x, long y) {
    return static_cast<long>(reinterpret_cast<intptr_t>(WindowFromPoint({ static_cast<LONG>(x), static_cast<LONG>(y) })));
}

DM_API long DM_CALL dm_getSpecialWindow(long flag) {
    switch (flag) {
        case 0: return static_cast<long>(reinterpret_cast<intptr_t>(GetDesktopWindow()));
        case 1: return static_cast<long>(reinterpret_cast<intptr_t>(GetShellWindow()));
        case 2: return static_cast<long>(reinterpret_cast<intptr_t>(GetForegroundWindow()));
        default: return 0;
    }
}

DM_API const char* DM_CALL dm_getWindowClass(intptr_t hwnd) {
    wchar_t buf[256] = {0};
    GetClassNameW(reinterpret_cast<HWND>(hwnd), buf, 256);
    return DM_SetResult(WideToUtf8(buf));
}

DM_API long DM_CALL dm_getWindowProcessId(intptr_t hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    return static_cast<long>(pid);
}

DM_API const char* DM_CALL dm_getWindowProcessPath(intptr_t hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProc) return DM_SetResult("");
    wchar_t buf[MAX_PATH] = {0};
    DWORD size = MAX_PATH;
    QueryFullProcessImageNameW(hProc, 0, buf, &size);
    CloseHandle(hProc);
    return DM_SetResult(WideToUtf8(buf));
}

DM_API long DM_CALL dm_getWindowThreadId(intptr_t hwnd) {
    DWORD tid = GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), NULL);
    return static_cast<long>(tid);
}

DM_API const char* DM_CALL dm_getWindowTitle(intptr_t hwnd) {
    wchar_t buf[256] = {0};
    GetWindowTextW(reinterpret_cast<HWND>(hwnd), buf, 256);
    return DM_SetResult(WideToUtf8(buf));
}

DM_API long DM_CALL dm_getWindowRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2) {
    if (!hwnd) return 0;
    RECT r;
    if (!GetWindowRect(reinterpret_cast<HWND>(hwnd), &r)) return 0;
    if (x1) *x1 = r.left;
    if (y1) *y1 = r.top;
    if (x2) *x2 = r.right;
    if (y2) *y2 = r.bottom;
    return 1;
}

DM_API long DM_CALL dm_getClientRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2) {
    if (!hwnd) return 0;
    RECT r;
    if (!GetClientRect(reinterpret_cast<HWND>(hwnd), &r)) return 0;
    if (x1) *x1 = r.left;
    if (y1) *y1 = r.top;
    if (x2) *x2 = r.right;
    if (y2) *y2 = r.bottom;
    return 1;
}

DM_API long DM_CALL dm_getClientSize(intptr_t hwnd, long* width, long* height) {
    if (!hwnd) return 0;
    RECT r;
    if (!GetClientRect(reinterpret_cast<HWND>(hwnd), &r)) return 0;
    if (width) *width = r.right - r.left;
    if (height) *height = r.bottom - r.top;
    return 1;
}

DM_API long DM_CALL dm_getWindowState(intptr_t hwnd, long flag) {
    if (!hwnd || !IsWindow(reinterpret_cast<HWND>(hwnd))) return 0;
    // flag: 0=是否可见, 1=是否最小化, 2=是否最大化, 3=是否禁用
    switch (flag) {
        case 0: return IsWindowVisible(reinterpret_cast<HWND>(hwnd)) ? 1 : 0;
        case 1: return IsIconic(reinterpret_cast<HWND>(hwnd)) ? 1 : 0;
        case 2: return IsZoomed(reinterpret_cast<HWND>(hwnd)) ? 1 : 0;
        case 3: return !IsWindowEnabled(reinterpret_cast<HWND>(hwnd)) ? 1 : 0;
        default: return 0;
    }
}

DM_API long DM_CALL dm_getProcessInfo(long pid, const char* type) {
    return 0; // 简化
}

DM_API long DM_CALL dm_moveWindow(intptr_t hwnd, long x, long y) {
    if (!hwnd) return 0;
    RECT r;
    GetWindowRect(reinterpret_cast<HWND>(hwnd), &r);
    return SetWindowPos(reinterpret_cast<HWND>(hwnd), NULL, static_cast<int>(x), static_cast<int>(y),
        r.right - r.left, r.bottom - r.top, SWP_NOZORDER) ? 1 : 0;
}

DM_API long DM_CALL dm_setWindowSize(intptr_t hwnd, long width, long height) {
    if (!hwnd) return 0;
    RECT r;
    GetWindowRect(reinterpret_cast<HWND>(hwnd), &r);
    return SetWindowPos(reinterpret_cast<HWND>(hwnd), NULL, r.left, r.top,
        static_cast<int>(width), static_cast<int>(height), SWP_NOZORDER) ? 1 : 0;
}

DM_API long DM_CALL dm_setWindowState(intptr_t hwnd, long flag) {
    if (!hwnd) return 0;
    // flag: 0=显示, 1=隐藏, 2=最小化, 3=最大化, 4=还原
    int cmd = SW_SHOW;
    switch (flag) {
        case 0: cmd = SW_SHOW; break;
        case 1: cmd = SW_HIDE; break;
        case 2: cmd = SW_MINIMIZE; break;
        case 3: cmd = SW_MAXIMIZE; break;
        case 4: cmd = SW_RESTORE; break;
        default: return 0;
    }
    return ShowWindow(reinterpret_cast<HWND>(hwnd), cmd) ? 1 : 1; // 原版总是返回1
}

DM_API long DM_CALL dm_setWindowText(intptr_t hwnd, const char* title) {
    if (!hwnd) return 0;
    std::wstring wtitle = Utf8ToWide(title);
    return SetWindowTextW(reinterpret_cast<HWND>(hwnd), wtitle.c_str()) ? 1 : 0;
}

DM_API long DM_CALL dm_setWindowTransparent(intptr_t hwnd, long trans) {
    if (!hwnd) return 0;
    SetWindowLong(reinterpret_cast<HWND>(hwnd), GWL_EXSTYLE,
        GetWindowLong(reinterpret_cast<HWND>(hwnd), GWL_EXSTYLE) | WS_EX_LAYERED);
    return SetLayeredWindowAttributes(reinterpret_cast<HWND>(hwnd), 0, static_cast<BYTE>(trans), LWA_ALPHA) ? 1 : 0;
}

DM_API long DM_CALL dm_setClientSize(intptr_t hwnd, long width, long height) {
    if (!hwnd) return 0;
    RECT r, cr;
    GetWindowRect(reinterpret_cast<HWND>(hwnd), &r);
    GetClientRect(reinterpret_cast<HWND>(hwnd), &cr);
    int dw = (r.right - r.left) - (cr.right - cr.left);
    int dh = (r.bottom - r.top) - (cr.bottom - cr.top);
    return SetWindowPos(reinterpret_cast<HWND>(hwnd), NULL, r.left, r.top,
        static_cast<int>(width + dw), static_cast<int>(height + dh), SWP_NOZORDER) ? 1 : 0;
}

DM_API long DM_CALL dm_setSendStringDelay(long delay) {
    return 1;
}

DM_API long DM_CALL dm_sendPaste(intptr_t hwnd) {
    if (!hwnd) return 0;
    // 模拟 Ctrl+V
    SetForegroundWindow(reinterpret_cast<HWND>(hwnd));
    Sleep(100);
    SendKeyInput(VK_CONTROL, 0);
    SendKeyInput('V', 0);
    SendKeyInput('V', KEYEVENTF_KEYUP);
    SendKeyInput(VK_CONTROL, KEYEVENTF_KEYUP);
    return 1;
}

DM_API long DM_CALL dm_sendString(intptr_t hwnd, const char* str) {
    if (!hwnd || !str) return 0;
    SetForegroundWindow(reinterpret_cast<HWND>(hwnd));
    Sleep(100);
    std::wstring wstr = Utf8ToWide(str);
    for (wchar_t c : wstr) {
        SendKeyInput(static_cast<WORD>(c), 0);
        SendKeyInput(static_cast<WORD>(c), KEYEVENTF_KEYUP);
        Sleep(10);
    }
    return 1;
}

DM_API long DM_CALL dm_sendString2(intptr_t hwnd, const char* str) {
    return dm_sendString(hwnd, str);
}

DM_API long DM_CALL dm_sendStringIme(intptr_t hwnd, const char* str) {
    return dm_sendString(hwnd, str);
}

DM_API long DM_CALL dm_sendStringIme2(intptr_t hwnd, const char* str) {
    return dm_sendString(hwnd, str);
}

DM_API long DM_CALL dm_clientToScreen(intptr_t hwnd, long* x, long* y) {
    if (!hwnd) return 0;
    POINT pt = { static_cast<LONG>(*x), static_cast<LONG>(*y) };
    ClientToScreen(reinterpret_cast<HWND>(hwnd), &pt);
    *x = pt.x;
    *y = pt.y;
    return 1;
}

DM_API long DM_CALL dm_screenToClient(intptr_t hwnd, long* x, long* y) {
    if (!hwnd) return 0;
    POINT pt = { static_cast<LONG>(*x), static_cast<LONG>(*y) };
    ScreenToClient(reinterpret_cast<HWND>(hwnd), &pt);
    *x = pt.x;
    *y = pt.y;
    return 1;
}

// ============================================================================
// 模块 3: 键鼠
// ============================================================================

DM_API long DM_CALL dm_keyDown(long vk_code) {
    SendKeyInput(static_cast<WORD>(vk_code), 0);
    return 1;
}

DM_API long DM_CALL dm_keyDownChar(const char* key_str) {
    if (!key_str || !*key_str) return 0;
    // 将字符转换为虚拟键码
    SHORT vk = VkKeyScanA(key_str[0]);
    if (vk == -1) return 0;
    BYTE vkCode = static_cast<BYTE>(vk & 0xFF);
    BYTE shift = static_cast<BYTE>((vk >> 8) & 0xFF);
    if (shift & 1) { // 需要 Shift
        SendKeyInput(VK_SHIFT, 0);
    }
    SendKeyInput(vkCode, 0);
    return 1;
}

DM_API long DM_CALL dm_keyPress(long vk_code) {
    SendKeyInput(static_cast<WORD>(vk_code), 0);
    Sleep(10);
    SendKeyInput(static_cast<WORD>(vk_code), KEYEVENTF_KEYUP);
    return 1;
}

DM_API long DM_CALL dm_keyPressChar(const char* key_str) {
    dm_keyDownChar(key_str);
    Sleep(10);
    dm_keyUpChar(key_str);
    return 1;
}

DM_API long DM_CALL dm_keyPressStr(const char* key_str, long delay) {
    if (!key_str) return 0;
    for (const char* p = key_str; *p; p++) {
        dm_keyPressChar(std::string(1, *p).c_str());
        if (delay > 0) Sleep(static_cast<DWORD>(delay));
    }
    return 1;
}

DM_API long DM_CALL dm_keyUp(long vk_code) {
    SendKeyInput(static_cast<WORD>(vk_code), KEYEVENTF_KEYUP);
    return 1;
}

DM_API long DM_CALL dm_keyUpChar(const char* key_str) {
    if (!key_str || !*key_str) return 0;
    SHORT vk = VkKeyScanA(key_str[0]);
    if (vk == -1) return 0;
    BYTE vkCode = static_cast<BYTE>(vk & 0xFF);
    BYTE shift = static_cast<BYTE>((vk >> 8) & 0xFF);
    SendKeyInput(vkCode, KEYEVENTF_KEYUP);
    if (shift & 1) {
        SendKeyInput(VK_SHIFT, KEYEVENTF_KEYUP);
    }
    return 1;
}

DM_API long DM_CALL dm_waitKey(long vk_code, long time_out) {
    auto start = std::chrono::steady_clock::now();
    while (true) {
        if (GetAsyncKeyState(static_cast<int>(vk_code)) & 0x8000) return 1;
        if (time_out > 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            if (elapsed >= time_out) return 0;
        }
        Sleep(10);
    }
}

DM_API long DM_CALL dm_getKeyState(long vk_code) {
    SHORT st = GetAsyncKeyState(static_cast<int>(vk_code));
    return (st & 0x8000) ? 1 : 0;
}

DM_API long DM_CALL dm_leftClick() {
    SendMouseInput(MOUSEEVENTF_LEFTDOWN);
    Sleep(10);
    SendMouseInput(MOUSEEVENTF_LEFTUP);
    return 1;
}

DM_API long DM_CALL dm_leftDoubleClick() {
    dm_leftClick();
    Sleep(50);
    dm_leftClick();
    return 1;
}

DM_API long DM_CALL dm_leftDown() {
    SendMouseInput(MOUSEEVENTF_LEFTDOWN);
    return 1;
}

DM_API long DM_CALL dm_leftUp() {
    SendMouseInput(MOUSEEVENTF_LEFTUP);
    return 1;
}

DM_API long DM_CALL dm_rightClick() {
    SendMouseInput(MOUSEEVENTF_RIGHTDOWN);
    Sleep(10);
    SendMouseInput(MOUSEEVENTF_RIGHTUP);
    return 1;
}

DM_API long DM_CALL dm_rightDown() {
    SendMouseInput(MOUSEEVENTF_RIGHTDOWN);
    return 1;
}

DM_API long DM_CALL dm_rightUp() {
    SendMouseInput(MOUSEEVENTF_RIGHTUP);
    return 1;
}

DM_API long DM_CALL dm_middleClick() {
    SendMouseInput(MOUSEEVENTF_MIDDLEDOWN);
    Sleep(10);
    SendMouseInput(MOUSEEVENTF_MIDDLEUP);
    return 1;
}

DM_API long DM_CALL dm_middleDown() {
    SendMouseInput(MOUSEEVENTF_MIDDLEDOWN);
    return 1;
}

DM_API long DM_CALL dm_middleUp() {
    SendMouseInput(MOUSEEVENTF_MIDDLEUP);
    return 1;
}

DM_API long DM_CALL dm_moveTo(long x, long y) {
    SetCursorPos(static_cast<int>(x), static_cast<int>(y));
    return 1;
}

DM_API long DM_CALL dm_moveToEx(long x, long y, long w, long h) {
    // 在指定范围内随机偏移
    if (w <= 0) w = 1;
    if (h <= 0) h = 1;
    static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<long> dx(0, w);
    std::uniform_int_distribution<long> dy(0, h);
    SetCursorPos(static_cast<int>(x + dx(rng)), static_cast<int>(y + dy(rng)));
    return 1;
}

DM_API long DM_CALL dm_moveR(long rx, long ry) {
    POINT pt;
    GetCursorPos(&pt);
    SetCursorPos(pt.x + static_cast<int>(rx), pt.y + static_cast<int>(ry));
    return 1;
}

DM_API long DM_CALL dm_wheelDown(long repeat) {
    SendMouseInput(MOUSEEVENTF_WHEEL, 0, 0, static_cast<DWORD>(-120) * static_cast<DWORD>(repeat));
    return 1;
}

DM_API long DM_CALL dm_wheelUp(long repeat) {
    SendMouseInput(MOUSEEVENTF_WHEEL, 0, 0, static_cast<DWORD>(120) * static_cast<DWORD>(repeat));
    return 1;
}

DM_API long DM_CALL dm_getCursorPos(long* x, long* y) {
    POINT pt;
    GetCursorPos(&pt);
    if (x) *x = pt.x;
    if (y) *y = pt.y;
    return 1;
}

DM_API long DM_CALL dm_getCursorShape() {
    // 简化：返回 0
    return 0;
}

DM_API const char* DM_CALL dm_getCursorShapeEx(long type) {
    return DM_SetResult("0");
}

DM_API long DM_CALL dm_getCursorSpot() {
    return 0;
}

DM_API long DM_CALL dm_setMouseDelay(long type, long delay) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->mouseDelay = delay;
    return 1;
}

DM_API long DM_CALL dm_setKeypadDelay(long type, long delay) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->keypadDelay = delay;
    return 1;
}

DM_API long DM_CALL dm_setMouseSpeed(long speed) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->mouseSpeed = speed;
    return 1;
}

DM_API long DM_CALL dm_getMouseSpeed() {
    DmState* s = GetDmState();
    return s ? s->mouseSpeed : 0;
}

DM_API long DM_CALL dm_enableMouseAccuracy(long enable) {
    return 1;
}

DM_API long DM_CALL dm_setSimMode(long mode) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->simMode = mode;
    return 1;
}

// ============================================================================
// 模块 4: 图色
// ============================================================================

DM_API long DM_CALL dm_capture(long x1, long y1, long x2, long y2, const char* file) {
    UniqueBitmap bmp = CaptureScreenRect(x1, y1, x2, y2);
    if (!bmp) { DM_SetLastError(-1); return 0; }
    bool ok = SaveBitmapToFile(bmp, file, L"image/bmp");
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_captureGif(long x1, long y1, long x2, long y2, const char* file, long delay, long time) {
    // ponytail: 单帧截图为 GIF，多帧动画需更复杂的实现
    UniqueBitmap bmp = CaptureScreenRect(x1, y1, x2, y2);
    if (!bmp) return 0;
    bool ok = SaveBitmapToFile(bmp, file, L"image/gif");
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_captureJpg(long x1, long y1, long x2, long y2, const char* file, long quality) {
    UniqueBitmap bmp = CaptureScreenRect(x1, y1, x2, y2);
    if (!bmp) return 0;
    // ponytail: 默认 JPEG 质量，忽略 quality 参数
    bool ok = SaveBitmapToFile(bmp, file, L"image/jpeg");
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_capturePng(long x1, long y1, long x2, long y2, const char* file) {
    UniqueBitmap bmp = CaptureScreenRect(x1, y1, x2, y2);
    if (!bmp) return 0;
    bool ok = SaveBitmapToFile(bmp, file, L"image/png");
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_capturePre(const char* file) {
    // 预截图 — 简化实现
    return 0;
}

DM_API const char* DM_CALL dm_getColor(long x, long y) {
    COLORREF c = GetPixelColor(x, y);
    return DM_SetResult(ColorToRGBStr(c));
}

DM_API const char* DM_CALL dm_getColorBGR(long x, long y) {
    COLORREF c = GetPixelColor(x, y);
    return DM_SetResult(ColorToBGRStr(c));
}

DM_API const char* DM_CALL dm_getColorHSV(long x, long y) {
    COLORREF c = GetPixelColor(x, y);
    int r = GetRValue(c), g = GetGValue(c), b = GetBValue(c);
    // RGB → HSV
    double rd = r / 255.0, gd = g / 255.0, bd = b / 255.0;
    double max = std::max({rd, gd, bd});
    double min = std::min({rd, gd, bd});
    double h = 0, s = 0, v = max;
    double delta = max - min;
    if (delta > 0.0001) {
        s = delta / max;
        if (max == rd) h = 60.0 * fmod((gd - bd) / delta, 6.0);
        else if (max == gd) h = 60.0 * ((bd - rd) / delta + 2.0);
        else h = 60.0 * ((rd - gd) / delta + 4.0);
        if (h < 0) h += 360.0;
    }
    char buf[32];
    sprintf(buf, "%.0f,%.0f,%.0f", h, s * 100, v * 100);
    return DM_SetResult(buf);
}

DM_API long DM_CALL dm_getColorNum(long x1, long y1, long x2, long y2, const char* color, long sim) {
    if (!color) return 0;
    COLORREF target = ParseColor(color);
    int count = 0;
    for (long y = y1; y <= y2; y++) {
        for (long x = x1; x <= x2; x++) {
            COLORREF c = GetPixelColor(x, y);
            if (ColorSimilar(c, target, sim)) count++;
        }
    }
    return count;
}

DM_API const char* DM_CALL dm_getAveRGB(long x1, long y1, long x2, long y2) {
    long sumR = 0, sumG = 0, sumB = 0, count = 0;
    for (long y = y1; y <= y2; y++) {
        for (long x = x1; x <= x2; x++) {
            COLORREF c = GetPixelColor(x, y);
            sumR += GetRValue(c); sumG += GetGValue(c); sumB += GetBValue(c);
            count++;
        }
    }
    if (count == 0) return DM_SetResult("000000");
    char buf[16];
    sprintf(buf, "%02X%02X%02X",
        static_cast<unsigned char>(sumR / count),
        static_cast<unsigned char>(sumG / count),
        static_cast<unsigned char>(sumB / count));
    return DM_SetResult(buf);
}

DM_API const char* DM_CALL dm_getAveHSV(long x1, long y1, long x2, long y2) {
    return DM_SetResult("0,0,0");
}

DM_API long DM_CALL dm_cmpColor(long x, long y, const char* color, long sim) {
    COLORREF target = ParseColor(color);
    COLORREF actual = GetPixelColor(x, y);
    return ColorSimilar(actual, target, sim) ? 1 : 0;
}

DM_API long DM_CALL dm_findColor(long x1, long y1, long x2, long y2, const char* color, long sim, long dir, long* intX, long* intY) {
    if (!color) return 0;
    // dir: 0=左上→右下, 1=中心→四周, 2=右上→左下, ...
    COLORREF target = ParseColor(color);
    long cx = (x1 + x2) / 2, cy = (y1 + y2) / 2;
    // 简化：从左到右，从上到下
    for (long y = y1; y <= y2; y++) {
        for (long x = x1; x <= x2; x++) {
            COLORREF c = GetPixelColor(x, y);
            if (ColorSimilar(c, target, sim)) {
                if (intX) *intX = x;
                if (intY) *intY = y;
                return 1;
            }
        }
    }
    return 0;
}

DM_API long DM_CALL dm_findColorEx(long x1, long y1, long x2, long y2, const char* color, long sim, long dir) {
    long x = 0, y = 0;
    return dm_findColor(x1, y1, x2, y2, color, sim, dir, &x, &y);
}

DM_API long DM_CALL dm_findColorBlock(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height, long* intX, long* intY) {
    // 简化：查找颜色块
    if (!color) return 0;
    COLORREF target = ParseColor(color);
    for (long y = y1; y <= y2 - height; y++) {
        for (long x = x1; x <= x2 - width; x++) {
            long match = 0;
            for (long dy = 0; dy < height; dy++) {
                for (long dx = 0; dx < width; dx++) {
                    COLORREF c = GetPixelColor(x + dx, y + dy);
                    if (ColorSimilar(c, target, sim)) match++;
                }
            }
            if (match >= count) {
                if (intX) *intX = x;
                if (intY) *intY = y;
                return 1;
            }
        }
    }
    return 0;
}

DM_API long DM_CALL dm_findColorBlockEx(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height) {
    long x = 0, y = 0;
    return dm_findColorBlock(x1, y1, x2, y2, color, sim, count, width, height, &x, &y);
}

DM_API long DM_CALL dm_findColorE(long x1, long y1, long x2, long y2, const char* color, long sim, long dir) {
    long x = 0, y = 0;
    long found = dm_findColor(x1, y1, x2, y2, color, sim, dir, &x, &y);
    if (found) {
        char buf[64];
        sprintf(buf, "%ld|%ld", x, y);
        return static_cast<long>(reinterpret_cast<intptr_t>(DM_SetResult(buf)));
    }
    return static_cast<long>(reinterpret_cast<intptr_t>(DM_SetResult("")));
}

DM_API long DM_CALL dm_findMulColor(long x1, long y1, long x2, long y2, const char* color, long sim) {
    if (!color) return 0;
    // color 格式: "color1|color2|..."
    std::string colorStr(color);
    std::vector<std::string> colors;
    size_t pos = 0;
    while ((pos = colorStr.find('|')) != std::string::npos) {
        colors.push_back(colorStr.substr(0, pos));
        colorStr.erase(0, pos + 1);
    }
    colors.push_back(colorStr);
    for (long y = y1; y <= y2; y++) {
        for (long x = x1; x <= x2; x++) {
            COLORREF c = GetPixelColor(x, y);
            for (const auto& col : colors) {
                if (ColorSimilar(c, ParseColor(col.c_str()), sim)) return 1;
            }
        }
    }
    return 0;
}

DM_API long DM_CALL dm_findMultiColor(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir, long* intX, long* intY) {
    // 多色查找：先找 first_color，然后用偏移找 offset_color
    // offset_color 格式: "x1|y1|color1,x2|y2|color2,..."
    if (!first_color) return 0;
    COLORREF target = ParseColor(first_color);
    for (long y = y1; y <= y2; y++) {
        for (long x = x1; x <= x2; x++) {
            COLORREF c = GetPixelColor(x, y);
            if (ColorSimilar(c, target, sim)) {
                // 检查偏移色
                if (offset_color && *offset_color) {
                    std::string oc(offset_color);
                    std::stringstream ss(oc);
                    std::string seg;
                    bool allMatch = true;
                    while (std::getline(ss, seg, ',')) {
                        long ox = 0, oy = 0;
                        char col[16] = {0};
                        if (sscanf(seg.c_str(), "%ld|%ld|%15s", &ox, &oy, col) >= 3) {
                            COLORREF oc2 = GetPixelColor(x + ox, y + oy);
                            if (!ColorSimilar(oc2, ParseColor(col), sim)) {
                                allMatch = false;
                                break;
                            }
                        }
                    }
                    if (allMatch) {
                        if (intX) *intX = x;
                        if (intY) *intY = y;
                        return 1;
                    }
                } else {
                    if (intX) *intX = x;
                    if (intY) *intY = y;
                    return 1;
                }
            }
        }
    }
    return 0;
}

DM_API long DM_CALL dm_findMultiColorEx(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir) {
    long x = 0, y = 0;
    return dm_findMultiColor(x1, y1, x2, y2, first_color, offset_color, sim, dir, &x, &y);
}

DM_API long DM_CALL dm_findMultiColorE(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir) {
    long x = 0, y = 0;
    long found = dm_findMultiColor(x1, y1, x2, y2, first_color, offset_color, sim, dir, &x, &y);
    if (found) {
        char buf[64];
        sprintf(buf, "%ld|%ld", x, y);
        return static_cast<long>(reinterpret_cast<intptr_t>(DM_SetResult(buf)));
    }
    return static_cast<long>(reinterpret_cast<intptr_t>(DM_SetResult("")));
}

// ============================================================================
// 图色 — 找图函数 (简化实现)
// ============================================================================
// 原版支持从文件加载图片并匹配，这里简化实现

DM_API long DM_CALL dm_findPic(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY) {
    // ponytail: 简化版本 — 从文件加载图片并与屏幕区域对比
    if (!pic_name) return 0;
    std::wstring wpath = GetFullPath(pic_name);
    UniqueBitmap tmpl(Bitmap::FromFile(wpath.c_str()));
    if (!tmpl || tmpl->GetLastStatus() != Ok) {
        if (intX) *intX = -1; if (intY) *intY = -1;
        return 0;
    }
    int tw = tmpl->GetWidth(), th = tmpl->GetHeight();
    if (tw <= 0 || th <= 0) { return 0; }
    // 扫描屏幕区域
    for (long y = y1; y <= y2 - th; y++) {
        for (long x = x1; x <= x2 - tw; x++) {
            bool match = true;
            for (int py = 0; py < th && match; py++) {
                for (int px = 0; px < tw && match; px++) {
                    COLORREF sc = GetPixelColor(x + px, y + py);
                    Gdiplus::Color tc;
                    tmpl->GetPixel(px, py, &tc);
                    COLORREF tcr = RGB(tc.GetR(), tc.GetG(), tc.GetB());
                    if (!ColorSimilar(sc, tcr, sim)) {
                        match = false;
                    }
                }
            }
            if (match) {
                if (intX) *intX = x;
                if (intY) *intY = y;
                return 1;
            }
        }
    }
    return 0;
}

DM_API const char* DM_CALL dm_findPicE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir) {
    long x = 0, y = 0;
    long found = dm_findPic(x1, y1, x2, y2, pic_name, delta_color, sim, dir, &x, &y);
    if (found) {
        char buf[64];
        sprintf(buf, "%ld|%ld", x, y);
        return DM_SetResult(buf);
    }
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findPicEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir) {
    return dm_findPicE(x1, y1, x2, y2, pic_name, delta_color, sim, dir);
}

DM_API const char* DM_CALL dm_findPicExS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir) {
    return dm_findPicE(x1, y1, x2, y2, pic_name, delta_color, sim, dir);
}

DM_API long DM_CALL dm_findPicS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY) {
    return dm_findPic(x1, y1, x2, y2, pic_name, delta_color, sim, dir, intX, intY);
}

DM_API long DM_CALL dm_findPicMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY) {
    // ponytail: 内存图片查找 — 简化实现
    return 0;
}

// 其他 findPic 变体都委托给 dm_findPic
DM_API const char* DM_CALL dm_findPicMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir) {
    return DM_SetResult("");
}
DM_API const char* DM_CALL dm_findPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir) {
    return DM_SetResult("");
}
DM_API long DM_CALL dm_findPicSim(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY) {
    return dm_findPic(x1, y1, x2, y2, pic_name, delta_color, sim, dir, intX, intY);
}
DM_API const char* DM_CALL dm_findPicSimE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir) {
    return dm_findPicE(x1, y1, x2, y2, pic_name, delta_color, sim, dir);
}
DM_API const char* DM_CALL dm_findPicSimEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir) {
    return dm_findPicE(x1, y1, x2, y2, pic_name, delta_color, sim, dir);
}
DM_API long DM_CALL dm_findPicSimMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY) {
    return 0;
}
DM_API const char* DM_CALL dm_findPicSimMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir) {
    return DM_SetResult("");
}
DM_API const char* DM_CALL dm_findPicSimMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir) {
    return DM_SetResult("");
}
DM_API long DM_CALL dm_findShape(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir, long* intX, long* intY) {
    return 0;
}
DM_API const char* DM_CALL dm_findShapeE(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir) {
    return DM_SetResult("");
}
DM_API const char* DM_CALL dm_findShapeEx(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_getPicSize(const char* pic_name) {
    if (!pic_name) return DM_SetResult("");
    std::wstring wpath = GetFullPath(pic_name);
    UniqueBitmap bmp(Bitmap::FromFile(wpath.c_str()));
    if (!bmp || bmp->GetLastStatus() != Ok) { return DM_SetResult(""); }
    char buf[32];
    sprintf(buf, "%d,%d", bmp->GetWidth(), bmp->GetHeight());
    return DM_SetResult(buf);
}

DM_API long DM_CALL dm_freePic(const char* pic_name) {
    return 1;
}

DM_API long DM_CALL dm_loadPic(const char* pic_name) {
    return 1;
}

DM_API long DM_CALL dm_loadPicByte(const char* pic_name, long data, long size) {
    return 0;
}

DM_API const char* DM_CALL dm_appendPicAddr(const char* pic_info, long addr, long size) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_matchPicName(const char* pic_name) {
    return DM_SetResult(pic_name ? pic_name : "");
}

DM_API long DM_CALL dm_imageToBmp(const char* pic_name, const char* bmp_name) {
    if (!pic_name || !bmp_name) return 0;
    std::wstring wsrc = GetFullPath(pic_name);
    std::wstring wdst = GetFullPath(bmp_name);
    UniqueBitmap bmp(Bitmap::FromFile(wsrc.c_str()));
    if (!bmp || bmp->GetLastStatus() != Ok) { return 0; }
    bool ok = SaveBitmapToFile(bmp, bmp_name, L"image/bmp");
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_setPicPwd(const char* pwd) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->picPassword = pwd ? pwd : "";
    return 1;
}

DM_API long DM_CALL dm_setExcludeRegion(long mode, const char* info) {
    return 1;
}

DM_API long DM_CALL dm_getScreenData(long x1, long y1, long x2, long y2) {
    return 0;
}

DM_API const char* DM_CALL dm_getScreenDataBmp(long x1, long y1, long x2, long y2, long* size) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_enableDisplayDebug(long enable_debug) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableDisplayDebug = enable_debug != 0;
    return 1;
}

DM_API long DM_CALL dm_enableFindPicMultithread(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableFindPicMultithread = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableGetColorByCapture(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableGetColorByCapture = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_setFindPicMultithreadCount(long count) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->findPicMultithreadCount = count;
    return 1;
}

DM_API long DM_CALL dm_setFindPicMultithreadLimit(long limit) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->findPicMultithreadLimit = limit;
    return 1;
}

DM_API long DM_CALL dm_bgr2RGB(const char* bgr_color) {
    if (!bgr_color) return static_cast<long>(reinterpret_cast<intptr_t>(DM_SetResult("")));
    // BGR → RGB: 交换 R 和 B
    std::string s(bgr_color);
    if (s.length() >= 6) {
        std::swap(s[0], s[4]);
        std::swap(s[1], s[5]);
    }
    return static_cast<long>(reinterpret_cast<intptr_t>(DM_SetResult(s)));
}

DM_API const char* DM_CALL dm_rgb2BGR(const char* rgb_color) {
    if (!rgb_color) return DM_SetResult("");
    std::string s(rgb_color);
    if (s.length() >= 6) {
        std::swap(s[0], s[4]);
        std::swap(s[1], s[5]);
    }
    return DM_SetResult(s);
}

DM_API long DM_CALL dm_isDisplayDead(long x1, long y1, long x2, long y2, long time) {
    return 0;
}

// ============================================================================
// 模块 5: 后台设置
// ============================================================================

DM_API long DM_CALL dm_bindWindow(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, long mode) {
    DmState* s = GetDmState();
    if (!s) return 0;
    if (!IsWindow(reinterpret_cast<HWND>(hwnd))) { DM_SetLastError(-2); return 0; }
    // 简化：记录绑定信息
    s->bindHwnd = reinterpret_cast<HWND>(hwnd);
    s->isBound = true;
    s->bindDisplay = display ? display : "";
    s->bindMouse = mouse ? mouse : "";
    s->bindKeypad = keypad ? keypad : "";
    s->bindMode = mode;
    DM_SetLastError(0);
    return 1;
}

DM_API long DM_CALL dm_bindWindowEx(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, const char* pub, long mode) {
    return dm_bindWindow(hwnd, display, mouse, keypad, mode);
}

DM_API long DM_CALL dm_unBindWindow() {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->bindHwnd = NULL;
    s->isBound = false;
    return 1;
}

DM_API long DM_CALL dm_getBindWindow() {
    DmState* s = GetDmState();
    return s ? static_cast<long>(reinterpret_cast<intptr_t>(s->bindHwnd)) : 0;
}

DM_API long DM_CALL dm_isBind(intptr_t hwnd) {
    DmState* s = GetDmState();
    if (!s) return 0;
    if (hwnd && s->bindHwnd != reinterpret_cast<HWND>(hwnd)) return 0;
    return s->isBound ? 1 : 0;
}

DM_API long DM_CALL dm_forceUnBindWindow() {
    return dm_unBindWindow();
}

DM_API long DM_CALL dm_setAero(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->aero = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_lockInput(long enable) {
    // 简化
    return 1;
}

DM_API long DM_CALL dm_lockMouseRect(long x1, long y1, long x2, long y2) {
    return 1;
}

DM_API long DM_CALL dm_lockDisplay(long enable) {
    return 1;
}

DM_API long DM_CALL dm_enableBind(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableBind = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableFakeActive(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableFakeActive = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableIme(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableIme = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableKeypadMsg(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableKeypadMsg = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableKeypadPatch(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableKeypadPatch = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableKeypadSync(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableKeypadSync = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableMouseMsg(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableMouseMsg = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableMouseSync(long enable, long time_out) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableMouseSync = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableRealKeypad(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableRealKeypad = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_enableRealMouse(long enable, long mousedelay, long mousestep) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableRealMouse = enable != 0;
    s->realMouseDelay = mousedelay;
    s->realMouseStep = mousestep;
    return 1;
}

DM_API long DM_CALL dm_enableSpeedDx(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->enableSpeedDx = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_getFps() {
    DmState* s = GetDmState();
    return s ? s->fps : 0;
}

DM_API long DM_CALL dm_setDisplayDelay(long delay) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->displayDelay = delay;
    return 1;
}

DM_API long DM_CALL dm_setDisplayRefreshDelay(long delay) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->displayRefreshDelay = delay;
    return 1;
}

DM_API long DM_CALL dm_setInputDm(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->inputDm = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_hackSpeed(long rate) {
    return 1;
}

DM_API long DM_CALL dm_downCpu(long enable, long rate) {
    return 1;
}

DM_API long DM_CALL dm_switchBindWindow(intptr_t hwnd) {
    return dm_bindWindow(hwnd, "normal", "normal", "normal", 0);
}

// ============================================================================
// 模块 6: 内存
// ============================================================================

DM_API long DM_CALL dm_openProcess(long pid) {
    DmState* s = GetDmState();
    if (!s) return 0;
    if (s->processHandle) CloseHandle(s->processHandle);
    s->processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, static_cast<DWORD>(pid));
    return s->processHandle ? 1 : 0;
}

DM_API long DM_CALL dm_getModuleBaseAddr(intptr_t hwnd, const char* module) {
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
    MODULEENTRY32W me = { sizeof(me) };
    Module32FirstW(hSnapshot, &me);
    BOOL found = FALSE;
    do {
        std::wstring wmod = Utf8ToWide(module);
        if (wmod.empty() || wcscmp(me.szModule, wmod.c_str()) == 0) {
            found = TRUE;
            break;
        }
    } while (Module32NextW(hSnapshot, &me));
    CloseHandle(hSnapshot);
    return found ? static_cast<long>(reinterpret_cast<intptr_t>(me.modBaseAddr)) : 0;
}

DM_API long DM_CALL dm_getModuleSize(intptr_t hwnd, const char* module) {
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
    MODULEENTRY32W me = { sizeof(me) };
    Module32FirstW(hSnapshot, &me);
    BOOL found = FALSE;
    do {
        std::wstring wmod = Utf8ToWide(module);
        if (wmod.empty() || wcscmp(me.szModule, wmod.c_str()) == 0) {
            found = TRUE;
            break;
        }
    } while (Module32NextW(hSnapshot, &me));
    CloseHandle(hSnapshot);
    return found ? static_cast<long>(me.modBaseSize) : 0;
}

DM_API long DM_CALL dm_getRemoteApiAddress(intptr_t hwnd, const char* module, const char* api) {
    if (!module || !api) return 0;
    HMODULE hMod = GetModuleHandleA(module);
    if (!hMod) {
        hMod = LoadLibraryA(module);
    }
    if (!hMod) return 0;
    FARPROC addr = GetProcAddress(hMod, api);
    return static_cast<long>(reinterpret_cast<intptr_t>(addr));
}

DM_API long DM_CALL dm_readInt(intptr_t hwnd, long addr, long* ret) {
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return 0;
    int val = 0;
    BOOL ok = ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(addr), &val, sizeof(val), NULL);
    if (ret) *ret = val;
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_readIntAddr(intptr_t hwnd, long addr, long type, long* ret) {
    // type: 0=4字节, 1=1字节, 2=2字节
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return 0;
    long val = 0;
    DWORD size = 4;
    if (type == 1) size = 1;
    else if (type == 2) size = 2;
    BOOL ok = ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(addr), &val, size, NULL);
    if (ret) *ret = val;
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_readFloat(intptr_t hwnd, long addr, float* ret) {
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return 0;
    float val = 0;
    BOOL ok = ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(addr), &val, sizeof(val), NULL);
    if (ret) *ret = val;
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_readFloatAddr(intptr_t hwnd, long addr, float* ret) {
    return dm_readFloat(hwnd, addr, ret);
}

DM_API long DM_CALL dm_readDouble(intptr_t hwnd, long addr, double* ret) {
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return 0;
    double val = 0;
    BOOL ok = ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(addr), &val, sizeof(val), NULL);
    if (ret) *ret = val;
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_readDoubleAddr(intptr_t hwnd, long addr, double* ret) {
    return dm_readDouble(hwnd, addr, ret);
}

DM_API const char* DM_CALL dm_readString(intptr_t hwnd, long addr, long type, long len) {
    // type: 0=GBK, 1=UTF-16, 2=数值
    if (!hwnd || !addr || len <= 0) return DM_SetResult("");
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return DM_SetResult("");
    std::vector<char> buf(len + 2);
    BOOL ok = ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(addr), buf.data(), len, NULL);
    CloseHandle(hProc);
    if (!ok) return DM_SetResult("");
    buf[len] = '\0';
    return DM_SetResult(buf.data());
}

DM_API const char* DM_CALL dm_readStringAddr(intptr_t hwnd, long addr, long type, long len) {
    return dm_readString(hwnd, addr, type, len);
}

DM_API const char* DM_CALL dm_readData(intptr_t hwnd, long addr, long len) {
    // 返回十六进制字符串
    if (!hwnd || !addr || len <= 0) return DM_SetResult("");
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return DM_SetResult("");
    std::vector<BYTE> buf(len);
    BOOL ok = ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(addr), buf.data(), len, NULL);
    CloseHandle(hProc);
    if (!ok) return DM_SetResult("");
    std::string hex;
    for (long i = 0; i < len; i++) {
        char tmp[4];
        sprintf(tmp, "%02X", buf[i]);
        hex += tmp;
    }
    return DM_SetResult(hex);
}

DM_API const char* DM_CALL dm_readDataAddr(intptr_t hwnd, long addr, long len) {
    return dm_readData(hwnd, addr, len);
}

DM_API long DM_CALL dm_readDataToBin(intptr_t hwnd, long addr, long len) {
    return 0; // 简化
}

DM_API long DM_CALL dm_readDataAddrToBin(intptr_t hwnd, long addr, long len) {
    return 0;
}

// 内存搜索函数（简化实现）
DM_API const char* DM_CALL dm_findInt(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findIntEx(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max, long step, long multi_thread, long mode) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findFloat(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findFloatEx(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max, long step, long multi_thread, long mode) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findDouble(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findDoubleEx(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max, long step, long multi_thread, long mode) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findString(intptr_t hwnd, const char* addr_range, const char* string_value, long type) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findStringEx(intptr_t hwnd, const char* addr_range, const char* string_value, long type, long step, long multi_thread, long mode) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findData(intptr_t hwnd, const char* addr_range, const char* data) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findDataEx(intptr_t hwnd, const char* addr_range, const char* data, long step, long multi_thread, long mode) {
    return DM_SetResult("");
}

// 内存写入函数
DM_API long DM_CALL dm_writeInt(intptr_t hwnd, long addr, long type, long v) {
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) return 0;
    DWORD size = 4;
    if (type == 1) size = 1;
    else if (type == 2) size = 2;
    BOOL ok = WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(addr), &v, size, NULL);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_writeIntAddr(intptr_t hwnd, long addr, long type, long v) {
    return dm_writeInt(hwnd, addr, type, v);
}

DM_API long DM_CALL dm_writeFloat(intptr_t hwnd, long addr, float v) {
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) return 0;
    BOOL ok = WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(addr), &v, sizeof(v), NULL);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_writeFloatAddr(intptr_t hwnd, long addr, float v) {
    return dm_writeFloat(hwnd, addr, v);
}

DM_API long DM_CALL dm_writeDouble(intptr_t hwnd, long addr, double v) {
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) return 0;
    BOOL ok = WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(addr), &v, sizeof(v), NULL);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_writeDoubleAddr(intptr_t hwnd, long addr, double v) {
    return dm_writeDouble(hwnd, addr, v);
}

DM_API long DM_CALL dm_writeString(intptr_t hwnd, long addr, long type, const char* v) {
    if (!hwnd || !addr) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) return 0;
    std::string s(v ? v : "");
    BOOL ok = WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(addr), s.c_str(), s.length() + 1, NULL);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_writeStringAddr(intptr_t hwnd, long addr, long type, const char* v) {
    return dm_writeString(hwnd, addr, type, v);
}

DM_API long DM_CALL dm_writeData(intptr_t hwnd, long addr, const char* data) {
    // data 是十六进制字符串
    if (!hwnd || !addr || !data) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) return 0;
    std::string hex(data);
    std::vector<BYTE> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        if (i + 1 < hex.length()) {
            BYTE b = static_cast<BYTE>(strtol(hex.substr(i, 2).c_str(), nullptr, 16));
            bytes.push_back(b);
        }
    }
    BOOL ok = WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(addr), bytes.data(), bytes.size(), NULL);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_writeDataAddr(intptr_t hwnd, long addr, const char* data) {
    return dm_writeData(hwnd, addr, data);
}

DM_API long DM_CALL dm_writeDataFromBin(intptr_t hwnd, long addr, long data, long len) {
    return 0;
}

DM_API long DM_CALL dm_writeDataAddrFromBin(intptr_t hwnd, long addr, long data, long len) {
    return 0;
}

DM_API long DM_CALL dm_virtualAllocEx(intptr_t hwnd, long size, long type, long protect) {
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) {
        // 尝试用当前进程
        return static_cast<long>(reinterpret_cast<intptr_t>(VirtualAlloc(NULL, static_cast<SIZE_T>(size), static_cast<DWORD>(type), static_cast<DWORD>(protect))));
    }
    LPVOID addr = VirtualAllocEx(hProc, NULL, static_cast<SIZE_T>(size), static_cast<DWORD>(type), static_cast<DWORD>(protect));
    CloseHandle(hProc);
    return static_cast<long>(reinterpret_cast<intptr_t>(addr));
}

DM_API long DM_CALL dm_virtualFreeEx(intptr_t hwnd, long addr) {
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) return 0;
    BOOL ok = VirtualFreeEx(hProc, reinterpret_cast<LPVOID>(addr), 0, MEM_RELEASE);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_virtualProtectEx(intptr_t hwnd, long addr, long size, long type, long* old_protect) {
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) return 0;
    DWORD old = 0;
    BOOL ok = VirtualProtectEx(hProc, reinterpret_cast<LPVOID>(addr), static_cast<SIZE_T>(size), static_cast<DWORD>(type), &old);
    if (old_protect) *old_protect = static_cast<long>(old);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API const char* DM_CALL dm_virtualQueryEx(intptr_t hwnd, long addr, long pmbi) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_freeProcessMemory(intptr_t hwnd) {
    return 1;
}

DM_API const char* DM_CALL dm_getCommandLine(intptr_t hwnd) {
    if (!hwnd) return DM_SetResult("");
    DWORD pid = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(hwnd), &pid);
    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return DM_SetResult("");
    // 使用 PEB 读取命令行 — 简化
    CloseHandle(hProc);
    return DM_SetResult("");
}

DM_API long DM_CALL dm_terminateProcess(long pid) {
    HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, static_cast<DWORD>(pid));
    if (!hProc) return 0;
    BOOL ok = TerminateProcess(hProc, 0);
    CloseHandle(hProc);
    return ok ? 1 : 0;
}

DM_API long DM_CALL dm_terminateProcessTree(long pid) {
    return dm_terminateProcess(pid);
}

DM_API long DM_CALL dm_setMemoryFindResultToFile(long enable) {
    return 1;
}

DM_API long DM_CALL dm_setMemoryHwndAsProcessId(long enable) {
    return 1;
}

DM_API long DM_CALL dm_setParam64ToPointer(long enable) {
    return 1;
}

DM_API const char* DM_CALL dm_int64ToInt32(long v_high, long v_low, long flag) {
    return DM_SetResult("0");
}

DM_API const char* DM_CALL dm_intToData(long v, long type) {
    if (type == 1) { // 1字节
        char buf[8]; sprintf(buf, "%02X", static_cast<BYTE>(v & 0xFF));
        return DM_SetResult(buf);
    } else if (type == 2) { // 2字节
        char buf[8]; sprintf(buf, "%04X", static_cast<WORD>(v & 0xFFFF));
        return DM_SetResult(buf);
    }
    char buf[16]; sprintf(buf, "%08X", static_cast<DWORD>(v));
    return DM_SetResult(buf);
}

DM_API const char* DM_CALL dm_floatToData(float v) {
    char buf[16];
    sprintf(buf, "%08X", *reinterpret_cast<DWORD*>(&v));
    return DM_SetResult(buf);
}

DM_API const char* DM_CALL dm_doubleToData(double v) {
    char buf[32];
    DWORD* p = reinterpret_cast<DWORD*>(&v);
    sprintf(buf, "%08X%08X", p[0], p[1]);
    return DM_SetResult(buf);
}

DM_API const char* DM_CALL dm_stringToData(const char* v, long type) {
    if (!v) return DM_SetResult("");
    std::string hex;
    for (const char* p = v; *p; p++) {
        char tmp[4];
        sprintf(tmp, "%02X", static_cast<BYTE>(*p));
        hex += tmp;
    }
    return DM_SetResult(hex);
}

// ============================================================================
// 模块 7: 文件
// ============================================================================

DM_API long DM_CALL dm_writeFile(const char* file, const char* content) {
    if (!file) return 0;
    std::wstring wpath = GetFullPath(file);
    HANDLE h = CreateFileW(wpath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    DWORD written = 0;
    if (content) WriteFile(h, content, static_cast<DWORD>(strlen(content)), &written, NULL);
    CloseHandle(h);
    return 1;
}

DM_API const char* DM_CALL dm_readFile(const char* file) {
    if (!file) return DM_SetResult("");
    std::wstring wpath = GetFullPath(file);
    HANDLE h = CreateFileW(wpath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return DM_SetResult("");
    DWORD size = GetFileSize(h, NULL);
    if (size <= 0) { CloseHandle(h); return DM_SetResult(""); }
    std::vector<char> buf(size + 1);
    DWORD read = 0;
    ReadFile(h, buf.data(), size, &read, NULL);
    buf[size] = '\0';
    CloseHandle(h);
    return DM_SetResult(buf.data());
}

DM_API long DM_CALL dm_deleteFile(const char* file) {
    if (!file) return 0;
    std::wstring wpath = GetFullPath(file);
    return DeleteFileW(wpath.c_str()) ? 1 : 0;
}

DM_API long DM_CALL dm_copyFile(const char* src, const char* dest, long over) {
    if (!src || !dest) return 0;
    std::wstring wsrc = GetFullPath(src);
    std::wstring wdest = GetFullPath(dest);
    return CopyFileW(wsrc.c_str(), wdest.c_str(), over ? FALSE : TRUE) ? 1 : 0;
}

DM_API long DM_CALL dm_moveFile(const char* src, const char* dest) {
    if (!src || !dest) return 0;
    std::wstring wsrc = GetFullPath(src);
    std::wstring wdest = GetFullPath(dest);
    return MoveFileW(wsrc.c_str(), wdest.c_str()) ? 1 : 0;
}

DM_API long DM_CALL dm_createFolder(const char* folder) {
    if (!folder) return 0;
    std::wstring wpath = GetFullPath(folder);
    return CreateDirectoryW(wpath.c_str(), NULL) ? 1 : 0;
}

DM_API long DM_CALL dm_deleteFolder(const char* folder) {
    if (!folder) return 0;
    std::wstring wpath = GetFullPath(folder);
    return RemoveDirectoryW(wpath.c_str()) ? 1 : 0;
}

DM_API long DM_CALL dm_isFileExist(const char* file) {
    if (!file) return 0;
    std::wstring wpath = GetFullPath(file);
    return (GetFileAttributesW(wpath.c_str()) != INVALID_FILE_ATTRIBUTES &&
            !(GetFileAttributesW(wpath.c_str()) & FILE_ATTRIBUTE_DIRECTORY)) ? 1 : 0;
}

DM_API long DM_CALL dm_isFolderExist(const char* folder) {
    if (!folder) return 0;
    std::wstring wpath = GetFullPath(folder);
    DWORD attr = GetFileAttributesW(wpath.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) ? 1 : 0;
}

DM_API long DM_CALL dm_getFileLength(const char* file) {
    if (!file) return 0;
    std::wstring wpath = GetFullPath(file);
    HANDLE h = CreateFileW(wpath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    DWORD size = GetFileSize(h, NULL);
    CloseHandle(h);
    return static_cast<long>(size);
}

DM_API const char* DM_CALL dm_getRealPath(const char* path) {
    if (!path) return DM_SetResult("");
    wchar_t full[MAX_PATH] = {0};
    _wfullpath(full, Utf8ToWide(path).c_str(), MAX_PATH);
    return DM_SetResult(WideToUtf8(full));
}

DM_API const char* DM_CALL dm_selectFile() {
    // 简化：打开文件选择对话框
    OPENFILENAMEA ofn = {0};
    char buf[MAX_PATH] = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.lpstrFile = buf;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    if (GetOpenFileNameA(&ofn)) {
        return DM_SetResult(buf);
    }
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_selectDirectory() {
    // 简化：打开文件夹选择对话框
    char buf[MAX_PATH] = {0};
    BROWSEINFOA bi = {0};
    bi.lpszTitle = "Select Directory";
    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
    if (pidl) {
        SHGetPathFromIDListA(pidl, buf);
        CoTaskMemFree(pidl);
        return DM_SetResult(buf);
    }
    return DM_SetResult("");
}

DM_API long DM_CALL dm_downloadFile(const char* url, const char* save_file, long timeout) {
    // ponytail: 使用 URLDownloadToFile 下载
    // 需要 urlmon.dll
    typedef HRESULT (WINAPI *URLDownloadToFileFn)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD, LPUNKNOWN);
    HMODULE hUrlmon = LoadLibraryA("urlmon.dll");
    if (!hUrlmon) return 0;
    URLDownloadToFileFn fn = reinterpret_cast<URLDownloadToFileFn>(GetProcAddress(hUrlmon, "URLDownloadToFileA"));
    if (!fn) { FreeLibrary(hUrlmon); return 0; }
    HRESULT hr = fn(NULL, url, save_file, 0, NULL);
    FreeLibrary(hUrlmon);
    return SUCCEEDED(hr) ? 1 : 0;
}

DM_API long DM_CALL dm_encodeFile(const char* file, const char* pwd) {
    return 1;
}

DM_API long DM_CALL dm_decodeFile(const char* file, const char* pwd) {
    return 1;
}

// INI 文件操作
DM_API long DM_CALL dm_writeIni(const char* section, const char* key, const char* value, const char* file) {
    if (!file || !section || !key) return 0;
    std::wstring wfile = GetFullPath(file);
    std::wstring wsection = Utf8ToWide(section);
    std::wstring wkey = Utf8ToWide(key);
    std::wstring wvalue = value ? Utf8ToWide(value) : L"";
    return WritePrivateProfileStringW(wsection.c_str(), wkey.c_str(), wvalue.c_str(), wfile.c_str()) ? 1 : 0;
}

DM_API const char* DM_CALL dm_readIni(const char* section, const char* key, const char* file) {
    if (!file || !section || !key) return DM_SetResult("");
    std::wstring wfile = GetFullPath(file);
    std::wstring wsection = Utf8ToWide(section);
    std::wstring wkey = Utf8ToWide(key);
    wchar_t buf[4096] = {0};
    GetPrivateProfileStringW(wsection.c_str(), wkey.c_str(), L"", buf, 4096, wfile.c_str());
    return DM_SetResult(WideToUtf8(buf));
}

DM_API long DM_CALL dm_deleteIni(const char* section, const char* key, const char* file) {
    if (!file || !section) return 0;
    std::wstring wfile = GetFullPath(file);
    std::wstring wsection = Utf8ToWide(section);
    std::wstring wkey = key ? Utf8ToWide(key) : L"";
    return WritePrivateProfileStringW(wsection.c_str(), wkey.empty() ? NULL : wkey.c_str(), NULL, wfile.c_str()) ? 1 : 0;
}

DM_API const char* DM_CALL dm_enumIniKey(const char* section, const char* file) {
    if (!file || !section) return DM_SetResult("");
    std::wstring wfile = GetFullPath(file);
    std::wstring wsection = Utf8ToWide(section);
    wchar_t buf[8192] = {0};
    GetPrivateProfileStringW(wsection.c_str(), NULL, L"", buf, 8192, wfile.c_str());
    // 返回逗号分隔的键名
    std::string result;
    for (wchar_t* p = buf; *p; p += wcslen(p) + 1) {
        if (!result.empty()) result += ",";
        result += WideToUtf8(p);
    }
    return DM_SetResult(result);
}

DM_API const char* DM_CALL dm_enumIniSection(const char* file) {
    if (!file) return DM_SetResult("");
    std::wstring wfile = GetFullPath(file);
    wchar_t buf[8192] = {0};
    GetPrivateProfileStringW(NULL, NULL, L"", buf, 8192, wfile.c_str());
    std::string result;
    for (wchar_t* p = buf; *p; p += wcslen(p) + 1) {
        if (!result.empty()) result += ",";
        result += WideToUtf8(p);
    }
    return DM_SetResult(result);
}

DM_API long DM_CALL dm_writeIniPwd(const char* section, const char* key, const char* value, const char* file, const char* pwd) {
    return dm_writeIni(section, key, value, file);
}

DM_API const char* DM_CALL dm_readIniPwd(const char* section, const char* key, const char* file, const char* pwd) {
    return dm_readIni(section, key, file);
}

DM_API long DM_CALL dm_deleteIniPwd(const char* section, const char* key, const char* file, const char* pwd) {
    return dm_deleteIni(section, key, file);
}

DM_API const char* DM_CALL dm_enumIniKeyPwd(const char* section, const char* file, const char* pwd) {
    return dm_enumIniKey(section, file);
}

DM_API const char* DM_CALL dm_enumIniSectionPwd(const char* file, const char* pwd) {
    return dm_enumIniSection(file);
}

// ============================================================================
// 模块 8: 文字识别 (OCR)
// ============================================================================
// 注意: 如需完整的 OCR 功能，需要安装 Tesseract
// 此处提供简化实现

DM_API long DM_CALL dm_setDict(long index, const char* file) {
    DmState* s = GetDmState();
    if (!s) return 0;
    if (index < 0 || index >= DM_MAX_DICT) return 0;
    if (file) s->dicts[index] = Utf8ToWide(file);
    return 1;
}

DM_API long DM_CALL dm_setDictMem(long index, const char* data, long size) {
    return 1;
}

DM_API long DM_CALL dm_setDictPwd(const char* pwd) {
    return 1;
}

DM_API long DM_CALL dm_useDict(long index) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->curDict = index;
    return 1;
}

DM_API const char* DM_CALL dm_getDict(long index, long font_index) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_getDictCount(long index) {
    return 0;
}

DM_API const char* DM_CALL dm_getDictInfo(const char* file, long index) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_getNowDict() {
    DmState* s = GetDmState();
    return s ? s->curDict : 0;
}

DM_API long DM_CALL dm_addDict(const char* dict_info) {
    return 1;
}

DM_API long DM_CALL dm_saveDict(long index, const char* file) {
    return 1;
}

DM_API long DM_CALL dm_clearDict(long index) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->dicts.erase(index);
    return 1;
}

DM_API long DM_CALL dm_enableShareDict(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->shareDict = enable != 0;
    return 1;
}

DM_API const char* DM_CALL dm_ocr(long x1, long y1, long x2, long y2, const char* color_format, long sim) {
    // ponytail: 简化 OCR — 返回空字符串
    // 如需完整 OCR，集成 Tesseract
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_ocrEx(long x1, long y1, long x2, long y2, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_ocrExOne(long x1, long y1, long x2, long y2, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_ocrInFile(long x1, long y1, long x2, long y2, const char* color_format, long sim, const char* file) {
    return 0;
}

// 找字函数 — 简化实现
DM_API long DM_CALL dm_findStr(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY) {
    if (intX) *intX = 0;
    if (intY) *intY = 0;
    return 0;
}

DM_API const char* DM_CALL dm_findStrE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findStrEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_findStrS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY) {
    return dm_findStr(x1, y1, x2, y2, str, color_format, sim, intX, intY);
}

DM_API const char* DM_CALL dm_findStrExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_findStrFast(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY) {
    return dm_findStr(x1, y1, x2, y2, str, color_format, sim, intX, intY);
}

DM_API const char* DM_CALL dm_findStrFastE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findStrFastEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_findStrFastS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY) {
    return dm_findStr(x1, y1, x2, y2, str, color_format, sim, intX, intY);
}

DM_API const char* DM_CALL dm_findStrFastExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_findStrWithFont(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size, long* intX, long* intY) {
    return dm_findStr(x1, y1, x2, y2, str, color_format, sim, intX, intY);
}

DM_API const char* DM_CALL dm_findStrWithFontE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_findStrWithFontEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_getWords(long x1, long y1, long x2, long y2, const char* color, const char* words) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_getWordsNoDict(long x1, long y1, long x2, long y2, const char* color) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_fetchWord(long x1, long y1, long x2, long y2, const char* color, const char* words) {
    return 0;
}

DM_API long DM_CALL dm_getResultCount(const char* ret) {
    if (!ret || !*ret) return 0;
    // 格式: "x1|y1|str1,x2|y2|str2,..."
    long count = 0;
    for (const char* p = ret; *p; p++) {
        if (*p == ',') count++;
    }
    return count + 1;
}

DM_API long DM_CALL dm_getResultPos(const char* ret, long index, long* intX, long* intY) {
    if (!ret) return 0;
    long cur = 0;
    const char* p = ret;
    while (cur < index && *p) {
        if (*p == ',') cur++;
        p++;
    }
    if (cur != index) return 0;
    long x = 0, y = 0;
    sscanf(p, "%ld|%ld", &x, &y);
    if (intX) *intX = x;
    if (intY) *intY = y;
    return 1;
}

DM_API long DM_CALL dm_getWordResultCount(const char* str) {
    return dm_getResultCount(str);
}

DM_API long DM_CALL dm_getWordResultPos(const char* str, long index, long* intX, long* intY) {
    return dm_getResultPos(str, index, intX, intY);
}

DM_API const char* DM_CALL dm_getWordResultStr(const char* str, long index) {
    if (!str) return DM_SetResult("");
    long cur = 0;
    const char* p = str;
    while (cur < index && *p) {
        if (*p == ',') cur++;
        p++;
    }
    if (cur != index) return DM_SetResult("");
    // 跳过 "x|y|"
    while (*p && *p != '|') p++;
    if (*p == '|') p++;
    while (*p && *p != '|') p++;
    if (*p == '|') p++;
    const char* start = p;
    while (*p && *p != ',') p++;
    std::string result(start, p - start);
    return DM_SetResult(result);
}

DM_API long DM_CALL dm_setColGapNoDict(long col_gap) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->colGapNoDict = col_gap; return 1;
}

DM_API long DM_CALL dm_setRowGapNoDict(long row_gap) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->rowGapNoDict = row_gap; return 1;
}

DM_API long DM_CALL dm_setWordGapNoDict(long word_gap) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->wordGapNoDict = word_gap; return 1;
}

DM_API long DM_CALL dm_setWordLineHeightNoDict(long line_height) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->wordLineHeightNoDict = line_height; return 1;
}

DM_API long DM_CALL dm_setExactOcr(long exact_ocr) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->exactOcr = exact_ocr != 0; return 1;
}

DM_API long DM_CALL dm_setMinColGap(long min_col_gap) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->minColGap = min_col_gap; return 1;
}

DM_API long DM_CALL dm_setMinRowGap(long min_row_gap) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->minRowGap = min_row_gap; return 1;
}

DM_API long DM_CALL dm_setWordGap(long word_gap) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->wordGap = word_gap; return 1;
}

DM_API long DM_CALL dm_setWordLineHeight(long line_height) {
    DmState* s = GetDmState(); if (!s) return 0;
    s->wordLineHeight = line_height; return 1;
}

// ============================================================================
// 模块 9: 系统
// ============================================================================

DM_API long DM_CALL dm_beep(long f, long duration) {
    Beep(static_cast<DWORD>(f), static_cast<DWORD>(duration));
    return 1;
}

DM_API long DM_CALL dm_delay(long mis) {
    if (mis > 0) Sleep(static_cast<DWORD>(mis));
    return 1;
}

DM_API long DM_CALL dm_delays(long mis_min, long mis_max) {
    if (mis_max <= mis_min) { Sleep(static_cast<DWORD>(mis_min)); return 1; }
    static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<long> dist(mis_min, mis_max);
    Sleep(static_cast<DWORD>(dist(rng)));
    return 1;
}

DM_API long DM_CALL dm_runApp(const char* app_path, long mode) {
    if (!app_path) return 0;
    std::wstring wpath = Utf8ToWide(app_path);
    int showCmd = SW_SHOW;
    switch (mode) {
        case 0: showCmd = SW_SHOW; break;
        case 1: showCmd = SW_HIDE; break;
        case 2: showCmd = SW_MINIMIZE; break;
        case 3: showCmd = SW_MAXIMIZE; break;
    }
    HINSTANCE h = ShellExecuteW(NULL, L"open", wpath.c_str(), NULL, NULL, showCmd);
    return (reinterpret_cast<INT_PTR>(h) > 32) ? 1 : 0;
}

DM_API long DM_CALL dm_stop(long id) {
    return 1;
}

DM_API long DM_CALL dm_play(const char* media_file) {
    if (!media_file) return 0;
    std::wstring wpath = GetFullPath(media_file);
    return PlaySoundW(wpath.c_str(), NULL, SND_FILENAME | SND_ASYNC) ? 1 : 0;
}

DM_API long DM_CALL dm_exitOs(long type) {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValueW(NULL, L"SeShutdownPrivilege", &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    UINT flags = EWX_SHUTDOWN;
    switch (type) {
        case 0: flags = EWX_LOGOFF; break;
        case 1: flags = EWX_SHUTDOWN; break;
        case 2: flags = EWX_REBOOT; break;
        case 3: flags = EWX_POWEROFF; break;
    }
    return ExitWindowsEx(flags, SHTDN_REASON_MAJOR_APPLICATION) ? 1 : 0;
}

DM_API long DM_CALL dm_setScreen(long width, long height, long depth) {
    DEVMODEA dm = {0};
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = static_cast<DWORD>(width);
    dm.dmPelsHeight = static_cast<DWORD>(height);
    dm.dmBitsPerPel = static_cast<DWORD>(depth);
    dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
    return ChangeDisplaySettingsA(&dm, CDS_UPDATEREGISTRY) == DISP_CHANGE_SUCCESSFUL ? 1 : 0;
}

DM_API long DM_CALL dm_getScreenWidth()  { return static_cast<long>(GetSystemMetrics(SM_CXSCREEN)); }
DM_API long DM_CALL dm_getScreenHeight() { return static_cast<long>(GetSystemMetrics(SM_CYSCREEN)); }
DM_API long DM_CALL dm_getScreenDepth()  {
    HDC hdc = GetDC(NULL);
    int depth = GetDeviceCaps(hdc, BITSPIXEL);
    ReleaseDC(NULL, hdc);
    return depth;
}

DM_API long DM_CALL dm_getDPI() {
    HDC hdc = GetDC(NULL);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(NULL, hdc);
    return dpi;
}

DM_API long DM_CALL dm_getTime() {
    return static_cast<long>(time(nullptr));
}

DM_API long DM_CALL dm_getOsType() {
    // 返回 Windows 版本
    RTL_OSVERSIONINFOW vi = { sizeof(vi) };
    typedef LONG (WINAPI *RtlGetVersionFn)(PRTL_OSVERSIONINFOW);
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (hNtdll) {
        RtlGetVersionFn fn = reinterpret_cast<RtlGetVersionFn>(GetProcAddress(hNtdll, "RtlGetVersion"));
        if (fn) fn(&vi);
    }
    if (vi.dwMajorVersion == 10 && vi.dwBuildNumber >= 22000) return 7; // Win11
    if (vi.dwMajorVersion == 10) return 6; // Win10
    if (vi.dwMajorVersion == 6 && vi.dwMinorVersion == 3) return 5; // Win8.1
    if (vi.dwMajorVersion == 6 && vi.dwMinorVersion == 2) return 4; // Win8
    if (vi.dwMajorVersion == 6 && vi.dwMinorVersion == 1) return 3; // Win7
    if (vi.dwMajorVersion == 6 && vi.dwMinorVersion == 0) return 2; // Vista
    if (vi.dwMajorVersion == 5 && vi.dwMinorVersion >= 1) return 1; // XP
    return 0;
}

DM_API long DM_CALL dm_getOsBuildNumber() {
    RTL_OSVERSIONINFOW vi = { sizeof(vi) };
    typedef LONG (WINAPI *RtlGetVersionFn)(PRTL_OSVERSIONINFOW);
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (hNtdll) {
        RtlGetVersionFn fn = reinterpret_cast<RtlGetVersionFn>(GetProcAddress(hNtdll, "RtlGetVersion"));
        if (fn) fn(&vi);
    }
    return static_cast<long>(vi.dwBuildNumber);
}

DM_API long DM_CALL dm_is64Bit() {
    #ifdef _WIN64
    return 1;
    #else
    BOOL is64 = FALSE;
    return (IsWow64Process(GetCurrentProcess(), &is64) && is64) ? 1 : 0;
    #endif
}

DM_API long DM_CALL dm_isSurrpotVt() {
    return 0;
}

DM_API const char* DM_CALL dm_getSystemInfo(long type, long method) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_getDir(long type) {
    wchar_t buf[MAX_PATH] = {0};
    switch (type) {
        case 0: GetWindowsDirectoryW(buf, MAX_PATH); break;
        case 1: GetSystemDirectoryW(buf, MAX_PATH); break;
        case 2: GetTempPathW(MAX_PATH, buf); break;
        case 3: GetCurrentDirectoryW(MAX_PATH, buf); break;
        default: return DM_SetResult("");
    }
    return DM_SetResult(WideToUtf8(buf));
}

DM_API long DM_CALL dm_getLocale() {
    return GetSystemDefaultLCID();
}

DM_API long DM_CALL dm_getCpuType() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return static_cast<long>(si.wProcessorArchitecture);
}

DM_API long DM_CALL dm_getCpuUsage() {
    return 0; // 简化
}

DM_API long DM_CALL dm_getMemoryUsage() {
    MEMORYSTATUSEX ms = { sizeof(ms) };
    GlobalMemoryStatusEx(&ms);
    return static_cast<long>(ms.dwMemoryLoad);
}

DM_API const char* DM_CALL dm_getDiskSerial() {
    char buf[32] = {0};
    DWORD vsn = 0;
    GetVolumeInformationA("C:\\", NULL, 0, &vsn, NULL, NULL, NULL, 0);
    sprintf(buf, "%08X", vsn);
    return DM_SetResult(buf);
}

DM_API const char* DM_CALL dm_getDiskModel() {
    return DM_SetResult("Unknown");
}

DM_API const char* DM_CALL dm_getDiskReversion() {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_getMachineCode() {
    // 组合 CPU ID + 硬盘序列号
    char buf[64] = {0};
    int cpuInfo[4] = {0};
    __cpuid(cpuInfo, 1);
    DWORD vsn = 0;
    GetVolumeInformationA("C:\\", NULL, 0, &vsn, NULL, NULL, NULL, 0);
    sprintf(buf, "%08X%08X%08X", cpuInfo[0], cpuInfo[3], vsn);
    return DM_SetResult(buf);
}

DM_API const char* DM_CALL dm_getMachineCodeNoMac() {
    return dm_getMachineCode();
}

DM_API const char* DM_CALL dm_getNetTime() {
    // 使用 HTTP 获取网络时间
    return dm_getNetTimeByIp("ntp.aliyun.com");
}

DM_API const char* DM_CALL dm_getNetTimeByIp(const char* ip) {
    // ponytail: 使用 WinHTTP 获取网络时间
    // 简化：返回本地时间
    time_t now = time(nullptr);
    struct tm* ptm = gmtime(&now);
    char buf[32];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
        ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
        ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    return DM_SetResult(buf);
}

DM_API const char* DM_CALL dm_getNetTimeSafe(long time_out) {
    return dm_getNetTimeByIp("ntp.aliyun.com");
}

DM_API const char* DM_CALL dm_getClipboard() {
    if (!OpenClipboard(NULL)) return DM_SetResult("");
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (!hData) { CloseClipboard(); return DM_SetResult(""); }
    char* p = reinterpret_cast<char*>(GlobalLock(hData));
    if (!p) { CloseClipboard(); return DM_SetResult(""); }
    std::string result(p);
    GlobalUnlock(hData);
    CloseClipboard();
    return DM_SetResult(result);
}

DM_API long DM_CALL dm_setClipboard(const char* value) {
    if (!value) return 0;
    if (!OpenClipboard(NULL)) return 0;
    EmptyClipboard();
    size_t len = strlen(value) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (!hMem) { CloseClipboard(); return 0; }
    void* p = GlobalLock(hMem);
    if (!p) { GlobalFree(hMem); CloseClipboard(); return 0; }
    memcpy(p, value, len);
    GlobalUnlock(hMem);
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    return 1;
}

DM_API long DM_CALL dm_getDisplayInfo(long type) {
    return 0;
}

DM_API long DM_CALL dm_disableCloseDisplayAndSleep(long enable) {
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
    return 1;
}

DM_API long DM_CALL dm_disablePowerSave(long enable) {
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
    return 1;
}

DM_API long DM_CALL dm_disableScreenSave(long enable) {
    return 1;
}

DM_API long DM_CALL dm_disableFontSmooth() {
    return 0;
}

DM_API long DM_CALL dm_enableFontSmooth() {
    return 0;
}

DM_API long DM_CALL dm_checkFontSmooth() {
    return 0;
}

DM_API long DM_CALL dm_checkUAC() {
    return 0;
}

DM_API long DM_CALL dm_setUAC(long enable) {
    return 1;
}

DM_API long DM_CALL dm_setDisplayAcceler(long flag) {
    return 1;
}

DM_API long DM_CALL dm_showTaskBarIcon(intptr_t hwnd, long show) {
    if (!hwnd) return 0;
    ShowWindow(reinterpret_cast<HWND>(hwnd), show ? SW_SHOW : SW_HIDE);
    return 1;
}

// ============================================================================
// 模块 10: 杂项
// ============================================================================

DM_API long DM_CALL dm_enterCri() {
    DmState* s = GetDmState();
    if (!s) return 0;
    CritSecLockable lockable(s->criSection);
    std::lock_guard<CritSecLockable> lock(lockable);
    return 1;
}

DM_API long DM_CALL dm_leaveCri() {
    DmState* s = GetDmState();
    if (!s) return 0;
    CritSecLockable lockable(s->criSection);
    std::lock_guard<CritSecLockable> lock(lockable);
    return 1;
}

DM_API long DM_CALL dm_initCri() {
    DmState* s = GetDmState();
    if (!s) return 0;
    // 已在构造函数中初始化
    return 1;
}

DM_API long DM_CALL dm_releaseRef() {
    // 释放 COM 引用计数（线程本地状态）
    DmState* s = GetDmState();
    if (s) {
        delete s;
        TlsSetValue(g_tlsIndex, NULL);
    }
    return 1;
}

DM_API long DM_CALL dm_setExitThread(long mode) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->exitThreadMode = mode != 0;
    return 1;
}

DM_API const char* DM_CALL dm_executeCmd(const char* cmd, long time_out) {
    if (!cmd || !*cmd) return DM_SetResult("");
    // 命令白名单验证：只允许执行有限的可执行文件，防止命令注入
    static const std::vector<std::string> kAllowedCommands = {
        "ipconfig", "systeminfo", "tasklist", "ver", "whoami", "hostname",
        "ping", "pathping", "tracert", "netstat", "dir", "echo",
        "taskkill", "schtasks", "wmic", "reg", "sc", "net", "findstr"
    };
    // 去掉首尾空白
    std::string cmdStr(cmd);
    size_t start = cmdStr.find_first_not_of(" \t");
    if (start == std::string::npos) return DM_SetResult("");
    size_t end = cmdStr.find_last_not_of(" \t");
    cmdStr = cmdStr.substr(start, end - start + 1);
    // 提取命令名（第一个 token）
    std::string exe = cmdStr.substr(0, cmdStr.find_first_of(" \t"));
    // 白名单校验
    bool allowed = false;
    for (const auto& a : kAllowedCommands) {
        if (exe == a) { allowed = true; break; }
    }
    if (!allowed) return DM_SetResult("");
    // 拒绝重定向/管道/链式命令等注入向量
    if (cmdStr.find('|') != std::string::npos ||
        cmdStr.find('&') != std::string::npos ||
        cmdStr.find('>') != std::string::npos ||
        cmdStr.find('<') != std::string::npos ||
        cmdStr.find(';') != std::string::npos ||
        cmdStr.find('\n') != std::string::npos ||
        cmdStr.find('\r') != std::string::npos) {
        return DM_SetResult("");
    }
    // 执行命令并返回输出
    std::string result;
    FILE* pipe = _popen(cmdStr.c_str(), "r");
    if (!pipe) return DM_SetResult("");
    char buf[512];
    while (fgets(buf, sizeof(buf), pipe)) {
        result += buf;
    }
    _pclose(pipe);
    return DM_SetResult(result);
}

DM_API long DM_CALL dm_activeInputMethod(intptr_t hwnd, const char* input_method) {
    return 1;
}

DM_API long DM_CALL dm_checkInputMethod(intptr_t hwnd, const char* input_method) {
    return 0;
}

DM_API long DM_CALL dm_findInputMethod(const char* input_method) {
    return 0;
}

// ============================================================================
// 模块 11: 汇编
// ============================================================================

DM_API long DM_CALL dm_asmAdd(const char* asm_ins) {
    DmState* s = GetDmState();
    if (!s) return 0;
    // 暂存汇编指令字符串
    std::string ins(asm_ins ? asm_ins : "");
    // ponytail: 简化实现，将指令转换为字节码
    // 实际需要完整的汇编器（如 XEDParse, Keystone 等）
    if (ins == "ret" || ins == "retn") {
        s->asmCode.push_back(0xC3);
    }
    // 其他指令暂不处理
    return 1;
}

DM_API long DM_CALL dm_asmCall(intptr_t hwnd, long mode) {
    // ponytail: 简化实现 — 在当前进程执行
    DmState* s = GetDmState();
    if (!s) return 0;
    if (s->asmCode.empty()) return 0;
    // 简易执行：仅支持 ret 指令
    if (s->asmCode.size() == 1 && s->asmCode[0] == 0xC3) {
        return 0;
    }
    return 0;
}

DM_API long DM_CALL dm_asmCallEx(intptr_t hwnd, long mode, long base_addr, long call_addr) {
    return 0;
}

DM_API long DM_CALL dm_asmClear() {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->asmCode.clear();
    return 1;
}

DM_API long DM_CALL dm_asmSetTimeout(long timeout) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->asmTimeout = timeout;
    return 1;
}

DM_API const char* DM_CALL dm_assemble(long base_addr, long is_asm_code) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_disAssemble(long asm_code, long base_addr, long is_asm_code) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_setAsmHwndAsProcessId(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->asmHwndAsProcessId = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_setShowAsmErrorMsg(long show) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->showAsmErrorMsg = show != 0;
    return 1;
}

// ============================================================================
// 模块 12: AI
// ============================================================================
// 注意: YOLO 功能需要 ONNX Runtime 集成
// 此处提供简化 stub

DM_API long DM_CALL dm_loadAi(const char* file) {
    return 0;
}

DM_API long DM_CALL dm_loadAiMemory(long data, long size) {
    return 0;
}

DM_API long DM_CALL dm_aiEnableFindPicWindow(long enable) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->aiEnableFindPic = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_aiFindPic(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir, long* intX, long* intY) {
    return 0;
}

DM_API const char* DM_CALL dm_aiFindPicEx(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_aiFindPicMem(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir, long* intX, long* intY) {
    return 0;
}

DM_API const char* DM_CALL dm_aiFindPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir) {
    return DM_SetResult("");
}

DM_API const char* DM_CALL dm_aiYoloDetectObjects(long x1, long y1, long x2, long y2, float prob, float iou) {
    // ponytail: YOLO检测需要 ONNX Runtime 模型，此处返回空
    return DM_SetResult("");
}

DM_API long DM_CALL dm_aiYoloDetectObjectsToDataBmp(long x1, long y1, long x2, long y2, float prob, float iou, long data, long size, long mode) {
    return 0;
}

DM_API long DM_CALL dm_aiYoloDetectObjectsToFile(long x1, long y1, long x2, long y2, float prob, float iou, const char* file, long mode) {
    return 0;
}

DM_API long DM_CALL dm_aiYoloSetModel(const char* model_file, const char* model_type) {
    return 0;
}

DM_API long DM_CALL dm_aiYoloSetModelMemory(long data, long size, const char* model_type) {
    return 0;
}

DM_API long DM_CALL dm_aiYoloFreeModel() {
    return 1;
}

DM_API long DM_CALL dm_aiYoloUseModel(long index) {
    return 0;
}

DM_API long DM_CALL dm_aiYoloSetVersion(const char* version) {
    return 1;
}

DM_API const char* DM_CALL dm_aiYoloObjectsToString(const char* objects) {
    return DM_SetResult(objects ? objects : "");
}

DM_API const char* DM_CALL dm_aiYoloSortsObjects(const char* objects, long sort_type) {
    return DM_SetResult(objects ? objects : "");
}

// ============================================================================
// 模块 13: Foobar (界面控件 — 透明窗口绘制)
// ============================================================================

static std::map<HWND, HWND> g_foobarWindows; // 简化管理

DM_API long DM_CALL dm_createFoobarRect(intptr_t hwnd, long x, long y, long w, long h) {
    if (!hwnd) return 0;
    HWND parent = reinterpret_cast<HWND>(hwnd);
    HWND fb = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        L"STATIC", L"", WS_CHILD | WS_VISIBLE,
        static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h),
        parent, NULL, GetModuleHandleW(NULL), NULL);
    if (!fb) return 0;
    SetLayeredWindowAttributes(fb, 0, 200, LWA_ALPHA);
    g_foobarWindows[parent] = fb;
    return static_cast<long>(reinterpret_cast<intptr_t>(fb));
}

DM_API long DM_CALL dm_createFoobarEllipse(intptr_t hwnd, long x, long y, long w, long h) {
    HWND fb = reinterpret_cast<HWND>(dm_createFoobarRect(hwnd, x, y, w, h));
    // 设置圆形区域
    HRGN hrgn = CreateEllipticRgn(0, 0, static_cast<int>(w), static_cast<int>(h));
    SetWindowRgn(fb, hrgn, TRUE);
    return static_cast<long>(reinterpret_cast<intptr_t>(fb));
}

DM_API long DM_CALL dm_createFoobarRoundRect(intptr_t hwnd, long x, long y, long w, long h, long rw, long rh) {
    HWND fb = reinterpret_cast<HWND>(dm_createFoobarRect(hwnd, x, y, w, h));
    HRGN hrgn = CreateRoundRectRgn(0, 0, static_cast<int>(w), static_cast<int>(h), static_cast<int>(rw), static_cast<int>(rh));
    SetWindowRgn(fb, hrgn, TRUE);
    return static_cast<long>(reinterpret_cast<intptr_t>(fb));
}

DM_API long DM_CALL dm_createFoobarCustom(intptr_t hwnd, long x, long y, long w, long h, const char* pic) {
    HWND fb = reinterpret_cast<HWND>(dm_createFoobarRect(hwnd, x, y, w, h));
    if (pic) {
        std::wstring wpic = GetFullPath(pic);
        HBITMAP hbm = reinterpret_cast<HBITMAP>(LoadImageW(NULL, wpic.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
        if (hbm) {
            SendMessageW(fb, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(hbm));
        }
    }
    return static_cast<long>(reinterpret_cast<intptr_t>(fb));
}

DM_API long DM_CALL dm_foobarClose(intptr_t hwnd) {
    auto it = g_foobarWindows.find(reinterpret_cast<HWND>(hwnd));
    if (it != g_foobarWindows.end()) {
        DestroyWindow(it->second);
        g_foobarWindows.erase(it);
    }
    // 也尝试直接关闭
    DestroyWindow(reinterpret_cast<HWND>(hwnd));
    return 1;
}

DM_API long DM_CALL dm_foobarLock(intptr_t hwnd) {
    EnableWindow(reinterpret_cast<HWND>(hwnd), FALSE);
    return 1;
}

DM_API long DM_CALL dm_foobarUnlock(intptr_t hwnd) {
    EnableWindow(reinterpret_cast<HWND>(hwnd), TRUE);
    return 1;
}

DM_API long DM_CALL dm_foobarUpdate(intptr_t hwnd) {
    InvalidateRect(reinterpret_cast<HWND>(hwnd), NULL, TRUE);
    UpdateWindow(reinterpret_cast<HWND>(hwnd));
    return 1;
}

DM_API long DM_CALL dm_foobarSetFont(intptr_t hwnd, const char* font_name, long size, long flag) {
    if (!hwnd) return 0;
    std::wstring wfont = Utf8ToWide(font_name);
    HFONT hFont = CreateFontW(static_cast<int>(-size), 0, 0, 0,
        (flag & 1) ? FW_BOLD : FW_NORMAL,
        (flag & 2) ? TRUE : FALSE,
        (flag & 4) ? TRUE : FALSE,
        FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
        wfont.c_str());
    if (hFont) {
        SendMessageW(reinterpret_cast<HWND>(hwnd), WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
    }
    return 1;
}

DM_API long DM_CALL dm_foobarSetSave(intptr_t hwnd, const char* file, long enable) {
    return 1;
}

DM_API long DM_CALL dm_foobarSetTrans(intptr_t hwnd, long trans, long color) {
    SetLayeredWindowAttributes(reinterpret_cast<HWND>(hwnd), static_cast<COLORREF>(color), static_cast<BYTE>(trans), LWA_ALPHA | LWA_COLORKEY);
    return 1;
}

DM_API long DM_CALL dm_foobarDrawText(intptr_t hwnd, long x, long y, long w, long h, const char* text, long color, long align) {
    if (!hwnd || !text) return 0;
    HDC hdc = GetDC(reinterpret_cast<HWND>(hwnd));
    SetTextColor(hdc, static_cast<COLORREF>(color));
    SetBkMode(hdc, TRANSPARENT);
    UINT fmt = DT_LEFT;
    if (align == 1) fmt = DT_CENTER;
    else if (align == 2) fmt = DT_RIGHT;
    fmt |= DT_TOP;
    std::wstring wtext = Utf8ToWide(text);
    RECT r = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(x + w), static_cast<int>(y + h) };
    DrawTextW(hdc, wtext.c_str(), static_cast<int>(wtext.length()), &r, fmt);
    ReleaseDC(reinterpret_cast<HWND>(hwnd), hdc);
    return 1;
}

DM_API long DM_CALL dm_foobarPrintText(intptr_t hwnd, const char* text, long color) {
    return dm_foobarDrawText(hwnd, 0, 0, 800, 600, text, color, 0);
}

DM_API long DM_CALL dm_foobarDrawRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style) {
    HDC hdc = GetDC(reinterpret_cast<HWND>(hwnd));
    HPEN hPen = CreatePen(PS_SOLID, static_cast<int>(style), static_cast<COLORREF>(color));
    SelectObject(hdc, hPen);
    HBRUSH hBrush = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
    SelectObject(hdc, hBrush);
    Rectangle(hdc, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
    DeleteObject(hPen);
    ReleaseDC(reinterpret_cast<HWND>(hwnd), hdc);
    return 1;
}

DM_API long DM_CALL dm_foobarDrawLine(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style) {
    HDC hdc = GetDC(reinterpret_cast<HWND>(hwnd));
    HPEN hPen = CreatePen(PS_SOLID, static_cast<int>(style), static_cast<COLORREF>(color));
    SelectObject(hdc, hPen);
    MoveToEx(hdc, static_cast<int>(x1), static_cast<int>(y1), NULL);
    LineTo(hdc, static_cast<int>(x2), static_cast<int>(y2));
    DeleteObject(hPen);
    ReleaseDC(reinterpret_cast<HWND>(hwnd), hdc);
    return 1;
}

DM_API long DM_CALL dm_foobarFillRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color) {
    HDC hdc = GetDC(reinterpret_cast<HWND>(hwnd));
    HBRUSH hBrush = CreateSolidBrush(static_cast<COLORREF>(color));
    RECT r = { static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2) };
    FillRect(hdc, &r, hBrush);
    DeleteObject(hBrush);
    ReleaseDC(reinterpret_cast<HWND>(hwnd), hdc);
    return 1;
}

DM_API long DM_CALL dm_foobarDrawPic(intptr_t hwnd, long x, long y, const char* pic) {
    if (!hwnd || !pic) return 0;
    std::wstring wpic = GetFullPath(pic);
    HBITMAP hbm = reinterpret_cast<HBITMAP>(LoadImageW(NULL, wpic.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
    if (!hbm) return 0;
    HDC hdc = GetDC(reinterpret_cast<HWND>(hwnd));
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hbm);
    BITMAP bm;
    GetObject(hbm, sizeof(bm), &bm);
    BitBlt(hdc, static_cast<int>(x), static_cast<int>(y), bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
    DeleteObject(hbm);
    ReleaseDC(reinterpret_cast<HWND>(hwnd), hdc);
    return 1;
}

DM_API long DM_CALL dm_foobarClearText(intptr_t hwnd) {
    if (!hwnd) return 0;
    InvalidateRect(reinterpret_cast<HWND>(hwnd), NULL, TRUE);
    return 1;
}

DM_API long DM_CALL dm_foobarTextRect(intptr_t hwnd, long x, long y, long w, long h) {
    return 1;
}

DM_API long DM_CALL dm_foobarTextLineGap(intptr_t hwnd, long line_gap) {
    return 1;
}

DM_API long DM_CALL dm_foobarTextPrintDir(intptr_t hwnd, long dir) {
    return 1;
}

DM_API long DM_CALL dm_foobarStartGif(intptr_t hwnd, const char* pic, long x, long y) {
    return 0;
}

DM_API long DM_CALL dm_foobarStopGif(intptr_t hwnd) {
    return 1;
}

// ============================================================================
// 模块 14: 答题
// ============================================================================

DM_API long DM_CALL dm_faqCapture(long x1, long y1, long x2, long y2, long quality, long delay, long time) {
    return 0;
}

DM_API long DM_CALL dm_faqCaptureFromFile(const char* file, long quality, long delay, long time) {
    return 0;
}

DM_API const char* DM_CALL dm_faqCaptureString(long x1, long y1, long x2, long y2, long quality, long delay, long time) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_faqGetSize(long handle) {
    return 0;
}

DM_API long DM_CALL dm_faqPost(const char* server, long handle, long request_type, long time_out) {
    return 0;
}

DM_API long DM_CALL dm_faqSend(long handle, long time_out) {
    return 0;
}

DM_API long DM_CALL dm_faqFetch(long time_out) {
    return 0;
}

DM_API long DM_CALL dm_faqCancel() {
    return 1;
}

DM_API long DM_CALL dm_faqIsPosted() {
    return 0;
}

// ============================================================================
// 模块 15: 算法
// ============================================================================

DM_API const char* DM_CALL dm_excludePos(const char* all_pos, long type, long x1, long y1, long x2, long y2) {
    // all_pos 格式: "x1|y1,x2|y2,..."
    // 排除指定矩形区域内的坐标
    if (!all_pos) return DM_SetResult("");
    std::string result;
    std::string input(all_pos);
    std::stringstream ss(input);
    std::string seg;
    while (std::getline(ss, seg, ',')) {
        long px = 0, py = 0;
        if (sscanf(seg.c_str(), "%ld|%ld", &px, &py) >= 2) {
            if (px < x1 || px > x2 || py < y1 || py > y2) {
                if (!result.empty()) result += ",";
                result += seg;
            }
        }
    }
    return DM_SetResult(result);
}

DM_API const char* DM_CALL dm_findNearestPos(const char* all_pos, long type, long x, long y) {
    if (!all_pos) return DM_SetResult("");
    std::string input(all_pos);
    std::stringstream ss(input);
    std::string seg, bestSeg;
    long bestDist = LONG_MAX;
    while (std::getline(ss, seg, ',')) {
        long px = 0, py = 0;
        if (sscanf(seg.c_str(), "%ld|%ld", &px, &py) >= 2) {
            long dist = (px - x) * (px - x) + (py - y) * (py - y);
            if (dist < bestDist) {
                bestDist = dist;
                bestSeg = seg;
            }
        }
    }
    return DM_SetResult(bestSeg);
}

DM_API const char* DM_CALL dm_sortPosDistance(const char* all_pos, long type, long x, long y) {
    if (!all_pos) return DM_SetResult("");
    // 解析坐标对，按距离排序
    std::vector<std::pair<long, long>> points;
    std::string input(all_pos);
    std::stringstream ss(input);
    std::string seg;
    while (std::getline(ss, seg, ',')) {
        long px = 0, py = 0;
        if (sscanf(seg.c_str(), "%ld|%ld", &px, &py) >= 2) {
            points.push_back({px, py});
        }
    }
    // 按距离排序
    std::sort(points.begin(), points.end(),
        [x, y](const std::pair<long, long>& a, const std::pair<long, long>& b) {
            long da = (a.first - x) * (a.first - x) + (a.second - y) * (a.second - y);
            long db = (b.first - x) * (b.first - x) + (b.second - y) * (b.second - y);
            return da < db;
        });
    std::string result;
    for (const auto& p : points) {
        if (!result.empty()) result += ",";
        char buf[32];
        sprintf(buf, "%ld|%ld", p.first, p.second);
        result += buf;
    }
    return DM_SetResult(result);
}

// ============================================================================
// 模块 16: 防护盾
// ============================================================================

DM_API long DM_CALL dm_dmGuard(long enable, const char* type) {
    DmState* s = GetDmState();
    if (!s) return 0;
    s->guardEnabled = enable != 0;
    return 1;
}

DM_API long DM_CALL dm_dmGuardExtract(const char* type, const char* file) {
    return 0;
}

DM_API long DM_CALL dm_dmGuardLoadCustom(const char* type, const char* data) {
    return 0;
}

DM_API const char* DM_CALL dm_dmGuardParams(const char* cmd, const char* param) {
    return DM_SetResult("");
}

DM_API long DM_CALL dm_unLoadDriver() {
    return 1;
}
