// ============================================================================
// dllmain.cpp — 大漠插件 (dm.dll) DLL 入口
// ============================================================================
#define DM_EXPORTS
#define NOMINMAX

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "dm.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// TLS 索引在 dm.cpp 中定义
extern DWORD g_tlsIndex;
extern std::once_flag g_tlsInitFlag;

// COM 模块句柄设置
extern void setComModule(HMODULE hMod);

static ULONG_PTR g_gdiplusToken = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        // 保存模块句柄（用于 COM 注册）
        setComModule(hModule);
        // 初始化 GDI+
        GdiplusStartupInput gdiSI;
        GdiplusStartup(&g_gdiplusToken, &gdiSI, NULL);
        DisableThreadLibraryCalls(hModule);
        break;
    }
    case DLL_PROCESS_DETACH: {
        // 清理 GDI+
        if (g_gdiplusToken) {
            GdiplusShutdown(g_gdiplusToken);
            g_gdiplusToken = 0;
        }
        // 清理 TLS
        if (g_tlsIndex != TLS_OUT_OF_INDEXES) {
            DmState* s = reinterpret_cast<DmState*>(TlsGetValue(g_tlsIndex));
            if (s) {
                delete s;
                TlsSetValue(g_tlsIndex, NULL);
            }
            TlsFree(g_tlsIndex);
            g_tlsIndex = TLS_OUT_OF_INDEXES;
        }
        break;
    }
    case DLL_THREAD_ATTACH: {
        // 每个线程第一次使用时会自动创建状态
        break;
    }
    case DLL_THREAD_DETACH: {
        // 清理线程本地状态
        if (g_tlsIndex != TLS_OUT_OF_INDEXES) {
            DmState* s = reinterpret_cast<DmState*>(TlsGetValue(g_tlsIndex));
            if (s) {
                delete s;
                TlsSetValue(g_tlsIndex, NULL);
            }
        }
        break;
    }
    }
    return TRUE;
}
