// ============================================================================
// dm.h — 大漠插件 (dm.dll) 开源重新实现
// 接口完全兼容原版大漠插件，采用 C 风格导出，支持跨语言调用
// 编译要求: Windows SDK, GDI+, Tesseract (可选), ONNX Runtime (可选)
// ============================================================================
#pragma once
#ifndef DM_H
#define DM_H

#include <windows.h>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <functional>

// 导出/导入宏
#ifdef DM_EXPORTS
#define DM_API extern "C" __declspec(dllexport)
#else
#define DM_API extern "C" __declspec(dllimport)
#endif

// 调用约定 — 与原始 dm.dll 一致 (__stdcall)
#define DM_CALL __stdcall

// 最大常量
constexpr auto DM_MAX_PATH = 260;
constexpr auto DM_MAX_REGCODE = 64;
constexpr auto DM_MAX_VERINFO = 32;
constexpr auto DM_MAX_DICT = 256;
constexpr auto DM_MAX_PIC_NAME = 260;
constexpr auto DM_MAX_STR_LEN = 2048;

// ============================================================================
// 错误码 (与 GetLastError 返回值一致)
// ============================================================================
constexpr int DMERR_SUCCESS = 0;
constexpr int DMERR_NEED_REG = -1;
constexpr int DMERR_WINDOW_PROTECTED = -2;
constexpr int DMERR_WINDOW_ABNORMAL = -3;
constexpr int DMERR_EXCEPTION = -4;
constexpr int DMERR_NEED_RESTART = -5;
constexpr int DMERR_BLOCKED_BY_AV = -6;
constexpr int DMERR_PROCESS_PROTECTED = -8;
constexpr int DMERR_BIND_NOT_RELEASED = -13;
constexpr int DMERR_MISSING_DLL = -14;

// ============================================================================
// 内部状态结构 — 每个 dm 对象实例一份
// ============================================================================
struct DmState {
    // 基本设置
    std::wstring globalPath;
    long lastError;
    long objectId;
    long showErrorMsg;
    bool isRegistered;
    std::wstring regCode;
    std::wstring verInfo;

    // 窗口绑定状态
    HWND bindHwnd;
    bool isBound;
    std::string bindDisplay;
    std::string bindMouse;
    std::string bindKeypad;
    long bindMode;

    // 键鼠设置
    long simMode;
    long mouseDelay;
    long keypadDelay;
    long mouseSpeed;

    // 图色设置
    bool enableDisplayDebug;
    bool enableFindPicMultithread;
    bool enableGetColorByCapture;
    long findPicMultithreadCount;
    long findPicMultithreadLimit;
    std::string picPassword;
    std::wstring excludeRegion;
    std::wstring screenData;

    // 文字识别
    long curDict;
    std::map<long, std::wstring> dicts;
    std::map<long, std::wstring> dictPasswords;
    bool shareDict;
    long colGapNoDict;
    long rowGapNoDict;
    long minColGap;
    long minRowGap;
    long wordGap;
    long wordGapNoDict;
    long wordLineHeight;
    long wordLineHeightNoDict;
    bool exactOcr;

    // 内存
    HANDLE processHandle;

    // 汇编
    std::vector<BYTE> asmCode;
    long asmTimeout;
    bool asmHwndAsProcessId;
    bool showAsmErrorMsg;

    // AI
    bool aiEnableFindPic;

    // 后台设置
    bool enableBind;
    bool enableFakeActive;
    bool enableIme;
    bool enableKeypadMsg;
    bool enableKeypadPatch;
    bool enableKeypadSync;
    bool enableMouseMsg;
    bool enableMouseSync;
    bool enableRealKeypad;
    bool enableRealMouse;
    long realMouseDelay;
    long realMouseStep;
    long displayDelay;
    long displayRefreshDelay;
    bool enableSpeedDx;
    bool inputDm;
    long fps;
    bool aero;

    // 杂项
    bool exitThreadMode;
    CRITICAL_SECTION criSection;

    // 答题
    long faqHandle;

    // 防护盾
    bool guardEnabled;

    DmState() : lastError(0), objectId(0), showErrorMsg(1), isRegistered(false),
        bindHwnd(NULL), isBound(false), bindMode(0),
        simMode(0), mouseDelay(10), keypadDelay(10), mouseSpeed(0),
        enableDisplayDebug(false), enableFindPicMultithread(false),
        enableGetColorByCapture(false), findPicMultithreadCount(0),
        findPicMultithreadLimit(0), curDict(0),
        colGapNoDict(0), rowGapNoDict(0), minColGap(0), minRowGap(0),
        wordGap(0), wordGapNoDict(0), wordLineHeight(0), wordLineHeightNoDict(0),
        exactOcr(false), processHandle(NULL), asmTimeout(0),
        asmHwndAsProcessId(false), showAsmErrorMsg(true),
        aiEnableFindPic(false),
        enableBind(false), enableFakeActive(false), enableIme(false),
        enableKeypadMsg(false), enableKeypadPatch(false), enableKeypadSync(false),
        enableMouseMsg(false), enableMouseSync(false),
        enableRealKeypad(false), enableRealMouse(false),
        realMouseDelay(0), realMouseStep(0), displayDelay(0),
        displayRefreshDelay(0), enableSpeedDx(false), inputDm(false),
        fps(0), aero(false), exitThreadMode(0), faqHandle(0),
        guardEnabled(false) {
        InitializeCriticalSection(&criSection);
    }
    ~DmState() { DeleteCriticalSection(&criSection); }
};

// 获取线程本地状态
DmState* GetDmState();
void DM_SetLastError(long err);

// ============================================================================
// 模块 1: 基本设置
// ============================================================================
DM_API long DM_CALL reg(const char* reg_code, const char* ver_info);
DM_API long DM_CALL regEx(const char* reg_code, const char* ver_info, const char* ip);
DM_API long DM_CALL regNoMac(const char* reg_code, const char* ver_info);
DM_API long DM_CALL regExNoMac(const char* reg_code, const char* ver_info, const char* ip);
DM_API const char* DM_CALL ver();
DM_API long DM_CALL getID();
DM_API long DM_CALL getDmCount();
DM_API long DM_CALL getLastError();
DM_API const char* DM_CALL getPath();
DM_API long DM_CALL setPath(const char* path);
DM_API const char* DM_CALL getBasePath();
DM_API long DM_CALL setShowErrorMsg(long show);
DM_API long DM_CALL enablePicCache(long enable);

// ============================================================================
// 模块 2: 窗口
// ============================================================================
DM_API long DM_CALL findWindow(const char* cls, const char* title);
DM_API long DM_CALL findWindowEx(intptr_t parent, long cls, const char* title);
DM_API long DM_CALL findWindowByProcess(const char* process_name, const char* cls, const char* title);
DM_API long DM_CALL findWindowByProcessId(long process_id, const char* cls, const char* title);
DM_API long DM_CALL findWindowSuper(const char* cls, const char* title, long flag);
DM_API long DM_CALL enumWindow(intptr_t parent, const char* title, long filter);
DM_API long DM_CALL enumWindowByProcess(const char* process_name, const char* title, long filter);
DM_API long DM_CALL enumWindowByProcessId(long pid, const char* title, long filter);
DM_API long DM_CALL enumWindowSuper(const char* spec1, long flag1, const char* spec2, long flag2, long filter);
DM_API long DM_CALL enumProcess(const char* name);
DM_API long DM_CALL getWindow(intptr_t hwnd, long flag);
DM_API long DM_CALL getForegroundWindow();
DM_API long DM_CALL getForegroundFocus();
DM_API long DM_CALL getMousePointWindow();
DM_API long DM_CALL getPointWindow(long x, long y);
DM_API long DM_CALL getSpecialWindow(long flag);
DM_API const char* DM_CALL getWindowClass(intptr_t hwnd);
DM_API long DM_CALL getWindowProcessId(intptr_t hwnd);
DM_API const char* DM_CALL getWindowProcessPath(intptr_t hwnd);
DM_API long DM_CALL getWindowThreadId(intptr_t hwnd);
DM_API const char* DM_CALL getWindowTitle(intptr_t hwnd);
DM_API long DM_CALL getWindowRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
DM_API long DM_CALL getClientRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
DM_API long DM_CALL getClientSize(intptr_t hwnd, long* width, long* height);
DM_API long DM_CALL getWindowState(intptr_t hwnd, long flag);
DM_API long DM_CALL getProcessInfo(long pid, const char* type);
DM_API long DM_CALL moveWindow(intptr_t hwnd, long x, long y);
DM_API long DM_CALL setWindowSize(intptr_t hwnd, long width, long height);
DM_API long DM_CALL setWindowState(intptr_t hwnd, long flag);
DM_API long DM_CALL setWindowText(intptr_t hwnd, const char* title);
DM_API long DM_CALL setWindowTransparent(intptr_t hwnd, long trans);
DM_API long DM_CALL setClientSize(intptr_t hwnd, long width, long height);
DM_API long DM_CALL setSendStringDelay(long delay);
DM_API long DM_CALL sendPaste(intptr_t hwnd);
DM_API long DM_CALL sendString(intptr_t hwnd, const char* str);
DM_API long DM_CALL sendString2(intptr_t hwnd, const char* str);
DM_API long DM_CALL sendStringIme(intptr_t hwnd, const char* str);
DM_API long DM_CALL sendStringIme2(intptr_t hwnd, const char* str);
DM_API long DM_CALL clientToScreen(intptr_t hwnd, long* x, long* y);
DM_API long DM_CALL screenToClient(intptr_t hwnd, long* x, long* y);

// ============================================================================
// 模块 3: 键鼠
// ============================================================================
DM_API long DM_CALL keyDown(long vk_code);
DM_API long DM_CALL keyDownChar(const char* key_str);
DM_API long DM_CALL keyPress(long vk_code);
DM_API long DM_CALL keyPressChar(const char* key_str);
DM_API long DM_CALL keyPressStr(const char* key_str, long delay);
DM_API long DM_CALL keyUp(long vk_code);
DM_API long DM_CALL keyUpChar(const char* key_str);
DM_API long DM_CALL waitKey(long vk_code, long time_out);
DM_API long DM_CALL getKeyState(long vk_code);
DM_API long DM_CALL leftClick();
DM_API long DM_CALL leftDoubleClick();
DM_API long DM_CALL leftDown();
DM_API long DM_CALL leftUp();
DM_API long DM_CALL rightClick();
DM_API long DM_CALL rightDown();
DM_API long DM_CALL rightUp();
DM_API long DM_CALL middleClick();
DM_API long DM_CALL middleDown();
DM_API long DM_CALL middleUp();
DM_API long DM_CALL moveTo(long x, long y);
DM_API long DM_CALL moveToEx(long x, long y, long w, long h);
DM_API long DM_CALL moveR(long rx, long ry);
DM_API long DM_CALL wheelDown(long repeat);
DM_API long DM_CALL wheelUp(long repeat);
DM_API long DM_CALL getCursorPos(long* x, long* y);
DM_API long DM_CALL getCursorShape();
DM_API const char* DM_CALL getCursorShapeEx(long type);
DM_API long DM_CALL getCursorSpot();
DM_API long DM_CALL setMouseDelay(long type, long delay);
DM_API long DM_CALL setKeypadDelay(long type, long delay);
DM_API long DM_CALL setMouseSpeed(long speed);
DM_API long DM_CALL getMouseSpeed();
DM_API long DM_CALL enableMouseAccuracy(long enable);
DM_API long DM_CALL setSimMode(long mode);

// ============================================================================
// 模块 4: 图色
// ============================================================================
DM_API long DM_CALL capture(long x1, long y1, long x2, long y2, const char* file);
DM_API long DM_CALL captureGif(long x1, long y1, long x2, long y2, const char* file, long delay, long time);
DM_API long DM_CALL captureJpg(long x1, long y1, long x2, long y2, const char* file, long quality);
DM_API long DM_CALL capturePng(long x1, long y1, long x2, long y2, const char* file);
DM_API long DM_CALL capturePre(const char* file);
DM_API const char* DM_CALL getColor(long x, long y);
DM_API const char* DM_CALL getColorBGR(long x, long y);
DM_API const char* DM_CALL getColorHSV(long x, long y);
DM_API long DM_CALL getColorNum(long x1, long y1, long x2, long y2, const char* color, double sim);
DM_API const char* DM_CALL getAveRGB(long x1, long y1, long x2, long y2);
DM_API const char* DM_CALL getAveHSV(long x1, long y1, long x2, long y2);
DM_API long DM_CALL cmpColor(long x, long y, const char* color, double sim);
DM_API long DM_CALL findColor(long x1, long y1, long x2, long y2, const char* color, double sim, long dir, long* intX, long* intY);
DM_API long DM_CALL findColorEx(long x1, long y1, long x2, long y2, const char* color, double sim, long dir);
DM_API long DM_CALL findColorBlock(long x1, long y1, long x2, long y2, const char* color, double sim, long count, long width, long height, long* intX, long* intY);
DM_API long DM_CALL findColorBlockEx(long x1, long y1, long x2, long y2, const char* color, double sim, long count, long width, long height);
DM_API long DM_CALL findColorE(long x1, long y1, long x2, long y2, const char* color, double sim, long dir);
DM_API long DM_CALL findMulColor(long x1, long y1, long x2, long y2, const char* color, double sim);
DM_API long DM_CALL findMultiColor(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, double sim, long dir, long* intX, long* intY);
DM_API long DM_CALL findMultiColorEx(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, double sim, long dir);
DM_API long DM_CALL findMultiColorE(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, double sim, long dir);
DM_API long DM_CALL findPic(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL findPicE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir);
DM_API const char* DM_CALL findPicEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir);
DM_API const char* DM_CALL findPicExS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir);
DM_API long DM_CALL findPicS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir, long* intX, long* intY);
DM_API long DM_CALL findPicMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, double sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL findPicMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, double sim, long dir);
DM_API const char* DM_CALL findPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, double sim, long dir);
DM_API long DM_CALL findPicSim(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL findPicSimE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir);
DM_API const char* DM_CALL findPicSimEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, double sim, long dir);
DM_API long DM_CALL findPicSimMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, double sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL findPicSimMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, double sim, long dir);
DM_API const char* DM_CALL findPicSimMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, double sim, long dir);
DM_API long DM_CALL findShape(long x1, long y1, long x2, long y2, const char* offset_color, double sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL findShapeE(long x1, long y1, long x2, long y2, const char* offset_color, double sim, long dir);
DM_API const char* DM_CALL findShapeEx(long x1, long y1, long x2, long y2, const char* offset_color, double sim, long dir);
DM_API const char* DM_CALL getPicSize(const char* pic_name);
DM_API long DM_CALL freePic(const char* pic_name);
DM_API long DM_CALL loadPic(const char* pic_name);
DM_API long DM_CALL loadPicByte(const char* pic_name, long data, long size);
DM_API const char* DM_CALL appendPicAddr(const char* pic_info, long addr, long size);
DM_API const char* DM_CALL matchPicName(const char* pic_name);
DM_API long DM_CALL imageToBmp(const char* pic_name, const char* bmp_name);
DM_API long DM_CALL setPicPwd(const char* pwd);
DM_API long DM_CALL setExcludeRegion(long mode, const char* info);
DM_API long DM_CALL getScreenData(long x1, long y1, long x2, long y2);
DM_API const char* DM_CALL getScreenDataBmp(long x1, long y1, long x2, long y2, long* size);
DM_API long DM_CALL enableDisplayDebug(long enable_debug);
DM_API long DM_CALL enableFindPicMultithread(long enable);
DM_API long DM_CALL enableGetColorByCapture(long enable);
DM_API long DM_CALL setFindPicMultithreadCount(long count);
DM_API long DM_CALL setFindPicMultithreadLimit(long limit);
DM_API long DM_CALL bgr2RGB(const char* bgr_color);
DM_API const char* DM_CALL rgb2BGR(const char* rgb_color);
DM_API long DM_CALL isDisplayDead(long x1, long y1, long x2, long y2, long time);

// ============================================================================
// 模块 5: 后台设置
// ============================================================================
DM_API long DM_CALL bindWindow(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, long mode);
DM_API long DM_CALL bindWindowEx(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, const char* pub, long mode);
DM_API long DM_CALL unBindWindow();
DM_API long DM_CALL getBindWindow();
DM_API long DM_CALL isBind(intptr_t hwnd);
DM_API long DM_CALL forceUnBindWindow();
DM_API long DM_CALL setAero(long enable);
DM_API long DM_CALL lockInput(long enable);
DM_API long DM_CALL lockMouseRect(long x1, long y1, long x2, long y2);
DM_API long DM_CALL lockDisplay(long enable);
DM_API long DM_CALL enableBind(long enable);
DM_API long DM_CALL enableFakeActive(long enable);
DM_API long DM_CALL enableIme(long enable);
DM_API long DM_CALL enableKeypadMsg(long enable);
DM_API long DM_CALL enableKeypadPatch(long enable);
DM_API long DM_CALL enableKeypadSync(long enable);
DM_API long DM_CALL enableMouseMsg(long enable);
DM_API long DM_CALL enableMouseSync(long enable, long time_out);
DM_API long DM_CALL enableRealKeypad(long enable);
DM_API long DM_CALL enableRealMouse(long enable, long mousedelay, long mousestep);
DM_API long DM_CALL enableSpeedDx(long enable);
DM_API long DM_CALL getFps();
DM_API long DM_CALL setDisplayDelay(long delay);
DM_API long DM_CALL setDisplayRefreshDelay(long delay);
DM_API long DM_CALL setInputDm(long enable);
DM_API long DM_CALL hackSpeed(long rate);
DM_API long DM_CALL downCpu(long enable, long rate);
DM_API long DM_CALL switchBindWindow(intptr_t hwnd);

// ============================================================================
// 模块 6: 内存
// ============================================================================
DM_API long DM_CALL openProcess(long pid);
DM_API long DM_CALL getModuleBaseAddr(intptr_t hwnd, const char* module);
DM_API long DM_CALL getModuleSize(intptr_t hwnd, const char* module);
DM_API long DM_CALL getRemoteApiAddress(intptr_t hwnd, const char* module, const char* api);
DM_API long DM_CALL readInt(intptr_t hwnd, long addr, long* ret);
DM_API long DM_CALL readIntAddr(intptr_t hwnd, long addr, long type, long* ret);
DM_API long DM_CALL readFloat(intptr_t hwnd, long addr, float* ret);
DM_API long DM_CALL readFloatAddr(intptr_t hwnd, long addr, float* ret);
DM_API long DM_CALL readDouble(intptr_t hwnd, long addr, double* ret);
DM_API long DM_CALL readDoubleAddr(intptr_t hwnd, long addr, double* ret);
DM_API const char* DM_CALL readString(intptr_t hwnd, long addr, long type, long len);
DM_API const char* DM_CALL readStringAddr(intptr_t hwnd, long addr, long type, long len);
DM_API const char* DM_CALL readData(intptr_t hwnd, long addr, long len);
DM_API const char* DM_CALL readDataAddr(intptr_t hwnd, long addr, long len);
DM_API long DM_CALL readDataToBin(intptr_t hwnd, long addr, long len);
DM_API long DM_CALL readDataAddrToBin(intptr_t hwnd, long addr, long len);
DM_API const char* DM_CALL findInt(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max);
DM_API const char* DM_CALL findIntEx(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max, long step, long multi_thread, long mode);
DM_API const char* DM_CALL findFloat(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max);
DM_API const char* DM_CALL findFloatEx(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max, long step, long multi_thread, long mode);
DM_API const char* DM_CALL findDouble(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max);
DM_API const char* DM_CALL findDoubleEx(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max, long step, long multi_thread, long mode);
DM_API const char* DM_CALL findString(intptr_t hwnd, const char* addr_range, const char* string_value, long type);
DM_API const char* DM_CALL findStringEx(intptr_t hwnd, const char* addr_range, const char* string_value, long type, long step, long multi_thread, long mode);
DM_API const char* DM_CALL findData(intptr_t hwnd, const char* addr_range, const char* data);
DM_API const char* DM_CALL findDataEx(intptr_t hwnd, const char* addr_range, const char* data, long step, long multi_thread, long mode);
DM_API long DM_CALL writeInt(intptr_t hwnd, long addr, long type, long v);
DM_API long DM_CALL writeIntAddr(intptr_t hwnd, long addr, long type, long v);
DM_API long DM_CALL writeFloat(intptr_t hwnd, long addr, float v);
DM_API long DM_CALL writeFloatAddr(intptr_t hwnd, long addr, float v);
DM_API long DM_CALL writeDouble(intptr_t hwnd, long addr, double v);
DM_API long DM_CALL writeDoubleAddr(intptr_t hwnd, long addr, double v);
DM_API long DM_CALL writeString(intptr_t hwnd, long addr, long type, const char* v);
DM_API long DM_CALL writeStringAddr(intptr_t hwnd, long addr, long type, const char* v);
DM_API long DM_CALL writeData(intptr_t hwnd, long addr, const char* data);
DM_API long DM_CALL writeDataAddr(intptr_t hwnd, long addr, const char* data);
DM_API long DM_CALL writeDataFromBin(intptr_t hwnd, long addr, long data, long len);
DM_API long DM_CALL writeDataAddrFromBin(intptr_t hwnd, long addr, long data, long len);
DM_API long DM_CALL virtualAllocEx(intptr_t hwnd, long size, long type, long protect);
DM_API long DM_CALL virtualFreeEx(intptr_t hwnd, long addr);
DM_API long DM_CALL virtualProtectEx(intptr_t hwnd, long addr, long size, long type, long* old_protect);
DM_API const char* DM_CALL virtualQueryEx(intptr_t hwnd, long addr, long pmbi);
DM_API long DM_CALL freeProcessMemory(intptr_t hwnd);
DM_API const char* DM_CALL getCommandLine(intptr_t hwnd);
DM_API long DM_CALL terminateProcess(long pid);
DM_API long DM_CALL terminateProcessTree(long pid);
DM_API long DM_CALL setMemoryFindResultToFile(long enable);
DM_API long DM_CALL setMemoryHwndAsProcessId(long enable);
DM_API long DM_CALL setParam64ToPointer(long enable);
DM_API const char* DM_CALL int64ToInt32(long v_high, long v_low, long flag);
DM_API const char* DM_CALL intToData(long v, long type);
DM_API const char* DM_CALL floatToData(float v);
DM_API const char* DM_CALL doubleToData(double v);
DM_API const char* DM_CALL stringToData(const char* v, long type);

// ============================================================================
// 模块 7: 文件
// ============================================================================
DM_API long DM_CALL writeFile(const char* file, const char* content);
DM_API const char* DM_CALL readFile(const char* file);
DM_API long DM_CALL deleteFile(const char* file);
DM_API long DM_CALL copyFile(const char* src, const char* dest, long over);
DM_API long DM_CALL moveFile(const char* src, const char* dest);
DM_API long DM_CALL createFolder(const char* folder);
DM_API long DM_CALL deleteFolder(const char* folder);
DM_API long DM_CALL isFileExist(const char* file);
DM_API long DM_CALL isFolderExist(const char* folder);
DM_API long DM_CALL getFileLength(const char* file);
DM_API const char* DM_CALL getRealPath(const char* path);
DM_API const char* DM_CALL selectFile();
DM_API const char* DM_CALL selectDirectory();
DM_API long DM_CALL downloadFile(const char* url, const char* save_file, long timeout);
DM_API long DM_CALL encodeFile(const char* file, const char* pwd);
DM_API long DM_CALL decodeFile(const char* file, const char* pwd);
DM_API long DM_CALL writeIni(const char* section, const char* key, const char* value, const char* file);
DM_API const char* DM_CALL readIni(const char* section, const char* key, const char* file);
DM_API long DM_CALL deleteIni(const char* section, const char* key, const char* file);
DM_API const char* DM_CALL enumIniKey(const char* section, const char* file);
DM_API const char* DM_CALL enumIniSection(const char* file);
DM_API long DM_CALL writeIniPwd(const char* section, const char* key, const char* value, const char* file, const char* pwd);
DM_API const char* DM_CALL readIniPwd(const char* section, const char* key, const char* file, const char* pwd);
DM_API long DM_CALL deleteIniPwd(const char* section, const char* key, const char* file, const char* pwd);
DM_API const char* DM_CALL enumIniKeyPwd(const char* section, const char* file, const char* pwd);
DM_API const char* DM_CALL enumIniSectionPwd(const char* file, const char* pwd);

// ============================================================================
// 模块 8: 文字识别
// ============================================================================
DM_API long DM_CALL setDict(long index, const char* file);
DM_API long DM_CALL setDictMem(long index, const char* data, long size);
DM_API long DM_CALL setDictPwd(const char* pwd);
DM_API long DM_CALL useDict(long index);
DM_API const char* DM_CALL getDict(long index, long font_index);
DM_API long DM_CALL getDictCount(long index);
DM_API const char* DM_CALL getDictInfo(const char* file, long index);
DM_API long DM_CALL getNowDict();
DM_API long DM_CALL addDict(const char* dict_info);
DM_API long DM_CALL saveDict(long index, const char* file);
DM_API long DM_CALL clearDict(long index);
DM_API long DM_CALL enableShareDict(long enable);
DM_API const char* DM_CALL ocr(long x1, long y1, long x2, long y2, const char* color_format, double sim);
DM_API const char* DM_CALL ocrEx(long x1, long y1, long x2, long y2, const char* color_format, double sim);
DM_API const char* DM_CALL ocrExOne(long x1, long y1, long x2, long y2, const char* color_format, double sim);
DM_API long DM_CALL ocrInFile(long x1, long y1, long x2, long y2, const char* color_format, double sim, const char* file);
DM_API long DM_CALL findStr(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim, long* intX, long* intY);
DM_API const char* DM_CALL findStrE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim);
DM_API const char* DM_CALL findStrEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim);
DM_API long DM_CALL findStrS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim, long* intX, long* intY);
DM_API const char* DM_CALL findStrExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim);
DM_API long DM_CALL findStrFast(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim, long* intX, long* intY);
DM_API const char* DM_CALL findStrFastE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim);
DM_API const char* DM_CALL findStrFastEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim);
DM_API long DM_CALL findStrFastS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim, long* intX, long* intY);
DM_API const char* DM_CALL findStrFastExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim);
DM_API long DM_CALL findStrWithFont(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim, const char* font_name, long font_flag, long font_size, long* intX, long* intY);
DM_API const char* DM_CALL findStrWithFontE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim, const char* font_name, long font_flag, long font_size);
DM_API const char* DM_CALL findStrWithFontEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, double sim, const char* font_name, long font_flag, long font_size);
DM_API const char* DM_CALL getWords(long x1, long y1, long x2, long y2, const char* color, const char* words);
DM_API const char* DM_CALL getWordsNoDict(long x1, long y1, long x2, long y2, const char* color);
DM_API long DM_CALL fetchWord(long x1, long y1, long x2, long y2, const char* color, const char* words);
DM_API long DM_CALL getResultCount(const char* ret);
DM_API long DM_CALL getResultPos(const char* ret, long index, long* intX, long* intY);
DM_API long DM_CALL getWordResultCount(const char* str);
DM_API long DM_CALL getWordResultPos(const char* str, long index, long* intX, long* intY);
DM_API const char* DM_CALL getWordResultStr(const char* str, long index);
DM_API long DM_CALL setColGapNoDict(long col_gap);
DM_API long DM_CALL setRowGapNoDict(long row_gap);
DM_API long DM_CALL setWordGapNoDict(long word_gap);
DM_API long DM_CALL setWordLineHeightNoDict(long line_height);
DM_API long DM_CALL setExactOcr(long exact_ocr);
DM_API long DM_CALL setMinColGap(long min_col_gap);
DM_API long DM_CALL setMinRowGap(long min_row_gap);
DM_API long DM_CALL setWordGap(long word_gap);
DM_API long DM_CALL setWordLineHeight(long line_height);

// ============================================================================
// 模块 9: 系统
// ============================================================================
DM_API long DM_CALL beep(long f, long duration);
DM_API long DM_CALL delay(long mis);
DM_API long DM_CALL delays(long mis_min, long mis_max);
DM_API long DM_CALL runApp(const char* app_path, long mode);
DM_API long DM_CALL stop(long id);
DM_API long DM_CALL play(const char* media_file);
DM_API long DM_CALL exitOs(long type);
DM_API long DM_CALL setScreen(long width, long height, long depth);
DM_API long DM_CALL getScreenWidth();
DM_API long DM_CALL getScreenHeight();
DM_API long DM_CALL getScreenDepth();
DM_API long DM_CALL getDPI();
DM_API long DM_CALL getTime();
DM_API long DM_CALL getOsType();
DM_API long DM_CALL getOsBuildNumber();
DM_API long DM_CALL is64Bit();
DM_API long DM_CALL isSurrpotVt();
DM_API const char* DM_CALL getSystemInfo(long type, long method);
DM_API const char* DM_CALL getDir(long type);
DM_API long DM_CALL getLocale();
DM_API long DM_CALL getCpuType();
DM_API long DM_CALL getCpuUsage();
DM_API long DM_CALL getMemoryUsage();
DM_API const char* DM_CALL getDiskSerial();
DM_API const char* DM_CALL getDiskModel();
DM_API const char* DM_CALL getDiskReversion();
DM_API const char* DM_CALL getMachineCode();
DM_API const char* DM_CALL getMachineCodeNoMac();
DM_API const char* DM_CALL getNetTime();
DM_API const char* DM_CALL getNetTimeByIp(const char* ip);
DM_API const char* DM_CALL getNetTimeSafe(long time_out);
DM_API const char* DM_CALL getClipboard();
DM_API long DM_CALL setClipboard(const char* value);
DM_API long DM_CALL getDisplayInfo(long type);
DM_API long DM_CALL disableCloseDisplayAndSleep(long enable);
DM_API long DM_CALL disablePowerSave(long enable);
DM_API long DM_CALL disableScreenSave(long enable);
DM_API long DM_CALL disableFontSmooth();
DM_API long DM_CALL enableFontSmooth();
DM_API long DM_CALL checkFontSmooth();
DM_API long DM_CALL checkUAC();
DM_API long DM_CALL setUAC(long enable);
DM_API long DM_CALL setDisplayAcceler(long flag);
DM_API long DM_CALL showTaskBarIcon(intptr_t hwnd, long show);

// ============================================================================
// 模块 10: 杂项
// ============================================================================
DM_API long DM_CALL enterCri();
DM_API long DM_CALL leaveCri();
DM_API long DM_CALL initCri();
DM_API long DM_CALL releaseRef();
DM_API long DM_CALL setExitThread(long mode);
DM_API const char* DM_CALL executeCmd(const char* cmd, long time_out);
DM_API long DM_CALL activeInputMethod(intptr_t hwnd, const char* input_method);
DM_API long DM_CALL checkInputMethod(intptr_t hwnd, const char* input_method);
DM_API long DM_CALL findInputMethod(const char* input_method);

// ============================================================================
// 模块 11: 汇编
// ============================================================================
DM_API long DM_CALL asmAdd(const char* asm_ins);
DM_API long DM_CALL asmCall(intptr_t hwnd, long mode);
DM_API long DM_CALL asmCallEx(intptr_t hwnd, long mode, long base_addr, long call_addr);
DM_API long DM_CALL asmClear();
DM_API long DM_CALL asmSetTimeout(long timeout);
DM_API const char* DM_CALL assemble(long base_addr, long is_asm_code);
DM_API const char* DM_CALL disAssemble(long asm_code, long base_addr, long is_asm_code);
DM_API long DM_CALL setAsmHwndAsProcessId(long enable);
DM_API long DM_CALL setShowAsmErrorMsg(long show);

// ============================================================================
// 模块 12: AI
// ============================================================================
DM_API long DM_CALL loadAi(const char* file);
DM_API long DM_CALL loadAiMemory(long data, long size);
DM_API long DM_CALL aiEnableFindPicWindow(long enable);
DM_API long DM_CALL aiFindPic(long x1, long y1, long x2, long y2, const char* pic_name, double sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL aiFindPicEx(long x1, long y1, long x2, long y2, const char* pic_name, double sim, long dir);
DM_API long DM_CALL aiFindPicMem(long x1, long y1, long x2, long y2, const char* pic_info, double sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL aiFindPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, double sim, long dir);
DM_API const char* DM_CALL aiYoloDetectObjects(long x1, long y1, long x2, long y2, float prob, float iou);
DM_API long DM_CALL aiYoloDetectObjectsToDataBmp(long x1, long y1, long x2, long y2, float prob, float iou, long data, long size, long mode);
DM_API long DM_CALL aiYoloDetectObjectsToFile(long x1, long y1, long x2, long y2, float prob, float iou, const char* file, long mode);
DM_API long DM_CALL aiYoloSetModel(const char* model_file, const char* model_type);
DM_API long DM_CALL aiYoloSetModelMemory(long data, long size, const char* model_type);
DM_API long DM_CALL aiYoloFreeModel();
DM_API long DM_CALL aiYoloUseModel(long index);
DM_API long DM_CALL aiYoloSetVersion(const char* version);
DM_API const char* DM_CALL aiYoloObjectsToString(const char* objects);
DM_API const char* DM_CALL aiYoloSortsObjects(const char* objects, long sort_type);

// ============================================================================
// 模块 13: Foobar (界面控件)
// ============================================================================
DM_API long DM_CALL createFoobarRect(intptr_t hwnd, long x, long y, long w, long h);
DM_API long DM_CALL createFoobarEllipse(intptr_t hwnd, long x, long y, long w, long h);
DM_API long DM_CALL createFoobarRoundRect(intptr_t hwnd, long x, long y, long w, long h, long rw, long rh);
DM_API long DM_CALL createFoobarCustom(intptr_t hwnd, long x, long y, long w, long h, const char* pic);
DM_API long DM_CALL foobarClose(intptr_t hwnd);
DM_API long DM_CALL foobarLock(intptr_t hwnd);
DM_API long DM_CALL foobarUnlock(intptr_t hwnd);
DM_API long DM_CALL foobarUpdate(intptr_t hwnd);
DM_API long DM_CALL foobarSetFont(intptr_t hwnd, const char* font_name, long size, long flag);
DM_API long DM_CALL foobarSetSave(intptr_t hwnd, const char* file, long enable);
DM_API long DM_CALL foobarSetTrans(intptr_t hwnd, long trans, long color);
DM_API long DM_CALL foobarDrawText(intptr_t hwnd, long x, long y, long w, long h, const char* text, long color, long align);
DM_API long DM_CALL foobarPrintText(intptr_t hwnd, const char* text, long color);
DM_API long DM_CALL foobarDrawRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
DM_API long DM_CALL foobarDrawLine(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
DM_API long DM_CALL foobarFillRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color);
DM_API long DM_CALL foobarDrawPic(intptr_t hwnd, long x, long y, const char* pic);
DM_API long DM_CALL foobarClearText(intptr_t hwnd);
DM_API long DM_CALL foobarTextRect(intptr_t hwnd, long x, long y, long w, long h);
DM_API long DM_CALL foobarTextLineGap(intptr_t hwnd, long line_gap);
DM_API long DM_CALL foobarTextPrintDir(intptr_t hwnd, long dir);
DM_API long DM_CALL foobarStartGif(intptr_t hwnd, const char* pic, long x, long y);
DM_API long DM_CALL foobarStopGif(intptr_t hwnd);

// ============================================================================
// 模块 14: 答题
// ============================================================================
DM_API long DM_CALL faqCapture(long x1, long y1, long x2, long y2, long quality, long delay, long time);
DM_API long DM_CALL faqCaptureFromFile(const char* file, long quality, long delay, long time);
DM_API const char* DM_CALL faqCaptureString(long x1, long y1, long x2, long y2, long quality, long delay, long time);
DM_API long DM_CALL faqGetSize(long handle);
DM_API long DM_CALL faqPost(const char* server, long handle, long request_type, long time_out);
DM_API long DM_CALL faqSend(long handle, long time_out);
DM_API long DM_CALL faqFetch(long time_out);
DM_API long DM_CALL faqCancel();
DM_API long DM_CALL faqIsPosted();

// ============================================================================
// 模块 15: 算法
// ============================================================================
DM_API const char* DM_CALL excludePos(const char* all_pos, long type, long x1, long y1, long x2, long y2);
DM_API const char* DM_CALL findNearestPos(const char* all_pos, long type, long x, long y);
DM_API const char* DM_CALL sortPosDistance(const char* all_pos, long type, long x, long y);

// ============================================================================
// 模块 16: 防护盾
// ============================================================================
DM_API long DM_CALL dmGuard(long enable, const char* type);
DM_API long DM_CALL dmGuardExtract(const char* type, const char* file);
DM_API long DM_CALL dmGuardLoadCustom(const char* type, const char* data);
DM_API const char* DM_CALL dmGuardParams(const char* cmd, const char* param);
DM_API long DM_CALL unLoadDriver();

// ============================================================================
// 帮助函数 — 内部字符串管理
// ============================================================================
// 返回线程安全的静态缓冲区（每个函数返回的 const char* 指向此缓冲区）
const char* DM_SetResult(const std::string& s);
const char* DM_SetResult(const char* s);

// ============================================================================
// COM 支持 — 状态切换
// ============================================================================
// 设置 COM 对象调用的状态覆盖，使 GetDmState() 返回 COM 对象私有状态
void setComState(DmState* s);

#endif // DM_H
