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
#include <vector>
#include <unordered_map>
#include <mutex>
#include <cassert>
#include <cstdio>
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
// 匿名命名空间 — 内部辅助函数与数据，避免符号污染
// ============================================================================
namespace {

// ============================================================================
// 辅助函数 — VARIANT 参数提取
// ============================================================================

/// 从 VARIANT 数组中提取 long 值（支持 VT_I4/I2/R4/R8/BOOL 隐式转换）
/// @param args  VARIANT 参数数组（已反转，args[0] 是第一个参数）
/// @param index 参数索引
/// @return 提取的 long 值，类型不匹配时返回 0
static long GetLongArg(const VARIANT* args, UINT index) {
    assert(args != nullptr);
    const VARIANT* v = &args[index];
    if (v->vt == VT_I4) return v->lVal;
    if (v->vt == VT_I2) return v->iVal;
    if (v->vt == VT_R4) return static_cast<long>(v->fltVal);
    if (v->vt == VT_R8) return static_cast<long>(v->dblVal);
    if (v->vt == VT_BOOL) return v->boolVal ? 1 : 0;
    return 0;
}

/// 从 VARIANT 数组中提取 float 值
static float GetFloatArg(const VARIANT* args, UINT index) {
    assert(args != nullptr);
    const VARIANT* v = &args[index];
    if (v->vt == VT_R4) return v->fltVal;
    if (v->vt == VT_R8) return static_cast<float>(v->dblVal);
    if (v->vt == VT_I4) return static_cast<float>(v->lVal);
    return 0.0f;
}

/// 从 VARIANT 数组中提取 double 值
static double GetDoubleArg(const VARIANT* args, UINT index) {
    assert(args != nullptr);
    const VARIANT* v = &args[index];
    if (v->vt == VT_R8) return v->dblVal;
    if (v->vt == VT_R4) return static_cast<double>(v->fltVal);
    if (v->vt == VT_I4) return static_cast<double>(v->lVal);
    return 0.0;
}

/// 从 VARIANT 数组中提取字符串（BSTR → UTF-8 std::string）
static std::string GetStringArg(const VARIANT* args, UINT index) {
    assert(args != nullptr);
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

/// 从 VARIANT 数组中提取指针（long → intptr_t）
static intptr_t GetPtrArg(const VARIANT* args, UINT index) {
    return static_cast<intptr_t>(GetLongArg(args, index));
}

// ============================================================================
// 辅助函数 — 设置 VARIANT 返回值
// ============================================================================

/// 设置 long 返回值
static void SetResultLong(VARIANT* ret, long val) {
    if (!ret) return;
    ret->vt = VT_I4;
    ret->lVal = val;
}

/// 设置字符串返回值（UTF-8 → BSTR）
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

/// 设置 double 返回值
static void SetResultDouble(VARIANT* ret, double val) {
    if (!ret) return;
    ret->vt = VT_R8;
    ret->dblVal = val;
}

/// 设置坐标返回值 "x|y"（用于 findColor 等返回位置的方法）
static void SetResultCoord(VARIANT* ret, long x, long y) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%ld|%ld", x, y);
    SetResultString(ret, buf);
}

/// 设置矩形返回值 "x1|y1|x2|y2"（用于 getWindowRect 等返回矩形的方法）
static void SetResultCoord4(VARIANT* ret, long x1, long y1, long x2, long y2) {
    char buf[128];
    snprintf(buf, sizeof(buf), "%ld|%ld|%ld|%ld", x1, y1, x2, y2);
    SetResultString(ret, buf);
}

// ============================================================================
// 特殊处理函数（用于有输出参数的函数）
// ============================================================================

// findColor(x1, y1, x2, y2, color, sim, dir, &x, &y) → "x|y" 或 ""
static HRESULT H_findColor(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string color = GetStringArg(args, 4);
    long r = findColor(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                          GetLongArg(args, 3), color.c_str(), GetLongArg(args, 5),
                          GetLongArg(args, 6), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// findColorBlock (9 args)
static HRESULT H_findColorBlock9(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 9) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string color = GetStringArg(args, 4);
    long r = findColorBlock(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                               GetLongArg(args, 3), color.c_str(), GetLongArg(args, 5),
                               GetLongArg(args, 6), GetLongArg(args, 7), GetLongArg(args, 8), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// findMultiColor
static HRESULT H_findMultiColor(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 8) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = findMultiColor(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                               GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                               GetLongArg(args, 6), GetLongArg(args, 7), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// findPic
static HRESULT H_findPic(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 8) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = findPic(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                        GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                        GetLongArg(args, 6), GetLongArg(args, 7), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// findStr
static HRESULT H_findStr(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = findStr(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                        GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                        GetLongArg(args, 6), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// findStrFast
static HRESULT H_findStrFast(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5);
    long r = findStrFast(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                            GetLongArg(args, 3), s0.c_str(), s1.c_str(),
                            GetLongArg(args, 6), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// getCursorPos
static HRESULT H_getCursorPos(VARIANT* ret, const VARIANT*, UINT) {
    long x = 0, y = 0; getCursorPos(&x, &y);
    SetResultCoord(ret, x, y);
    return S_OK;
}

// getWindowRect
static HRESULT H_getWindowRect(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 1) return DISP_E_BADPARAMCOUNT;
    long x1=0,y1=0,x2=0,y2=0;
    long r = getWindowRect(GetPtrArg(args,0), &x1,&y1,&x2,&y2);
    if (r == 0) SetResultCoord4(ret, x1, y1, x2, y2);
    else SetResultString(ret, "");
    return S_OK;
}

// getClientRect
static HRESULT H_getClientRect(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 1) return DISP_E_BADPARAMCOUNT;
    long x1=0,y1=0,x2=0,y2=0;
    long r = getClientRect(GetPtrArg(args,0), &x1,&y1,&x2,&y2);
    if (r == 0) SetResultCoord4(ret, x1, y1, x2, y2);
    else SetResultString(ret, "");
    return S_OK;
}

// getClientSize
static HRESULT H_getClientSize(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 1) return DISP_E_BADPARAMCOUNT;
    long w=0,h=0;
    long r = getClientSize(GetPtrArg(args,0), &w, &h);
    if (r == 0) SetResultCoord(ret, w, h);
    else SetResultString(ret, "");
    return S_OK;
}

// clientToScreen
static HRESULT H_clientToScreen(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    long x = GetLongArg(args,1), y = GetLongArg(args,2);
    long r = clientToScreen(GetPtrArg(args,0), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// screenToClient
static HRESULT H_screenToClient(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    long x = GetLongArg(args,1), y = GetLongArg(args,2);
    long r = screenToClient(GetPtrArg(args,0), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// getResultPos
static HRESULT H_getResultPos(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    long x=0,y=0; std::string s = GetStringArg(args,0);
    long r = getResultPos(s.c_str(), GetLongArg(args,1), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// getWordResultPos
static HRESULT H_getWordResultPos(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    long x=0,y=0; std::string s = GetStringArg(args,0);
    long r = getWordResultPos(s.c_str(), GetLongArg(args,1), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// virtualProtectEx
static HRESULT H_virtualProtectEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    long old = 0;
    long r = virtualProtectEx(GetPtrArg(args,0), GetLongArg(args,1), GetLongArg(args,2), GetLongArg(args,3), &old);
    if (r == 0) { char buf[64]; snprintf(buf, sizeof(buf), "%ld", old); SetResultString(ret, buf); }
    else SetResultString(ret, "");
    return S_OK;
}

// readInt
static HRESULT H_readInt(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    long val = 0;
    readInt(GetPtrArg(args,0), GetLongArg(args,1), &val);
    SetResultLong(ret, val);
    return S_OK;
}

// readFloat
static HRESULT H_readFloat(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    float val = 0;
    readFloat(GetPtrArg(args,0), GetLongArg(args,1), &val);
    SetResultDouble(ret, val);
    return S_OK;
}

// readDouble
static HRESULT H_readDouble(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    double val = 0;
    readDouble(GetPtrArg(args,0), GetLongArg(args,1), &val);
    SetResultDouble(ret, val);
    return S_OK;
}

// getScreenDataBmp
static HRESULT H_getScreenDataBmp(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    long size = 0;
    const char* data = getScreenDataBmp(GetLongArg(args,0), GetLongArg(args,1),
                                           GetLongArg(args,2), GetLongArg(args,3), &size);
    SetResultString(ret, data);
    return S_OK;
}

// ============================================================================
// 补充特殊处理函数 — 为之前缺失的 COM 分发表条目提供支持
// ============================================================================

// --- 模块 2: 窗口操作 ---

// enumWindowSuper(spec1, flag1, spec2, flag2, filter) -> long
static HRESULT H_enumWindowSuper(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 5) return DISP_E_BADPARAMCOUNT;
    std::string s0 = GetStringArg(args, 0), s2 = GetStringArg(args, 2);
    long r = enumWindowSuper(s0.c_str(), GetLongArg(args, 1), s2.c_str(),
                                GetLongArg(args, 3), GetLongArg(args, 4));
    SetResultLong(ret, r);
    return S_OK;
}

// --- 模块 6: 内存操作 ---

// readFloatAddr(hwnd, addr) -> float value
static HRESULT H_readFloatAddr(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    float val = 0;
    readFloatAddr(GetPtrArg(args, 0), GetLongArg(args, 1), &val);
    SetResultDouble(ret, val);
    return S_OK;
}

// readDoubleAddr(hwnd, addr) -> double value
static HRESULT H_readDoubleAddr(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    double val = 0;
    readDoubleAddr(GetPtrArg(args, 0), GetLongArg(args, 1), &val);
    SetResultDouble(ret, val);
    return S_OK;
}

// readStringAddr(hwnd, addr, type, len) -> string
static HRESULT H_readStringAddr(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    const char* result = readStringAddr(GetPtrArg(args, 0), GetLongArg(args, 1),
                                           GetLongArg(args, 2), GetLongArg(args, 3));
    SetResultString(ret, result);
    return S_OK;
}

// readDataAddr(hwnd, addr, len) -> string
static HRESULT H_readDataAddr(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    const char* result = readDataAddr(GetPtrArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2));
    SetResultString(ret, result);
    return S_OK;
}

// findIntEx(hwnd, addr_range, int_min, int_max, step, multi_thread, mode) -> string
static HRESULT H_findIntEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 1);
    const char* result = findIntEx(GetPtrArg(args, 0), s.c_str(),
                                      GetLongArg(args, 2), GetLongArg(args, 3),
                                      GetLongArg(args, 4), GetLongArg(args, 5), GetLongArg(args, 6));
    SetResultString(ret, result);
    return S_OK;
}

// findFloat(hwnd, addr_range, float_min, float_max) -> string
static HRESULT H_findFloat(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 1);
    const char* result = findFloat(GetPtrArg(args, 0), s.c_str(),
                                      GetFloatArg(args, 2), GetFloatArg(args, 3));
    SetResultString(ret, result);
    return S_OK;
}

// findFloatEx(hwnd, addr_range, float_min, float_max, step, multi_thread, mode) -> string
static HRESULT H_findFloatEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 1);
    const char* result = findFloatEx(GetPtrArg(args, 0), s.c_str(),
                                        GetFloatArg(args, 2), GetFloatArg(args, 3),
                                        GetLongArg(args, 4), GetLongArg(args, 5), GetLongArg(args, 6));
    SetResultString(ret, result);
    return S_OK;
}

// findDouble(hwnd, addr_range, double_min, double_max) -> string
static HRESULT H_findDouble(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 1);
    const char* result = findDouble(GetPtrArg(args, 0), s.c_str(),
                                       GetDoubleArg(args, 2), GetDoubleArg(args, 3));
    SetResultString(ret, result);
    return S_OK;
}

// findDoubleEx(hwnd, addr_range, double_min, double_max, step, multi_thread, mode) -> string
static HRESULT H_findDoubleEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 1);
    const char* result = findDoubleEx(GetPtrArg(args, 0), s.c_str(),
                                         GetDoubleArg(args, 2), GetDoubleArg(args, 3),
                                         GetLongArg(args, 4), GetLongArg(args, 5), GetLongArg(args, 6));
    SetResultString(ret, result);
    return S_OK;
}

// findString(hwnd, addr_range, string_value, type) -> string
static HRESULT H_findString(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 4) return DISP_E_BADPARAMCOUNT;
    std::string s1 = GetStringArg(args, 1), s2 = GetStringArg(args, 2);
    const char* result = findString(GetPtrArg(args, 0), s1.c_str(), s2.c_str(), GetLongArg(args, 3));
    SetResultString(ret, result);
    return S_OK;
}

// findStringEx(hwnd, addr_range, string_value, type, step, multi_thread, mode) -> string
static HRESULT H_findStringEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    std::string s1 = GetStringArg(args, 1), s2 = GetStringArg(args, 2);
    const char* result = findStringEx(GetPtrArg(args, 0), s1.c_str(), s2.c_str(),
                                         GetLongArg(args, 3), GetLongArg(args, 4),
                                         GetLongArg(args, 5), GetLongArg(args, 6));
    SetResultString(ret, result);
    return S_OK;
}

// findData(hwnd, addr_range, data) -> string
static HRESULT H_findData(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    std::string s1 = GetStringArg(args, 1), s2 = GetStringArg(args, 2);
    const char* result = findData(GetPtrArg(args, 0), s1.c_str(), s2.c_str());
    SetResultString(ret, result);
    return S_OK;
}

// findDataEx(hwnd, addr_range, data, step, multi_thread, mode) -> string
static HRESULT H_findDataEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 6) return DISP_E_BADPARAMCOUNT;
    std::string s1 = GetStringArg(args, 1), s2 = GetStringArg(args, 2);
    const char* result = findDataEx(GetPtrArg(args, 0), s1.c_str(), s2.c_str(),
                                       GetLongArg(args, 3), GetLongArg(args, 4), GetLongArg(args, 5));
    SetResultString(ret, result);
    return S_OK;
}

// writeDouble(hwnd, addr, double v) -> long
static HRESULT H_writeDouble(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    long r = writeDouble(GetPtrArg(args, 0), GetLongArg(args, 1), GetDoubleArg(args, 2));
    SetResultLong(ret, r);
    return S_OK;
}

// writeDoubleAddr(hwnd, addr, double v) -> long
static HRESULT H_writeDoubleAddr(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    long r = writeDoubleAddr(GetPtrArg(args, 0), GetLongArg(args, 1), GetDoubleArg(args, 2));
    SetResultLong(ret, r);
    return S_OK;
}

// virtualQueryEx(hwnd, addr, pmbi) -> string
static HRESULT H_virtualQueryEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    const char* result = virtualQueryEx(GetPtrArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2));
    SetResultString(ret, result);
    return S_OK;
}

// --- 模块 8: 文字识别 ---

// setDictMem(index, data, size) -> long
static HRESULT H_setDictMem(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 1);
    long r = setDictMem(GetLongArg(args, 0), s.c_str(), GetLongArg(args, 2));
    SetResultLong(ret, r);
    return S_OK;
}

// findStrWithFont(x1, y1, x2, y2, str, color, sim, font_name, font_flag, font_size) -> "x|y"
static HRESULT H_findStrWithFont(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 10) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5), s2 = GetStringArg(args, 7);
    long r = findStrWithFont(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                                GetLongArg(args, 3), s0.c_str(), s1.c_str(), GetLongArg(args, 6),
                                s2.c_str(), GetLongArg(args, 8), GetLongArg(args, 9), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// findStrWithFontE(x1, y1, x2, y2, str, color, sim, font_name, font_flag, font_size) -> string
static HRESULT H_findStrWithFontE(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 10) return DISP_E_BADPARAMCOUNT;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5), s2 = GetStringArg(args, 7);
    const char* result = findStrWithFontE(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                                             GetLongArg(args, 3), s0.c_str(), s1.c_str(), GetLongArg(args, 6),
                                             s2.c_str(), GetLongArg(args, 8), GetLongArg(args, 9));
    SetResultString(ret, result);
    return S_OK;
}

// findStrWithFontEx(x1, y1, x2, y2, str, color, sim, font_name, font_flag, font_size) -> string
static HRESULT H_findStrWithFontEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 10) return DISP_E_BADPARAMCOUNT;
    std::string s0 = GetStringArg(args, 4), s1 = GetStringArg(args, 5), s2 = GetStringArg(args, 7);
    const char* result = findStrWithFontEx(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                                              GetLongArg(args, 3), s0.c_str(), s1.c_str(), GetLongArg(args, 6),
                                              s2.c_str(), GetLongArg(args, 8), GetLongArg(args, 9));
    SetResultString(ret, result);
    return S_OK;
}

// --- 模块 12: AI ---

// loadAiMemory(data, size) -> long
static HRESULT H_loadAiMemory(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 2) return DISP_E_BADPARAMCOUNT;
    long r = loadAiMemory(GetLongArg(args, 0), GetLongArg(args, 1));
    SetResultLong(ret, r);
    return S_OK;
}

// aiFindPicMem(x1, y1, x2, y2, pic_info, sim, dir) -> "x|y"
static HRESULT H_aiFindPicMem(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long x = 0, y = 0;
    std::string s = GetStringArg(args, 4);
    long r = aiFindPicMem(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                             GetLongArg(args, 3), s.c_str(), GetLongArg(args, 5),
                             GetLongArg(args, 6), &x, &y);
    if (r == 0) SetResultCoord(ret, x, y);
    else SetResultString(ret, "");
    return S_OK;
}

// aiFindPicMemEx(x1, y1, x2, y2, pic_info, sim, dir) -> string
static HRESULT H_aiFindPicMemEx(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 4);
    const char* result = aiFindPicMemEx(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                                           GetLongArg(args, 3), s.c_str(), GetLongArg(args, 5),
                                           GetLongArg(args, 6));
    SetResultString(ret, result);
    return S_OK;
}

// aiYoloDetectObjectsToDataBmp(x1, y1, x2, y2, prob, iou, data, size, mode) -> long
static HRESULT H_aiYoloDetectObjectsToDataBmp(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 9) return DISP_E_BADPARAMCOUNT;
    long r = aiYoloDetectObjectsToDataBmp(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                                             GetLongArg(args, 3), GetFloatArg(args, 4), GetFloatArg(args, 5),
                                             GetLongArg(args, 6), GetLongArg(args, 7), GetLongArg(args, 8));
    SetResultLong(ret, r);
    return S_OK;
}

// aiYoloDetectObjectsToFile(x1, y1, x2, y2, prob, iou, file, mode) -> long
static HRESULT H_aiYoloDetectObjectsToFile(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 8) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 6);
    long r = aiYoloDetectObjectsToFile(GetLongArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                                          GetLongArg(args, 3), GetFloatArg(args, 4), GetFloatArg(args, 5),
                                          s.c_str(), GetLongArg(args, 7));
    SetResultLong(ret, r);
    return S_OK;
}

// aiYoloSetModelMemory(data, size, model_type) -> long
static HRESULT H_aiYoloSetModelMemory(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 3) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 2);
    long r = aiYoloSetModelMemory(GetLongArg(args, 0), GetLongArg(args, 1), s.c_str());
    SetResultLong(ret, r);
    return S_OK;
}

// --- 模块 13: Foobar ---

// foobarDrawText(hwnd, x, y, w, h, text, color, align) -> long
static HRESULT H_foobarDrawText(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 8) return DISP_E_BADPARAMCOUNT;
    std::string s = GetStringArg(args, 5);
    long r = foobarDrawText(GetPtrArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                               GetLongArg(args, 3), GetLongArg(args, 4), s.c_str(),
                               GetLongArg(args, 6), GetLongArg(args, 7));
    SetResultLong(ret, r);
    return S_OK;
}

// foobarDrawRect(hwnd, x1, y1, x2, y2, color, style) -> long
static HRESULT H_foobarDrawRect(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long r = foobarDrawRect(GetPtrArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                               GetLongArg(args, 3), GetLongArg(args, 4), GetLongArg(args, 5),
                               GetLongArg(args, 6));
    SetResultLong(ret, r);
    return S_OK;
}

// foobarDrawLine(hwnd, x1, y1, x2, y2, color, style) -> long
static HRESULT H_foobarDrawLine(VARIANT* ret, const VARIANT* args, UINT argCount) {
    if (argCount < 7) return DISP_E_BADPARAMCOUNT;
    long r = foobarDrawLine(GetPtrArg(args, 0), GetLongArg(args, 1), GetLongArg(args, 2),
                               GetLongArg(args, 3), GetLongArg(args, 4), GetLongArg(args, 5),
                               GetLongArg(args, 6));
    SetResultLong(ret, r);
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
    ENTRY_L2S(reg, DISPID_reg),
    ENTRY_REGEX(regEx, DISPID_regEx),
    ENTRY_L2S(regNoMac, DISPID_regNoMac),
    ENTRY_REGEX(regExNoMac, DISPID_regExNoMac),
    ENTRY_S0(ver, DISPID_ver),
    ENTRY_L0(getID, DISPID_getID),
    ENTRY_L0(getDmCount, DISPID_getDmCount),
    ENTRY_L0(getLastError, DISPID_getLastError),
    ENTRY_S0(getPath, DISPID_getPath),
    ENTRY_L1S(setPath, DISPID_setPath),
    ENTRY_S0(getBasePath, DISPID_getBasePath),
    ENTRY_L1L(setShowErrorMsg, DISPID_setShowErrorMsg),
    ENTRY_L1L(enablePicCache, DISPID_enablePicCache),
    // ====== 模块2: 窗口操作 ======
    ENTRY_L2S(findWindow, DISPID_findWindow),
    ENTRY_P1L1L1S(findWindowEx, DISPID_findWindowEx),
    ENTRY_L2S(findWindowByProcess, DISPID_findWindowByProcess),
    ENTRY_L3L(findWindowByProcessId, DISPID_findWindowByProcessId),
    ENTRY_L1S1L(findWindowSuper, DISPID_findWindowSuper),
    ENTRY_L1S1L(enumWindow, DISPID_enumWindow),
    ENTRY_L1S1L(enumWindowByProcess, DISPID_enumWindowByProcess),
    ENTRY_L1S1L(enumWindowByProcessId, DISPID_enumWindowByProcessId),
    ENTRY_SPECIAL(H_enumWindowSuper, DISPID_enumWindowSuper, 5, 5),
    ENTRY_L1S(enumProcess, DISPID_enumProcess),
    ENTRY_P1L1L(getWindow, DISPID_getWindow),
    ENTRY_L0(getForegroundWindow, DISPID_getForegroundWindow),
    ENTRY_L0(getForegroundFocus, DISPID_getForegroundFocus),
    ENTRY_L0(getMousePointWindow, DISPID_getMousePointWindow),
    ENTRY_L2L(getPointWindow, DISPID_getPointWindow),
    ENTRY_L1L(getSpecialWindow, DISPID_getSpecialWindow),
    ENTRY_P1S(getWindowClass, DISPID_getWindowClass),
    ENTRY_P1L(getWindowProcessId, DISPID_getWindowProcessId),
    ENTRY_P1S(getWindowProcessPath, DISPID_getWindowProcessPath),
    ENTRY_P1L(getWindowThreadId, DISPID_getWindowThreadId),
    ENTRY_P1S(getWindowTitle, DISPID_getWindowTitle),
    ENTRY_SPECIAL(H_getWindowRect, DISPID_getWindowRect, 1, 1),
    ENTRY_SPECIAL(H_getClientRect, DISPID_getClientRect, 1, 1),
    ENTRY_SPECIAL(H_getClientSize, DISPID_getClientSize, 1, 1),
    ENTRY_P1L1L(getWindowState, DISPID_getWindowState),
    ENTRY_L1S1L(getProcessInfo, DISPID_getProcessInfo),
    ENTRY_P1L2L(moveWindow, DISPID_moveWindow),
    ENTRY_P1L2L(setWindowSize, DISPID_setWindowSize),
    ENTRY_P1L1L(setWindowState, DISPID_setWindowState),
    ENTRY_P1L1S(setWindowText, DISPID_setWindowText),
    ENTRY_P1L1L(setWindowTransparent, DISPID_setWindowTransparent),
    ENTRY_P1L2L(setClientSize, DISPID_setClientSize),
    ENTRY_L1L(setSendStringDelay, DISPID_setSendStringDelay),
    ENTRY_P1L(sendPaste, DISPID_sendPaste),
    ENTRY_P1L1S(sendString, DISPID_sendString),
    ENTRY_P1L1S(sendString2, DISPID_sendString2),
    ENTRY_P1L1S(sendStringIme, DISPID_sendStringIme),
    ENTRY_P1L1S(sendStringIme2, DISPID_sendStringIme2),
    ENTRY_SPECIAL(H_clientToScreen, DISPID_clientToScreen, 3, 3),
    ENTRY_SPECIAL(H_screenToClient, DISPID_screenToClient, 3, 3),
    // ====== 模块3: 键鼠操作 ======
    ENTRY_L1L(keyDown, DISPID_keyDown),
    ENTRY_L1S(keyDownChar, DISPID_keyDownChar),
    ENTRY_L1L(keyPress, DISPID_keyPress),
    ENTRY_L1S(keyPressChar, DISPID_keyPressChar),
    ENTRY_L1S1L(keyPressStr, DISPID_keyPressStr),
    ENTRY_L1L(keyUp, DISPID_keyUp),
    ENTRY_L1S(keyUpChar, DISPID_keyUpChar),
    ENTRY_L2L(waitKey, DISPID_waitKey),
    ENTRY_L1L(getKeyState, DISPID_getKeyState),
    ENTRY_L0(leftClick, DISPID_leftClick),
    ENTRY_L0(leftDoubleClick, DISPID_leftDoubleClick),
    ENTRY_L0(leftDown, DISPID_leftDown),
    ENTRY_L0(leftUp, DISPID_leftUp),
    ENTRY_L0(rightClick, DISPID_rightClick),
    ENTRY_L0(rightDown, DISPID_rightDown),
    ENTRY_L0(rightUp, DISPID_rightUp),
    ENTRY_L0(middleClick, DISPID_middleClick),
    ENTRY_L0(middleDown, DISPID_middleDown),
    ENTRY_L0(middleUp, DISPID_middleUp),
    ENTRY_L2L(moveTo, DISPID_moveTo),
    ENTRY_L4L(moveToEx, DISPID_moveToEx),
    ENTRY_L2L(moveR, DISPID_moveR),
    ENTRY_L1L(wheelDown, DISPID_wheelDown),
    ENTRY_L1L(wheelUp, DISPID_wheelUp),
    ENTRY_SPECIAL(H_getCursorPos, DISPID_getCursorPos, 0, 0),
    ENTRY_L0(getCursorShape, DISPID_getCursorShape),
    ENTRY_L1L(getCursorShapeEx, DISPID_getCursorShapeEx),
    ENTRY_L0(getCursorSpot, DISPID_getCursorSpot),
    ENTRY_L2L(setMouseDelay, DISPID_setMouseDelay),
    ENTRY_L2L(setKeypadDelay, DISPID_setKeypadDelay),
    ENTRY_L1L(setMouseSpeed, DISPID_setMouseSpeed),
    ENTRY_L0(getMouseSpeed, DISPID_getMouseSpeed),
    ENTRY_L1L(enableMouseAccuracy, DISPID_enableMouseAccuracy),
    ENTRY_L1L(setSimMode, DISPID_setSimMode),
    // ====== 模块4: 图色操作 ======
    ENTRY_L4L1S(capture, DISPID_capture),
    ENTRY_L4L1S1L(captureGif, DISPID_captureGif),
    ENTRY_L4L1S1L(captureJpg, DISPID_captureJpg),
    ENTRY_L4L1S(capturePng, DISPID_capturePng),
    ENTRY_L1S(capturePre, DISPID_capturePre),
    ENTRY_S2L(getColor, DISPID_getColor),
    ENTRY_S2L(getColorBGR, DISPID_getColorBGR),
    ENTRY_S2L(getColorHSV, DISPID_getColorHSV),
    ENTRY_L4L1S(getColorNum, DISPID_getColorNum),
    ENTRY_S2L(getAveRGB, DISPID_getAveRGB),
    ENTRY_S2L(getAveHSV, DISPID_getAveHSV),
    ENTRY_L2L1S(cmpColor, DISPID_cmpColor),
    ENTRY_SPECIAL(H_findColor, DISPID_findColor, 7, 7),
    ENTRY_L4L2S(findColorEx, DISPID_findColorEx),
    ENTRY_SPECIAL(H_findColorBlock9, DISPID_findColorBlock, 9, 9),
    ENTRY_L4L2S(findColorBlockEx, DISPID_findColorBlockEx),
    ENTRY_L4L2S(findColorE, DISPID_findColorE),
    ENTRY_L4L1S(findMulColor, DISPID_findMulColor),
    ENTRY_SPECIAL(H_findMultiColor, DISPID_findMultiColor, 8, 8),
    ENTRY_S2L2S(findMultiColorEx, DISPID_findMultiColorEx),
    ENTRY_S2L2S(findMultiColorE, DISPID_findMultiColorE),
    ENTRY_SPECIAL(H_findPic, DISPID_findPic, 8, 8),
    ENTRY_S2L2S(findPicE, DISPID_findPicE),
    ENTRY_S2L2S(findPicEx, DISPID_findPicEx),
    ENTRY_S2L2S(findPicExS, DISPID_findPicExS),
    ENTRY_L4L2S(findPicS, DISPID_findPicS),
    ENTRY_L4L2S(findPicMem, DISPID_findPicMem),
    ENTRY_S2L2S(findPicMemE, DISPID_findPicMemE),
    ENTRY_S2L2S(findPicMemEx, DISPID_findPicMemEx),
    ENTRY_L4L2S(findPicSim, DISPID_findPicSim),
    ENTRY_S2L2S(findPicSimE, DISPID_findPicSimE),
    ENTRY_S2L2S(findPicSimEx, DISPID_findPicSimEx),
    ENTRY_L4L2S(findPicSimMem, DISPID_findPicSimMem),
    ENTRY_S2L2S(findPicSimMemE, DISPID_findPicSimMemE),
    ENTRY_S2L2S(findPicSimMemEx, DISPID_findPicSimMemEx),
    ENTRY_L4L1S(findShape, DISPID_findShape),
    ENTRY_S2L1S(findShapeE, DISPID_findShapeE),
    ENTRY_S2L1S(findShapeEx, DISPID_findShapeEx),
    ENTRY_S1S(getPicSize, DISPID_getPicSize),
    ENTRY_L1S(freePic, DISPID_freePic),
    ENTRY_L1S(loadPic, DISPID_loadPic),
    ENTRY_L1S1L(loadPicByte, DISPID_loadPicByte),
    ENTRY_L1S1L(appendPicAddr, DISPID_appendPicAddr),
    ENTRY_S1S(matchPicName, DISPID_matchPicName),
    ENTRY_L2S(imageToBmp, DISPID_imageToBmp),
    ENTRY_L1S(setPicPwd, DISPID_setPicPwd),
    ENTRY_L1S1L(setExcludeRegion, DISPID_setExcludeRegion),
    ENTRY_L4L(getScreenData, DISPID_getScreenData),
    ENTRY_SPECIAL(H_getScreenDataBmp, DISPID_getScreenDataBmp, 4, 4),
    ENTRY_L1L(enableDisplayDebug, DISPID_enableDisplayDebug),
    ENTRY_L1L(enableFindPicMultithread, DISPID_enableFindPicMultithread),
    ENTRY_L1L(enableGetColorByCapture, DISPID_enableGetColorByCapture),
    ENTRY_L1L(setFindPicMultithreadCount, DISPID_setFindPicMultithreadCount),
    ENTRY_L1L(setFindPicMultithreadLimit, DISPID_setFindPicMultithreadLimit),
    ENTRY_L1S(bgr2RGB, DISPID_bgr2RGB),
    ENTRY_S1S(rgb2BGR, DISPID_rgb2BGR),
    ENTRY_L5L(isDisplayDead, DISPID_isDisplayDead),
    // ====== 模块5: 后台设置 ======
    ENTRY_L1L2S(bindWindow, DISPID_bindWindow),
    ENTRY_L1L2S(bindWindowEx, DISPID_bindWindowEx),
    ENTRY_L0(unBindWindow, DISPID_unBindWindow),
    ENTRY_L0(getBindWindow, DISPID_getBindWindow),
    ENTRY_P1L(isBind, DISPID_isBind),
    ENTRY_L0(forceUnBindWindow, DISPID_forceUnBindWindow),
    ENTRY_L1L(setAero, DISPID_setAero),
    ENTRY_L1L(lockInput, DISPID_lockInput),
    ENTRY_L4L(lockMouseRect, DISPID_lockMouseRect),
    ENTRY_L1L(lockDisplay, DISPID_lockDisplay),
    ENTRY_L1L(enableBind, DISPID_enableBind),
    ENTRY_L1L(enableFakeActive, DISPID_enableFakeActive),
    ENTRY_L1L(enableIme, DISPID_enableIme),
    ENTRY_L1L(enableKeypadMsg, DISPID_enableKeypadMsg),
    ENTRY_L1L(enableKeypadPatch, DISPID_enableKeypadPatch),
    ENTRY_L1L(enableKeypadSync, DISPID_enableKeypadSync),
    ENTRY_L1L(enableMouseMsg, DISPID_enableMouseMsg),
    ENTRY_L2L(enableMouseSync, DISPID_enableMouseSync),
    ENTRY_L1L(enableRealKeypad, DISPID_enableRealKeypad),
    ENTRY_L3L(enableRealMouse, DISPID_enableRealMouse),
    ENTRY_L1L(enableSpeedDx, DISPID_enableSpeedDx),
    ENTRY_L0(getFps, DISPID_getFps),
    ENTRY_L1L(setDisplayDelay, DISPID_setDisplayDelay),
    ENTRY_L1L(setDisplayRefreshDelay, DISPID_setDisplayRefreshDelay),
    ENTRY_L1L(setInputDm, DISPID_setInputDm),
    ENTRY_L1L(hackSpeed, DISPID_hackSpeed),
    ENTRY_L2L(downCpu, DISPID_downCpu),
    ENTRY_P1L(switchBindWindow, DISPID_switchBindWindow),
    // ====== 模块6: 内存操作 ======
    ENTRY_L1L(openProcess, DISPID_openProcess),
    ENTRY_P1L1S(getModuleBaseAddr, DISPID_getModuleBaseAddr),
    ENTRY_P1L1S(getModuleSize, DISPID_getModuleSize),
    ENTRY_L3S(getRemoteApiAddress, DISPID_getRemoteApiAddress),
    ENTRY_SPECIAL(H_readInt, DISPID_readInt, 2, 2),
    ENTRY_L3L(readIntAddr, DISPID_readIntAddr),
    ENTRY_SPECIAL(H_readFloat, DISPID_readFloat, 2, 2),
    ENTRY_SPECIAL(H_readFloatAddr, DISPID_readFloatAddr, 2, 2),
    ENTRY_SPECIAL(H_readDouble, DISPID_readDouble, 2, 2),
    ENTRY_SPECIAL(H_readDoubleAddr, DISPID_readDoubleAddr, 2, 2),
    ENTRY_L4L(readString, DISPID_readString),
    ENTRY_SPECIAL(H_readStringAddr, DISPID_readStringAddr, 4, 4),
    ENTRY_L3L(readData, DISPID_readData),
    ENTRY_SPECIAL(H_readDataAddr, DISPID_readDataAddr, 3, 3),
    ENTRY_L3L(readDataToBin, DISPID_readDataToBin),
    ENTRY_P1L2L(readDataAddrToBin, DISPID_readDataAddrToBin),
    ENTRY_L1S1L(findInt, DISPID_findInt),
    ENTRY_SPECIAL(H_findIntEx, DISPID_findIntEx, 7, 7),
    ENTRY_SPECIAL(H_findFloat, DISPID_findFloat, 4, 4),
    ENTRY_SPECIAL(H_findFloatEx, DISPID_findFloatEx, 7, 7),
    ENTRY_SPECIAL(H_findDouble, DISPID_findDouble, 4, 4),
    ENTRY_SPECIAL(H_findDoubleEx, DISPID_findDoubleEx, 7, 7),
    ENTRY_SPECIAL(H_findString, DISPID_findString, 4, 4),
    ENTRY_SPECIAL(H_findStringEx, DISPID_findStringEx, 7, 7),
    ENTRY_SPECIAL(H_findData, DISPID_findData, 3, 3),
    ENTRY_SPECIAL(H_findDataEx, DISPID_findDataEx, 6, 6),
    ENTRY_L4L(writeInt, DISPID_writeInt),
    ENTRY_L4L(writeIntAddr, DISPID_writeIntAddr),
    ENTRY_P1L1L1F(writeFloat, DISPID_writeFloat),
    ENTRY_P1L1L1F(writeFloatAddr, DISPID_writeFloatAddr),
    ENTRY_SPECIAL(H_writeDouble, DISPID_writeDouble, 3, 3),
    ENTRY_SPECIAL(H_writeDoubleAddr, DISPID_writeDoubleAddr, 3, 3),
    ENTRY_P1L2L1S(writeString, DISPID_writeString),
    ENTRY_P1L2L1S(writeStringAddr, DISPID_writeStringAddr),
    ENTRY_P1L1L1S(writeData, DISPID_writeData),
    ENTRY_P1L1L1S(writeDataAddr, DISPID_writeDataAddr),
    ENTRY_L4L(writeDataFromBin, DISPID_writeDataFromBin),
    ENTRY_L4L(writeDataAddrFromBin, DISPID_writeDataAddrFromBin),
    ENTRY_L3L(virtualAllocEx, DISPID_virtualAllocEx),
    ENTRY_P1L1L(virtualFreeEx, DISPID_virtualFreeEx),
    ENTRY_SPECIAL(H_virtualProtectEx, DISPID_virtualProtectEx, 4, 4),
    ENTRY_SPECIAL(H_virtualQueryEx, DISPID_virtualQueryEx, 3, 3),
    ENTRY_P1L(freeProcessMemory, DISPID_freeProcessMemory),
    ENTRY_P1S(getCommandLine, DISPID_getCommandLine),
    ENTRY_L1L(terminateProcess, DISPID_terminateProcess),
    ENTRY_L1L(terminateProcessTree, DISPID_terminateProcessTree),
    ENTRY_L1L(setMemoryFindResultToFile, DISPID_setMemoryFindResultToFile),
    ENTRY_L1L(setMemoryHwndAsProcessId, DISPID_setMemoryHwndAsProcessId),
    ENTRY_L1L(setParam64ToPointer, DISPID_setParam64ToPointer),
    ENTRY_L3L(int64ToInt32, DISPID_int64ToInt32),
    ENTRY_L1S1L(intToData, DISPID_intToData),
    ENTRY_L1L(floatToData, DISPID_floatToData),
    ENTRY_L1L(doubleToData, DISPID_doubleToData),
    ENTRY_L1S1L(stringToData, DISPID_stringToData),
    // ====== 模块7: 文件操作 ======
    ENTRY_L2S(writeFile, DISPID_writeFile),
    ENTRY_S1S(readFile, DISPID_readFile),
    ENTRY_L1S(deleteFile, DISPID_deleteFile),
    ENTRY_L2S1L(copyFile, DISPID_copyFile),
    ENTRY_L2S(moveFile, DISPID_moveFile),
    ENTRY_L1S(createFolder, DISPID_createFolder),
    ENTRY_L1S(deleteFolder, DISPID_deleteFolder),
    ENTRY_L1S(isFileExist, DISPID_isFileExist),
    ENTRY_L1S(isFolderExist, DISPID_isFolderExist),
    ENTRY_L1S(getFileLength, DISPID_getFileLength),
    ENTRY_S1S(getRealPath, DISPID_getRealPath),
    ENTRY_S0(selectFile, DISPID_selectFile),
    ENTRY_S0(selectDirectory, DISPID_selectDirectory),
    ENTRY_L1S1L(downloadFile, DISPID_downloadFile),
    ENTRY_L2S(encodeFile, DISPID_encodeFile),
    ENTRY_L2S(decodeFile, DISPID_decodeFile),
    ENTRY_L3S(writeIni, DISPID_writeIni),
    ENTRY_L2S(readIni, DISPID_readIni),
    ENTRY_L2S(deleteIni, DISPID_deleteIni),
    ENTRY_L2S(enumIniKey, DISPID_enumIniKey),
    ENTRY_L1S(enumIniSection, DISPID_enumIniSection),
    ENTRY_L5S(writeIniPwd, DISPID_writeIniPwd),
    ENTRY_S4S(readIniPwd, DISPID_readIniPwd),
    ENTRY_L4S(deleteIniPwd, DISPID_deleteIniPwd),
    ENTRY_S3S(enumIniKeyPwd, DISPID_enumIniKeyPwd),
    ENTRY_S2S(enumIniSectionPwd, DISPID_enumIniSectionPwd),
    // ====== 模块8: 文字识别 ======
    ENTRY_L1S1L(setDict, DISPID_setDict),
    ENTRY_SPECIAL(H_setDictMem, DISPID_setDictMem, 3, 3),
    ENTRY_L1S(setDictPwd, DISPID_setDictPwd),
    ENTRY_L1L(useDict, DISPID_useDict),
    ENTRY_L2L(getDict, DISPID_getDict),
    ENTRY_L1L(getDictCount, DISPID_getDictCount),
    ENTRY_L1S1L(getDictInfo, DISPID_getDictInfo),
    ENTRY_L0(getNowDict, DISPID_getNowDict),
    ENTRY_L1S(addDict, DISPID_addDict),
    ENTRY_L1S1L(saveDict, DISPID_saveDict),
    ENTRY_L1L(clearDict, DISPID_clearDict),
    ENTRY_L1L(enableShareDict, DISPID_enableShareDict),
    ENTRY_S2L1S(ocr, DISPID_ocr),
    ENTRY_S2L1S(ocrEx, DISPID_ocrEx),
    ENTRY_S2L1S(ocrExOne, DISPID_ocrExOne),
    ENTRY_L4L1S(ocrInFile, DISPID_ocrInFile),
    ENTRY_SPECIAL(H_findStr, DISPID_findStr, 7, 7),
    ENTRY_S2L1S(findStrE, DISPID_findStrE),
    ENTRY_S2L1S(findStrEx, DISPID_findStrEx),
    ENTRY_L4L1S(findStrS, DISPID_findStrS),
    ENTRY_S2L1S(findStrExS, DISPID_findStrExS),
    ENTRY_SPECIAL(H_findStrFast, DISPID_findStrFast, 7, 7),
    ENTRY_S2L1S(findStrFastE, DISPID_findStrFastE),
    ENTRY_S2L1S(findStrFastEx, DISPID_findStrFastEx),
    ENTRY_L4L1S(findStrFastS, DISPID_findStrFastS),
    ENTRY_S2L1S(findStrFastExS, DISPID_findStrFastExS),
    ENTRY_SPECIAL(H_findStrWithFont, DISPID_findStrWithFont, 10, 10),
    ENTRY_SPECIAL(H_findStrWithFontE, DISPID_findStrWithFontE, 10, 10),
    ENTRY_SPECIAL(H_findStrWithFontEx, DISPID_findStrWithFontEx, 10, 10),
    ENTRY_S2L1S(getWords, DISPID_getWords),
    ENTRY_S2L1S(getWordsNoDict, DISPID_getWordsNoDict),
    ENTRY_L2L1S(fetchWord, DISPID_fetchWord),
    ENTRY_L1S(getResultCount, DISPID_getResultCount),
    ENTRY_SPECIAL(H_getResultPos, DISPID_getResultPos, 2, 2),
    ENTRY_L1S(getWordResultCount, DISPID_getWordResultCount),
    ENTRY_SPECIAL(H_getWordResultPos, DISPID_getWordResultPos, 2, 2),
    ENTRY_L1S1L(getWordResultStr, DISPID_getWordResultStr),
    ENTRY_L1L(setColGapNoDict, DISPID_setColGapNoDict),
    ENTRY_L1L(setRowGapNoDict, DISPID_setRowGapNoDict),
    ENTRY_L1L(setWordGapNoDict, DISPID_setWordGapNoDict),
    ENTRY_L1L(setWordLineHeightNoDict, DISPID_setWordLineHeightNoDict),
    ENTRY_L1L(setExactOcr, DISPID_setExactOcr),
    ENTRY_L1L(setMinColGap, DISPID_setMinColGap),
    ENTRY_L1L(setMinRowGap, DISPID_setMinRowGap),
    ENTRY_L1L(setWordGap, DISPID_setWordGap),
    ENTRY_L1L(setWordLineHeight, DISPID_setWordLineHeight),
    // ====== 模块9: 系统操作 ======
    ENTRY_L2L(beep, DISPID_beep),
    ENTRY_L1L(delay, DISPID_delay),
    ENTRY_L2L(delays, DISPID_delays),
    ENTRY_L1S1L(runApp, DISPID_runApp),
    ENTRY_L1L(stop, DISPID_stop),
    ENTRY_L1S(play, DISPID_play),
    ENTRY_L1L(exitOs, DISPID_exitOs),
    ENTRY_L3L(setScreen, DISPID_setScreen),
    ENTRY_L0(getScreenWidth, DISPID_getScreenWidth),
    ENTRY_L0(getScreenHeight, DISPID_getScreenHeight),
    ENTRY_L0(getScreenDepth, DISPID_getScreenDepth),
    ENTRY_L0(getDPI, DISPID_getDPI),
    ENTRY_L0(getTime, DISPID_getTime),
    ENTRY_L0(getOsType, DISPID_getOsType),
    ENTRY_L0(getOsBuildNumber, DISPID_getOsBuildNumber),
    ENTRY_L0(is64Bit, DISPID_is64Bit),
    ENTRY_L0(isSurrpotVt, DISPID_isSurrpotVt),
    ENTRY_L2L(getSystemInfo, DISPID_getSystemInfo),
    ENTRY_L1L(getDir, DISPID_getDir),
    ENTRY_L0(getLocale, DISPID_getLocale),
    ENTRY_L0(getCpuType, DISPID_getCpuType),
    ENTRY_L0(getCpuUsage, DISPID_getCpuUsage),
    ENTRY_L0(getMemoryUsage, DISPID_getMemoryUsage),
    ENTRY_S0(getDiskSerial, DISPID_getDiskSerial),
    ENTRY_S0(getDiskModel, DISPID_getDiskModel),
    ENTRY_S0(getDiskReversion, DISPID_getDiskReversion),
    ENTRY_S0(getMachineCode, DISPID_getMachineCode),
    ENTRY_S0(getMachineCodeNoMac, DISPID_getMachineCodeNoMac),
    ENTRY_S0(getNetTime, DISPID_getNetTime),
    ENTRY_S1S(getNetTimeByIp, DISPID_getNetTimeByIp),
    ENTRY_L1L(getNetTimeSafe, DISPID_getNetTimeSafe),
    ENTRY_S0(getClipboard, DISPID_getClipboard),
    ENTRY_L1S(setClipboard, DISPID_setClipboard),
    ENTRY_L1L(getDisplayInfo, DISPID_getDisplayInfo),
    ENTRY_L1L(disableCloseDisplayAndSleep, DISPID_disableCloseDisplayAndSleep),
    ENTRY_L1L(disablePowerSave, DISPID_disablePowerSave),
    ENTRY_L1L(disableScreenSave, DISPID_disableScreenSave),
    ENTRY_L0(disableFontSmooth, DISPID_disableFontSmooth),
    ENTRY_L0(enableFontSmooth, DISPID_enableFontSmooth),
    ENTRY_L0(checkFontSmooth, DISPID_checkFontSmooth),
    ENTRY_L0(checkUAC, DISPID_checkUAC),
    ENTRY_L1L(setUAC, DISPID_setUAC),
    ENTRY_L1L(setDisplayAcceler, DISPID_setDisplayAcceler),
    ENTRY_P1L1L(showTaskBarIcon, DISPID_showTaskBarIcon),
    // ====== 模块10: 杂项 ======
    ENTRY_L0(enterCri, DISPID_enterCri),
    ENTRY_L0(leaveCri, DISPID_leaveCri),
    ENTRY_L0(initCri, DISPID_initCri),
    ENTRY_L0(releaseRef, DISPID_releaseRef),
    ENTRY_L1L(setExitThread, DISPID_setExitThread),
    ENTRY_L1S1L(executeCmd, DISPID_executeCmd),
    ENTRY_P1L1S(activeInputMethod, DISPID_activeInputMethod),
    ENTRY_P1L1S(checkInputMethod, DISPID_checkInputMethod),
    ENTRY_L1S(findInputMethod, DISPID_findInputMethod),
    // ====== 模块11: 汇编 ======
    ENTRY_L1S(asmAdd, DISPID_asmAdd),
    ENTRY_P1L1L(asmCall, DISPID_asmCall),
    ENTRY_L4L(asmCallEx, DISPID_asmCallEx),
    ENTRY_L0(asmClear, DISPID_asmClear),
    ENTRY_L1L(asmSetTimeout, DISPID_asmSetTimeout),
    ENTRY_L2L(assemble, DISPID_assemble),
    ENTRY_L3L(disAssemble, DISPID_disAssemble),
    ENTRY_L1L(setAsmHwndAsProcessId, DISPID_setAsmHwndAsProcessId),
    ENTRY_L1L(setShowAsmErrorMsg, DISPID_setShowAsmErrorMsg),
    // ====== 模块12: AI ======
    ENTRY_L1S(loadAi, DISPID_loadAi),
    ENTRY_SPECIAL(H_loadAiMemory, DISPID_loadAiMemory, 2, 2),
    ENTRY_L1L(aiEnableFindPicWindow, DISPID_aiEnableFindPicWindow),
    ENTRY_L4L1S(aiFindPic, DISPID_aiFindPic),
    ENTRY_S2L1S(aiFindPicEx, DISPID_aiFindPicEx),
    ENTRY_SPECIAL(H_aiFindPicMem, DISPID_aiFindPicMem, 7, 7),
    ENTRY_SPECIAL(H_aiFindPicMemEx, DISPID_aiFindPicMemEx, 7, 7),
    ENTRY_S2L1S(aiYoloDetectObjects, DISPID_aiYoloDetectObjects),
    ENTRY_SPECIAL(H_aiYoloDetectObjectsToDataBmp, DISPID_aiYoloDetectObjectsToDataBmp, 9, 9),
    ENTRY_SPECIAL(H_aiYoloDetectObjectsToFile, DISPID_aiYoloDetectObjectsToFile, 8, 8),
    ENTRY_L1S1L(aiYoloSetModel, DISPID_aiYoloSetModel),
    ENTRY_SPECIAL(H_aiYoloSetModelMemory, DISPID_aiYoloSetModelMemory, 3, 3),
    ENTRY_L0(aiYoloFreeModel, DISPID_aiYoloFreeModel),
    ENTRY_L1L(aiYoloUseModel, DISPID_aiYoloUseModel),
    ENTRY_L1S(aiYoloSetVersion, DISPID_aiYoloSetVersion),
    ENTRY_S1S(aiYoloObjectsToString, DISPID_aiYoloObjectsToString),
    ENTRY_L1S1L(aiYoloSortsObjects, DISPID_aiYoloSortsObjects),
    // ====== 模块13: Foobar ======
    ENTRY_L5L(createFoobarRect, DISPID_createFoobarRect),
    ENTRY_L5L(createFoobarEllipse, DISPID_createFoobarEllipse),
    ENTRY_L5L(createFoobarRoundRect, DISPID_createFoobarRoundRect),
    ENTRY_L4L1S(createFoobarCustom, DISPID_createFoobarCustom),
    ENTRY_P1L(foobarClose, DISPID_foobarClose),
    ENTRY_P1L(foobarLock, DISPID_foobarLock),
    ENTRY_P1L(foobarUnlock, DISPID_foobarUnlock),
    ENTRY_P1L(foobarUpdate, DISPID_foobarUpdate),
    ENTRY_P1L1S2L(foobarSetFont, DISPID_foobarSetFont),
    ENTRY_P1L1S1L(foobarSetSave, DISPID_foobarSetSave),
    ENTRY_P1L2L(foobarSetTrans, DISPID_foobarSetTrans),
    ENTRY_SPECIAL(H_foobarDrawText, DISPID_foobarDrawText, 8, 8),
    ENTRY_P1L1S1L(foobarPrintText, DISPID_foobarPrintText),
    ENTRY_SPECIAL(H_foobarDrawRect, DISPID_foobarDrawRect, 7, 7),
    ENTRY_SPECIAL(H_foobarDrawLine, DISPID_foobarDrawLine, 7, 7),
    ENTRY_P1L5L(foobarFillRect, DISPID_foobarFillRect),
    ENTRY_P1L2L1S(foobarDrawPic, DISPID_foobarDrawPic),
    ENTRY_P1L(foobarClearText, DISPID_foobarClearText),
    ENTRY_P1L4L(foobarTextRect, DISPID_foobarTextRect),
    ENTRY_P1L1L(foobarTextLineGap, DISPID_foobarTextLineGap),
    ENTRY_P1L1L(foobarTextPrintDir, DISPID_foobarTextPrintDir),
    ENTRY_P1L1S2L(foobarStartGif, DISPID_foobarStartGif),
    ENTRY_P1L(foobarStopGif, DISPID_foobarStopGif),
    // ====== 模块14: 答题 ======
    ENTRY_L5L(faqCapture, DISPID_faqCapture),
    ENTRY_L1S1L(faqCaptureFromFile, DISPID_faqCaptureFromFile),
    ENTRY_L5L(faqCaptureString, DISPID_faqCaptureString),
    ENTRY_L1L(faqGetSize, DISPID_faqGetSize),
    ENTRY_L1S1L(faqPost, DISPID_faqPost),
    ENTRY_L2L(faqSend, DISPID_faqSend),
    ENTRY_L1L(faqFetch, DISPID_faqFetch),
    ENTRY_L0(faqCancel, DISPID_faqCancel),
    ENTRY_L0(faqIsPosted, DISPID_faqIsPosted),
    // ====== 模块15: 算法 ======
    ENTRY_L4L1S(excludePos, DISPID_excludePos),
    ENTRY_L1S1L(findNearestPos, DISPID_findNearestPos),
    ENTRY_L1S1L(sortPosDistance, DISPID_sortPosDistance),
    // ====== 模块16: 防护盾 ======
    ENTRY_L1S1L(dmGuard, DISPID_dmGuard),
    ENTRY_L2S(dmGuardExtract, DISPID_dmGuardExtract),
    ENTRY_L2S(dmGuardLoadCustom, DISPID_dmGuardLoadCustom),
    ENTRY_L2S(dmGuardParams, DISPID_dmGuardParams),
    ENTRY_L0(unLoadDriver, DISPID_unLoadDriver),
};

const UINT g_dispTableCount = sizeof(g_dispTable) / sizeof(g_dispTable[0]);

} // 关闭匿名命名空间

// ============================================================================
// 公共 API（声明于 dm_com.h，必须在全局命名空间）
// ============================================================================
const DispFuncEntry* GetDispTable() { return g_dispTable; }
UINT GetDispTableCount() { return g_dispTableCount; }

// ============================================================================
// 查找索引缓存 — 将 O(n) 线性查找优化为 O(1) hash map 查找
// ============================================================================
namespace {

/// 名称 → 分发表条目的映射缓存（线程安全懒初始化）
static std::unordered_map<std::string, const DispFuncEntry*>& GetNameMap() {
    static std::unordered_map<std::string, const DispFuncEntry*> map;
    static std::once_flag initFlag;
    std::call_once(initFlag, []() {
        for (UINT i = 0; i < g_dispTableCount; i++) {
            // 键统一转小写，实现大小写不敏感查找
            std::string key = g_dispTable[i].name;
            for (auto& c : key) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
            map[key] = &g_dispTable[i];
        }
    });
    return map;
}

/// DISPID → 分发表条目的映射缓存（线程安全懒初始化）
static std::unordered_map<DISPID, const DispFuncEntry*>& GetIdMap() {
    static std::unordered_map<DISPID, const DispFuncEntry*> map;
    static std::once_flag initFlag;
    std::call_once(initFlag, []() {
        for (UINT i = 0; i < g_dispTableCount; i++) {
            map[g_dispTable[i].id] = &g_dispTable[i];
        }
    });
    return map;
}

/// 按名称查找分发表条目（O(1) hash map）
static const DispFuncEntry* FindByName(const char* name) {
    auto& nameMap = GetNameMap();
    auto it = nameMap.find(name);
    if (it != nameMap.end()) return it->second;

    return nullptr;
}

/// 按 DISPID 查找分发表条目（O(1) hash map）
static const DispFuncEntry* FindById(DISPID id) {
    auto& idMap = GetIdMap();
    auto it = idMap.find(id);
    return (it != idMap.end()) ? it->second : nullptr;
}

// ============================================================================
// Invoke 分发函数 — 根据 FuncPattern 调用对应的 C 函数
// ============================================================================
static HRESULT DispatchByPattern(const DispFuncEntry* entry, VARIANT* ret,
                                  const VARIANT* args, UINT argCount) {
    if (entry->pattern == FuncPattern::SPECIAL) {
        // 特殊处理函数 — 直接调用
        auto handler = reinterpret_cast<HRESULT(*)(VARIANT*, const VARIANT*, UINT)>(entry->funcPtr);
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
        auto f = reinterpret_cast<long(*)()>(entry->funcPtr);
        SetResultLong(ret, f()); return S_OK;
    }
    case FuncPattern::S0: {
        auto f = reinterpret_cast<const char*(*)()>(entry->funcPtr);
        SetResultString(ret, f()); return S_OK;
    }
    case FuncPattern::L1L: {
        auto f = reinterpret_cast<long(*)(long)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0))); return S_OK;
    }
    case FuncPattern::L1S: {
        auto f = reinterpret_cast<long(*)(const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str())); return S_OK;
    }
    case FuncPattern::L2L: {
        auto f = reinterpret_cast<long(*)(long, long)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1))); return S_OK;
    }
    case FuncPattern::L3L: {
        auto f = reinterpret_cast<long(*)(long, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1), a0(2))); return S_OK;
    }
    case FuncPattern::L4L: {
        auto f = reinterpret_cast<long(*)(long, long, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3))); return S_OK;
    }
    case FuncPattern::L5L: {
        auto f = reinterpret_cast<long(*)(long, long, long, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), a0(4))); return S_OK;
    }
    case FuncPattern::L2S: {
        auto f = reinterpret_cast<long(*)(const char*, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::L3S: {
        auto f = reinterpret_cast<long(*)(const char*, const char*, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::L1L1S: {
        auto f = reinterpret_cast<long(*)(long, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::L1S1L: {
        auto f = reinterpret_cast<long(*)(const char*, long)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), a0(1))); return S_OK;
    }
    case FuncPattern::L2L1S: {
        auto f = reinterpret_cast<long(*)(long, long, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::L1S2L: {
        auto f = reinterpret_cast<long(*)(const char*, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), a0(1), a0(2))); return S_OK;
    }
    case FuncPattern::L4L1S: {
        auto f = reinterpret_cast<long(*)(long, long, long, long, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), aS(4).c_str())); return S_OK;
    }
    case FuncPattern::S2L: {
        auto f = reinterpret_cast<const char*(*)(long, long)>(entry->funcPtr);
        SetResultString(ret, f(a0(0), a0(1))); return S_OK;
    }
    case FuncPattern::S2L1S: {
        auto f = reinterpret_cast<const char*(*)(long, long, const char*)>(entry->funcPtr);
        std::string s = aS(2); SetResultString(ret, f(a0(0), a0(1), s.c_str())); return S_OK;
    }
    case FuncPattern::S1S: {
        auto f = reinterpret_cast<const char*(*)(const char*)>(entry->funcPtr);
        SetResultString(ret, f(aS(0).c_str())); return S_OK;
    }
    case FuncPattern::L4L2S: {
        auto f = reinterpret_cast<long(*)(long, long, long, long, const char*, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), aS(4).c_str(), aS(5).c_str())); return S_OK;
    }
    case FuncPattern::S2L2S: {
        auto f = reinterpret_cast<const char*(*)(long, long, const char*, const char*)>(entry->funcPtr);
        SetResultString(ret, f(a0(0), a0(1), aS(2).c_str(), aS(3).c_str())); return S_OK;
    }
    case FuncPattern::L4L1S1L: {
        auto f = reinterpret_cast<long(*)(long, long, long, long, const char*, long)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), a0(1), a0(2), a0(3), aS(4).c_str(), a0(5))); return S_OK;
    }
    case FuncPattern::L1L2S: {
        auto f = reinterpret_cast<long(*)(long, const char*, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(a0(0), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::P1L: {
        auto f = reinterpret_cast<long(*)(intptr_t)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0))); return S_OK;
    }
    case FuncPattern::P1L1S: {
        auto f = reinterpret_cast<long(*)(intptr_t, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::P1L1L: {
        auto f = reinterpret_cast<long(*)(intptr_t, long)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1))); return S_OK;
    }
    case FuncPattern::P1L2L: {
        auto f = reinterpret_cast<long(*)(intptr_t, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1), a0(2))); return S_OK;
    }
    case FuncPattern::P1L1L1S: {
        auto f = reinterpret_cast<long(*)(intptr_t, long, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::P1L2L1S: {
        auto f = reinterpret_cast<long(*)(intptr_t, long, long, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1), a0(2), aS(3).c_str())); return S_OK;
    }
    case FuncPattern::L2S1L: {
        auto f = reinterpret_cast<long(*)(const char*, const char*, long)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), a0(2))); return S_OK;
    }
    case FuncPattern::P1S: {
        auto f = reinterpret_cast<const char*(*)(intptr_t)>(entry->funcPtr);
        SetResultString(ret, f(aP(0))); return S_OK;
    }
    case FuncPattern::REGEX: {
        auto f = reinterpret_cast<long(*)(const char*, const char*, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::P1L1S1L: {
        auto f = reinterpret_cast<long(*)(intptr_t, const char*, long)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), aS(1).c_str(), a0(2))); return S_OK;
    }
    case FuncPattern::P1L1L1F: {
        auto f = reinterpret_cast<long(*)(intptr_t, long, float)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1), aF(2))); return S_OK;
    }
    case FuncPattern::P1L4L: {
        auto f = reinterpret_cast<long(*)(intptr_t, long, long, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1), a0(2), a0(3), a0(4))); return S_OK;
    }
    case FuncPattern::P1L5L: {
        auto f = reinterpret_cast<long(*)(intptr_t, long, long, long, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1), a0(2), a0(3), a0(4), a0(5))); return S_OK;
    }
    case FuncPattern::P1L1S2L: {
        auto f = reinterpret_cast<long(*)(intptr_t, const char*, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), aS(1).c_str(), a0(2), a0(3))); return S_OK;
    }
    case FuncPattern::P1L2L1S2L: {
        auto f = reinterpret_cast<long(*)(intptr_t, long, long, const char*, long, long)>(entry->funcPtr);
        SetResultLong(ret, f(aP(0), a0(1), a0(2), aS(3).c_str(), a0(4), a0(5))); return S_OK;
    }
    case FuncPattern::L4S: {
        auto f = reinterpret_cast<long(*)(const char*, const char*, const char*, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str(), aS(3).c_str())); return S_OK;
    }
    case FuncPattern::L5S: {
        auto f = reinterpret_cast<long(*)(const char*, const char*, const char*, const char*, const char*)>(entry->funcPtr);
        SetResultLong(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str(), aS(3).c_str(), aS(4).c_str())); return S_OK;
    }
    case FuncPattern::S2S: {
        auto f = reinterpret_cast<const char*(*)(const char*, const char*)>(entry->funcPtr);
        SetResultString(ret, f(aS(0).c_str(), aS(1).c_str())); return S_OK;
    }
    case FuncPattern::S3S: {
        auto f = reinterpret_cast<const char*(*)(const char*, const char*, const char*)>(entry->funcPtr);
        SetResultString(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str())); return S_OK;
    }
    case FuncPattern::S4S: {
        auto f = reinterpret_cast<const char*(*)(const char*, const char*, const char*, const char*)>(entry->funcPtr);
        SetResultString(ret, f(aS(0).c_str(), aS(1).c_str(), aS(2).c_str(), aS(3).c_str())); return S_OK;
    }
    default:
        return DISP_E_MEMBERNOTFOUND;
    }
}

} // 关闭匿名命名空间

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

    for (char* p = nameBuf; *p; ++p)
        *p = static_cast<char>(tolower(static_cast<unsigned char>(*p)));

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

    // O(1) hash map 查找替代 O(n) 线性遍历
    const DispFuncEntry* entry = FindById(dispIdMember);
    if (!entry) return DISP_E_MEMBERNOTFOUND;

    UINT argCount = pDispParams ? pDispParams->cArgs : 0;
    if (argCount < entry->minArgs || argCount > entry->maxArgs)
        return DISP_E_BADPARAMCOUNT;

    // 反转参数数组（rgvarg[0] 是最后一个参数）
    // 使用 std::vector 替代裸 new，确保异常安全（RAII）
    // 使用 VariantCopy 替代浅拷贝，正确管理 BSTR 引用计数
    std::vector<VARIANT> reversedArgsVec;
    const VARIANT* reversedArgs = nullptr;
    if (argCount > 0 && pDispParams) {
        reversedArgsVec.resize(argCount);
        for (UINT i = 0; i < argCount; i++)
            VariantCopy(&reversedArgsVec[i], &pDispParams->rgvarg[argCount - 1 - i]);
        reversedArgs = reversedArgsVec.data();
    }

    // 清理 reversedArgsVec 中的 VARIANT 副本
    struct VariantArrayGuard {
        std::vector<VARIANT>* vec;
        ~VariantArrayGuard() {
            for (auto& v : *vec) VariantClear(&v);
        }
    } guard{&reversedArgsVec};

    {
        StateGuard stateGuard(&m_state);
        return DispatchByPattern(entry, pVarResult, reversedArgs, argCount);
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

void setComModule(HMODULE hMod) {
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

// ============================================================================
// RAII 包装 HKEY — 自动关闭注册表句柄，替代 goto 错误处理
// ============================================================================
struct ScopedHKey {
    HKEY h = NULL;
    ~ScopedHKey() { if (h) RegCloseKey(h); }
    operator HKEY*() { return &h; }
    operator bool() const { return h != NULL; }
};

STDAPI DllRegisterServer() {
    WCHAR dllPath[MAX_PATH];
    if (!GetModuleFileNameW(g_comModule, dllPath, MAX_PATH))
        return HRESULT_FROM_WIN32(GetLastError());

    ScopedHKey hKeyClsid, hKeyApp, hKeyInproc, hKeyProgId, hKeyProgIdClsid;

    WCHAR clsidStr[64];
    StringFromGUID2(CLSID_DmSoft, clsidStr, 64);

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, L"CLSID", 0, NULL, 0, KEY_WRITE, NULL, hKeyClsid, NULL) != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(GetLastError());
    if (RegCreateKeyExW(hKeyClsid.h, clsidStr, 0, NULL, 0, KEY_WRITE, NULL, hKeyApp, NULL) != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(GetLastError());

    // ProgID 设置
    const WCHAR* progIdW = PROGID_DM;
    DWORD progIdBytes = static_cast<DWORD>((wcslen(progIdW) + 1) * sizeof(WCHAR));
    RegSetValueExW(hKeyApp.h, NULL, 0, REG_SZ, reinterpret_cast<const BYTE*>(progIdW), progIdBytes);

    if (RegCreateKeyExW(hKeyApp.h, L"InprocServer32", 0, NULL, 0, KEY_WRITE, NULL, hKeyInproc, NULL) != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(GetLastError());
    RegSetValueExW(hKeyInproc.h, NULL, 0, REG_SZ, reinterpret_cast<const BYTE*>(dllPath),
                   static_cast<DWORD>((wcslen(dllPath) + 1) * sizeof(WCHAR)));
    const WCHAR* both = L"Both";
    RegSetValueExW(hKeyInproc.h, L"ThreadingModel", 0, REG_SZ,
                   reinterpret_cast<const BYTE*>(both), static_cast<DWORD>((wcslen(both) + 1) * sizeof(WCHAR)));

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, progIdW, 0, NULL, 0, KEY_WRITE, NULL, hKeyProgId, NULL) != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(GetLastError());
    RegSetValueExW(hKeyProgId.h, NULL, 0, REG_SZ, reinterpret_cast<const BYTE*>(progIdW), progIdBytes);

    if (RegCreateKeyExW(hKeyProgId.h, L"CLSID", 0, NULL, 0, KEY_WRITE, NULL, hKeyProgIdClsid, NULL) != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(GetLastError());
    RegSetValueExW(hKeyProgIdClsid.h, NULL, 0, REG_SZ, reinterpret_cast<const BYTE*>(clsidStr),
                   static_cast<DWORD>((wcslen(clsidStr) + 1) * sizeof(WCHAR)));

    // ScopedHKey 析构函数自动关闭所有句柄
    return S_OK;
}

STDAPI DllUnregisterServer() {
    WCHAR clsidStr[64];
    StringFromGUID2(CLSID_DmSoft, clsidStr, 64);

    // 使用 std::wstring 替代固定缓冲区拼接，避免溢出风险
    std::wstring clsidKey = std::wstring(L"CLSID\\") + clsidStr;
    std::wstring inprocKey = clsidKey + L"\\InprocServer32";

    RegDeleteTreeW(HKEY_CLASSES_ROOT, inprocKey.c_str());
    RegDeleteTreeW(HKEY_CLASSES_ROOT, clsidKey.c_str());

    const WCHAR* progIdW = PROGID_DM;
    RegDeleteTreeW(HKEY_CLASSES_ROOT, progIdW);

    return S_OK;
}