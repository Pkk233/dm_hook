// ============================================================================
// dm_com.cpp — 大漠插件 COM 支持实现
// 使用 enum 分发机制（兼容 MSVC 模板推导限制）
// ============================================================================
#define DM_EXPORTS
#define NOMINMAX

#include "dm_com.h"
#include <shlwapi.h>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>
#pragma comment(lib, "shlwapi.lib")

// GUID 定义（生成 GUID 数据）
// {9AEDF74E-E647-443C-A42A-2D7B50AE7345}
EXTERN_C const GUID CLSID_DmSoft = { 0x9aedf74e, 0xe647, 0x443c, { 0xa4, 0x2a, 0x2d, 0x7b, 0x50, 0xae, 0x73, 0x45 } };

// ============================================================================
// 全局引用计数
// ============================================================================
LONG g_dllRefCount = 0;
LONG g_dllLockCount = 0;

// ============================================================================
// 辅助函数 — VARIANT 参数提取
// ============================================================================

static long GetLongArg(const VARIANT* args, UINT index) {
    const VARIANT* v = &args[index];
    if (v->vt == VT_I4) return v->lVal;
    if (v->vt == VT_I2) return v->iVal;
    if (v->vt == VT_R4) return (long)v->fltVal;
    if (v->vt == VT_R8) return (long)v->dblVal;
    if (v->vt == VT_BOOL) return v->boolVal ? 1 : 0;
    return 0;
}

static float GetFloatArg(const VARIANT* args, UINT index) {
    const VARIANT* v = &args[index];
    if (v->vt == VT_R4) return v->fltVal;
    if (v->vt == VT_R8) return (float)v->dblVal;
    if (v->vt == VT_I4) return (float)v->lVal;
    return 0.0f;
}

static double GetDoubleArg(const VARIANT* args, UINT index) {
    const VARIANT* v = &args[index];
    if (v->vt == VT_R8) return v->dblVal;
    if (v->vt == VT_R4) return (double)v->fltVal;
    if (v->vt == VT_I4) return (double)v->lVal;
    return 0.0;
}

static std::string GetStringArg(const VARIANT* args, UINT index) {
    const VARIANT* v = &args[index];
    if (v->vt == VT_BSTR) {
        int len = WideCharToMultiByte(CP_UTF8, 0, v->bstrVal, -1, NULL, 0, NULL, NULL);
        if (len <= 0) return "";
        std::string result(len - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, v->bstrVal, -1, &result[0], len, NULL, NULL);
        return result;
    }
    if (v->vt == VT_I4) return std::to_string(v->lVal);
    return "";
}

static intptr_t GetPtrArg(const VARIANT* args, UINT index) {
    return (intptr_t)GetLongArg(args, index);
}

// 设置返回值
static void SetResultLong(VARIANT* ret, long val) {
    if (!ret) return;
    ret->vt = VT_I4;
    ret->lVal = val;
}

static void SetResultString(VARIANT* ret, const char* val) {
    if (!ret) return;
    if (!val) val = "";
    int len = MultiByteToWideChar(CP_UTF8, 0, val, -1, NULL, 0);
    ret->vt = VT_BSTR;
    ret->bstrVal = SysAllocStringLen(NULL, len - 1);
    if (ret->bstrVal) {
        MultiByteToWideChar(CP_UTF8, 0, val, -1, ret->bstrVal, len);
    }
}

static void SetResultDouble(VARIANT* ret, double val) {
    if (!ret) return;
    ret->vt = VT_R8;
    ret->dblVal = val;
}

// ============================================================================
// 特殊处理函数（用于有输出参数的函数）
// ============================================================================

// findColor(x1, y1, x2, y2, color, sim, dir, &x, &y) → "x|y" 或 ""
static HRESULT H_findColor(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string color = GetStringArg(args, 4);
    long r = dm_findColor(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                          GetLongArg(args, 3), color.c_str(), GetLongArg(args, 5),
                          GetLongArg(args, 6), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// findColorBlock (9 args)
static HRESULT H_findColorBlock9(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 9) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string color = GetStringArg(args, 4);
    long r = dm_findColorBlock(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                               GetLongArg(args, 3), color.c_str(), GetLongArg(args, 5),
                               GetLongArg(args, 6), GetLongArg(args, 7), GetLongArg(args, 8), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// findMultiColor
static HRESULT H_findMultiColor(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 8) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = dm_findMultiColor(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                               GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                               GetLongArg(args, 6), GetLongArg(args, 7), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// findPic
static HRESULT H_findPic(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 8) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = dm_findPic(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                        GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                        GetLongArg(args, 6), GetLongArg(args, 7), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// findStr
static HRESULT H_findStr(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = dm_findStr(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                        GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                        GetLongArg(args, 6), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// findStrFast
static HRESULT H_findStrFast(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = dm_findStrFast(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                            GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                            GetLongArg(args, 6), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// getCursorPos
static HRESULT H_getCursorPos(VARIANT* ret, const VARIANT*, UINT) {
    long x = 0, y = 0; dm_getCursorPos(&x, &y);
    char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf);
    return S_OK;
}

// getWindowRect
static HRESULT H_getWindowRect(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 1) return DISP_E_BADPARAMCOUNT;
    long x1=0,y1=0,x2=0,y2=0;
    long r = dm_getWindowRect(GetPtrArg(args,0), &x1,&y1,&x2,&y2);
    if (r == 0) { char buf[128]; sprintf_s(buf, "%ld|%ld|%ld|%ld", x1,y1,x2,y2); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// getClientRect
static HRESULT H_getClientRect(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 1) return DISP_E_BADPARAMCOUNT;
    long x1=0,y1=0,x2=0,y2=0;
    long r = dm_getClientRect(GetPtrArg(args,0), &x1,&y1,&x2,&y2);
    if (r == 0) { char buf[128]; sprintf_s(buf, "%ld|%ld|%ld|%ld", x1,y1,x2,y2); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// getClientSize
static HRESULT H_getClientSize(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 1) return DISP_E_BADPARAMCOUNT;
    long w=0,h=0;
    long r = dm_getClientSize(GetPtrArg(args,0), &w, &h);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", w, h); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// clientToScreen
static HRESULT H_clientToScreen(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    long x = GetLongArg(args,1), y = GetLongArg(args,2);
    long r = dm_clientToScreen(GetPtrArg(args,0), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// screenToClient
static HRESULT H_screenToClient(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    long x = GetLongArg(args,1), y = GetLongArg(args,2);
    long r = dm_screenToClient(GetPtrArg(args,0), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// getResultPos
static HRESULT H_getResultPos(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    long x=0,y=0; std::string s = GetStringArg(args,0);
    long r = dm_getResultPos(s.c_str(), GetLongArg(args,1), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// getWordResultPos
static HRESULT H_getWordResultPos(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    long x=0,y=0; std::string s = GetStringArg(args,0);
    long r = dm_getWordResultPos(s.c_str(), GetLongArg(args,1), &x, &y);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld|%ld", x, y); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// virtualProtectEx
static HRESULT H_virtualProtectEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    long old = 0;
    long r = dm_virtualProtectEx(GetPtrArg(args,0), GetLongArg(args,1), GetLongArg(args,2), GetLongArg(args,3), &old);
    if (r == 0) { char buf[64]; sprintf_s(buf, "%ld", old); SetResultString(ret, buf); }
    else { SetResultString(ret, ""); }
    return S_OK;
}

// readInt
static HRESULT H_readInt(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    long val = 0;
    dm_readInt(GetPtrArg(args,0), GetLongArg(args,1), &val);
    SetResultLong(ret, val);
    return S_OK;
}

// readFloat
static HRESULT H_readFloat(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    float val = 0;
    dm_readFloat(GetPtrArg(args,0), GetLongArg(args,1), &val);
    SetResultDouble(ret, val);
    return S_OK;
}

// readDouble
static HRESULT H_readDouble(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    double val = 0;
    dm_readDouble(GetPtrArg(args,0), GetLongArg(args,1), &val);
    SetResultDouble(ret, val);
    return S_OK;
}

// getScreenDataBmp
static HRESULT H_getScreenDataBmp(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    long size = 0;
    const char* data = dm_getScreenDataBmp(GetLongArg(args,0), GetLongArg(args,1),
                                           GetLongArg(args,2), GetLongArg(args,3), &size);
    SetResultString(ret, data);
    return S_OK;
}

// ============================================================================
// 分发表定义
// ============================================================================

// 宏：快速定义分发表项
#define ENTRY_L0(func, id) { #func, id, 0, 0, FuncPattern::L0, (void*)func }
#define ENTRY_S0(func, id) { #func, id, 0, 0, FuncPattern::S0, (void*)func }
#define ENTRY_L1L(func, id) { #func, id, 1, 1, FuncPattern::L1L, (void*)func }
#define ENTRY_L1S(func, id) { #func, id, 1, 1, FuncPattern::L1S, (void*)func }
#define ENTRY_L2L(func, id) { #func, id, 2, 2, FuncPattern::L2L, (void*)func }
#define ENTRY_L3L(func, id) { #func, id, 3, 3, FuncPattern::L3L, (void*)func }
#define ENTRY_L4L(func, id) { #func, id, 4, 4, FuncPattern::L4L, (void*)func }
#define ENTRY_L5L(func, id) { #func, id, 5, 5, FuncPattern::L5L, (void*)func }
#define ENTRY_L2S(func, id) { #func, id, 2, 2, FuncPattern::L2S, (void*)func }
#define ENTRY_L3S(func, id) { #func, id, 3, 3, FuncPattern::L3S, (void*)func }
#define ENTRY_L1L1S(func, id) { #func, id, 2, 2, FuncPattern::L1L1S, (void*)func }
#define ENTRY_L1S1L(func, id) { #func, id, 2, 2, FuncPattern::L1S1L, (void*)func }
#define ENTRY_L2L1S(func, id) { #func, id, 3, 3, FuncPattern::L2L1S, (void*)func }
#define ENTRY_L1S2L(func, id) { #func, id, 3, 3, FuncPattern::L1S2L, (void*)func }
#define ENTRY_L4L1S(func, id) { #func, id, 5, 5, FuncPattern::L4L1S, (void*)func }
#define ENTRY_S2L(func, id) { #func, id, 2, 2, FuncPattern::S2L, (void*)func }
#define ENTRY_S2L1S(func, id) { #func, id, 3, 3, FuncPattern::S2L1S, (void*)func }
#define ENTRY_S1S(func, id) { #func, id, 1, 1, FuncPattern::S1S, (void*)func }
#define ENTRY_L4L2S(func, id) { #func, id, 6, 6, FuncPattern::L4L2S, (void*)func }
#define ENTRY_S2L2S(func, id) { #func, id, 4, 4, FuncPattern::S2L2S, (void*)func }
#define ENTRY_L4L1S1L(func, id) { #func, id, 6, 6, FuncPattern::L4L1S1L, (void*)func }
#define ENTRY_L1L2S(func, id) { #func, id, 3, 3, FuncPattern::L1L2S, (void*)func }
#define ENTRY_P1L(func, id) { #func, id, 1, 1, FuncPattern::P1L, (void*)func }
#define ENTRY_P1L1S(func, id) { #func, id, 2, 2, FuncPattern::P1L1S, (void*)func }
#define ENTRY_P1L1L(func, id) { #func, id, 2, 2, FuncPattern::P1L1L, (void*)func }
#define ENTRY_P1L2L(func, id) { #func, id, 3, 3, FuncPattern::P1L2L, (void*)func }
#define ENTRY_P1L1L1S(func, id) { #func, id, 3, 3, FuncPattern::P1L1L1S, (void*)func }
#define ENTRY_P1L2L1S(func, id) { #func, id, 4, 4, FuncPattern::P1L2L1S, (void*)func }
#define ENTRY_L2S1L(func, id) { #func, id, 3, 3, FuncPattern::L2S1L, (void*)func }
#define ENTRY_P1S(func, id) { #func, id, 1, 1, FuncPattern::P1S, (void*)func }
#define ENTRY_REGEX(func, id) { #func, id, 3, 3, FuncPattern::REGEX, (void*)func }
#define ENTRY_P1L1S1L(func, id) { #func, id, 3, 3, FuncPattern::P1L1S1L, (void*)func }
#define ENTRY_P1L1L1F(func, id) { #func, id, 3, 3, FuncPattern::P1L1L1F, (void*)func }
#define ENTRY_P1L4L(func, id) { #func, id, 5, 5, FuncPattern::P1L4L, (void*)func }
#define ENTRY_P1L5L(func, id) { #func, id, 6, 6, FuncPattern::P1L5L, (void*)func }
#define ENTRY_P1L1S2L(func, id) { #func, id, 4, 4, FuncPattern::P1L1S2L, (void*)func }
#define ENTRY_P1L2L1S2L(func, id) { #func, id, 6, 6, FuncPattern::P1L2L1S2L, (void*)func }
#define ENTRY_L4S(func, id) { #func, id, 4, 4, FuncPattern::L4S, (void*)func }
#define ENTRY_L5S(func, id) { #func, id, 5, 5, FuncPattern::L5S, (void*)func }
#define ENTRY_S2S(func, id) { #func, id, 2, 2, FuncPattern::S2S, (void*)func }
#define ENTRY_S3S(func, id) { #func, id, 3, 3, FuncPattern::S3S, (void*)func }
#define ENTRY_S4S(func, id) { #func, id, 4, 4, FuncPattern::S4S, (void*)func }
#define ENTRY_SPECIAL(handler, id, min, max) { #handler, id, min, max, FuncPattern::SPECIAL, (void*)handler }

// ============================================================================
// DISPID 枚举
// ============================================================================
enum DispIds {
    // 模块1: 基本设置 (1-13)
    DISPID_reg = 1, DISPID_regEx, DISPID_regNoMac, DISPID_regExNoMac,
    DISPID_ver, DISPID_getID, DISPID_getDmCount, DISPID_getLastError,
    DISPID_getPath, DISPID_setPath, DISPID_getBasePath, DISPID_setShowErrorMsg,
    DISPID_enablePicCache,
    // 模块2: 窗口操作 (14-53)
    DISPID_findWindow = 14, DISPID_findWindowEx, DISPID_findWindowByProcess,
    DISPID_findWindowByProcessId, DISPID_findWindowSuper, DISPID_enumWindow,
    DISPID_enumWindowByProcess, DISPID_enumWindowByProcessId, DISPID_enumWindowSuper,
    DISPID_enumProcess, DISPID_getWindow, DISPID_getForegroundWindow,
    DISPID_getForegroundFocus, DISPID_getMousePointWindow, DISPID_getPointWindow,
    DISPID_getSpecialWindow, DISPID_getWindowClass, DISPID_getWindowProcessId,
    DISPID_getWindowProcessPath, DISPID_getWindowThreadId, DISPID_getWindowTitle,
    DISPID_getWindowRect, DISPID_getClientRect, DISPID_getClientSize,
    DISPID_getWindowState, DISPID_getProcessInfo, DISPID_moveWindow,
    DISPID_setWindowSize, DISPID_setWindowState, DISPID_setWindowText,
    DISPID_setWindowTransparent, DISPID_setClientSize, DISPID_setSendStringDelay,
    DISPID_sendPaste, DISPID_sendString, DISPID_sendString2, DISPID_sendStringIme,
    DISPID_sendStringIme2, DISPID_clientToScreen, DISPID_screenToClient,
    // 模块3: 键鼠操作 (54-87)
    DISPID_keyDown = 54, DISPID_keyDownChar, DISPID_keyPress, DISPID_keyPressChar,
    DISPID_keyPressStr, DISPID_keyUp, DISPID_keyUpChar, DISPID_waitKey, DISPID_getKeyState,
    DISPID_leftClick, DISPID_leftDoubleClick, DISPID_leftDown, DISPID_leftUp,
    DISPID_rightClick, DISPID_rightDown, DISPID_rightUp, DISPID_middleClick,
    DISPID_middleDown, DISPID_middleUp, DISPID_moveTo, DISPID_moveToEx, DISPID_moveR,
    DISPID_wheelDown, DISPID_wheelUp, DISPID_getCursorPos, DISPID_getCursorShape,
    DISPID_getCursorShapeEx, DISPID_getCursorSpot, DISPID_setMouseDelay,
    DISPID_setKeypadDelay, DISPID_setMouseSpeed, DISPID_getMouseSpeed,
    DISPID_enableMouseAccuracy, DISPID_setSimMode,
    // 模块4: 图色操作 (88-144)
    DISPID_capture = 88, DISPID_captureGif, DISPID_captureJpg, DISPID_capturePng,
    DISPID_capturePre, DISPID_getColor, DISPID_getColorBGR, DISPID_getColorHSV,
    DISPID_getColorNum, DISPID_getAveRGB, DISPID_getAveHSV, DISPID_cmpColor,
    DISPID_findColor, DISPID_findColorEx, DISPID_findColorBlock, DISPID_findColorBlockEx,
    DISPID_findColorE, DISPID_findMulColor, DISPID_findMultiColor, DISPID_findMultiColorEx,
    DISPID_findMultiColorE, DISPID_findPic, DISPID_findPicE, DISPID_findPicEx,
    DISPID_findPicExS, DISPID_findPicS, DISPID_findPicMem, DISPID_findPicMemE,
    DISPID_findPicMemEx, DISPID_findPicSim, DISPID_findPicSimE, DISPID_findPicSimEx,
    DISPID_findPicSimMem, DISPID_findPicSimMemE, DISPID_findPicSimMemEx,
    DISPID_findShape, DISPID_findShapeE, DISPID_findShapeEx,
    DISPID_getPicSize, DISPID_freePic, DISPID_loadPic, DISPID_loadPicByte,
    DISPID_appendPicAddr, DISPID_matchPicName, DISPID_imageToBmp, DISPID_setPicPwd,
    DISPID_setExcludeRegion, DISPID_getScreenData, DISPID_getScreenDataBmp,
    DISPID_enableDisplayDebug, DISPID_enableFindPicMultithread, DISPID_enableGetColorByCapture,
    DISPID_setFindPicMultithreadCount, DISPID_setFindPicMultithreadLimit,
    DISPID_bgr2RGB, DISPID_rgb2BGR, DISPID_isDisplayDead,
    // 模块5: 后台设置 (145-172)
    DISPID_bindWindow = 145, DISPID_bindWindowEx, DISPID_unBindWindow, DISPID_getBindWindow,
    DISPID_isBind, DISPID_forceUnBindWindow, DISPID_setAero, DISPID_lockInput,
    DISPID_lockMouseRect, DISPID_lockDisplay, DISPID_enableBind, DISPID_enableFakeActive,
    DISPID_enableIme, DISPID_enableKeypadMsg, DISPID_enableKeypadPatch, DISPID_enableKeypadSync,
    DISPID_enableMouseMsg, DISPID_enableMouseSync, DISPID_enableRealKeypad, DISPID_enableRealMouse,
    DISPID_enableSpeedDx, DISPID_getFps, DISPID_setDisplayDelay, DISPID_setDisplayRefreshDelay,
    DISPID_setInputDm, DISPID_hackSpeed, DISPID_downCpu, DISPID_switchBindWindow,
    // 模块6: 内存操作 (173-226)
    DISPID_openProcess = 173, DISPID_getModuleBaseAddr, DISPID_getModuleSize,
    DISPID_getRemoteApiAddress, DISPID_readInt, DISPID_readIntAddr, DISPID_readFloat,
    DISPID_readFloatAddr, DISPID_readDouble, DISPID_readDoubleAddr, DISPID_readString,
    DISPID_readStringAddr, DISPID_readData, DISPID_readDataAddr, DISPID_readDataToBin,
    DISPID_readDataAddrToBin, DISPID_findInt, DISPID_findIntEx, DISPID_findFloat,
    DISPID_findFloatEx, DISPID_findDouble, DISPID_findDoubleEx, DISPID_findString,
    DISPID_findStringEx, DISPID_findData, DISPID_findDataEx, DISPID_writeInt,
    DISPID_writeIntAddr, DISPID_writeFloat, DISPID_writeFloatAddr, DISPID_writeDouble,
    DISPID_writeDoubleAddr, DISPID_writeString, DISPID_writeStringAddr, DISPID_writeData,
    DISPID_writeDataAddr, DISPID_writeDataFromBin, DISPID_writeDataAddrFromBin,
    DISPID_virtualAllocEx, DISPID_virtualFreeEx, DISPID_virtualProtectEx, DISPID_virtualQueryEx,
    DISPID_freeProcessMemory, DISPID_getCommandLine, DISPID_terminateProcess,
    DISPID_terminateProcessTree, DISPID_setMemoryFindResultToFile, DISPID_setMemoryHwndAsProcessId,
    DISPID_setParam64ToPointer, DISPID_int64ToInt32, DISPID_intToData, DISPID_floatToData,
    DISPID_doubleToData, DISPID_stringToData,
    // 模块7: 文件操作 (227-252)
    DISPID_writeFile = 227, DISPID_readFile, DISPID_deleteFile, DISPID_copyFile,
    DISPID_moveFile, DISPID_createFolder, DISPID_deleteFolder, DISPID_isFileExist,
    DISPID_isFolderExist, DISPID_getFileLength, DISPID_getRealPath, DISPID_selectFile,
    DISPID_selectDirectory, DISPID_downloadFile, DISPID_encodeFile, DISPID_decodeFile,
    DISPID_writeIni, DISPID_readIni, DISPID_deleteIni, DISPID_enumIniKey, DISPID_enumIniSection,
    DISPID_writeIniPwd, DISPID_readIniPwd, DISPID_deleteIniPwd, DISPID_enumIniKeyPwd,
    DISPID_enumIniSectionPwd,
    // 模块8: 文字识别 (253-298)
    DISPID_setDict = 253, DISPID_setDictMem, DISPID_setDictPwd, DISPID_useDict,
    DISPID_getDict, DISPID_getDictCount, DISPID_getDictInfo, DISPID_getNowDict,
    DISPID_addDict, DISPID_saveDict, DISPID_clearDict, DISPID_enableShareDict,
    DISPID_ocr, DISPID_ocrEx, DISPID_ocrExOne, DISPID_ocrInFile,
    DISPID_findStr, DISPID_findStrE, DISPID_findStrEx, DISPID_findStrS, DISPID_findStrExS,
    DISPID_findStrFast, DISPID_findStrFastE, DISPID_findStrFastEx, DISPID_findStrFastS,
    DISPID_findStrFastExS, DISPID_findStrWithFont, DISPID_findStrWithFontE, DISPID_findStrWithFontEx,
    DISPID_getWords, DISPID_getWordsNoDict, DISPID_fetchWord,
    DISPID_getResultCount, DISPID_getResultPos, DISPID_getWordResultCount,
    DISPID_getWordResultPos, DISPID_getWordResultStr,
    DISPID_setColGapNoDict, DISPID_setRowGapNoDict, DISPID_setWordGapNoDict,
    DISPID_setWordLineHeightNoDict, DISPID_setExactOcr, DISPID_setMinColGap,
    DISPID_setMinRowGap, DISPID_setWordGap, DISPID_setWordLineHeight,
    // 模块9: 系统操作 (299-342)
    DISPID_beep = 299, DISPID_delay, DISPID_delays, DISPID_runApp, DISPID_stop,
    DISPID_play, DISPID_exitOs, DISPID_setScreen, DISPID_getScreenWidth, DISPID_getScreenHeight,
    DISPID_getScreenDepth, DISPID_getDPI, DISPID_getTime, DISPID_getOsType, DISPID_getOsBuildNumber,
    DISPID_is64Bit, DISPID_isSurrpotVt, DISPID_getSystemInfo, DISPID_getDir, DISPID_getLocale,
    DISPID_getCpuType, DISPID_getCpuUsage, DISPID_getMemoryUsage, DISPID_getDiskSerial,
    DISPID_getDiskModel, DISPID_getDiskReversion, DISPID_getMachineCode, DISPID_getMachineCodeNoMac,
    DISPID_getNetTime, DISPID_getNetTimeByIp, DISPID_getNetTimeSafe, DISPID_getClipboard,
    DISPID_setClipboard, DISPID_getDisplayInfo, DISPID_disableCloseDisplayAndSleep,
    DISPID_disablePowerSave, DISPID_disableScreenSave, DISPID_disableFontSmooth,
    DISPID_enableFontSmooth, DISPID_checkFontSmooth, DISPID_checkUAC, DISPID_setUAC,
    DISPID_setDisplayAcceler, DISPID_showTaskBarIcon,
    // 模块10: 杂项 (343-351)
    DISPID_enterCri = 343, DISPID_leaveCri, DISPID_initCri, DISPID_releaseRef,
    DISPID_setExitThread, DISPID_executeCmd, DISPID_activeInputMethod, DISPID_checkInputMethod,
    DISPID_findInputMethod,
    // 模块11: 汇编 (352-360)
    DISPID_asmAdd = 352, DISPID_asmCall, DISPID_asmCallEx, DISPID_asmClear,
    DISPID_asmSetTimeout, DISPID_assemble, DISPID_disAssemble, DISPID_setAsmHwndAsProcessId,
    DISPID_setShowAsmErrorMsg,
    // 模块12: AI (361-377)
    DISPID_loadAi = 361, DISPID_loadAiMemory, DISPID_aiEnableFindPicWindow,
    DISPID_aiFindPic, DISPID_aiFindPicEx, DISPID_aiFindPicMem, DISPID_aiFindPicMemEx,
    DISPID_aiYoloDetectObjects, DISPID_aiYoloDetectObjectsToDataBmp, DISPID_aiYoloDetectObjectsToFile,
    DISPID_aiYoloSetModel, DISPID_aiYoloSetModelMemory, DISPID_aiYoloFreeModel,
    DISPID_aiYoloUseModel, DISPID_aiYoloSetVersion, DISPID_aiYoloObjectsToString,
    DISPID_aiYoloSortsObjects,
    // 模块13: Foobar (378-400)
    DISPID_createFoobarRect = 378, DISPID_createFoobarEllipse, DISPID_createFoobarRoundRect,
    DISPID_createFoobarCustom, DISPID_foobarClose, DISPID_foobarLock, DISPID_foobarUnlock,
    DISPID_foobarUpdate, DISPID_foobarSetFont, DISPID_foobarSetSave, DISPID_foobarSetTrans,
    DISPID_foobarDrawText, DISPID_foobarPrintText, DISPID_foobarDrawRect, DISPID_foobarDrawLine,
    DISPID_foobarFillRect, DISPID_foobarDrawPic, DISPID_foobarClearText, DISPID_foobarTextRect,
    DISPID_foobarTextLineGap, DISPID_foobarTextPrintDir, DISPID_foobarStartGif, DISPID_foobarStopGif,
    // 模块14: 答题 (401-409)
    DISPID_faqCapture = 401, DISPID_faqCaptureFromFile, DISPID_faqCaptureString,
    DISPID_faqGetSize, DISPID_faqPost, DISPID_faqSend, DISPID_faqFetch, DISPID_faqCancel,
    DISPID_faqIsPosted,
    // 模块15: 算法 (410-412)
    DISPID_excludePos = 410, DISPID_findNearestPos, DISPID_sortPosDistance,
    // 模块16: 防护盾 (413-417)
    DISPID_dmGuard = 413, DISPID_dmGuardExtract, DISPID_dmGuardLoadCustom,
    DISPID_dmGuardParams, DISPID_unLoadDriver,
};

// ============================================================================
// 分发表
// ============================================================================
const DispFuncEntry g_dispTable[] = {
    // ====== 模块1: 基本设置 ======
    ENTRY_L2S(dm_reg, DISPID_reg),
    ENTRY_REGEX(dm_regEx, DISPID_regEx),
    ENTRY_L2S(dm_regNoMac, DISPID_regNoMac),
    ENTRY_REGEX(dm_regExNoMac, DISPID_regExNoMac),
    ENTRY_S0(dm_ver, DISPID_ver),
    ENTRY_L0(dm_getID, DISPID_getID),
    ENTRY_L0(dm_getDmCount, DISPID_getDmCount),
    ENTRY_L0(dm_getLastError, DISPID_getLastError),
    ENTRY_S0(dm_getPath, DISPID_getPath),
    ENTRY_L1S(dm_setPath, DISPID_setPath),
    ENTRY_S0(dm_getBasePath, DISPID_getBasePath),
    ENTRY_L1L(dm_setShowErrorMsg, DISPID_setShowErrorMsg),
    ENTRY_L1L(dm_enablePicCache, DISPID_enablePicCache),
    // ====== 模块2: 窗口操作 ======
    ENTRY_L2S(dm_findWindow, DISPID_findWindow),
    ENTRY_L2S(dm_findWindowByProcess, DISPID_findWindowByProcess),
    ENTRY_L3L(dm_findWindowByProcessId, DISPID_findWindowByProcessId),
    ENTRY_L1S1L(dm_findWindowSuper, DISPID_findWindowSuper),
    ENTRY_L1S1L(dm_enumWindow, DISPID_enumWindow),
    ENTRY_L1S1L(dm_enumWindowByProcess, DISPID_enumWindowByProcess),
    ENTRY_L1S1L(dm_enumWindowByProcessId, DISPID_enumWindowByProcessId),
    ENTRY_L1S(dm_enumProcess, DISPID_enumProcess),
    ENTRY_P1L1L(dm_getWindow, DISPID_getWindow),
    ENTRY_L0(dm_getForegroundWindow, DISPID_getForegroundWindow),
    ENTRY_L0(dm_getForegroundFocus, DISPID_getForegroundFocus),
    ENTRY_L0(dm_getMousePointWindow, DISPID_getMousePointWindow),
    ENTRY_L2L(dm_getPointWindow, DISPID_getPointWindow),
    ENTRY_L1L(dm_getSpecialWindow, DISPID_getSpecialWindow),
    ENTRY_P1S(dm_getWindowClass, DISPID_getWindowClass),
    ENTRY_P1L(dm_getWindowProcessId, DISPID_getWindowProcessId),
    ENTRY_P1S(dm_getWindowProcessPath, DISPID_getWindowProcessPath),
    ENTRY_P1L(dm_getWindowThreadId, DISPID_getWindowThreadId),
    ENTRY_P1S(dm_getWindowTitle, DISPID_getWindowTitle),
    ENTRY_SPECIAL(H_getWindowRect, DISPID_getWindowRect, 1, 1),
    ENTRY_SPECIAL(H_getClientRect, DISPID_getClientRect, 1, 1),
    ENTRY_SPECIAL(H_getClientSize, DISPID_getClientSize, 1, 1),
    ENTRY_P1L1L(dm_getWindowState, DISPID_getWindowState),
    ENTRY_L1S1L(dm_getProcessInfo, DISPID_getProcessInfo),
    ENTRY_P1L2L(dm_moveWindow, DISPID_moveWindow),
    ENTRY_P1L2L(dm_setWindowSize, DISPID_setWindowSize),
    ENTRY_P1L1L(dm_setWindowState, DISPID_setWindowState),
    ENTRY_P1L1S(dm_setWindowText, DISPID_setWindowText),
    ENTRY_P1L1L(dm_setWindowTransparent, DISPID_setWindowTransparent),
    ENTRY_P1L2L(dm_setClientSize, DISPID_setClientSize),
    ENTRY_L1L(dm_setSendStringDelay, DISPID_setSendStringDelay),
    ENTRY_P1L(dm_sendPaste, DISPID_sendPaste),
    ENTRY_P1L1S(dm_sendString, DISPID_sendString),
    ENTRY_P1L1S(dm_sendString2, DISPID_sendString2),
    ENTRY_P1L1S(dm_sendStringIme, DISPID_sendStringIme),
    ENTRY_P1L1S(dm_sendStringIme2, DISPID_sendStringIme2),
    ENTRY_SPECIAL(H_clientToScreen, DISPID_clientToScreen, 3, 3),
    ENTRY_SPECIAL(H_screenToClient, DISPID_screenToClient, 3, 3),
    // ====== 模块3: 键鼠操作 ======
    ENTRY_L1L(dm_keyDown, DISPID_keyDown),
    ENTRY_L1S(dm_keyDownChar, DISPID_keyDownChar),
    ENTRY_L1L(dm_keyPress, DISPID_keyPress),
    ENTRY_L1S(dm_keyPressChar, DISPID_keyPressChar),
    ENTRY_L1S1L(dm_keyPressStr, DISPID_keyPressStr),
    ENTRY_L1L(dm_keyUp, DISPID_keyUp),
    ENTRY_L1S(dm_keyUpChar, DISPID_keyUpChar),
    ENTRY_L2L(dm_waitKey, DISPID_waitKey),
    ENTRY_L1L(dm_getKeyState, DISPID_getKeyState),
    ENTRY_L0(dm_leftClick, DISPID_leftClick),
    ENTRY_L0(dm_leftDoubleClick, DISPID_leftDoubleClick),
    ENTRY_L0(dm_leftDown, DISPID_leftDown),
    ENTRY_L0(dm_leftUp, DISPID_leftUp),
    ENTRY_L0(dm_rightClick, DISPID_rightClick),
    ENTRY_L0(dm_rightDown, DISPID_rightDown),
    ENTRY_L0(dm_rightUp, DISPID_rightUp),
    ENTRY_L0(dm_middleClick, DISPID_middleClick),
    ENTRY_L0(dm_middleDown, DISPID_middleDown),
    ENTRY_L0(dm_middleUp, DISPID_middleUp),
    ENTRY_L2L(dm_moveTo, DISPID_moveTo),
    ENTRY_L4L(dm_moveToEx, DISPID_moveToEx),
    ENTRY_L2L(dm_moveR, DISPID_moveR),
    ENTRY_L1L(dm_wheelDown, DISPID_wheelDown),
    ENTRY_L1L(dm_wheelUp, DISPID_wheelUp),
    ENTRY_SPECIAL(H_getCursorPos, DISPID_getCursorPos, 0, 0),
    ENTRY_L0(dm_getCursorShape, DISPID_getCursorShape),
    ENTRY_L1L(dm_getCursorShapeEx, DISPID_getCursorShapeEx),
    ENTRY_L0(dm_getCursorSpot, DISPID_getCursorSpot),
    ENTRY_L2L(dm_setMouseDelay, DISPID_setMouseDelay),
    ENTRY_L2L(dm_setKeypadDelay, DISPID_setKeypadDelay),
    ENTRY_L1L(dm_setMouseSpeed, DISPID_setMouseSpeed),
    ENTRY_L0(dm_getMouseSpeed, DISPID_getMouseSpeed),
    ENTRY_L1L(dm_enableMouseAccuracy, DISPID_enableMouseAccuracy),
    ENTRY_L1L(dm_setSimMode, DISPID_setSimMode),
    // ====== 模块4: 图色操作 ======
    ENTRY_L4L1S(dm_capture, DISPID_capture),
    ENTRY_L4L1S1L(dm_captureGif, DISPID_captureGif),
    ENTRY_L4L1S1L(dm_captureJpg, DISPID_captureJpg),
    ENTRY_L4L1S(dm_capturePng, DISPID_capturePng),
    ENTRY_L1S(dm_capturePre, DISPID_capturePre),
    ENTRY_S2L(dm_getColor, DISPID_getColor),
    ENTRY_S2L(dm_getColorBGR, DISPID_getColorBGR),
    ENTRY_S2L(dm_getColorHSV, DISPID_getColorHSV),
    ENTRY_L4L1S(dm_getColorNum, DISPID_getColorNum),
    ENTRY_S2L(dm_getAveRGB, DISPID_getAveRGB),
    ENTRY_S2L(dm_getAveHSV, DISPID_getAveHSV),
    ENTRY_L2L1S(dm_cmpColor, DISPID_cmpColor),
    ENTRY_SPECIAL(H_findColor, DISPID_findColor, 7, 7),
    ENTRY_L4L2S(dm_findColorEx, DISPID_findColorEx),
    ENTRY_SPECIAL(H_findColorBlock9, DISPID_findColorBlock, 9, 9),
    ENTRY_L4L2S(dm_findColorBlockEx, DISPID_findColorBlockEx),
    ENTRY_L4L2S(dm_findColorE, DISPID_findColorE),
    ENTRY_L4L1S(dm_findMulColor, DISPID_findMulColor),
    ENTRY_SPECIAL(H_findMultiColor, DISPID_findMultiColor, 8, 8),
    ENTRY_S2L2S(dm_findMultiColorEx, DISPID_findMultiColorEx),
    ENTRY_S2L2S(dm_findMultiColorE, DISPID_findMultiColorE),
    ENTRY_SPECIAL(H_findPic, DISPID_findPic, 8, 8),
    ENTRY_S2L2S(dm_findPicE, DISPID_findPicE),
    ENTRY_S2L2S(dm_findPicEx, DISPID_findPicEx),
    ENTRY_S2L2S(dm_findPicExS, DISPID_findPicExS),
    ENTRY_L4L2S(dm_findPicS, DISPID_findPicS),
    ENTRY_L4L2S(dm_findPicMem, DISPID_findPicMem),
    ENTRY_S2L2S(dm_findPicMemE, DISPID_findPicMemE),
    ENTRY_S2L2S(dm_findPicMemEx, DISPID_findPicMemEx),
    ENTRY_L4L2S(dm_findPicSim, DISPID_findPicSim),
    ENTRY_S2L2S(dm_findPicSimE, DISPID_findPicSimE),
    ENTRY_S2L2S(dm_findPicSimEx, DISPID_findPicSimEx),
    ENTRY_L4L2S(dm_findPicSimMem, DISPID_findPicSimMem),
    ENTRY_S2L2S(dm_findPicSimMemE, DISPID_findPicSimMemE),
    ENTRY_S2L2S(dm_findPicSimMemEx, DISPID_findPicSimMemEx),
    ENTRY_L4L1S(dm_findShape, DISPID_findShape),
    ENTRY_S2L1S(dm_findShapeE, DISPID_findShapeE),
    ENTRY_S2L1S(dm_findShapeEx, DISPID_findShapeEx),
    ENTRY_S1S(dm_getPicSize, DISPID_getPicSize),
    ENTRY_L1S(dm_freePic, DISPID_freePic),
    ENTRY_L1S(dm_loadPic, DISPID_loadPic),
    ENTRY_L1S1L(dm_loadPicByte, DISPID_loadPicByte),
    ENTRY_L1S1L(dm_appendPicAddr, DISPID_appendPicAddr),
    ENTRY_S1S(dm_matchPicName, DISPID_matchPicName),
    ENTRY_L2S(dm_imageToBmp, DISPID_imageToBmp),
    ENTRY_L1S(dm_setPicPwd, DISPID_setPicPwd),
    ENTRY_L1S1L(dm_setExcludeRegion, DISPID_setExcludeRegion),
    ENTRY_L4L(dm_getScreenData, DISPID_getScreenData),
    ENTRY_SPECIAL(H_getScreenDataBmp, DISPID_getScreenDataBmp, 4, 4),
    ENTRY_L1L(dm_enableDisplayDebug, DISPID_enableDisplayDebug),
    ENTRY_L1L(dm_enableFindPicMultithread, DISPID_enableFindPicMultithread),
    ENTRY_L1L(dm_enableGetColorByCapture, DISPID_enableGetColorByCapture),
    ENTRY_L1L(dm_setFindPicMultithreadCount, DISPID_setFindPicMultithreadCount),
    ENTRY_L1L(dm_setFindPicMultithreadLimit, DISPID_setFindPicMultithreadLimit),
    ENTRY_L1S(dm_bgr2RGB, DISPID_bgr2RGB),
    ENTRY_S1S(dm_rgb2BGR, DISPID_rgb2BGR),
    ENTRY_L5L(dm_isDisplayDead, DISPID_isDisplayDead),
    // ====== 模块5: 后台设置 ======
    ENTRY_L1L2S(dm_bindWindow, DISPID_bindWindow),
    ENTRY_L1L2S(dm_bindWindowEx, DISPID_bindWindowEx),
    ENTRY_L0(dm_unBindWindow, DISPID_unBindWindow),
    ENTRY_L0(dm_getBindWindow, DISPID_getBindWindow),
    ENTRY_P1L(dm_isBind, DISPID_isBind),
    ENTRY_L0(dm_forceUnBindWindow, DISPID_forceUnBindWindow),
    ENTRY_L1L(dm_setAero, DISPID_setAero),
    ENTRY_L1L(dm_lockInput, DISPID_lockInput),
    ENTRY_L4L(dm_lockMouseRect, DISPID_lockMouseRect),
    ENTRY_L1L(dm_lockDisplay, DISPID_lockDisplay),
    ENTRY_L1L(dm_enableBind, DISPID_enableBind),
    ENTRY_L1L(dm_enableFakeActive, DISPID_enableFakeActive),
    ENTRY_L1L(dm_enableIme, DISPID_enableIme),
    ENTRY_L1L(dm_enableKeypadMsg, DISPID_enableKeypadMsg),
    ENTRY_L1L(dm_enableKeypadPatch, DISPID_enableKeypadPatch),
    ENTRY_L1L(dm_enableKeypadSync, DISPID_enableKeypadSync),
    ENTRY_L1L(dm_enableMouseMsg, DISPID_enableMouseMsg),
    ENTRY_L2L(dm_enableMouseSync, DISPID_enableMouseSync),
    ENTRY_L1L(dm_enableRealKeypad, DISPID_enableRealKeypad),
    ENTRY_L3L(dm_enableRealMouse, DISPID_enableRealMouse),
    ENTRY_L1L(dm_enableSpeedDx, DISPID_enableSpeedDx),
    ENTRY_L0(dm_getFps, DISPID_getFps),
    ENTRY_L1L(dm_setDisplayDelay, DISPID_setDisplayDelay),
    ENTRY_L1L(dm_setDisplayRefreshDelay, DISPID_setDisplayRefreshDelay),
    ENTRY_L1L(dm_setInputDm, DISPID_setInputDm),
    ENTRY_L1L(dm_hackSpeed, DISPID_hackSpeed),
    ENTRY_L2L(dm_downCpu, DISPID_downCpu),
    ENTRY_P1L(dm_switchBindWindow, DISPID_switchBindWindow),
    // ====== 模块6: 内存操作 ======
    ENTRY_L1L(dm_openProcess, DISPID_openProcess),
    ENTRY_P1L1S(dm_getModuleBaseAddr, DISPID_getModuleBaseAddr),
    ENTRY_P1L1S(dm_getModuleSize, DISPID_getModuleSize),
    ENTRY_L3S(dm_getRemoteApiAddress, DISPID_getRemoteApiAddress),
    ENTRY_SPECIAL(H_readInt, DISPID_readInt, 2, 2),
    ENTRY_L3L(dm_readIntAddr, DISPID_readIntAddr),
    ENTRY_SPECIAL(H_readFloat, DISPID_readFloat, 2, 2),
    ENTRY_SPECIAL(H_readDouble, DISPID_readDouble, 2, 2),
    ENTRY_L4L(dm_readString, DISPID_readString),
    ENTRY_L3L(dm_readData, DISPID_readData),
    ENTRY_L3L(dm_readDataToBin, DISPID_readDataToBin),
    ENTRY_L1S1L(dm_findInt, DISPID_findInt),
    ENTRY_L4L(dm_writeInt, DISPID_writeInt),
    ENTRY_P1L1L1F(dm_writeFloat, DISPID_writeFloat),
    ENTRY_P1L2L1S(dm_writeString, DISPID_writeString),
    ENTRY_P1L1L1S(dm_writeData, DISPID_writeData),
    ENTRY_L3L(dm_virtualAllocEx, DISPID_virtualAllocEx),
    ENTRY_P1L1L(dm_virtualFreeEx, DISPID_virtualFreeEx),
    ENTRY_SPECIAL(H_virtualProtectEx, DISPID_virtualProtectEx, 4, 4),
    ENTRY_P1L(dm_freeProcessMemory, DISPID_freeProcessMemory),
    ENTRY_P1S(dm_getCommandLine, DISPID_getCommandLine),
    ENTRY_L1L(dm_terminateProcess, DISPID_terminateProcess),
    ENTRY_L1L(dm_terminateProcessTree, DISPID_terminateProcessTree),
    ENTRY_L1L(dm_setMemoryFindResultToFile, DISPID_setMemoryFindResultToFile),
    ENTRY_L1L(dm_setMemoryHwndAsProcessId, DISPID_setMemoryHwndAsProcessId),
    ENTRY_L1L(dm_setParam64ToPointer, DISPID_setParam64ToPointer),
    ENTRY_L3L(dm_int64ToInt32, DISPID_int64ToInt32),
    ENTRY_L1S1L(dm_intToData, DISPID_intToData),
    ENTRY_L1L(dm_floatToData, DISPID_floatToData),
    ENTRY_L1L(dm_doubleToData, DISPID_doubleToData),
    ENTRY_L1S1L(dm_stringToData, DISPID_stringToData),
    // ====== 模块7: 文件操作 ======
    ENTRY_L2S(dm_writeFile, DISPID_writeFile),
    ENTRY_S1S(dm_readFile, DISPID_readFile),
    ENTRY_L1S(dm_deleteFile, DISPID_deleteFile),
    ENTRY_L2S1L(dm_copyFile, DISPID_copyFile),
    ENTRY_L2S(dm_moveFile, DISPID_moveFile),
    ENTRY_L1S(dm_createFolder, DISPID_createFolder),
    ENTRY_L1S(dm_deleteFolder, DISPID_deleteFolder),
    ENTRY_L1S(dm_isFileExist, DISPID_isFileExist),
    ENTRY_L1S(dm_isFolderExist, DISPID_isFolderExist),
    ENTRY_L1S(dm_getFileLength, DISPID_getFileLength),
    ENTRY_S1S(dm_getRealPath, DISPID_getRealPath),
    ENTRY_S0(dm_selectFile, DISPID_selectFile),
    ENTRY_S0(dm_selectDirectory, DISPID_selectDirectory),
    ENTRY_L1S1L(dm_downloadFile, DISPID_downloadFile),
    ENTRY_L2S(dm_encodeFile, DISPID_encodeFile),
    ENTRY_L2S(dm_decodeFile, DISPID_decodeFile),
    ENTRY_L3S(dm_writeIni, DISPID_writeIni),
    ENTRY_L2S(dm_readIni, DISPID_readIni),
    ENTRY_L2S(dm_deleteIni, DISPID_deleteIni),
    ENTRY_L2S(dm_enumIniKey, DISPID_enumIniKey),
    ENTRY_L1S(dm_enumIniSection, DISPID_enumIniSection),
    ENTRY_L5S(dm_writeIniPwd, DISPID_writeIniPwd),
    ENTRY_S4S(dm_readIniPwd, DISPID_readIniPwd),
    ENTRY_L4S(dm_deleteIniPwd, DISPID_deleteIniPwd),
    ENTRY_S3S(dm_enumIniKeyPwd, DISPID_enumIniKeyPwd),
    ENTRY_S2S(dm_enumIniSectionPwd, DISPID_enumIniSectionPwd),
    // ====== 模块8: 文字识别 ======
    ENTRY_L1S1L(dm_setDict, DISPID_setDict),
    ENTRY_L1S(dm_setDictPwd, DISPID_setDictPwd),
    ENTRY_L1L(dm_useDict, DISPID_useDict),
    ENTRY_L2L(dm_getDict, DISPID_getDict),
    ENTRY_L1L(dm_getDictCount, DISPID_getDictCount),
    ENTRY_L1S1L(dm_getDictInfo, DISPID_getDictInfo),
    ENTRY_L0(dm_getNowDict, DISPID_getNowDict),
    ENTRY_L1S(dm_addDict, DISPID_addDict),
    ENTRY_L1S1L(dm_saveDict, DISPID_saveDict),
    ENTRY_L1L(dm_clearDict, DISPID_clearDict),
    ENTRY_L1L(dm_enableShareDict, DISPID_enableShareDict),
    ENTRY_S2L1S(dm_ocr, DISPID_ocr),
    ENTRY_S2L1S(dm_ocrEx, DISPID_ocrEx),
    ENTRY_S2L1S(dm_ocrExOne, DISPID_ocrExOne),
    ENTRY_L4L1S(dm_ocrInFile, DISPID_ocrInFile),
    ENTRY_SPECIAL(H_findStr, DISPID_findStr, 7, 7),
    ENTRY_S2L1S(dm_findStrE, DISPID_findStrE),
    ENTRY_S2L1S(dm_findStrEx, DISPID_findStrEx),
    ENTRY_L4L1S(dm_findStrS, DISPID_findStrS),
    ENTRY_S2L1S(dm_findStrExS, DISPID_findStrExS),
    ENTRY_SPECIAL(H_findStrFast, DISPID_findStrFast, 7, 7),
    ENTRY_S2L1S(dm_findStrFastE, DISPID_findStrFastE),
    ENTRY_S2L1S(dm_findStrFastEx, DISPID_findStrFastEx),
    ENTRY_L4L1S(dm_findStrFastS, DISPID_findStrFastS),
    ENTRY_S2L1S(dm_findStrFastExS, DISPID_findStrFastExS),
    ENTRY_S2L1S(dm_getWords, DISPID_getWords),
    ENTRY_S2L1S(dm_getWordsNoDict, DISPID_getWordsNoDict),
    ENTRY_L2L1S(dm_fetchWord, DISPID_fetchWord),
    ENTRY_L1S(dm_getResultCount, DISPID_getResultCount),
    ENTRY_SPECIAL(H_getResultPos, DISPID_getResultPos, 2, 2),
    ENTRY_L1S(dm_getWordResultCount, DISPID_getWordResultCount),
    ENTRY_SPECIAL(H_getWordResultPos, DISPID_getWordResultPos, 2, 2),
    ENTRY_L1S1L(dm_getWordResultStr, DISPID_getWordResultStr),
    ENTRY_L1L(dm_setColGapNoDict, DISPID_setColGapNoDict),
    ENTRY_L1L(dm_setRowGapNoDict, DISPID_setRowGapNoDict),
    ENTRY_L1L(dm_setWordGapNoDict, DISPID_setWordGapNoDict),
    ENTRY_L1L(dm_setWordLineHeightNoDict, DISPID_setWordLineHeightNoDict),
    ENTRY_L1L(dm_setExactOcr, DISPID_setExactOcr),
    ENTRY_L1L(dm_setMinColGap, DISPID_setMinColGap),
    ENTRY_L1L(dm_setMinRowGap, DISPID_setMinRowGap),
    ENTRY_L1L(dm_setWordGap, DISPID_setWordGap),
    ENTRY_L1L(dm_setWordLineHeight, DISPID_setWordLineHeight),
    // ====== 模块9: 系统操作 ======
    ENTRY_L2L(dm_beep, DISPID_beep),
    ENTRY_L1L(dm_delay, DISPID_delay),
    ENTRY_L2L(dm_delays, DISPID_delays),
    ENTRY_L1S1L(dm_runApp, DISPID_runApp),
    ENTRY_L1L(dm_stop, DISPID_stop),
    ENTRY_L1S(dm_play, DISPID_play),
    ENTRY_L1L(dm_exitOs, DISPID_exitOs),
    ENTRY_L3L(dm_setScreen, DISPID_setScreen),
    ENTRY_L0(dm_getScreenWidth, DISPID_getScreenWidth),
    ENTRY_L0(dm_getScreenHeight, DISPID_getScreenHeight),
    ENTRY_L0(dm_getScreenDepth, DISPID_getScreenDepth),
    ENTRY_L0(dm_getDPI, DISPID_getDPI),
    ENTRY_L0(dm_getTime, DISPID_getTime),
    ENTRY_L0(dm_getOsType, DISPID_getOsType),
    ENTRY_L0(dm_getOsBuildNumber, DISPID_getOsBuildNumber),
    ENTRY_L0(dm_is64Bit, DISPID_is64Bit),
    ENTRY_L0(dm_isSurrpotVt, DISPID_isSurrpotVt),
    ENTRY_L2L(dm_getSystemInfo, DISPID_getSystemInfo),
    ENTRY_L1L(dm_getDir, DISPID_getDir),
    ENTRY_L0(dm_getLocale, DISPID_getLocale),
    ENTRY_L0(dm_getCpuType, DISPID_getCpuType),
    ENTRY_L0(dm_getCpuUsage, DISPID_getCpuUsage),
    ENTRY_L0(dm_getMemoryUsage, DISPID_getMemoryUsage),
    ENTRY_S0(dm_getDiskSerial, DISPID_getDiskSerial),
    ENTRY_S0(dm_getDiskModel, DISPID_getDiskModel),
    ENTRY_S0(dm_getDiskReversion, DISPID_getDiskReversion),
    ENTRY_S0(dm_getMachineCode, DISPID_getMachineCode),
    ENTRY_S0(dm_getMachineCodeNoMac, DISPID_getMachineCodeNoMac),
    ENTRY_S0(dm_getNetTime, DISPID_getNetTime),
    ENTRY_S1S(dm_getNetTimeByIp, DISPID_getNetTimeByIp),
    ENTRY_L1L(dm_getNetTimeSafe, DISPID_getNetTimeSafe),
    ENTRY_S0(dm_getClipboard, DISPID_getClipboard),
    ENTRY_L1S(dm_setClipboard, DISPID_setClipboard),
    ENTRY_L1L(dm_getDisplayInfo, DISPID_getDisplayInfo),
    ENTRY_L1L(dm_disableCloseDisplayAndSleep, DISPID_disableCloseDisplayAndSleep),
    ENTRY_L1L(dm_disablePowerSave, DISPID_disablePowerSave),
    ENTRY_L1L(dm_disableScreenSave, DISPID_disableScreenSave),
    ENTRY_L0(dm_disableFontSmooth, DISPID_disableFontSmooth),
    ENTRY_L0(dm_enableFontSmooth, DISPID_enableFontSmooth),
    ENTRY_L0(dm_checkFontSmooth, DISPID_checkFontSmooth),
    ENTRY_L0(dm_checkUAC, DISPID_checkUAC),
    ENTRY_L1L(dm_setUAC, DISPID_setUAC),
    ENTRY_L1L(dm_setDisplayAcceler, DISPID_setDisplayAcceler),
    ENTRY_P1L1L(dm_showTaskBarIcon, DISPID_showTaskBarIcon),
    // ====== 模块10: 杂项 ======
    ENTRY_L0(dm_enterCri, DISPID_enterCri),
    ENTRY_L0(dm_leaveCri, DISPID_leaveCri),
    ENTRY_L0(dm_initCri, DISPID_initCri),
    ENTRY_L0(dm_releaseRef, DISPID_releaseRef),
    ENTRY_L1L(dm_setExitThread, DISPID_setExitThread),
    ENTRY_L1S1L(dm_executeCmd, DISPID_executeCmd),
    ENTRY_P1L1S(dm_activeInputMethod, DISPID_activeInputMethod),
    ENTRY_P1L1S(dm_checkInputMethod, DISPID_checkInputMethod),
    ENTRY_L1S(dm_findInputMethod, DISPID_findInputMethod),
    // ====== 模块11: 汇编 ======
    ENTRY_L1S(dm_asmAdd, DISPID_asmAdd),
    ENTRY_P1L1L(dm_asmCall, DISPID_asmCall),
    ENTRY_L4L(dm_asmCallEx, DISPID_asmCallEx),
    ENTRY_L0(dm_asmClear, DISPID_asmClear),
    ENTRY_L1L(dm_asmSetTimeout, DISPID_asmSetTimeout),
    ENTRY_L2L(dm_assemble, DISPID_assemble),
    ENTRY_L3L(dm_disAssemble, DISPID_disAssemble),
    ENTRY_L1L(dm_setAsmHwndAsProcessId, DISPID_setAsmHwndAsProcessId),
    ENTRY_L1L(dm_setShowAsmErrorMsg, DISPID_setShowAsmErrorMsg),
    // ====== 模块12: AI ======
    ENTRY_L1S(dm_loadAi, DISPID_loadAi),
    ENTRY_L1L(dm_aiEnableFindPicWindow, DISPID_aiEnableFindPicWindow),
    ENTRY_L4L1S(dm_aiFindPic, DISPID_aiFindPic),
    ENTRY_S2L1S(dm_aiFindPicEx, DISPID_aiFindPicEx),
    ENTRY_S2L1S(dm_aiYoloDetectObjects, DISPID_aiYoloDetectObjects),
    ENTRY_L1S1L(dm_aiYoloSetModel, DISPID_aiYoloSetModel),
    ENTRY_L0(dm_aiYoloFreeModel, DISPID_aiYoloFreeModel),
    ENTRY_L1L(dm_aiYoloUseModel, DISPID_aiYoloUseModel),
    ENTRY_L1S(dm_aiYoloSetVersion, DISPID_aiYoloSetVersion),
    ENTRY_S1S(dm_aiYoloObjectsToString, DISPID_aiYoloObjectsToString),
    ENTRY_L1S1L(dm_aiYoloSortsObjects, DISPID_aiYoloSortsObjects),
    // ====== 模块13: Foobar ======
    ENTRY_L5L(dm_createFoobarRect, DISPID_createFoobarRect),
    ENTRY_L5L(dm_createFoobarEllipse, DISPID_createFoobarEllipse),
    ENTRY_L5L(dm_createFoobarRoundRect, DISPID_createFoobarRoundRect),
    ENTRY_L4L1S(dm_createFoobarCustom, DISPID_createFoobarCustom),
    ENTRY_P1L(dm_foobarClose, DISPID_foobarClose),
    ENTRY_P1L(dm_foobarLock, DISPID_foobarLock),
    ENTRY_P1L(dm_foobarUnlock, DISPID_foobarUnlock),
    ENTRY_P1L(dm_foobarUpdate, DISPID_foobarUpdate),
    ENTRY_P1L1S2L(dm_foobarSetFont, DISPID_foobarSetFont),
    ENTRY_P1L1S1L(dm_foobarPrintText, DISPID_foobarPrintText),
    ENTRY_P1L5L(dm_foobarFillRect, DISPID_foobarFillRect),
    ENTRY_P1L2L1S(dm_foobarDrawPic, DISPID_foobarDrawPic),
    ENTRY_P1L(dm_foobarClearText, DISPID_foobarClearText),
    ENTRY_P1L4L(dm_foobarTextRect, DISPID_foobarTextRect),
    ENTRY_P1L1L(dm_foobarTextLineGap, DISPID_foobarTextLineGap),
    ENTRY_P1L1L(dm_foobarTextPrintDir, DISPID_foobarTextPrintDir),
    ENTRY_P1L1S2L(dm_foobarStartGif, DISPID_foobarStartGif),
    ENTRY_P1L(dm_foobarStopGif, DISPID_foobarStopGif),
    // ====== 模块14: 答题 ======
    ENTRY_L5L(dm_faqCapture, DISPID_faqCapture),
    ENTRY_L1S1L(dm_faqCaptureFromFile, DISPID_faqCaptureFromFile),
    ENTRY_L5L(dm_faqCaptureString, DISPID_faqCaptureString),
    ENTRY_L1L(dm_faqGetSize, DISPID_faqGetSize),
    ENTRY_L1S1L(dm_faqPost, DISPID_faqPost),
    ENTRY_L2L(dm_faqSend, DISPID_faqSend),
    ENTRY_L1L(dm_faqFetch, DISPID_faqFetch),
    ENTRY_L0(dm_faqCancel, DISPID_faqCancel),
    ENTRY_L0(dm_faqIsPosted, DISPID_faqIsPosted),
    // ====== 模块15: 算法 ======
    ENTRY_L4L1S(dm_excludePos, DISPID_excludePos),
    ENTRY_L1S1L(dm_findNearestPos, DISPID_findNearestPos),
    ENTRY_L1S1L(dm_sortPosDistance, DISPID_sortPosDistance),
    // ====== 模块16: 防护盾 ======
    ENTRY_L1S1L(dm_dmGuard, DISPID_dmGuard),
    ENTRY_L2S(dm_dmGuardExtract, DISPID_dmGuardExtract),
    ENTRY_L2S(dm_dmGuardLoadCustom, DISPID_dmGuardLoadCustom),
    ENTRY_L2S(dm_dmGuardParams, DISPID_dmGuardParams),
    ENTRY_L0(dm_unLoadDriver, DISPID_unLoadDriver),
};

const UINT g_dispTableCount = sizeof(g_dispTable) / sizeof(g_dispTable[0]);

const DispFuncEntry* GetDispTable() { return g_dispTable; }
UINT GetDispTableCount() { return g_dispTableCount; }

static const DispFuncEntry* FindByName(const char* name) {
    for (UINT i = 0; i < g_dispTableCount; i++) {
        if (strcmp(g_dispTable[i].name, name) == 0)
            return &g_dispTable[i];
    }
    return nullptr;
}

// ============================================================================
// Invoke 分发函数 — 根据 FuncPattern 调用对应的 C 函数
// ============================================================================
static HRESULT DispatchByPattern(const DispFuncEntry* entry, VARIANT* ret,
                                  const VARIANT* args, UINT argCount) {
    if (entry->pattern == FuncPattern::SPECIAL) {
        // 特殊处理函数 — 直接调用
        auto handler = (HRESULT(*)(VARIANT*, const VARIANT*, UINT))entry->funcPtr;
        return handler(ret, args, argCount);
    }

    // 获取参数
    auto a0 = [&](UINT i) { return GetLongArg(args, i); };
    auto aS = [&](UINT i) { return GetStringArg(args, i); };
    auto aP = [&](UINT i) { return GetPtrArg(args, i); };
    auto aF = [&](UINT i) { return GetFloatArg(args, i); };
    auto aD = [&](UINT i) { return GetDoubleArg(args, i); };

    switch (entry->pattern) {
    case FuncPattern::L0: {
        auto f = (long(*)())entry->funcPtr;
        SetResultLong(ret, f()); return S_OK;
    }
    case FuncPattern::S0: {
        auto f = (const char*(*)())entry->funcPtr;
        SetResultString(ret, f()); return S_OK;
    }
    case FuncPattern::L1L: {
        auto f = (long(*)(long))entry->funcPtr;
        SetResultLong(ret, f(a0(0))); return S_OK;
    }
    case FuncPattern::L1S: {
        auto f = (long(*)(const char*))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str())); return S_OK;
    }
    case FuncPattern::L2L: {
        auto f = (long(*)(long, long))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1))); return S_OK;
    }
    case FuncPattern::L3L: {
        auto f = (long(*)(long, long, long))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1), a0(2))); return S_OK;
    }
    case FuncPattern::L4L: {
        auto f = (long(*)(long, long, long, long))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3))); return S_OK;
    }
    case FuncPattern::L5L: {
        auto f = (long(*)(long, long, long, long, long))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), a0(4))); return S_OK;
    }
    case FuncPattern::L2S: {
        auto f = (long(*)(const char*, const char*))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::L3S: {
        auto f = (long(*)(const char*, const char*, const char*))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::L1L1S: {
        auto f = (long(*)(long, const char*))entry->funcPtr;
        SetResultLong(ret, f(a0(0), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::L1S1L: {
        auto f = (long(*)(const char*, long))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), a0(1))); return S_OK;
    }
    case FuncPattern::L2L1S: {
        auto f = (long(*)(long, long, const char*))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::L1S2L: {
        auto f = (long(*)(const char*, long, long))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), a0(1), a0(2))); return S_OK;
    }
    case FuncPattern::L4L1S: {
        auto f = (long(*)(long, long, long, long, const char*))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), aS(4).c_str())); return S_OK;
    }
    case FuncPattern::S2L: {
        auto f = (const char*(*)(long, long))entry->funcPtr;
        SetResultString(ret, f(a0(0), a0(1))); return S_OK;
    }
    case FuncPattern::S2L1S: {
        auto f = (const char*(*)(long, long, const char*))entry->funcPtr;
        std::string s = aS(2); SetResultString(ret, f(a0(0), a0(1), s.c_str())); return S_OK;
    }
    case FuncPattern::S1S: {
        auto f = (const char*(*)(const char*))entry->funcPtr;
        SetResultString(ret, f(aS(0).c_str())); return S_OK;
    }
    case FuncPattern::L4L2S: {
        auto f = (long(*)(long, long, long, long, const char*, const char*))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), aS(4).c_str(), aS(5).c_str())); return S_OK;
    }
    case FuncPattern::S2L2S: {
        auto f = (const char*(*)(long, long, const char*, const char*))entry->funcPtr;
        SetResultString(ret, f(a0(0), a0(1), aS(2).c_str(), aS(3).c_str())); return S_OK;
    }
    case FuncPattern::L4L1S1L: {
        auto f = (long(*)(long, long, long, long, const char*, long))entry->funcPtr;
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), aS(4).c_str(), a0(5))); return S_OK;
    }
    case FuncPattern::L1L2S: {
        auto f = (long(*)(long, const char*, const char*))entry->funcPtr;
        SetResultLong(ret, f(a0(0), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::P1L: {
        auto f = (long(*)(intptr_t))entry->funcPtr;
        SetResultLong(ret, f(aP(0))); return S_OK;
    }
    case FuncPattern::P1L1S: {
        auto f = (long(*)(intptr_t, const char*))entry->funcPtr;
        SetResultLong(ret, f(aP(0), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::P1L1L: {
        auto f = (long(*)(intptr_t, long))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1))); return S_OK;
    }
    case FuncPattern::P1L2L: {
        auto f = (long(*)(intptr_t, long, long))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1), a0(2))); return S_OK;
    }
    case FuncPattern::P1L1L1S: {
        auto f = (long(*)(intptr_t, long, const char*))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::P1L2L1S: {
        auto f = (long(*)(intptr_t, long, long, const char*))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1), a0(2), aS(3).c_str())); return S_OK;
    }
    case FuncPattern::L2S1L: {
        auto f = (long(*)(const char*, const char*, long))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), a0(2))); return S_OK;
    }
    case FuncPattern::P1S: {
        auto f = (const char*(*)(intptr_t))entry->funcPtr;
        SetResultString(ret, f(aP(0))); return S_OK;
    }
    case FuncPattern::REGEX: {
        auto f = (long(*)(const char*, const char*, const char*))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::P1L1S1L: {
        auto f = (long(*)(intptr_t, const char*, long))entry->funcPtr;
        SetResultLong(ret, f(aP(0), aS(1).c_str(), a0(2))); return S_OK;
    }
    case FuncPattern::P1L1L1F: {
        auto f = (long(*)(intptr_t, long, float))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1), aF(2))); return S_OK;
    }
    case FuncPattern::P1L4L: {
        auto f = (long(*)(intptr_t, long, long, long, long))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1), a0(2), a0(3), a0(4))); return S_OK;
    }
    case FuncPattern::P1L5L: {
        auto f = (long(*)(intptr_t, long, long, long, long, long))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1), a0(2), a0(3), a0(4), a0(5))); return S_OK;
    }
    case FuncPattern::P1L1S2L: {
        auto f = (long(*)(intptr_t, const char*, long, long))entry->funcPtr;
        SetResultLong(ret, f(aP(0), aS(1).c_str(), a0(2), a0(3))); return S_OK;
    }
    case FuncPattern::P1L2L1S2L: {
        auto f = (long(*)(intptr_t, long, long, const char*, long, long))entry->funcPtr;
        SetResultLong(ret, f(aP(0), a0(1), a0(2), aS(3).c_str(), a0(4), a0(5))); return S_OK;
    }
    case FuncPattern::L4S: {
        auto f = (long(*)(const char*, const char*, const char*, const char*))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str(), aS(3).c_str())); return S_OK;
    }
    case FuncPattern::L5S: {
        auto f = (long(*)(const char*, const char*, const char*, const char*, const char*))entry->funcPtr;
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str(), aS(3).c_str(), aS(4).c_str())); return S_OK;
    }
    case FuncPattern::S2S: {
        auto f = (const char*(*)(const char*, const char*))entry->funcPtr;
        SetResultString(ret, f(aS(0).c_str(), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::S3S: {
        auto f = (const char*(*)(const char*, const char*, const char*))entry->funcPtr;
        SetResultString(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::S4S: {
        auto f = (const char*(*)(const char*, const char*, const char*, const char*))entry->funcPtr;
        SetResultString(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str(), aS(3).c_str())); return S_OK;
    }
    default:
        return DISP_E_MEMBERNOTFOUND;
    }
}

// ============================================================================
// CDmObject 实现
// ============================================================================

CDmObject::CDmObject() : m_refCount(1) {
    InterlockedIncrement(&g_dllRefCount);
}

CDmObject::~CDmObject() {
    InterlockedDecrement(&g_dllRefCount);
}

STDMETHODIMP CDmObject::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) return E_POINTER;
    if (riid == IID_IUnknown || riid == IID_IDispatch) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CDmObject::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) CDmObject::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) delete this;
    return ref;
}

STDMETHODIMP CDmObject::GetTypeInfoCount(UINT* pctinfo) {
    if (!pctinfo) return E_POINTER;
    *pctinfo = 0;
    return S_OK;
}

STDMETHODIMP CDmObject::GetTypeInfo(UINT, LCID, ITypeInfo**) {
    return E_NOTIMPL;
}

STDMETHODIMP CDmObject::GetIDsOfNames(REFIID, LPOLESTR* rgszNames, UINT cNames,
                                      LCID, DISPID* rgDispId) {
    if (!rgszNames || !rgDispId) return E_POINTER;
    if (cNames == 0) return S_OK;

    char nameBuf[256];
    int len = WideCharToMultiByte(CP_UTF8, 0, rgszNames[0], -1, nameBuf, sizeof(nameBuf), NULL, NULL);
    if (len <= 0) { rgDispId[0] = DISPID_UNKNOWN; return DISP_E_UNKNOWNNAME; }

    for (char* p = nameBuf; *p; ++p) *p = (char)tolower(*p);

    const DispFuncEntry* entry = FindByName(nameBuf);
    if (entry) {
        rgDispId[0] = entry->id;
        for (UINT i = 1; i < cNames; i++) rgDispId[i] = DISPID_UNKNOWN;
        return S_OK;
    }

    rgDispId[0] = DISPID_UNKNOWN;
    return DISP_E_UNKNOWNNAME;
}

STDMETHODIMP CDmObject::Invoke(DISPID dispIdMember, REFIID, LCID,
                               WORD wFlags, DISPPARAMS* pDispParams,
                               VARIANT* pVarResult, EXCEPINFO*, UINT*) {
    if (!(wFlags & DISPATCH_METHOD)) return DISP_E_MEMBERNOTFOUND;

    const DispFuncEntry* entry = nullptr;
    for (UINT i = 0; i < g_dispTableCount; i++) {
        if (g_dispTable[i].id == dispIdMember) { entry = &g_dispTable[i]; break; }
    }
    if (!entry) return DISP_E_MEMBERNOTFOUND;

    UINT argCount = pDispParams ? pDispParams->cArgs : 0;
    if (argCount < entry->minArgs || argCount > entry->maxArgs)
        return DISP_E_BADPARAMCOUNT;

    // 反转参数数组（rgvarg[0] 是最后一个参数）
    VARIANT* reversedArgs = nullptr;
    if (argCount > 0 && pDispParams) {
        reversedArgs = new VARIANT[argCount];
        for (UINT i = 0; i < argCount; i++)
            reversedArgs[i] = pDispParams->rgvarg[argCount - 1 - i];
    }

    {
        StateGuard guard(&m_state);
        HRESULT hr = DispatchByPattern(entry, pVarResult, reversedArgs, argCount);
        delete[] reversedArgs;
        return hr;
    }
}

// ============================================================================
// CDmClassFactory 实现
// ============================================================================

CDmClassFactory::CDmClassFactory() : m_refCount(1) {
    InterlockedIncrement(&g_dllRefCount);
}

CDmClassFactory::~CDmClassFactory() {
    InterlockedDecrement(&g_dllRefCount);
}

STDMETHODIMP CDmClassFactory::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) return E_POINTER;
    if (riid == IID_IUnknown || riid == IID_IClassFactory) {
        *ppvObject = this; AddRef(); return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CDmClassFactory::AddRef() { return InterlockedIncrement(&m_refCount); }
STDMETHODIMP_(ULONG) CDmClassFactory::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) delete this;
    return ref;
}

STDMETHODIMP CDmClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) {
    if (!ppvObject) return E_POINTER;
    if (pUnkOuter) return CLASS_E_NOAGGREGATION;
    CDmObject* obj = new (std::nothrow) CDmObject();
    if (!obj) return E_OUTOFMEMORY;
    HRESULT hr = obj->QueryInterface(riid, ppvObject);
    obj->Release();
    return hr;
}

STDMETHODIMP CDmClassFactory::LockServer(BOOL fLock) {
    if (fLock) InterlockedIncrement(&g_dllLockCount);
    else InterlockedDecrement(&g_dllLockCount);
    return S_OK;
}

// ============================================================================
// DLL 导出函数
// ============================================================================

// 保存 DLL 模块句柄（由 DllMain 设置）
static HMODULE g_comModule = NULL;

void dm_setComModule(HMODULE hMod) {
    g_comModule = hMod;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    if (!ppv) return E_POINTER;
    if (rclsid != CLSID_DmSoft) return CLASS_E_CLASSNOTAVAILABLE;
    CDmClassFactory* factory = new (std::nothrow) CDmClassFactory();
    if (!factory) return E_OUTOFMEMORY;
    HRESULT hr = factory->QueryInterface(riid, ppv);
    factory->Release();
    return hr;
}

STDAPI DllCanUnloadNow() {
    return (g_dllRefCount == 0 && g_dllLockCount == 0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer() {
    WCHAR dllPath[MAX_PATH];
    if (!GetModuleFileNameW(g_comModule, dllPath, MAX_PATH))
        return HRESULT_FROM_WIN32(GetLastError());

    HKEY hKeyClsid = NULL, hKeyApp = NULL, hKeyInproc = NULL;
    HKEY hKeyProgId = NULL, hKeyProgIdClsid = NULL;

    WCHAR clsidStr[64];
    StringFromGUID2(CLSID_DmSoft, clsidStr, 64);

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, L"CLSID", 0, NULL, 0, KEY_WRITE, NULL, &hKeyClsid, NULL) != ERROR_SUCCESS)
        goto error;
    if (RegCreateKeyExW(hKeyClsid, clsidStr, 0, NULL, 0, KEY_WRITE, NULL, &hKeyApp, NULL) != ERROR_SUCCESS)
        goto error;

    WCHAR progIdW[] = PROGID_DM;
    RegSetValueExW(hKeyApp, NULL, 0, REG_SZ, (BYTE*)progIdW, sizeof(progIdW));

    if (RegCreateKeyExW(hKeyApp, L"InprocServer32", 0, NULL, 0, KEY_WRITE, NULL, &hKeyInproc, NULL) != ERROR_SUCCESS)
        goto error;
    RegSetValueExW(hKeyInproc, NULL, 0, REG_SZ, (BYTE*)dllPath, (DWORD)((wcslen(dllPath) + 1) * sizeof(WCHAR)));
    WCHAR both[] = L"Both";
    RegSetValueExW(hKeyInproc, L"ThreadingModel", 0, REG_SZ, (BYTE*)both, sizeof(both));

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, progIdW, 0, NULL, 0, KEY_WRITE, NULL, &hKeyProgId, NULL) != ERROR_SUCCESS)
        goto error;
    RegSetValueExW(hKeyProgId, NULL, 0, REG_SZ, (BYTE*)progIdW, sizeof(progIdW));

    if (RegCreateKeyExW(hKeyProgId, L"CLSID", 0, NULL, 0, KEY_WRITE, NULL, &hKeyProgIdClsid, NULL) != ERROR_SUCCESS)
        goto error;
    RegSetValueExW(hKeyProgIdClsid, NULL, 0, REG_SZ, (BYTE*)clsidStr, (DWORD)((wcslen(clsidStr) + 1) * sizeof(WCHAR)));

    RegCloseKey(hKeyInproc); RegCloseKey(hKeyApp); RegCloseKey(hKeyClsid);
    RegCloseKey(hKeyProgIdClsid); RegCloseKey(hKeyProgId);
    return S_OK;

error:
    if (hKeyInproc) RegCloseKey(hKeyInproc);
    if (hKeyApp) RegCloseKey(hKeyApp);
    if (hKeyClsid) RegCloseKey(hKeyClsid);
    if (hKeyProgIdClsid) RegCloseKey(hKeyProgIdClsid);
    if (hKeyProgId) RegCloseKey(hKeyProgId);
    return HRESULT_FROM_WIN32(GetLastError());
}

STDAPI DllUnregisterServer() {
    WCHAR clsidStr[64];
    StringFromGUID2(CLSID_DmSoft, clsidStr, 64);

    WCHAR clsidKey[128];
    wcscpy_s(clsidKey, L"CLSID\\");
    wcscat_s(clsidKey, clsidStr);

    WCHAR inprocKey[256];
    wcscpy_s(inprocKey, clsidKey);
    wcscat_s(inprocKey, L"\\InprocServer32");
    RegDeleteTreeW(HKEY_CLASSES_ROOT, inprocKey);
    RegDeleteTreeW(HKEY_CLASSES_ROOT, clsidKey);

    WCHAR progIdW[] = PROGID_DM;
    RegDeleteTreeW(HKEY_CLASSES_ROOT, progIdW);

    return S_OK;
}