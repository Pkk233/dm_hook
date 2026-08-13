// ============================================================================
// dm_com.h — 大漠插件 COM 接口定义
// 提供 IDispatch 接口，支持 win32com.client.Dispatch('dm.dmsoft') 调用
// ============================================================================
#pragma once
#ifndef DM_COM_H
#define DM_COM_H

#include <windows.h>
#include <oleauto.h>
#include "dm.h"

// ============================================================================
// CLSID 和 ProgID
// ============================================================================
// {9AEDF74E-E647-443C-A42A-2D7B50AE7345}
EXTERN_C const GUID CLSID_DmSoft;

#define PROGID_DM L"dm.dmsoft"

// ============================================================================
// 函数分发表 — 将 COM 方法名映射到 C 函数调用
// ============================================================================

// 最大 DISPID
#define DISPID_START 1

// 函数模式枚举（用于 Invoke 中分发）
enum class FuncPattern : uint8_t {
    L0, S0, L1L, L1S, L2L, L3L, L4L, L5L,
    L2S, L3S, L1L1S, L1S1L, L2L1S, L1S2L, L4L1S,
    S2L, S2L1S, S1S, L4L2S, S2L2S, L4L1S1L, L1L2S,
    P1L, P1L1S, P1L1L, P1L2L, P1L1L1S, P1L2L1S,
    L2S1L, P1S, REGEX, P1L1S1L, P1L1L1F, P1L4L, P1L5L,
    P1L1S2L, P1L2L1S2L, L4S, L5S, S2S, S3S, S4S,
    SPECIAL
};

// 单个分发表项
struct DispFuncEntry {
    const char* name;       // 方法名（小写，如 "ver"）
    DISPID id;              // 分发 ID
    UINT minArgs;           // 最少参数数
    UINT maxArgs;           // 最多参数数
    FuncPattern pattern;    // 函数模式
    void* funcPtr;          // 指向实际 C 函数
};

// 获取分发表
const DispFuncEntry* GetDispTable();
UINT GetDispTableCount();

// ============================================================================
// CDmObject — IDispatch 实现
// ============================================================================
class CDmObject : public IDispatch {
public:
    CDmObject();
    virtual ~CDmObject();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IDispatch
    STDMETHODIMP GetTypeInfoCount(UINT* pctinfo) override;
    STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override;
    STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
                               LCID lcid, DISPID* rgDispId) override;
    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
                        WORD wFlags, DISPPARAMS* pDispParams,
                        VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
                        UINT* puArgErr) override;

private:
    LONG m_refCount;
    DmState m_state;    // COM 对象私有状态（非 TLS）

    // 在调用 C 函数前/后切换状态
    class StateGuard {
    public:
        StateGuard(DmState* s) { dm_setComState(s); }
        ~StateGuard() { dm_setComState(nullptr); }
    };
};

// ============================================================================
// CDmClassFactory — IClassFactory 实现
// ============================================================================
class CDmClassFactory : public IClassFactory {
public:
    CDmClassFactory();
    virtual ~CDmClassFactory();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IClassFactory
    STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
    STDMETHODIMP LockServer(BOOL fLock) override;

private:
    LONG m_refCount;
};

// ============================================================================
// DLL 导出函数
// ============================================================================
STDAPI_(HRESULT) DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
STDAPI_(HRESULT) DllCanUnloadNow();
STDAPI_(HRESULT) DllRegisterServer();
STDAPI_(HRESULT) DllUnregisterServer();

// 全局引用计数
extern LONG g_dllRefCount;
extern LONG g_dllLockCount;

#endif // DM_COM_H