// test_staged.cpp — 逐步测试 dm_hook.dll 各模块
// 发现问题模块后停止，便于定位崩溃原因
#include <windows.h>
#include <cstdio>
#include <cstring>

int g_passed = 0, g_failed = 0;

#define TEST(name, expr) do { \
    if (expr) { g_passed++; printf("  [PASS] %s\n", name); } \
    else { g_failed++; printf("  [FAIL] %s\n", name); } \
} while(0)

#define GET(name) ((decltype(&name))GetProcAddress(hDll, "dm_" #name))

int main() {
    // 禁用 stdout 缓冲，确保实时输出（便于定位崩溃点）
    setvbuf(stdout, NULL, _IONBF, 0);

    HMODULE hDll = LoadLibraryA("..\\build\\dm_hook_mingw.dll");
    if (!hDll) hDll = LoadLibraryA("..\\build\\dm_hook.dll");
    if (!hDll) {
        printf("LoadLibrary failed: %lu\n", GetLastError());
        return 1;
    }
    printf("[OK] DLL loaded: %p\n\n", hDll);

    // ===== Module 1: 基本设置 =====
    printf("===== Module 1: 基本设置 =====\n");
    {
        auto ver = (const char* (*)())GetProcAddress(hDll, "dm_ver");
        auto getID = (long (*)())GetProcAddress(hDll, "dm_getID");
        auto getLastErr = (long (*)())GetProcAddress(hDll, "dm_getLastError");
        auto reg = (long (*)(const char*, const char*))GetProcAddress(hDll, "dm_reg");
        auto setPath = (long (*)(const char*))GetProcAddress(hDll, "dm_setPath");
        auto getPath = (const char* (*)())GetProcAddress(hDll, "dm_getPath");
        auto getBasePath = (const char* (*)())GetProcAddress(hDll, "dm_getBasePath");
        auto getMachineCode = (const char* (*)())GetProcAddress(hDll, "dm_getMachineCode");
        auto getDmCount = (long (*)())GetProcAddress(hDll, "dm_getDmCount");
        auto setShowErr = (long (*)(long))GetProcAddress(hDll, "dm_setShowErrorMsg");
        auto enablePicCache = (long (*)(long))GetProcAddress(hDll, "dm_enablePicCache");

        if (!ver) { printf("[FAIL] dm_ver not found\n"); return 1; }
        printf("    dm_ver() = \"%s\"\n", ver());
        TEST("getID() > 0", getID() > 0);
        TEST("getLastError() == 0", getLastErr() == 0);
        TEST("reg() == 1", reg("test", "3.1233") == 1);
        TEST("setPath() == 1", setPath("C:\\temp") == 1);
        TEST("getPath() != null", getPath() && getPath()[0]);
        TEST("getBasePath() != null", getBasePath() && getBasePath()[0]);
        TEST("getMachineCode() != null", getMachineCode() && getMachineCode()[0]);
        TEST("getDmCount() == 1", getDmCount() == 1);
        TEST("setShowErrorMsg() == 1", setShowErr(1) == 1);
        TEST("enablePicCache() == 1", enablePicCache(1) == 1);
    }
    printf("  Module 1 done. Passing: %d, Failed: %d\n", g_passed, g_failed);

    // ===== Module 2: 窗口 =====
    printf("\n===== Module 2: 窗口 =====\n");
    {
        using GetLong = long (*)(long);
        using GetLongStr = const char* (*)(long);
        using GetRect = long (*)(long, long*, long*, long*, long*);
        using GetClientSize = long (*)(long, long*, long*);

        auto getFG = (long (*)())GetProcAddress(hDll, "dm_getForegroundWindow");
        auto getTitle = (GetLongStr)GetProcAddress(hDll, "dm_getWindowTitle");
        auto getClass = (GetLongStr)GetProcAddress(hDll, "dm_getWindowClass");
        auto getPid = (GetLong)GetProcAddress(hDll, "dm_getWindowProcessId");
        auto getTid = (GetLong)GetProcAddress(hDll, "dm_getWindowThreadId");
        auto getRect = (GetRect)GetProcAddress(hDll, "dm_getWindowRect");
        auto getClientSize = (GetClientSize)GetProcAddress(hDll, "dm_getClientSize");
        auto getWin = (GetLong)GetProcAddress(hDll, "dm_getWindow");
        auto findWnd = (long (*)(const char*, const char*))GetProcAddress(hDll, "dm_findWindow");
        auto getState = (long (*)(long, long))GetProcAddress(hDll, "dm_getWindowState");
        auto getSpecial = (long (*)(long))GetProcAddress(hDll, "dm_getSpecialWindow");

        if (!getFG) { printf("[FAIL] getForegroundWindow not found\n"); return 1; }
        long fg = getFG();
        TEST("getForegroundWindow() != 0", fg != 0);
        printf("    fg = %ld\n", fg);

        if (fg && getTitle) {
            const char* t = getTitle(fg);
            TEST("getWindowTitle() != null", t != nullptr);
            printf("    title = \"%s\"\n", t ? t : "(null)");
        }

        if (fg && getClass) {
            const char* c = getClass(fg);
            TEST("getWindowClass() != null", c != nullptr);
            printf("    class = \"%s\"\n", c ? c : "(null)");
        }

        if (getPid && fg) {
            long pid = getPid(fg);
            TEST("getWindowProcessId() > 0", pid > 0);
            printf("    pid = %ld\n", pid);
        }

        if (getTid && fg) {
            long tid = getTid(fg);
            TEST("getWindowThreadId() > 0", tid > 0);
            printf("    tid = %ld\n", tid);
        }

        if (getRect && fg) {
            long x1=0,y1=0,x2=0,y2=0;
            long r = getRect(fg, &x1, &y1, &x2, &y2);
            TEST("getWindowRect() == 1", r == 1);
            printf("    rect = %ld,%ld-%ld,%ld\n", x1, y1, x2, y2);
        }

        if (getClientSize && fg) {
            long w=0,h=0;
            long r = getClientSize(fg, &w, &h);
            TEST("getClientSize() == 1", r == 1);
            printf("    client = %ld x %ld\n", w, h);
        }

        if (getSpecial) {
            long desk = getSpecial(0);
            TEST("getSpecialWindow(0) != 0", desk != 0);
            printf("    desktop = %ld\n", desk);
        }

        if (getState && fg) {
            long vis = getState(fg, 0);
            printf("    isVisible = %ld\n", vis);
        }
    }
    printf("  Module 2 done. Passing: %d, Failed: %d\n", g_passed, g_failed);

    // ===== Module 3: 键鼠 =====
    printf("\n===== Module 3: 键鼠 =====\n");
    {
        auto moveTo = (long (*)(long, long))GetProcAddress(hDll, "dm_moveTo");
        auto leftClick = (long (*)())GetProcAddress(hDll, "dm_leftClick");
        auto keyPress = (long (*)(long))GetProcAddress(hDll, "dm_keyPress");
        auto getCursorPos = (long (*)(long*, long*))GetProcAddress(hDll, "dm_getCursorPos");

        if (moveTo) TEST("moveTo(100,100) == 1", moveTo(100, 100) == 1);
        if (leftClick) TEST("leftClick() == 1", leftClick() == 1);
        if (keyPress) TEST("keyPress(VK_RETURN) == 1", keyPress(VK_RETURN) == 1);
        if (getCursorPos) {
            long x=0, y=0;
            TEST("getCursorPos() == 1", getCursorPos(&x, &y) == 1);
            printf("    cursor = %ld,%ld\n", x, y);
        }
    }
    printf("  Module 3 done. Passing: %d, Failed: %d\n", g_passed, g_failed);

    // ===== Module 4: 图色 =====
    printf("\n===== Module 4: 图色 =====\n");
    {
        auto getColor = (const char* (*)(long, long))GetProcAddress(hDll, "dm_getColor");
        auto getColorBGR = (const char* (*)(long, long))GetProcAddress(hDll, "dm_getColorBGR");
        auto getColorHSV = (const char* (*)(long, long))GetProcAddress(hDll, "dm_getColorHSV");
        auto cmpColor = (long (*)(long, long, const char*, long))GetProcAddress(hDll, "dm_cmpColor");
        auto findColor = (long (*)(long, long, long, long, const char*, long, long, long*, long*))GetProcAddress(hDll, "dm_findColor");
        auto getAveRGB = (const char* (*)(long, long, long, long))GetProcAddress(hDll, "dm_getAveRGB");
        auto getColorNum = (long (*)(long, long, long, long, const char*, long))GetProcAddress(hDll, "dm_getColorNum");
        auto capture = (long (*)(long, long, long, long, const char*))GetProcAddress(hDll, "dm_capture");
        auto capturePng = (long (*)(long, long, long, long, const char*))GetProcAddress(hDll, "dm_capturePng");

        if (getColor) {
            const char* c = getColor(0, 0);
            TEST("getColor(0,0) != null", c && strlen(c) >= 6);
            printf("    color(0,0) = %s\n", c ? c : "(null)");
        }
        if (getColorBGR) {
            const char* c = getColorBGR(0, 0);
            TEST("getColorBGR(0,0) != null", c && strlen(c) >= 6);
        }
        if (getColorHSV) {
            const char* c = getColorHSV(0, 0);
            TEST("getColorHSV(0,0) != null", c && c[0]);
            printf("    hsv(0,0) = %s\n", c ? c : "(null)");
        }
        if (getColor && cmpColor) {
            const char* c = getColor(0, 0);
            TEST("cmpColor() == 1", cmpColor(0, 0, c, 100) == 1);
        }
        if (findColor) {
            long fx=0, fy=0;
            long found = findColor(0, 0, 0, 0, "000000", 0, 0, &fx, &fy);
            printf("    findColor(0,0,0,0,'000000') = %ld\n", found);
        }
        if (getAveRGB) {
            const char* avg = getAveRGB(0, 0, 1, 1);
            TEST("getAveRGB() != null", avg && strlen(avg) >= 6);
        }
        if (capture) {
            long r = capture(0, 0, 100, 100, "dm_test_cap.bmp");
            TEST("capture() == 1", r == 1);
            if (r == 1) DeleteFileA("dm_test_cap.bmp");
        }
        if (capturePng) {
            long r = capturePng(0, 0, 100, 100, "dm_test_cap.png");
            TEST("capturePng() == 1", r == 1);
            if (r == 1) DeleteFileA("dm_test_cap.png");
        }
    }
    printf("  Module 4 done. Passing: %d, Failed: %d\n", g_passed, g_failed);

    // ===== Module 5: 后台设置 =====
    printf("\n===== Module 5: 后台设置 =====\n");
    {
        auto getFG = (long (*)())GetProcAddress(hDll, "dm_getForegroundWindow");
        auto bindWnd = (long (*)(long, const char*, const char*, const char*, long))GetProcAddress(hDll, "dm_bindWindow");
        auto unbind = (long (*)())GetProcAddress(hDll, "dm_unBindWindow");
        auto isBind = (long (*)(long))GetProcAddress(hDll, "dm_isBind");
        auto getBind = (long (*)())GetProcAddress(hDll, "dm_getBindWindow");
        auto forceUnbind = (long (*)())GetProcAddress(hDll, "dm_forceUnBindWindow");

        if (getFG && bindWnd && unbind) {
            long fg = getFG();
            TEST("bindWindow() == 1", bindWnd(fg, "normal", "normal", "normal", 0) == 1);
            if (isBind) TEST("isBind() == 1", isBind(fg) == 1);
            if (getBind) TEST("getBindWindow() == fg", getBind() == fg);
            TEST("unBindWindow() == 1", unbind() == 1);
            if (isBind) TEST("isBind() == 0 (after unbind)", isBind(fg) == 0);
            if (forceUnbind) TEST("forceUnBindWindow() == 1", forceUnbind() == 1);
        }
    }
    printf("  Module 5 done. Passing: %d, Failed: %d\n", g_passed, g_failed);

    // ===== Module 7: 文件 =====
    printf("\n===== Module 7: 文件 =====\n");
    {
        auto writeFile = (long (*)(const char*, const char*))GetProcAddress(hDll, "dm_writeFile");
        auto readFile = (const char* (*)(const char*))GetProcAddress(hDll, "dm_readFile");
        auto isExist = (long (*)(const char*))GetProcAddress(hDll, "dm_isFileExist");
        auto delFile = (long (*)(const char*))GetProcAddress(hDll, "dm_deleteFile");
        auto getLen = (long (*)(const char*))GetProcAddress(hDll, "dm_getFileLength");
        auto createFolder = (long (*)(const char*))GetProcAddress(hDll, "dm_createFolder");
        auto delFolder = (long (*)(const char*))GetProcAddress(hDll, "dm_deleteFolder");
        auto isFolder = (long (*)(const char*))GetProcAddress(hDll, "dm_isFolderExist");
        auto copyFile = (long (*)(const char*, const char*, long))GetProcAddress(hDll, "dm_copyFile");
        auto moveFile = (long (*)(const char*, const char*))GetProcAddress(hDll, "dm_moveFile");

        if (writeFile && readFile && isExist && delFile) {
            const char* testFile = "dm_test_temp.txt";
            const char* content = "Hello dm_hook!";
            TEST("writeFile() == 1", writeFile(testFile, content) == 1);
            TEST("isFileExist() == 1", isExist(testFile) == 1);
            const char* read = readFile(testFile);
            TEST("readFile() != null", read != nullptr);
            if (read) TEST("readFile() content match", strcmp(read, content) == 0);
            if (getLen) TEST("getFileLength() > 0", getLen(testFile) > 0);
            TEST("deleteFile() == 1", delFile(testFile) == 1);
            TEST("isFileExist() == 0 (after delete)", isExist(testFile) == 0);
        }
        if (createFolder && delFolder && isFolder) {
            TEST("createFolder() == 1", createFolder("dm_test_dir") == 1);
            TEST("isFolderExist() == 1", isFolder("dm_test_dir") == 1);
            TEST("deleteFolder() == 1", delFolder("dm_test_dir") == 1);
            TEST("isFolderExist() == 0", isFolder("dm_test_dir") == 0);
        }
        if (copyFile && writeFile && isExist && delFile) {
            writeFile("dm_src.txt", "copy test");
            TEST("copyFile() == 1", copyFile("dm_src.txt", "dm_dst.txt", 1) == 1);
            TEST("dst exists", isExist("dm_dst.txt") == 1);
            delFile("dm_dst.txt");
            delFile("dm_src.txt");
        }
        if (moveFile && writeFile && isExist && delFile) {
            writeFile("dm_move_src.txt", "move test");
            TEST("moveFile() == 1", moveFile("dm_move_src.txt", "dm_move_dst.txt") == 1);
            TEST("src gone", isExist("dm_move_src.txt") == 0);
            TEST("dst exists", isExist("dm_move_dst.txt") == 1);
            delFile("dm_move_dst.txt");
        }
    }
    printf("  Module 7 done. Passing: %d, Failed: %d\n", g_passed, g_failed);

    // ===== Module 9: 系统 =====
    printf("\n===== Module 9: 系统 =====\n");
    {
        auto getSW = (long (*)())GetProcAddress(hDll, "dm_getScreenWidth");
        auto getSH = (long (*)())GetProcAddress(hDll, "dm_getScreenHeight");
        auto getSD = (long (*)())GetProcAddress(hDll, "dm_getScreenDepth");
        auto getDPI = (long (*)())GetProcAddress(hDll, "dm_getDPI");
        auto getOsType = (long (*)())GetProcAddress(hDll, "dm_getOsType");
        auto getTime = (long (*)())GetProcAddress(hDll, "dm_getTime");
        auto getCpuType = (long (*)())GetProcAddress(hDll, "dm_getCpuType");
        auto getCpuUsage = (long (*)())GetProcAddress(hDll, "dm_getCpuUsage");
        auto getMemUsage = (long (*)())GetProcAddress(hDll, "dm_getMemoryUsage");
        auto getBuild = (long (*)())GetProcAddress(hDll, "dm_getOsBuildNumber");
        auto is64 = (long (*)())GetProcAddress(hDll, "dm_is64Bit");
        auto getLocale = (long (*)())GetProcAddress(hDll, "dm_getLocale");
        auto delay = (long (*)(long))GetProcAddress(hDll, "dm_delay");
        auto runApp = (long (*)(const char*, long))GetProcAddress(hDll, "dm_runApp");
        auto getSerial = (const char* (*)())GetProcAddress(hDll, "dm_getDiskSerial");
        auto getModel = (const char* (*)())GetProcAddress(hDll, "dm_getDiskModel");
        auto getMC = (const char* (*)())GetProcAddress(hDll, "dm_getMachineCode");
        auto getNetTime = (const char* (*)())GetProcAddress(hDll, "dm_getNetTime");
        auto getClip = (const char* (*)())GetProcAddress(hDll, "dm_getClipboard");
        auto setClip = (long (*)(const char*))GetProcAddress(hDll, "dm_setClipboard");
        auto execCmd = (const char* (*)(const char*, long))GetProcAddress(hDll, "dm_executeCmd");

        if (getSW) TEST("getScreenWidth() > 0", getSW() > 0);
        if (getSH) TEST("getScreenHeight() > 0", getSH() > 0);
        if (getSW && getSH) printf("    screen = %ld x %ld\n", getSW(), getSH());
        if (getSD) { long sd = getSD(); TEST("getScreenDepth() > 0", sd > 0); }
        if (getDPI) { long dpi = getDPI(); TEST("getDPI() > 0", dpi > 0); printf("    dpi = %ld\n", dpi); }
        if (getOsType) { long os = getOsType(); TEST("getOsType() >= 0", os >= 0); printf("    os = %ld\n", os); }
        if (getBuild) printf("    build = %ld\n", getBuild());
        if (is64) printf("    is64 = %ld\n", is64());
        if (getLocale) printf("    locale = %ld\n", getLocale());
        if (getTime) { long t = getTime(); TEST("getTime() > 1700000000", t > 1700000000); printf("    time = %ld\n", t); }
        if (getCpuType) { long ct = getCpuType(); printf("    cpuType = %ld\n", ct); }
        if (getCpuUsage) { long cu = getCpuUsage(); printf("    cpuUsage = %ld%%\n", cu); }
        if (getMemUsage) { long mu = getMemUsage(); printf("    memUsage = %ld%%\n", mu); }
        if (delay) TEST("delay(50) == 1", delay(50) == 1);
        if (getSerial) { const char* s = getSerial(); printf("    diskSerial = %s\n", s ? s : "(null)"); }
        if (getModel) { const char* m = getModel(); printf("    diskModel = %s\n", m ? m : "(null)"); }
        if (getMC) { const char* m = getMC(); printf("    machineCode = %s\n", m ? m : "(null)"); }
        if (getNetTime) { const char* n = getNetTime(); printf("    netTime = %s\n", n ? n : "(null)"); }
        if (setClip && getClip) {
            TEST("setClipboard() == 1", setClip("dm_hook_test") == 1);
            const char* clip = getClip();
            printf("    clipboard = %s\n", clip ? clip : "(null)");
        }
        if (execCmd) {
            const char* cmd = execCmd("echo dm_hook_test_123", 5000);
            printf("    executeCmd = %s\n", cmd ? cmd : "(null)");
        }
    }
    printf("  Module 9 done. Passing: %d, Failed: %d\n", g_passed, g_failed);

    // ===== Results =====
    printf("\n========================================\n");
    printf("  PASSED: %d   FAILED: %d   TOTAL: %d\n", g_passed, g_failed, g_passed + g_failed);
    printf("========================================\n");

    FreeLibrary(hDll);
    return g_failed > 0 ? 1 : 0;
}