#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
dm_hook.py — 大漠插件 (dm_hook.dll) Python 绑定示例
=====================================================
使用 ctypes 调用 dm_hook.dll 导出函数

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
    """大漠插件 dm_hook.dll 的 Python 封装"""
    
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
        self._setup_functions()
    
    def _setup_functions(self):
        dll = self._dll
        
        # 字符串返回类型
        dll.dm_ver.restype = ctypes.c_char_p
        dll.dm_getPath.restype = ctypes.c_char_p
        dll.dm_getBasePath.restype = ctypes.c_char_p
        dll.dm_getID.restype = ctypes.c_long
        dll.dm_getLastError.restype = ctypes.c_long
        
        # 基本设置
        dll.dm_reg.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        dll.dm_reg.restype = ctypes.c_long
        dll.dm_setPath.argtypes = [ctypes.c_char_p]
        dll.dm_setPath.restype = ctypes.c_long
        dll.dm_ver.restype = ctypes.c_char_p
        
        # 窗口
        dll.dm_findWindow.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
        dll.dm_findWindow.restype = ctypes.c_long
        dll.dm_getForegroundWindow.restype = ctypes.c_long
        dll.dm_getWindowTitle.argtypes = [ctypes.c_long]
        dll.dm_getWindowTitle.restype = ctypes.c_char_p
        dll.dm_getWindowClass.argtypes = [ctypes.c_long]
        dll.dm_getWindowClass.restype = ctypes.c_char_p
        dll.dm_getWindowProcessPath.argtypes = [ctypes.c_long]
        dll.dm_getWindowProcessPath.restype = ctypes.c_char_p
        
        # 键鼠
        dll.dm_moveTo.argtypes = [ctypes.c_long, ctypes.c_long]
        dll.dm_moveTo.restype = ctypes.c_long
        dll.dm_leftClick.restype = ctypes.c_long
        dll.dm_keyPress.argtypes = [ctypes.c_long]
        dll.dm_keyPress.restype = ctypes.c_long
        
        # 图色
        dll.dm_getColor.argtypes = [ctypes.c_long, ctypes.c_long]
        dll.dm_getColor.restype = ctypes.c_char_p
        dll.dm_capture.argtypes = [ctypes.c_long, ctypes.c_long, ctypes.c_long, ctypes.c_long, ctypes.c_char_p]
        dll.dm_capture.restype = ctypes.c_long
        dll.dm_findColor.restype = ctypes.c_long
        dll.dm_findPic.restype = ctypes.c_long
        
        # 系统
        dll.dm_getScreenWidth.restype = ctypes.c_long
        dll.dm_getScreenHeight.restype = ctypes.c_long
        dll.dm_delay.argtypes = [ctypes.c_long]
        dll.dm_delay.restype = ctypes.c_long
        dll.dm_getTime.restype = ctypes.c_long
        dll.dm_getOsType.restype = ctypes.c_long
        dll.dm_getMachineCode.restype = ctypes.c_char_p
        dll.dm_executeCmd.argtypes = [ctypes.c_char_p, ctypes.c_long]
        dll.dm_executeCmd.restype = ctypes.c_char_p
    
    # ---- 基本设置 ----
    def reg(self, reg_code: str, ver_info: str = "") -> int:
        return self._dll.dm_reg(reg_code.encode(), ver_info.encode())
    
    def ver(self) -> str:
        return self._dll.dm_ver().decode('utf-8', errors='replace')
    
    def get_id(self) -> int:
        return self._dll.dm_getID()
    
    def get_last_error(self) -> int:
        return self._dll.dm_getLastError()
    
    def get_path(self) -> str:
        return self._dll.dm_getPath().decode('utf-8', errors='replace')
    
    def set_path(self, path: str) -> int:
        return self._dll.dm_setPath(path.encode())
    
    def get_base_path(self) -> str:
        return self._dll.dm_getBasePath().decode('utf-8', errors='replace')
    
    # ---- 窗口 ----
    def find_window(self, cls: str = "", title: str = "") -> int:
        return self._dll.dm_findWindow(cls.encode(), title.encode())
    
    def get_foreground_window(self) -> int:
        return self._dll.dm_getForegroundWindow()
    
    def get_window_title(self, hwnd: int) -> str:
        return self._dll.dm_getWindowTitle(hwnd).decode('utf-8', errors='replace')
    
    def get_window_class(self, hwnd: int) -> str:
        return self._dll.dm_getWindowClass(hwnd).decode('utf-8', errors='replace')
    
    def get_window_process_path(self, hwnd: int) -> str:
        return self._dll.dm_getWindowProcessPath(hwnd).decode('utf-8', errors='replace')
    
    # ---- 键鼠 ----
    def move_to(self, x: int, y: int) -> int:
        return self._dll.dm_moveTo(x, y)
    
    def left_click(self) -> int:
        return self._dll.dm_leftClick()
    
    def key_press(self, vk: int) -> int:
        return self._dll.dm_keyPress(vk)
    
    # ---- 图色 ----
    def get_color(self, x: int, y: int) -> str:
        return self._dll.dm_getColor(x, y).decode()
    
    def capture(self, x1: int, y1: int, x2: int, y2: int, file: str) -> int:
        return self._dll.dm_capture(x1, y1, x2, y2, file.encode())
    
    # ---- 系统 ----
    def get_screen_width(self) -> int:
        return self._dll.dm_getScreenWidth()
    
    def get_screen_height(self) -> int:
        return self._dll.dm_getScreenHeight()
    
    def delay(self, ms: int):
        self._dll.dm_delay(ms)
    
    def get_time(self) -> int:
        return self._dll.dm_getTime()
    
    def get_os_type(self) -> int:
        return self._dll.dm_getOsType()
    
    def get_machine_code(self) -> str:
        return self._dll.dm_getMachineCode().decode('utf-8', errors='replace')
    
    def execute_cmd(self, cmd: str, timeout: int = 30000) -> str:
        return self._dll.dm_executeCmd(cmd.encode(), timeout).decode('utf-8', errors='replace')


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
    print(f"  对象ID: {dm.get_id()}")
    print(f"  全局路径: {dm.get_path()}")
    print(f"  基础路径: {dm.get_base_path()}")
    print(f"  机器码: {dm.get_machine_code()}")
    
    # 系统信息
    print(f"\n[系统信息]")
    print(f"  屏幕: {dm.get_screen_width()}x{dm.get_screen_height()}")
    print(f"  系统类型: {dm.get_os_type()}")
    print(f"  当前时间戳: {dm.get_time()}")
    
    # 窗口信息
    print(f"\n[窗口信息]")
    fg = dm.get_foreground_window()
    if fg:
        print(f"  前台窗口句柄: {fg}")
        print(f"  窗口标题: {dm.get_window_title(fg)}")
        print(f"  窗口类: {dm.get_window_class(fg)}")
        print(f"  进程路径: {dm.get_window_process_path(fg)}")

    # 键鼠示例：把鼠标移动到 (100, 100)
    print(f"\n[键鼠]")
    dm.move_to(100, 100)
    print(f"  移动鼠标到 (100, 100) 完成")

    print(f"\n[完成] dm_hook.dll 加载成功，所有 API 可用。")
    print(f"      详细 API 文档请参考: dm.h 头文件")

if __name__ == "__main__":
    main()