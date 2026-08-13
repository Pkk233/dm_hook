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
DM_API long DM_CALL dm_reg(const char* reg_code, const char* ver_info);
DM_API long DM_CALL dm_regEx(const char* reg_code, const char* ver_info, const char* ip);
DM_API long DM_CALL dm_regNoMac(const char* reg_code, const char* ver_info);
DM_API long DM_CALL dm_regExNoMac(const char* reg_code, const char* ver_info, const char* ip);
DM_API const char* DM_CALL dm_ver();
DM_API long DM_CALL dm_getID();
DM_API long DM_CALL dm_getDmCount();
DM_API long DM_CALL dm_getLastError();
DM_API const char* DM_CALL dm_getPath();
DM_API long DM_CALL dm_setPath(const char* path);
DM_API const char* DM_CALL dm_getBasePath();
DM_API long DM_CALL dm_setShowErrorMsg(long show);
DM_API long DM_CALL dm_enablePicCache(long enable);

// ============================================================================
// 模块 2: 窗口
// ============================================================================
DM_API long DM_CALL dm_findWindow(const char* cls, const char* title);
DM_API long DM_CALL dm_findWindowEx(intptr_t parent, long cls, const char* title);
DM_API long DM_CALL dm_findWindowByProcess(const char* process_name, const char* cls, const char* title);
DM_API long DM_CALL dm_findWindowByProcessId(long process_id, const char* cls, const char* title);
DM_API long DM_CALL dm_findWindowSuper(const char* cls, const char* title, long flag);
DM_API long DM_CALL dm_enumWindow(intptr_t parent, const char* title, long filter);
DM_API long DM_CALL dm_enumWindowByProcess(const char* process_name, const char* title, long filter);
DM_API long DM_CALL dm_enumWindowByProcessId(long pid, const char* title, long filter);
DM_API long DM_CALL dm_enumWindowSuper(const char* spec1, long flag1, const char* spec2, long flag2, long filter);
DM_API long DM_CALL dm_enumProcess(const char* name);
DM_API long DM_CALL dm_getWindow(intptr_t hwnd, long flag);
DM_API long DM_CALL dm_getForegroundWindow();
DM_API long DM_CALL dm_getForegroundFocus();
DM_API long DM_CALL dm_getMousePointWindow();
DM_API long DM_CALL dm_getPointWindow(long x, long y);
DM_API long DM_CALL dm_getSpecialWindow(long flag);
DM_API const char* DM_CALL dm_getWindowClass(intptr_t hwnd);
DM_API long DM_CALL dm_getWindowProcessId(intptr_t hwnd);
DM_API const char* DM_CALL dm_getWindowProcessPath(intptr_t hwnd);
DM_API long DM_CALL dm_getWindowThreadId(intptr_t hwnd);
DM_API const char* DM_CALL dm_getWindowTitle(intptr_t hwnd);
DM_API long DM_CALL dm_getWindowRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
DM_API long DM_CALL dm_getClientRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
DM_API long DM_CALL dm_getClientSize(intptr_t hwnd, long* width, long* height);
DM_API long DM_CALL dm_getWindowState(intptr_t hwnd, long flag);
DM_API long DM_CALL dm_getProcessInfo(long pid, const char* type);
DM_API long DM_CALL dm_moveWindow(intptr_t hwnd, long x, long y);
DM_API long DM_CALL dm_setWindowSize(intptr_t hwnd, long width, long height);
DM_API long DM_CALL dm_setWindowState(intptr_t hwnd, long flag);
DM_API long DM_CALL dm_setWindowText(intptr_t hwnd, const char* title);
DM_API long DM_CALL dm_setWindowTransparent(intptr_t hwnd, long trans);
DM_API long DM_CALL dm_setClientSize(intptr_t hwnd, long width, long height);
DM_API long DM_CALL dm_setSendStringDelay(long delay);
DM_API long DM_CALL dm_sendPaste(intptr_t hwnd);
DM_API long DM_CALL dm_sendString(intptr_t hwnd, const char* str);
DM_API long DM_CALL dm_sendString2(intptr_t hwnd, const char* str);
DM_API long DM_CALL dm_sendStringIme(intptr_t hwnd, const char* str);
DM_API long DM_CALL dm_sendStringIme2(intptr_t hwnd, const char* str);
DM_API long DM_CALL dm_clientToScreen(intptr_t hwnd, long* x, long* y);
DM_API long DM_CALL dm_screenToClient(intptr_t hwnd, long* x, long* y);

// ============================================================================
// 模块 3: 键鼠
// ============================================================================
DM_API long DM_CALL dm_keyDown(long vk_code);
DM_API long DM_CALL dm_keyDownChar(const char* key_str);
DM_API long DM_CALL dm_keyPress(long vk_code);
DM_API long DM_CALL dm_keyPressChar(const char* key_str);
DM_API long DM_CALL dm_keyPressStr(const char* key_str, long delay);
DM_API long DM_CALL dm_keyUp(long vk_code);
DM_API long DM_CALL dm_keyUpChar(const char* key_str);
DM_API long DM_CALL dm_waitKey(long vk_code, long time_out);
DM_API long DM_CALL dm_getKeyState(long vk_code);
DM_API long DM_CALL dm_leftClick();
DM_API long DM_CALL dm_leftDoubleClick();
DM_API long DM_CALL dm_leftDown();
DM_API long DM_CALL dm_leftUp();
DM_API long DM_CALL dm_rightClick();
DM_API long DM_CALL dm_rightDown();
DM_API long DM_CALL dm_rightUp();
DM_API long DM_CALL dm_middleClick();
DM_API long DM_CALL dm_middleDown();
DM_API long DM_CALL dm_middleUp();
DM_API long DM_CALL dm_moveTo(long x, long y);
DM_API long DM_CALL dm_moveToEx(long x, long y, long w, long h);
DM_API long DM_CALL dm_moveR(long rx, long ry);
DM_API long DM_CALL dm_wheelDown(long repeat);
DM_API long DM_CALL dm_wheelUp(long repeat);
DM_API long DM_CALL dm_getCursorPos(long* x, long* y);
DM_API long DM_CALL dm_getCursorShape();
DM_API const char* DM_CALL dm_getCursorShapeEx(long type);
DM_API long DM_CALL dm_getCursorSpot();
DM_API long DM_CALL dm_setMouseDelay(long type, long delay);
DM_API long DM_CALL dm_setKeypadDelay(long type, long delay);
DM_API long DM_CALL dm_setMouseSpeed(long speed);
DM_API long DM_CALL dm_getMouseSpeed();
DM_API long DM_CALL dm_enableMouseAccuracy(long enable);
DM_API long DM_CALL dm_setSimMode(long mode);

// ============================================================================
// 模块 4: 图色
// ============================================================================
DM_API long DM_CALL dm_capture(long x1, long y1, long x2, long y2, const char* file);
DM_API long DM_CALL dm_captureGif(long x1, long y1, long x2, long y2, const char* file, long delay, long time);
DM_API long DM_CALL dm_captureJpg(long x1, long y1, long x2, long y2, const char* file, long quality);
DM_API long DM_CALL dm_capturePng(long x1, long y1, long x2, long y2, const char* file);
DM_API long DM_CALL dm_capturePre(const char* file);
DM_API const char* DM_CALL dm_getColor(long x, long y);
DM_API const char* DM_CALL dm_getColorBGR(long x, long y);
DM_API const char* DM_CALL dm_getColorHSV(long x, long y);
DM_API long DM_CALL dm_getColorNum(long x1, long y1, long x2, long y2, const char* color, long sim);
DM_API const char* DM_CALL dm_getAveRGB(long x1, long y1, long x2, long y2);
DM_API const char* DM_CALL dm_getAveHSV(long x1, long y1, long x2, long y2);
DM_API long DM_CALL dm_cmpColor(long x, long y, const char* color, long sim);
DM_API long DM_CALL dm_findColor(long x1, long y1, long x2, long y2, const char* color, long sim, long dir, long* intX, long* intY);
DM_API long DM_CALL dm_findColorEx(long x1, long y1, long x2, long y2, const char* color, long sim, long dir);
DM_API long DM_CALL dm_findColorBlock(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height, long* intX, long* intY);
DM_API long DM_CALL dm_findColorBlockEx(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height);
DM_API long DM_CALL dm_findColorE(long x1, long y1, long x2, long y2, const char* color, long sim, long dir);
DM_API long DM_CALL dm_findMulColor(long x1, long y1, long x2, long y2, const char* color, long sim);
DM_API long DM_CALL dm_findMultiColor(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir, long* intX, long* intY);
DM_API long DM_CALL dm_findMultiColorEx(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir);
DM_API long DM_CALL dm_findMultiColorE(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir);
DM_API long DM_CALL dm_findPic(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL dm_findPicE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
DM_API const char* DM_CALL dm_findPicEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
DM_API const char* DM_CALL dm_findPicExS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
DM_API long DM_CALL dm_findPicS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY);
DM_API long DM_CALL dm_findPicMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL dm_findPicMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
DM_API const char* DM_CALL dm_findPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
DM_API long DM_CALL dm_findPicSim(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL dm_findPicSimE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
DM_API const char* DM_CALL dm_findPicSimEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
DM_API long DM_CALL dm_findPicSimMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL dm_findPicSimMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
DM_API const char* DM_CALL dm_findPicSimMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
DM_API long DM_CALL dm_findShape(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL dm_findShapeE(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir);
DM_API const char* DM_CALL dm_findShapeEx(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir);
DM_API const char* DM_CALL dm_getPicSize(const char* pic_name);
DM_API long DM_CALL dm_freePic(const char* pic_name);
DM_API long DM_CALL dm_loadPic(const char* pic_name);
DM_API long DM_CALL dm_loadPicByte(const char* pic_name, long data, long size);
DM_API const char* DM_CALL dm_appendPicAddr(const char* pic_info, long addr, long size);
DM_API const char* DM_CALL dm_matchPicName(const char* pic_name);
DM_API long DM_CALL dm_imageToBmp(const char* pic_name, const char* bmp_name);
DM_API long DM_CALL dm_setPicPwd(const char* pwd);
DM_API long DM_CALL dm_setExcludeRegion(long mode, const char* info);
DM_API long DM_CALL dm_getScreenData(long x1, long y1, long x2, long y2);
DM_API const char* DM_CALL dm_getScreenDataBmp(long x1, long y1, long x2, long y2, long* size);
DM_API long DM_CALL dm_enableDisplayDebug(long enable_debug);
DM_API long DM_CALL dm_enableFindPicMultithread(long enable);
DM_API long DM_CALL dm_enableGetColorByCapture(long enable);
DM_API long DM_CALL dm_setFindPicMultithreadCount(long count);
DM_API long DM_CALL dm_setFindPicMultithreadLimit(long limit);
DM_API long DM_CALL dm_bgr2RGB(const char* bgr_color);
DM_API const char* DM_CALL dm_rgb2BGR(const char* rgb_color);
DM_API long DM_CALL dm_isDisplayDead(long x1, long y1, long x2, long y2, long time);

// ============================================================================
// 模块 5: 后台设置
// ============================================================================
DM_API long DM_CALL dm_bindWindow(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, long mode);
DM_API long DM_CALL dm_bindWindowEx(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, const char* pub, long mode);
DM_API long DM_CALL dm_unBindWindow();
DM_API long DM_CALL dm_getBindWindow();
DM_API long DM_CALL dm_isBind(intptr_t hwnd);
DM_API long DM_CALL dm_forceUnBindWindow();
DM_API long DM_CALL dm_setAero(long enable);
DM_API long DM_CALL dm_lockInput(long enable);
DM_API long DM_CALL dm_lockMouseRect(long x1, long y1, long x2, long y2);
DM_API long DM_CALL dm_lockDisplay(long enable);
DM_API long DM_CALL dm_enableBind(long enable);
DM_API long DM_CALL dm_enableFakeActive(long enable);
DM_API long DM_CALL dm_enableIme(long enable);
DM_API long DM_CALL dm_enableKeypadMsg(long enable);
DM_API long DM_CALL dm_enableKeypadPatch(long enable);
DM_API long DM_CALL dm_enableKeypadSync(long enable);
DM_API long DM_CALL dm_enableMouseMsg(long enable);
DM_API long DM_CALL dm_enableMouseSync(long enable, long time_out);
DM_API long DM_CALL dm_enableRealKeypad(long enable);
DM_API long DM_CALL dm_enableRealMouse(long enable, long mousedelay, long mousestep);
DM_API long DM_CALL dm_enableSpeedDx(long enable);
DM_API long DM_CALL dm_getFps();
DM_API long DM_CALL dm_setDisplayDelay(long delay);
DM_API long DM_CALL dm_setDisplayRefreshDelay(long delay);
DM_API long DM_CALL dm_setInputDm(long enable);
DM_API long DM_CALL dm_hackSpeed(long rate);
DM_API long DM_CALL dm_downCpu(long enable, long rate);
DM_API long DM_CALL dm_switchBindWindow(intptr_t hwnd);

// ============================================================================
// 模块 6: 内存
// ============================================================================
DM_API long DM_CALL dm_openProcess(long pid);
DM_API long DM_CALL dm_getModuleBaseAddr(intptr_t hwnd, const char* module);
DM_API long DM_CALL dm_getModuleSize(intptr_t hwnd, const char* module);
DM_API long DM_CALL dm_getRemoteApiAddress(intptr_t hwnd, const char* module, const char* api);
DM_API long DM_CALL dm_readInt(intptr_t hwnd, long addr, long* ret);
DM_API long DM_CALL dm_readIntAddr(intptr_t hwnd, long addr, long type, long* ret);
DM_API long DM_CALL dm_readFloat(intptr_t hwnd, long addr, float* ret);
DM_API long DM_CALL dm_readFloatAddr(intptr_t hwnd, long addr, float* ret);
DM_API long DM_CALL dm_readDouble(intptr_t hwnd, long addr, double* ret);
DM_API long DM_CALL dm_readDoubleAddr(intptr_t hwnd, long addr, double* ret);
DM_API const char* DM_CALL dm_readString(intptr_t hwnd, long addr, long type, long len);
DM_API const char* DM_CALL dm_readStringAddr(intptr_t hwnd, long addr, long type, long len);
DM_API const char* DM_CALL dm_readData(intptr_t hwnd, long addr, long len);
DM_API const char* DM_CALL dm_readDataAddr(intptr_t hwnd, long addr, long len);
DM_API long DM_CALL dm_readDataToBin(intptr_t hwnd, long addr, long len);
DM_API long DM_CALL dm_readDataAddrToBin(intptr_t hwnd, long addr, long len);
DM_API const char* DM_CALL dm_findInt(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max);
DM_API const char* DM_CALL dm_findIntEx(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max, long step, long multi_thread, long mode);
DM_API const char* DM_CALL dm_findFloat(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max);
DM_API const char* DM_CALL dm_findFloatEx(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max, long step, long multi_thread, long mode);
DM_API const char* DM_CALL dm_findDouble(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max);
DM_API const char* DM_CALL dm_findDoubleEx(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max, long step, long multi_thread, long mode);
DM_API const char* DM_CALL dm_findString(intptr_t hwnd, const char* addr_range, const char* string_value, long type);
DM_API const char* DM_CALL dm_findStringEx(intptr_t hwnd, const char* addr_range, const char* string_value, long type, long step, long multi_thread, long mode);
DM_API const char* DM_CALL dm_findData(intptr_t hwnd, const char* addr_range, const char* data);
DM_API const char* DM_CALL dm_findDataEx(intptr_t hwnd, const char* addr_range, const char* data, long step, long multi_thread, long mode);
DM_API long DM_CALL dm_writeInt(intptr_t hwnd, long addr, long type, long v);
DM_API long DM_CALL dm_writeIntAddr(intptr_t hwnd, long addr, long type, long v);
DM_API long DM_CALL dm_writeFloat(intptr_t hwnd, long addr, float v);
DM_API long DM_CALL dm_writeFloatAddr(intptr_t hwnd, long addr, float v);
DM_API long DM_CALL dm_writeDouble(intptr_t hwnd, long addr, double v);
DM_API long DM_CALL dm_writeDoubleAddr(intptr_t hwnd, long addr, double v);
DM_API long DM_CALL dm_writeString(intptr_t hwnd, long addr, long type, const char* v);
DM_API long DM_CALL dm_writeStringAddr(intptr_t hwnd, long addr, long type, const char* v);
DM_API long DM_CALL dm_writeData(intptr_t hwnd, long addr, const char* data);
DM_API long DM_CALL dm_writeDataAddr(intptr_t hwnd, long addr, const char* data);
DM_API long DM_CALL dm_writeDataFromBin(intptr_t hwnd, long addr, long data, long len);
DM_API long DM_CALL dm_writeDataAddrFromBin(intptr_t hwnd, long addr, long data, long len);
DM_API long DM_CALL dm_virtualAllocEx(intptr_t hwnd, long size, long type, long protect);
DM_API long DM_CALL dm_virtualFreeEx(intptr_t hwnd, long addr);
DM_API long DM_CALL dm_virtualProtectEx(intptr_t hwnd, long addr, long size, long type, long* old_protect);
DM_API const char* DM_CALL dm_virtualQueryEx(intptr_t hwnd, long addr, long pmbi);
DM_API long DM_CALL dm_freeProcessMemory(intptr_t hwnd);
DM_API const char* DM_CALL dm_getCommandLine(intptr_t hwnd);
DM_API long DM_CALL dm_terminateProcess(long pid);
DM_API long DM_CALL dm_terminateProcessTree(long pid);
DM_API long DM_CALL dm_setMemoryFindResultToFile(long enable);
DM_API long DM_CALL dm_setMemoryHwndAsProcessId(long enable);
DM_API long DM_CALL dm_setParam64ToPointer(long enable);
DM_API const char* DM_CALL dm_int64ToInt32(long v_high, long v_low, long flag);
DM_API const char* DM_CALL dm_intToData(long v, long type);
DM_API const char* DM_CALL dm_floatToData(float v);
DM_API const char* DM_CALL dm_doubleToData(double v);
DM_API const char* DM_CALL dm_stringToData(const char* v, long type);

// ============================================================================
// 模块 7: 文件
// ============================================================================
DM_API long DM_CALL dm_writeFile(const char* file, const char* content);
DM_API const char* DM_CALL dm_readFile(const char* file);
DM_API long DM_CALL dm_deleteFile(const char* file);
DM_API long DM_CALL dm_copyFile(const char* src, const char* dest, long over);
DM_API long DM_CALL dm_moveFile(const char* src, const char* dest);
DM_API long DM_CALL dm_createFolder(const char* folder);
DM_API long DM_CALL dm_deleteFolder(const char* folder);
DM_API long DM_CALL dm_isFileExist(const char* file);
DM_API long DM_CALL dm_isFolderExist(const char* folder);
DM_API long DM_CALL dm_getFileLength(const char* file);
DM_API const char* DM_CALL dm_getRealPath(const char* path);
DM_API const char* DM_CALL dm_selectFile();
DM_API const char* DM_CALL dm_selectDirectory();
DM_API long DM_CALL dm_downloadFile(const char* url, const char* save_file, long timeout);
DM_API long DM_CALL dm_encodeFile(const char* file, const char* pwd);
DM_API long DM_CALL dm_decodeFile(const char* file, const char* pwd);
DM_API long DM_CALL dm_writeIni(const char* section, const char* key, const char* value, const char* file);
DM_API const char* DM_CALL dm_readIni(const char* section, const char* key, const char* file);
DM_API long DM_CALL dm_deleteIni(const char* section, const char* key, const char* file);
DM_API const char* DM_CALL dm_enumIniKey(const char* section, const char* file);
DM_API const char* DM_CALL dm_enumIniSection(const char* file);
DM_API long DM_CALL dm_writeIniPwd(const char* section, const char* key, const char* value, const char* file, const char* pwd);
DM_API const char* DM_CALL dm_readIniPwd(const char* section, const char* key, const char* file, const char* pwd);
DM_API long DM_CALL dm_deleteIniPwd(const char* section, const char* key, const char* file, const char* pwd);
DM_API const char* DM_CALL dm_enumIniKeyPwd(const char* section, const char* file, const char* pwd);
DM_API const char* DM_CALL dm_enumIniSectionPwd(const char* file, const char* pwd);

// ============================================================================
// 模块 8: 文字识别
// ============================================================================
DM_API long DM_CALL dm_setDict(long index, const char* file);
DM_API long DM_CALL dm_setDictMem(long index, const char* data, long size);
DM_API long DM_CALL dm_setDictPwd(const char* pwd);
DM_API long DM_CALL dm_useDict(long index);
DM_API const char* DM_CALL dm_getDict(long index, long font_index);
DM_API long DM_CALL dm_getDictCount(long index);
DM_API const char* DM_CALL dm_getDictInfo(const char* file, long index);
DM_API long DM_CALL dm_getNowDict();
DM_API long DM_CALL dm_addDict(const char* dict_info);
DM_API long DM_CALL dm_saveDict(long index, const char* file);
DM_API long DM_CALL dm_clearDict(long index);
DM_API long DM_CALL dm_enableShareDict(long enable);
DM_API const char* DM_CALL dm_ocr(long x1, long y1, long x2, long y2, const char* color_format, long sim);
DM_API const char* DM_CALL dm_ocrEx(long x1, long y1, long x2, long y2, const char* color_format, long sim);
DM_API const char* DM_CALL dm_ocrExOne(long x1, long y1, long x2, long y2, const char* color_format, long sim);
DM_API long DM_CALL dm_ocrInFile(long x1, long y1, long x2, long y2, const char* color_format, long sim, const char* file);
DM_API long DM_CALL dm_findStr(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
DM_API const char* DM_CALL dm_findStrE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
DM_API const char* DM_CALL dm_findStrEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
DM_API long DM_CALL dm_findStrS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
DM_API const char* DM_CALL dm_findStrExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
DM_API long DM_CALL dm_findStrFast(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
DM_API const char* DM_CALL dm_findStrFastE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
DM_API const char* DM_CALL dm_findStrFastEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
DM_API long DM_CALL dm_findStrFastS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
DM_API const char* DM_CALL dm_findStrFastExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
DM_API long DM_CALL dm_findStrWithFont(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size, long* intX, long* intY);
DM_API const char* DM_CALL dm_findStrWithFontE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size);
DM_API const char* DM_CALL dm_findStrWithFontEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size);
DM_API const char* DM_CALL dm_getWords(long x1, long y1, long x2, long y2, const char* color, const char* words);
DM_API const char* DM_CALL dm_getWordsNoDict(long x1, long y1, long x2, long y2, const char* color);
DM_API long DM_CALL dm_fetchWord(long x1, long y1, long x2, long y2, const char* color, const char* words);
DM_API long DM_CALL dm_getResultCount(const char* ret);
DM_API long DM_CALL dm_getResultPos(const char* ret, long index, long* intX, long* intY);
DM_API long DM_CALL dm_getWordResultCount(const char* str);
DM_API long DM_CALL dm_getWordResultPos(const char* str, long index, long* intX, long* intY);
DM_API const char* DM_CALL dm_getWordResultStr(const char* str, long index);
DM_API long DM_CALL dm_setColGapNoDict(long col_gap);
DM_API long DM_CALL dm_setRowGapNoDict(long row_gap);
DM_API long DM_CALL dm_setWordGapNoDict(long word_gap);
DM_API long DM_CALL dm_setWordLineHeightNoDict(long line_height);
DM_API long DM_CALL dm_setExactOcr(long exact_ocr);
DM_API long DM_CALL dm_setMinColGap(long min_col_gap);
DM_API long DM_CALL dm_setMinRowGap(long min_row_gap);
DM_API long DM_CALL dm_setWordGap(long word_gap);
DM_API long DM_CALL dm_setWordLineHeight(long line_height);

// ============================================================================
// 模块 9: 系统
// ============================================================================
DM_API long DM_CALL dm_beep(long f, long duration);
DM_API long DM_CALL dm_delay(long mis);
DM_API long DM_CALL dm_delays(long mis_min, long mis_max);
DM_API long DM_CALL dm_runApp(const char* app_path, long mode);
DM_API long DM_CALL dm_stop(long id);
DM_API long DM_CALL dm_play(const char* media_file);
DM_API long DM_CALL dm_exitOs(long type);
DM_API long DM_CALL dm_setScreen(long width, long height, long depth);
DM_API long DM_CALL dm_getScreenWidth();
DM_API long DM_CALL dm_getScreenHeight();
DM_API long DM_CALL dm_getScreenDepth();
DM_API long DM_CALL dm_getDPI();
DM_API long DM_CALL dm_getTime();
DM_API long DM_CALL dm_getOsType();
DM_API long DM_CALL dm_getOsBuildNumber();
DM_API long DM_CALL dm_is64Bit();
DM_API long DM_CALL dm_isSurrpotVt();
DM_API const char* DM_CALL dm_getSystemInfo(long type, long method);
DM_API const char* DM_CALL dm_getDir(long type);
DM_API long DM_CALL dm_getLocale();
DM_API long DM_CALL dm_getCpuType();
DM_API long DM_CALL dm_getCpuUsage();
DM_API long DM_CALL dm_getMemoryUsage();
DM_API const char* DM_CALL dm_getDiskSerial();
DM_API const char* DM_CALL dm_getDiskModel();
DM_API const char* DM_CALL dm_getDiskReversion();
DM_API const char* DM_CALL dm_getMachineCode();
DM_API const char* DM_CALL dm_getMachineCodeNoMac();
DM_API const char* DM_CALL dm_getNetTime();
DM_API const char* DM_CALL dm_getNetTimeByIp(const char* ip);
DM_API const char* DM_CALL dm_getNetTimeSafe(long time_out);
DM_API const char* DM_CALL dm_getClipboard();
DM_API long DM_CALL dm_setClipboard(const char* value);
DM_API long DM_CALL dm_getDisplayInfo(long type);
DM_API long DM_CALL dm_disableCloseDisplayAndSleep(long enable);
DM_API long DM_CALL dm_disablePowerSave(long enable);
DM_API long DM_CALL dm_disableScreenSave(long enable);
DM_API long DM_CALL dm_disableFontSmooth();
DM_API long DM_CALL dm_enableFontSmooth();
DM_API long DM_CALL dm_checkFontSmooth();
DM_API long DM_CALL dm_checkUAC();
DM_API long DM_CALL dm_setUAC(long enable);
DM_API long DM_CALL dm_setDisplayAcceler(long flag);
DM_API long DM_CALL dm_showTaskBarIcon(intptr_t hwnd, long show);

// ============================================================================
// 模块 10: 杂项
// ============================================================================
DM_API long DM_CALL dm_enterCri();
DM_API long DM_CALL dm_leaveCri();
DM_API long DM_CALL dm_initCri();
DM_API long DM_CALL dm_releaseRef();
DM_API long DM_CALL dm_setExitThread(long mode);
DM_API const char* DM_CALL dm_executeCmd(const char* cmd, long time_out);
DM_API long DM_CALL dm_activeInputMethod(intptr_t hwnd, const char* input_method);
DM_API long DM_CALL dm_checkInputMethod(intptr_t hwnd, const char* input_method);
DM_API long DM_CALL dm_findInputMethod(const char* input_method);

// ============================================================================
// 模块 11: 汇编
// ============================================================================
DM_API long DM_CALL dm_asmAdd(const char* asm_ins);
DM_API long DM_CALL dm_asmCall(intptr_t hwnd, long mode);
DM_API long DM_CALL dm_asmCallEx(intptr_t hwnd, long mode, long base_addr, long call_addr);
DM_API long DM_CALL dm_asmClear();
DM_API long DM_CALL dm_asmSetTimeout(long timeout);
DM_API const char* DM_CALL dm_assemble(long base_addr, long is_asm_code);
DM_API const char* DM_CALL dm_disAssemble(long asm_code, long base_addr, long is_asm_code);
DM_API long DM_CALL dm_setAsmHwndAsProcessId(long enable);
DM_API long DM_CALL dm_setShowAsmErrorMsg(long show);

// ============================================================================
// 模块 12: AI
// ============================================================================
DM_API long DM_CALL dm_loadAi(const char* file);
DM_API long DM_CALL dm_loadAiMemory(long data, long size);
DM_API long DM_CALL dm_aiEnableFindPicWindow(long enable);
DM_API long DM_CALL dm_aiFindPic(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL dm_aiFindPicEx(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir);
DM_API long DM_CALL dm_aiFindPicMem(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir, long* intX, long* intY);
DM_API const char* DM_CALL dm_aiFindPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir);
DM_API const char* DM_CALL dm_aiYoloDetectObjects(long x1, long y1, long x2, long y2, float prob, float iou);
DM_API long DM_CALL dm_aiYoloDetectObjectsToDataBmp(long x1, long y1, long x2, long y2, float prob, float iou, long data, long size, long mode);
DM_API long DM_CALL dm_aiYoloDetectObjectsToFile(long x1, long y1, long x2, long y2, float prob, float iou, const char* file, long mode);
DM_API long DM_CALL dm_aiYoloSetModel(const char* model_file, const char* model_type);
DM_API long DM_CALL dm_aiYoloSetModelMemory(long data, long size, const char* model_type);
DM_API long DM_CALL dm_aiYoloFreeModel();
DM_API long DM_CALL dm_aiYoloUseModel(long index);
DM_API long DM_CALL dm_aiYoloSetVersion(const char* version);
DM_API const char* DM_CALL dm_aiYoloObjectsToString(const char* objects);
DM_API const char* DM_CALL dm_aiYoloSortsObjects(const char* objects, long sort_type);

// ============================================================================
// 模块 13: Foobar (界面控件)
// ============================================================================
DM_API long DM_CALL dm_createFoobarRect(intptr_t hwnd, long x, long y, long w, long h);
DM_API long DM_CALL dm_createFoobarEllipse(intptr_t hwnd, long x, long y, long w, long h);
DM_API long DM_CALL dm_createFoobarRoundRect(intptr_t hwnd, long x, long y, long w, long h, long rw, long rh);
DM_API long DM_CALL dm_createFoobarCustom(intptr_t hwnd, long x, long y, long w, long h, const char* pic);
DM_API long DM_CALL dm_foobarClose(intptr_t hwnd);
DM_API long DM_CALL dm_foobarLock(intptr_t hwnd);
DM_API long DM_CALL dm_foobarUnlock(intptr_t hwnd);
DM_API long DM_CALL dm_foobarUpdate(intptr_t hwnd);
DM_API long DM_CALL dm_foobarSetFont(intptr_t hwnd, const char* font_name, long size, long flag);
DM_API long DM_CALL dm_foobarSetSave(intptr_t hwnd, const char* file, long enable);
DM_API long DM_CALL dm_foobarSetTrans(intptr_t hwnd, long trans, long color);
DM_API long DM_CALL dm_foobarDrawText(intptr_t hwnd, long x, long y, long w, long h, const char* text, long color, long align);
DM_API long DM_CALL dm_foobarPrintText(intptr_t hwnd, const char* text, long color);
DM_API long DM_CALL dm_foobarDrawRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
DM_API long DM_CALL dm_foobarDrawLine(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
DM_API long DM_CALL dm_foobarFillRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color);
DM_API long DM_CALL dm_foobarDrawPic(intptr_t hwnd, long x, long y, const char* pic);
DM_API long DM_CALL dm_foobarClearText(intptr_t hwnd);
DM_API long DM_CALL dm_foobarTextRect(intptr_t hwnd, long x, long y, long w, long h);
DM_API long DM_CALL dm_foobarTextLineGap(intptr_t hwnd, long line_gap);
DM_API long DM_CALL dm_foobarTextPrintDir(intptr_t hwnd, long dir);
DM_API long DM_CALL dm_foobarStartGif(intptr_t hwnd, const char* pic, long x, long y);
DM_API long DM_CALL dm_foobarStopGif(intptr_t hwnd);

// ============================================================================
// 模块 14: 答题
// ============================================================================
DM_API long DM_CALL dm_faqCapture(long x1, long y1, long x2, long y2, long quality, long delay, long time);
DM_API long DM_CALL dm_faqCaptureFromFile(const char* file, long quality, long delay, long time);
DM_API const char* DM_CALL dm_faqCaptureString(long x1, long y1, long x2, long y2, long quality, long delay, long time);
DM_API long DM_CALL dm_faqGetSize(long handle);
DM_API long DM_CALL dm_faqPost(const char* server, long handle, long request_type, long time_out);
DM_API long DM_CALL dm_faqSend(long handle, long time_out);
DM_API long DM_CALL dm_faqFetch(long time_out);
DM_API long DM_CALL dm_faqCancel();
DM_API long DM_CALL dm_faqIsPosted();

// ============================================================================
// 模块 15: 算法
// ============================================================================
DM_API const char* DM_CALL dm_excludePos(const char* all_pos, long type, long x1, long y1, long x2, long y2);
DM_API const char* DM_CALL dm_findNearestPos(const char* all_pos, long type, long x, long y);
DM_API const char* DM_CALL dm_sortPosDistance(const char* all_pos, long type, long x, long y);

// ============================================================================
// 模块 16: 防护盾
// ============================================================================
DM_API long DM_CALL dm_dmGuard(long enable, const char* type);
DM_API long DM_CALL dm_dmGuardExtract(const char* type, const char* file);
DM_API long DM_CALL dm_dmGuardLoadCustom(const char* type, const char* data);
DM_API const char* DM_CALL dm_dmGuardParams(const char* cmd, const char* param);
DM_API long DM_CALL dm_unLoadDriver();

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
void dm_setComState(DmState* s);

#endif // DM_H
