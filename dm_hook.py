#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
dm_hook.py — 大漠插件 (dm_hook.dll) Python 绑定示例
=====================================================
使用 ctypes 调用 dm_hook.dll 导出函数（camelCase 命名风格）

用法:
    pip install -r requirements.txt  # 不需要额外依赖
    python dm_hook.py

支持:
    - 通过 ctypes 直接调用 C 导出函数
    - 面向对象的封装 (DM 类)
    - 便捷的跨语言调用方式
"""

import ctypes
import ctypes.wintypes
import os
import sys
from typing import Optional, Tuple, List

# ============================================================================
# 底层绑定: ctypes 直接调用 DLL
# ============================================================================

class DmHook:
    """大漠插件 dm_hook.dll 的 Python 封装（camelCase 命名）"""
    
    def __init__(self, dll_path: str = None):
        if dll_path is None:
            # 默认搜索路径
            candidates = [
                os.path.join(os.path.dirname(__file__), "dm_hook.dll"),
                os.path.join(os.path.dirname(__file__), "build", "Release", "dm_hook.dll"),
                os.path.join(os.path.dirname(__file__), "build", "dm_hook.dll"),
                "dm_hook.dll",
            ]
            for c in candidates:
                if os.path.exists(c):
                    dll_path = c
                    break
            if dll_path is None:
                raise FileNotFoundError(
                    "找不到 dm_hook.dll，请先编译项目。\n"
                    "构建命令: cmake -B build && cmake --build build --config Release"
                )
        
        self._dll = ctypes.CDLL(dll_path)
        self._setupFunctions()
    
    def _setupFunctions(self):
        dll = self._dll
        
        # 字符串返回类型
        dll.ver.restype = ctypes.c_char_p
        dll.getPath.restype = ctypes.c_char_p
        dll.getBasePath.restype = ctypes.c_char_p
        dll.getID.restype = ctypes.c_long
        dll.getLastError.restype = ctypes.c_long
        
        # 基本设置
        dll.reg.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        dll.reg.restype = ctypes.c_long
        dll.setPath.argtypes = [ctypes.c_char_p]
        dll.setPath.restype = ctypes.c_long
        dll.ver.restype = ctypes.c_char_p
        
        # 窗口
        dll.findWindow.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        dll.findWindow.restype = ctypes.c_long
        dll.getForegroundWindow.restype = ctypes.c_long
        dll.getWindowTitle.argtypes = [ctypes.c_long]
        dll.getWindowTitle.restype = ctypes.c_char_p
        dll.getWindowClass.argtypes = [ctypes.c_long]
        dll.getWindowClass.restype = ctypes.c_char_p
        dll.getWindowProcessPath.argtypes = [ctypes.c_long]
        dll.getWindowProcessPath.restype = ctypes.c_char_p
        
        # 键鼠
        dll.moveTo.argtypes = [ctypes.c_long, ctypes.c_long]
        dll.moveTo.restype = ctypes.c_long
        dll.leftClick.restype = ctypes.c_long
        dll.keyPress.argtypes = [ctypes.c_long]
        dll.keyPress.restype = ctypes.c_long
        
        # 图色
        dll.getColor.argtypes = [ctypes.c_long, ctypes.c_long]
        dll.getColor.restype = ctypes.c_char_p
        dll.capture.argtypes = [ctypes.c_long, ctypes.c_long, ctypes.c_long, ctypes.c_long, ctypes.c_char_p]
        dll.capture.restype = ctypes.c_long
        dll.findColor.restype = ctypes.c_long
        dll.findPic.restype = ctypes.c_long
        
        # 系统
        dll.getScreenWidth.restype = ctypes.c_long
        dll.getScreenHeight.restype = ctypes.c_long
        dll.delay.argtypes = [ctypes.c_long]
        dll.delay.restype = ctypes.c_long
        dll.getTime.restype = ctypes.c_long
        dll.getOsType.restype = ctypes.c_long
        dll.getMachineCode.restype = ctypes.c_char_p
        dll.executeCmd.argtypes = [ctypes.c_char_p, ctypes.c_long]
        dll.executeCmd.restype = ctypes.c_char_p
    
    # ---- 基本设置 ----
    def reg(self, reg_code: str, ver_info: str = "") -> int:
        return self._dll.reg(reg_code.encode(), ver_info.encode())
    
    def ver(self) -> str:
        return self._dll.ver().decode('utf-8', errors='replace')
    
    def getID(self) -> int:
        return self._dll.getID()
    
    def getLastError(self) -> int:
        return self._dll.getLastError()
    
    def getPath(self) -> str:
        return self._dll.getPath().decode('utf-8', errors='replace')
    
    def setPath(self, path: str) -> int:
        return self._dll.setPath(path.encode())
    
    def getBasePath(self) -> str:
        return self._dll.getBasePath().decode('utf-8', errors='replace')
    
    # ---- 窗口 ----
    def findWindow(self, cls: str = "", title: str = "") -> int:
        return self._dll.findWindow(cls.encode(), title.encode())
    
    def getForegroundWindow(self) -> int:
        return self._dll.getForegroundWindow()
    
    def getWindowTitle(self, hwnd: int) -> str:
        return self._dll.getWindowTitle(hwnd).decode('utf-8', errors='replace')
    
    def getWindowClass(self, hwnd: int) -> str:
        return self._dll.getWindowClass(hwnd).decode('utf-8', errors='replace')
    
    def getWindowProcessPath(self, hwnd: int) -> str:
        return self._dll.getWindowProcessPath(hwnd).decode('utf-8', errors='replace')
    
    # ---- 键鼠 ----
    def moveTo(self, x: int, y: int) -> int:
        return self._dll.moveTo(x, y)
    
    def leftClick(self) -> int:
        return self._dll.leftClick()
    
    def keyPress(self, vk: int) -> int:
        return self._dll.keyPress(vk)
    
    # ---- 图色 ----
    def getColor(self, x: int, y: int) -> str:
        return self._dll.getColor(x, y).decode()
    
    def capture(self, x1: int, y1: int, x2: int, y2: int, file: str) -> int:
        return self._dll.capture(x1, y1, x2, y2, file.encode())
    
    # ---- 系统 ----
    def getScreenWidth(self) -> int:
        return self._dll.getScreenWidth()
    
    def getScreenHeight(self) -> int:
        return self._dll.getScreenHeight()
    
    def delay(self, ms: int):
        self._dll.delay(ms)
    
    def getTime(self) -> int:
        return self._dll.getTime()
    
    def getOsType(self) -> int:
        return self._dll.getOsType()
    
    def getMachineCode(self) -> str:
        return self._dll.getMachineCode().decode('utf-8', errors='replace')
    
    def executeCmd(self, cmd: str, timeout: int = 30000) -> str:
        return self._dll.executeCmd(cmd.encode(), timeout).decode('utf-8', errors='replace')


# ============================================================================
# 使用示例
# ============================================================================

def main():
    print("=" * 60)
    print("大漠插件 (dm_hook) Python 调用示例")
    print("=" * 60)
    
    try:
        dm = DmHook()
    except FileNotFoundError as e:
        print(f"错误: {e}")
        sys.exit(1)
    
    # 基本信息
    print(f"\n[基本信息]")
    print(f"  版本: {dm.ver()}")
    print(f"  对象ID: {dm.getID()}")
    print(f"  全局路径: {dm.getPath()}")
    print(f"  基础路径: {dm.getBasePath()}")
    print(f"  机器码: {dm.getMachineCode()}")
    
    # 系统信息
    print(f"\n[系统信息]")
    print(f"  屏幕: {dm.getScreenWidth()}x{dm.getScreenHeight()}")
    print(f"  系统类型: {dm.getOsType()}")
    print(f"  当前时间戳: {dm.getTime()}")
    
    # 窗口信息
    print(f"\n[窗口信息]")
    fg = dm.getForegroundWindow()
    if fg:
        print(f"  前台窗口句柄: {fg}")
        print(f"  窗口标题: {dm.getWindowTitle(fg)}")
        print(f"  窗口类: {dm.getWindowClass(fg)}")
        print(f"  进程路径: {dm.getWindowProcessPath(fg)}")

    # 键鼠示例：把鼠标移动到 (100, 100)
    print(f"\n[键鼠]")
    dm.moveTo(100, 100)
    print(f"  移动鼠标到 (100, 100) 完成")

    print(f"\n[完成] dm_hook.dll 加载成功，所有 API 可用。")
    print(f"      详细 API 文档请参考: dm.h 头文件")

if __name__ == "__main__":
    main()