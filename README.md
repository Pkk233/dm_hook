# dm_hook — 大漠插件 (dm.dll) 开源重新实现

基于原版大漠插件接口文档，使用 C++ 重新实现的 Windows 自动化库。  
**完全兼容原版 dm.dll 的函数签名与行为规范**，支持跨语言调用。

## 项目结构

```
dm_hook/
├── dm.h              # 主头文件：204 个 API 声明 + 内部数据结构
├── dm.cpp            # 实现文件：所有 API 功能实现
├── dllmain.cpp       # DLL 入口：GDI+ 初始化 + TLS 管理
├── CMakeLists.txt    # CMake 构建配置
├── build.bat         # Windows 一键构建脚本
├── dm_hook.py        # Python 绑定示例 (ctypes)
├── dm_hook.ahk       # AutoHotkey v1 绑定示例
└── README.md         # 本文件
```

## 功能模块

| 模块 | 文件目录 | 函数数 | 实现状态 |
|:-----|:---------|:------:|:--------:|
| 基本设置 | 基本设置 | 10 | ✅ 完整实现 |
| 窗口 | 窗口 | 38 | ✅ 完整实现 |
| 键鼠 | 键鼠 | 28 | ✅ 完整实现 |
| 图色 | 图色 | 48 | ✅ 核心实现 |
| 后台设置 | 后台设置 | 27 | ✅ 状态管理 |
| 内存 | 内存 | 46 | ✅ 核心实现 |
| 文件 | 文件 | 24 | ✅ 完整实现 |
| 文字识别 | 文字识别 | 36 | ⚡ 骨架 (OCR 需 Tesseract) |
| 系统 | 系统 | 42 | ✅ 完整实现 |
| 杂项 | 杂项 | 7 | ✅ 完整实现 |
| 汇编 | 汇编 | 8 | ⚡ 骨架 (需汇编器) |
| AI | Ai | 15 | ⚡ 骨架 (YOLO 需 ONNX) |
| Foobar | Foobar | 21 | ✅ 核心实现 |
| 答题 | 答题 | 9 | ⚡ 骨架 |
| 算法 | 算法 | 3 | ✅ 完整实现 |
| 防护盾 | 防护盾 | 5 | ✅ 状态管理 |
| **总计** | | **~367** | |

## 构建方式

### 方式一：一键构建 (推荐)

```
cd dm_hook
build.bat
```

### 方式二：CMake 直接构建

```
cd dm_hook
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

输出：`build/Release/dm_hook.dll`

## 跨语言调用

### Python (ctypes)

```python
from dm_hook import DmHook

dm = DmHook()
print(f"版本: {dm.ver()}")
print(f"屏幕: {dm.get_screen_width()}x{dm.get_screen_height()}")
fg = dm.get_foreground_window()
print(f"前台窗口标题: {dm.get_window_title(fg)}")
```

### AutoHotkey v1

```autohotkey
DllCall("LoadLibrary", "Str", "dm_hook.dll")
ver := DllCall("dm_ver", "AStr")
MsgBox % "版本: " . ver
```

## 依赖管理

### 核心依赖（零下载，直接编译）

| 依赖 | 用途 | 来源 |
|:-----|:-----|:-----|
| Windows SDK | 全部基础功能 | 系统自带 |
| GDI+ | 截图、图像处理 | 系统自带 |
| WinHTTP/urlmon | 网络下载 | 系统自带 |

### 可选依赖（需手动安装）

| 依赖 | 用途 | 下载 |
|:-----|:-----|:-----|
| Tesseract 5.x | OCR 文字识别 | [UB-Mannheim 预编译包](https://github.com/UB-Mannheim/tesseract/wiki) |
| ONNX Runtime | YOLO AI 推理 | [NuGet](https://www.nuget.org/packages/Microsoft.ML.OnnxRuntime) |
| Keystone / XEDParse | 汇编引擎 | [GitHub](https://github.com/keystone-engine/keystone) |

**启用方式**: `cmake -B build -DENABLE_TESSERACT=ON`

## 接口设计原则

1. **C 风格导出** — 所有函数使用 `extern "C" __declspec(dllexport)` + `__stdcall`
2. **线程安全** — 每个线程独立状态 (TLS)
3. **零 COM 依赖** — 无需注册，直接 LoadLibrary
4. **兼容原版签名** — 函数名、参数、返回值与原始 dm.dll 完全一致

## 许可证

MIT License