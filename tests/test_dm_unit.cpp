// tests/test_dm_unit.cpp
// 行为单元测试（自包含，无第三方框架）：针对无桌面/进程/OCR/AI 模型依赖、
// 可在无头环境安全调用的 dm_* 函数，覆盖 正常输入 / 边界条件 / 异常情况。
//
// dm_com.cpp 内部辅助函数位于匿名命名空间，无法从外部链接，因此本测试通过
// 公开 C API（LoadLibrary + GetProcAddress）间接覆盖其入口与错误路径；
// 纯逻辑（参数提取、坐标拼接、分发路由）的错误路径由 test_dm_all 冒烟测试
// 与本报告统一覆盖。
#include <windows.h>
#include <cstring>
#include <cstdio>
#include <cstdint>

static HMODULE g_h = nullptr;
static int g_pass = 0, g_fail = 0;

static void check(const char* name, bool cond) {
    if (cond) { g_pass++; printf("  [PASS] %s\n", name); }
    else { g_fail++; printf("  [FAIL] %s\n", name); }
}

// SEH 守护：在 __try 中执行 f，返回 true 表示未发生访问违规（未崩溃）
template <typename F>
static bool safe(F&& f) {
    bool ok = true;
    __try { f(); }
    __except (EXCEPTION_EXECUTE_HANDLER) { ok = false; }
    return ok;
}

// ---- 函数指针类型 ----
typedef const char* (*F_ver)();
typedef long (*F_long0)();
typedef long (*F_long1)(long);
typedef long (*F_long2)(long, long);
typedef long (*F_str1)(const char*);
typedef long (*F_str2)(const char*, const char*);
typedef const char* (*F_str_ret0)();
typedef long (*F_rect)(long, long*, long*, long*, long*);
typedef long (*F_clientsize)(long, long*, long*);
typedef const char* (*F_getstr1)(long);
typedef long (*F_bind)(long, const char*, const char*, const char*, long);
typedef const char* (*F_readfile)(const char*);
typedef long (*F_writefile)(const char*, const char*);
typedef long (*F_fileexist)(const char*);
typedef long (*F_delfile)(const char*);
typedef long (*F_createfolder)(const char*);
typedef long (*F_delfolder)(const char*);
typedef long (*F_folder)(const char*);
typedef long (*F_setclip)(const char*);
typedef const char* (*F_getclip)();
typedef long (*F_execcmd)(const char*, long);
typedef long (*F_findwndex)(intptr_t, long, const char*);
typedef long (*F_readintaddr)(intptr_t, long, long, long*);
typedef long (*F_delay)(long);

static F_ver        dm_ver;
static F_long0      dm_getID, dm_getLastError, dm_getScreenWidth, dm_getScreenHeight,
                    dm_getTime, dm_getDmCount, dm_unBindWindow, dm_isBind, dm_getBindWindow;
static F_long1      dm_setShowErrorMsg, dm_enablePicCache, dm_getWindowProcessId,
                    dm_getWindowThreadId, dm_getWindowState;
static F_long2      dm_moveTo, dm_keyPress;
static F_str1       dm_setPath;
static F_str2       dm_reg, dm_findWindow;
static F_str_ret0   dm_getPath, dm_getBasePath, dm_getMachineCode;
static F_rect       dm_getWindowRect;
static F_clientsize dm_getClientSize;
static F_getstr1    dm_getWindowTitle, dm_getWindowClass;
static F_bind       dm_bindWindow;
static F_readfile   dm_readFile;
static F_writefile  dm_writeFile;
static F_fileexist  dm_isFileExist;
static F_delfile    dm_deleteFile;
static F_createfolder dm_createFolder;
static F_delfolder  dm_deleteFolder;
static F_folder     dm_isFolderExist;
static F_setclip    dm_setClipboard;
static F_getclip    dm_getClipboard;
static F_execcmd    dm_executeCmd;
static F_findwndex  dm_findWindowEx;
static F_readintaddr dm_readIntAddr;
static F_delay      dm_delay;

#define BIND(name, type) (type)GetProcAddress(g_h, "dm_" #name)

static void setup() {
    g_h = LoadLibraryA("D:\\dm\\dm_hook\\build\\dm_hook.dll");
    dm_ver = BIND(ver, F_ver);
    dm_getID = BIND(getID, F_long0);
    dm_getLastError = BIND(getLastError, F_long0);
    dm_getScreenWidth = BIND(getScreenWidth, F_long0);
    dm_getScreenHeight = BIND(getScreenHeight, F_long0);
    dm_getTime = BIND(getTime, F_long0);
    dm_getDmCount = BIND(getDmCount, F_long0);
    dm_unBindWindow = BIND(unBindWindow, F_long0);
    dm_isBind = BIND(isBind, F_long0);
    dm_getBindWindow = BIND(getBindWindow, F_long0);
    dm_setShowErrorMsg = BIND(setShowErrorMsg, F_long1);
    dm_enablePicCache = BIND(enablePicCache, F_long1);
    dm_getWindowProcessId = BIND(getWindowProcessId, F_long1);
    dm_getWindowThreadId = BIND(getWindowThreadId, F_long1);
    dm_getWindowState = BIND(getWindowState, F_long1);
    dm_moveTo = BIND(moveTo, F_long2);
    dm_keyPress = BIND(keyPress, F_long2);
    dm_setPath = BIND(setPath, F_str1);
    dm_reg = BIND(reg, F_str2);
    dm_findWindow = BIND(findWindow, F_str2);
    dm_getPath = BIND(getPath, F_str_ret0);
    dm_getBasePath = BIND(getBasePath, F_str_ret0);
    dm_getMachineCode = BIND(getMachineCode, F_str_ret0);
    dm_getWindowRect = BIND(getWindowRect, F_rect);
    dm_getClientSize = BIND(getClientSize, F_clientsize);
    dm_getWindowTitle = BIND(getWindowTitle, F_getstr1);
    dm_getWindowClass = BIND(getWindowClass, F_getstr1);
    dm_bindWindow = BIND(bindWindow, F_bind);
    dm_readFile = BIND(readFile, F_readfile);
    dm_writeFile = BIND(writeFile, F_writefile);
    dm_isFileExist = BIND(isFileExist, F_fileexist);
    dm_deleteFile = BIND(deleteFile, F_delfile);
    dm_createFolder = BIND(createFolder, F_createfolder);
    dm_deleteFolder = BIND(deleteFolder, F_delfolder);
    dm_isFolderExist = BIND(isFolderExist, F_folder);
    dm_setClipboard = BIND(setClipboard, F_setclip);
    dm_getClipboard = BIND(getClipboard, F_getclip);
    dm_executeCmd = BIND(executeCmd, F_execcmd);
    dm_findWindowEx = BIND(findWindowEx, F_findwndex);
    dm_readIntAddr = BIND(readIntAddr, F_readintaddr);
    dm_delay = BIND(delay, F_delay);
}

int main() {
    setup();
    if (!g_h) {
        fprintf(stderr, "LoadLibrary failed: %lu\n", GetLastError());
        return 1;
    }

    printf("===== 正常输入 (Normal) =====\n");
    check("dm_ver 非空", dm_ver && dm_ver()[0] != '\0');
    check("getLastError 初始为0", dm_getLastError() == 0);
    {
        long w = dm_getScreenWidth(), h = dm_getScreenHeight();
        check("getScreenWidth/Height > 0", w > 0 && h > 0);
    }
    check("getTime > 1700000000", dm_getTime() > 1700000000);
    check("getDmCount == 1", dm_getDmCount() == 1);
    check("setShowErrorMsg(1)==1", dm_setShowErrorMsg(1) == 1);
    check("setShowErrorMsg(0)==1", dm_setShowErrorMsg(0) == 1);
    check("enablePicCache(1)==1", dm_enablePicCache(1) == 1);
    check("enablePicCache(0)==1", dm_enablePicCache(0) == 1);
    {
        const char* tf = "dm_ut_temp.txt";
        const char* content = "dm_hook_ut";
        check("writeFile==1", dm_writeFile(tf, content) == 1);
        check("isFileExist==1", dm_isFileExist(tf) == 1);
        const char* r = dm_readFile(tf);
        check("readFile 非空", r != nullptr);
        check("readFile 内容一致", r && strcmp(r, content) == 0);
        check("deleteFile==1", dm_deleteFile(tf) == 1);
        check("isFileExist==0(删除后)", dm_isFileExist(tf) == 0);
    }
    {
        check("createFolder==1", dm_createFolder("dm_ut_dir") == 1);
        check("isFolderExist==1", dm_isFolderExist("dm_ut_dir") == 1);
        check("deleteFolder==1", dm_deleteFolder("dm_ut_dir") == 1);
        check("isFolderExist==0", dm_isFolderExist("dm_ut_dir") == 0);
    }
    {
        check("setClipboard==1", dm_setClipboard("dm_ut_clip") == 1);
        const char* c = dm_getClipboard();
        check("getClipboard 非空", c != nullptr);
        check("clipboard 内容一致", c && strcmp(c, "dm_ut_clip") == 0);
    }
    check("delay(10)==1", dm_delay(10) == 1);
    check("setPath==1", dm_setPath("C:\\temp") == 1);
    {
        const char* p = dm_getPath();
        check("getPath 非空", p && p[0] != '\0');
    }

    printf("\n===== 边界条件 (Boundary) =====\n");
    check("getWindowRect(0,NULL) 不崩溃",
          safe([] { long a=0,b=0,c=0,d=0; dm_getWindowRect(0,&a,&b,&c,&d); }));
    check("findWindowEx(0,0,nullptr) 不崩溃",
          safe([] { dm_findWindowEx(0, 0, nullptr); }));
    check("readIntAddr(0,0,0,&out) 不崩溃",
          safe([] { long out=0; dm_readIntAddr(0,0,0,&out); }));
    check("getWindowProcessId(0) 不崩溃",
          safe([] { dm_getWindowProcessId(0); }));
    check("moveTo(-100000,-100000) 不崩溃",
          safe([] { dm_moveTo(-100000, -100000); }));

    printf("\n===== 异常情况 (Exception / Error path) =====\n");
    check("readFile(不存在文件) 不崩溃",
          safe([] { dm_readFile("dm_ut_not_exist_xyz.txt"); }));
    check("deleteFile(不存在文件) 不崩溃",
          safe([] { dm_deleteFile("dm_ut_not_exist_xyz.txt"); }));
    check("executeCmd(\"\") 不崩溃",
          safe([] { dm_executeCmd("", 1000); }));
    check("getWindowTitle(0) 不崩溃",
          safe([] { dm_getWindowTitle(0); }));
    check("unBindWindow() 不崩溃(无绑定)",
          safe([] { dm_unBindWindow(); }));

    printf("\n========================================\n");
    printf("  PASSED: %d   FAILED: %d\n", g_pass, g_fail);
    printf("========================================\n");
    FreeLibrary(g_h);
    return g_fail == 0 ? 0 : 1;
}
