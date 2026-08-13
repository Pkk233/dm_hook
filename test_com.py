#!/usr/bin/env python3
"""测试 dm_hook.dll COM 支持"""
import ctypes
import ctypes.wintypes
import sys

# 加载 DLL
dll_path = r"D:\dm\dm_hook\dm_hook.dll"
dll = ctypes.CDLL(dll_path)

# 定义 COM 函数
dll.DllGetClassObject.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.POINTER(ctypes.c_void_p)]
dll.DllGetClassObject.restype = ctypes.c_long

# CLSID_DmSoft = {9AEDF74E-E647-443C-A42A-2D7B50AE7345}
CLSID_DmSoft = (ctypes.c_ubyte * 16)(0x4e, 0xf7, 0xed, 0x9a, 0x47, 0xe6, 0x3c, 0x44, 0xa4, 0x2a, 0x2d, 0x7b, 0x50, 0xae, 0x73, 0x45)

# IID_IDispatch = {00020400-0000-0000-C000-000000000046}
IID_IDispatch = (ctypes.c_ubyte * 16)(0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46)

# 获取类工厂
ppv = ctypes.c_void_p()
hr = dll.DllGetClassObject(ctypes.byref(CLSID_DmSoft), ctypes.byref(IID_IDispatch), ctypes.byref(ppv))
print(f"DllGetClassObject 返回: {hr}, ppv = {ppv.value}")

if hr == 0 and ppv:
    dispatch = ctypes.c_void_p(ppv.value)
    # 简单测试: 调用 dm_ver
    dm_ver = dll.dm_ver
    dm_ver.restype = ctypes.c_char_p
    ver = dm_ver()
    print(f"dm_ver() = {ver.decode('utf-8')}")
    
    # 测试 dm_getID
    dm_getID = dll.dm_getID
    dm_getID.restype = ctypes.c_long
    print(f"dm_getID() = {dm_getID()}")
    
    # 测试 dm_getScreenWidth
    dm_getScreenWidth = dll.dm_getScreenWidth
    dm_getScreenWidth.restype = ctypes.c_long
    print(f"dm_getScreenWidth() = {dm_getScreenWidth()}")
    
    # 测试 dm_getMachineCode
    dm_getMachineCode = dll.dm_getMachineCode
    dm_getMachineCode.restype = ctypes.c_char_p
    print(f"dm_getMachineCode() = {dm_getMachineCode().decode('utf-8')}")
    
    print("\n=== 所有测试通过！===")
else:
    print(f"COM 初始化失败")
    sys.exit(1)