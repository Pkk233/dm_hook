# dm_hook.dll 函数使用文档

> 大漠插件 (dm.dll) 开源重新实现 — 接口完全兼容原版大漠插件
> 采用 C 风格导出，支持跨语言调用（C++、Python、AHK 等）

---

## 目录

- [1. 基本设置](#1-基本设置)
- [2. 窗口操作](#2-窗口操作)
- [3. 键鼠操作](#3-键鼠操作)
- [4. 图色操作](#4-图色操作)
- [5. 后台设置](#5-后台设置)
- [6. 内存操作](#6-内存操作)
- [7. 文件操作](#7-文件操作)
- [8. 文字识别](#8-文字识别)
- [9. 系统操作](#9-系统操作)
- [10. 杂项](#10-杂项)
- [11. 汇编](#11-汇编)
- [12. AI 功能](#12-ai-功能)
- [13. Foobar 界面控件](#13-foobar-界面控件)
- [14. 答题](#14-答题)
- [15. 算法](#15-算法)
- [16. 防护盾](#16-防护盾)
- [附录：错误码](#附录错误码)
- [附录：跨语言调用示例](#附录跨语言调用示例)

---

## 1. 基本设置

### dm_reg
```c
long dm_reg(const char* reg_code, const char* ver_info);
```
**说明**: 注册插件（按机器码）。

**参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| reg_code | const char* | 注册码 |
| ver_info | const char* | 版本信息（可空） |

**返回**: 0=成功，非0=失败（见错误码）

**示例 (Python)**:
```python
dm.reg("my_reg_code", "1.0.0")
```

---

### dm_regEx
```c
long dm_regEx(const char* reg_code, const char* ver_info, const char* ip);
```
**说明**: 注册插件（按 IP 地址）。

---

### dm_ver
```c
const char* dm_ver();
```
**说明**: 获取插件版本号。

**返回**: 版本字符串，如 `"1.0.0"`

**示例 (Python)**:
```python
ver = dm.ver()
print(f"版本: {ver}")
```

---

### dm_getID
```c
long dm_getID();
```
**说明**: 获取当前对象 ID（每个实例唯一）。

---

### dm_getDmCount
```c
long dm_getDmCount();
```
**说明**: 获取当前进程中的 dm 对象数量。

---

### dm_getLastError
```c
long dm_getLastError();
```
**说明**: 获取最后错误码。

---

### dm_getPath / dm_setPath
```c
const char* dm_getPath();
long dm_setPath(const char* path);
```
**说明**: 获取/设置全局路径（用于图色、字库等文件查找）。

**示例 (Python)**:
```python
dm.set_path("C:\\dm_project")
path = dm.get_path()
```

---

### dm_getBasePath
```c
const char* dm_getBasePath();
```
**说明**: 获取 DLL 所在的基础路径。

---

### dm_setShowErrorMsg
```c
long dm_setShowErrorMsg(long show);
```
**说明**: 设置是否显示错误消息框。`show=0` 隐藏，`show=1` 显示。

---

### dm_enablePicCache
```c
long dm_enablePicCache(long enable);
```
**说明**: 启用/禁用图片缓存。`enable=1` 启用，可提高重复找图速度。

---

## 2. 窗口操作

### dm_findWindow
```c
long dm_findWindow(const char* cls, const char* title);
```
**说明**: 查找窗口（类名和标题支持模糊匹配）。

**参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| cls | const char* | 窗口类名（可空，传空字符串时忽略） |
| title | const char* | 窗口标题（可空） |

**返回**: 窗口句柄（HWND），0=未找到

**示例 (Python)**:
```python
hwnd = dm.find_window("Notepad", "")       # 按类名
hwnd = dm.find_window("", "计算器")         # 按标题
hwnd = dm.find_window("Notepad", "无标题")  # 组合查找
```

---

### dm_findWindowEx
```c
long dm_findWindowEx(intptr_t parent, long cls, const char* title);
```
**说明**: 在指定父窗口中查找子窗口。

---

### dm_findWindowByProcess
```c
long dm_findWindowByProcess(const char* process_name, const char* cls, const char* title);
```
**说明**: 按进程名查找窗口。

---

### dm_findWindowByProcessId
```c
long dm_findWindowByProcessId(long process_id, const char* cls, const char* title);
```
**说明**: 按进程 ID 查找窗口。

---

### dm_findWindowSuper
```c
long dm_findWindowSuper(const char* cls, const char* title, long flag);
```
**说明**: 高级窗口查找。

**flag 取值**:
| 值 | 说明 |
|----|------|
| 0 | 匹配窗口标题完全一致 |
| 1 | 匹配窗口标题模糊 |
| 2 | 匹配窗口类名 |
| 4 | 匹配窗口标题正则 |
| 8 | 匹配窗口类名正则 |

---

### dm_enumWindow
```c
long dm_enumWindow(intptr_t parent, const char* title, long filter);
```
**说明**: 枚举窗口，返回句柄字符串（以 `|` 分隔）。

---

### dm_enumWindowByProcess
```c
long dm_enumWindowByProcess(const char* process_name, const char* title, long filter);
```
**说明**: 枚举指定进程的所有窗口。

---

### dm_getForegroundWindow
```c
long dm_getForegroundWindow();
```
**说明**: 获取前台窗口句柄。

**示例 (Python)**:
```python
hwnd = dm.get_foreground_window()
title = dm.get_window_title(hwnd)
print(f"当前窗口: {title}")
```

---

### dm_getWindowTitle / dm_getWindowClass
```c
const char* dm_getWindowTitle(intptr_t hwnd);
const char* dm_getWindowClass(intptr_t hwnd);
```
**说明**: 获取窗口标题/类名。

---

### dm_getWindowProcessId / dm_getWindowProcessPath
```c
long dm_getWindowProcessId(intptr_t hwnd);
const char* dm_getWindowProcessPath(intptr_t hwnd);
```
**说明**: 获取窗口所属进程的 PID/路径。

---

### dm_getWindowRect / dm_getClientRect
```c
long dm_getWindowRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
long dm_getClientRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
```
**说明**: 获取窗口矩形/客户区矩形。

---

### dm_moveWindow / dm_setWindowSize
```c
long dm_moveWindow(intptr_t hwnd, long x, long y);
long dm_setWindowSize(intptr_t hwnd, long width, long height);
```
**说明**: 移动窗口/调整窗口大小。

---

### dm_setWindowState
```c
long dm_setWindowState(intptr_t hwnd, long flag);
```
**说明**: 设置窗口状态。

**flag 取值**:
| 值 | 说明 |
|----|------|
| 0 | 关闭窗口 |
| 1 | 激活窗口 |
| 2 | 最小化 |
| 3 | 最大化 |
| 4 | 还原 |

---

### dm_sendString / dm_sendString2
```c
long dm_sendString(intptr_t hwnd, const char* str);
long dm_sendString2(intptr_t hwnd, const char* str);
```
**说明**: 向窗口发送文本字符串（后台）。

---

## 3. 键鼠操作

### 键盘操作

```c
long dm_keyDown(long vk_code);       // 按下键
long dm_keyUp(long vk_code);         // 弹起键
long dm_keyPress(long vk_code);      // 按键（按下+弹起）
long dm_keyPressChar(const char* key_str);  // 按字符键
```

**示例 (Python)**:
```python
dm.key_press(0x0D)     # 按回车键 (VK_RETURN)
dm.key_press(0x1B)     # 按 ESC 键
dm.key_press(ord('A')) # 按 A 键
```

**常用虚拟键码**:
| 键 | 虚拟键码 |
|----|---------|
| 鼠标左键 | 0x01 |
| 鼠标右键 | 0x02 |
| Backspace | 0x08 |
| Tab | 0x09 |
| Enter | 0x0D |
| Shift | 0x10 |
| Ctrl | 0x11 |
| Alt | 0x12 |
| ESC | 0x1B |
| Space | 0x20 |
| PageUp | 0x21 |
| PageDown | 0x22 |
| End | 0x23 |
| Home | 0x24 |
| ← | 0x25 |
| ↑ | 0x26 |
| → | 0x27 |
| ↓ | 0x28 |
| A-Z | 0x41-0x5A |
| 0-9 | 0x30-0x39 |
| F1-F12 | 0x70-0x7B |

---

### 鼠标操作

```c
long dm_moveTo(long x, long y);           // 移动鼠标到绝对坐标
long dm_moveToEx(long x, long y, long w, long h);  // 移动到目标区域内的随机点
long dm_moveR(long rx, long ry);          // 相对移动
long dm_leftClick();                      // 左键单击
long dm_leftDoubleClick();                // 左键双击
long dm_leftDown();                       // 左键按下
long dm_leftUp();                         // 左键弹起
long dm_rightClick();                     // 右键单击
long dm_middleClick();                    // 中键单击
long dm_wheelDown(long repeat);           // 滚轮向下
long dm_wheelUp(long repeat);             // 滚轮向上
```

**示例 (Python)**:
```python
dm.move_to(500, 300)      # 移动到 (500, 300)
dm.left_click()            # 左键单击
dm.move_to(500, 300)
dm.left_double_click()     # 左键双击
```

---

### 键鼠设置

```c
long dm_setMouseDelay(long type, long delay);  // 设置鼠标延迟
long dm_setKeypadDelay(long type, long delay); // 设置键盘延迟
long dm_setMouseSpeed(long speed);             // 设置鼠标速度 (1-20)
long dm_setSimMode(long mode);                 // 设置模拟模式
```

**type (延迟类型)**: 0=普通延迟, 1=Windows 延迟

---

## 4. 图色操作

### 截图

```c
long dm_capture(long x1, long y1, long x2, long y2, const char* file);
long dm_capturePng(long x1, long y1, long x2, long y2, const char* file);
long dm_captureJpg(long x1, long y1, long x2, long y2, const char* file, long quality);
long dm_captureGif(long x1, long y1, long x2, long y2, const char* file, long delay, long time);
```

**示例 (Python)**:
```python
dm.capture(0, 0, 800, 600, "screen.bmp")
dm.capture_png(100, 100, 400, 300, "region.png")
dm.capture_jpg(0, 0, 1920, 1080, "screenshot.jpg", 85)
```

---

### 颜色获取

```c
const char* dm_getColor(long x, long y);       // 获取颜色 (RRGGBB 格式)
const char* dm_getColorBGR(long x, long y);    // 获取颜色 (BBGGRR 格式)
const char* dm_getColorHSV(long x, long y);    // 获取颜色 (HSV 格式)
```

**示例 (Python)**:
```python
color = dm.get_color(100, 100)
print(f"(100,100) 的颜色: #{color}")  # 如 "AABBCC"
```

---

### 颜色比较

```c
long dm_cmpColor(long x, long y, const char* color, long sim);
```

**说明**: 比较指定点的颜色。

**参数**:
| 参数 | 说明 |
|------|------|
| color | 颜色字符串，格式 `"RRGGBB"` 或 `"RRGGBB-色偏"` |
| sim | 相似度 (0.0-1.0)，如 0.9 表示 90% 相似 |

---

### 找色

```c
long dm_findColor(long x1, long y1, long x2, long y2, const char* color, long sim, long dir, long* intX, long* intY);
const char* dm_findColorEx(long x1, long y1, long x2, long y2, const char* color, long sim, long dir);
```

**dir (查找方向)**:
| 值 | 说明 |
|----|------|
| 0 | 从左到右，从上到下 |
| 1 | 从左到右，从下到上 |
| 2 | 从右到左，从上到下 |
| 3 | 从右到左，从下到上 |
| 4 | 从中心开始 |
| 5 | 从左上角开始 |

**示例 (Python)**:
```python
# 在屏幕区域找红色
x, y = ctypes.c_long(0), ctypes.c_long(0)
ret = dm.find_color(0, 0, 1920, 1080, "FF0000-000000", 0.9, 0, x, y)
if ret == 0:
    print(f"找到红色在 ({x.value}, {y.value})")
```

---

### 找图

```c
long dm_findPic(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY);
const char* dm_findPicEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
```

**参数**:
| 参数 | 说明 |
|------|------|
| pic_name | 图片文件名（多个用 `|` 分隔） |
| delta_color | 偏色值，如 `"202020-000000"` |
| sim | 相似度 (0.0-1.0) |

**示例 (Python)**:
```python
x, y = ctypes.c_long(0), ctypes.c_long(0)
ret = dm.find_pic(0, 0, 1920, 1080, "button.bmp", "202020", 0.9, 0, x, y)
if ret == 0:
    print(f"找到按钮在 ({x.value}, {y.value})")
else:
    print("未找到按钮")
```

---

### 高级找图

```c
long dm_findPicS(...);       // 找图，支持多图，返回第一个找到的
long dm_findPicMem(...);     // 从内存中找图
long dm_findPicSim(...);     // 智能找图，支持透明图
long dm_findMultiColor(...); // 多色找图
long dm_findColorBlock(...); // 色块找图
long dm_findShape(...);      // 形状找图
```

---

### 图片加载/管理

```c
long dm_loadPic(const char* pic_name);             // 加载图片到内存
long dm_freePic(const char* pic_name);             // 释放图片
const char* dm_getPicSize(const char* pic_name);   // 获取图片尺寸
long dm_imageToBmp(const char* pic_name, const char* bmp_name); // 转换图片格式
```

---

## 5. 后台设置

### 绑定窗口

```c
long dm_bindWindow(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, long mode);
long dm_bindWindowEx(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, const char* pub, long mode);
```

**display/鼠标/键盘模式**:
| 模式 | 说明 |
|------|------|
| "normal" | 普通模式（前台） |
| "dx" | DirectX 模式 |
| "dx2" | DirectX 2 模式 |
| "dx3" | DirectX 3 模式 |
| "gdi" | GDI 模式 |
| "gdi2" | GDI 2 模式 |
| "opengl" | OpenGL 模式 |

**示例 (Python)**:
```python
dm.bind_window(hwnd, "dx", "dx", "dx", 0)
# 后台操作...
dm.un_bind_window()
```

---

### 其他后台设置

```c
long dm_unBindWindow();                   // 解绑窗口
long dm_getBindWindow();                  // 获取绑定的窗口句柄
long dm_forceUnBindWindow();              // 强制解绑
long dm_lockInput(long enable);           // 锁定鼠标键盘输入
long dm_enableBind(long enable);          // 启用绑定模式
long dm_enableFakeActive(long enable);    // 启用虚假激活
long dm_enableSpeedDx(long enable);       // 启用 DX 加速
long dm_setDisplayDelay(long delay);      // 设置显示延迟 (毫秒)
long dm_getFps();                         // 获取当前帧率
long dm_downCpu(long enable, long rate);  // 降低 CPU 占用
```

---

## 6. 内存操作

### 进程操作

```c
long dm_openProcess(long pid);                        // 打开进程
long dm_getModuleBaseAddr(intptr_t hwnd, const char* module);  // 获取模块基址
long dm_getModuleSize(intptr_t hwnd, const char* module);       // 获取模块大小
long dm_terminateProcess(long pid);                   // 终止进程
long dm_freeProcessMemory(intptr_t hwnd);             // 释放进程内存
```

---

### 读取内存

```c
long dm_readInt(intptr_t hwnd, long addr, long* ret);          // 读整数 (4字节)
long dm_readFloat(intptr_t hwnd, long addr, float* ret);      // 读浮点数
long dm_readDouble(intptr_t hwnd, long addr, double* ret);    // 读双精度浮点
const char* dm_readString(intptr_t hwnd, long addr, long type, long len);  // 读字符串
const char* dm_readData(intptr_t hwnd, long addr, long len);  // 读字节数据
```

---

### 写入内存

```c
long dm_writeInt(intptr_t hwnd, long addr, long type, long v);     // 写整数
long dm_writeFloat(intptr_t hwnd, long addr, float v);            // 写浮点数
long dm_writeDouble(intptr_t hwnd, long addr, double v);          // 写双精度
long dm_writeString(intptr_t hwnd, long addr, long type, const char* v);  // 写字符串
long dm_writeData(intptr_t hwnd, long addr, const char* data);   // 写字节数据
```

---

### 内存搜索

```c
const char* dm_findInt(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max);
const char* dm_findFloat(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max);
const char* dm_findString(intptr_t hwnd, const char* addr_range, const char* string_value, long type);
const char* dm_findData(intptr_t hwnd, const char* addr_range, const char* data);
```

**addr_range 格式**: `"起始地址-结束地址"`，如 `"00400000-004FFFFF"`

---

### 虚拟内存操作

```c
long dm_virtualAllocEx(intptr_t hwnd, long size, long type, long protect);  // 分配虚拟内存
long dm_virtualFreeEx(intptr_t hwnd, long addr);                            // 释放虚拟内存
long dm_virtualProtectEx(intptr_t hwnd, long addr, long size, long type, long* old_protect);  // 修改保护属性
const char* dm_virtualQueryEx(intptr_t hwnd, long addr, long pmbi);        // 查询虚拟内存信息
```

---

## 7. 文件操作

```c
long dm_writeFile(const char* file, const char* content);     // 写文件
const char* dm_readFile(const char* file);                    // 读文件
long dm_deleteFile(const char* file);                         // 删除文件
long dm_copyFile(const char* src, const char* dest, long over); // 复制文件
long dm_moveFile(const char* src, const char* dest);          // 移动文件
long dm_createFolder(const char* folder);                      // 创建文件夹
long dm_deleteFolder(const char* folder);                      // 删除文件夹
long dm_isFileExist(const char* file);                         // 判断文件是否存在
long dm_isFolderExist(const char* folder);                     // 判断文件夹是否存在
long dm_getFileLength(const char* file);                       // 获取文件大小
long dm_downloadFile(const char* url, const char* save_file, long timeout);  // 下载文件
```

### INI 文件操作

```c
long dm_writeIni(const char* section, const char* key, const char* value, const char* file);
const char* dm_readIni(const char* section, const char* key, const char* file);
long dm_deleteIni(const char* section, const char* key, const char* file);
const char* dm_enumIniKey(const char* section, const char* file);
const char* dm_enumIniSection(const char* file);
```

**示例 (Python)**:
```python
dm.write_ini("config", "username", "admin", "settings.ini")
username = dm.read_ini("config", "username", "settings.ini")
```

---

## 8. 文字识别

### 字库管理

```c
long dm_setDict(long index, const char* file);         // 加载字库文件
long dm_setDictMem(long index, const char* data, long size);  // 从内存加载字库
long dm_useDict(long index);                           // 使用指定字库
long dm_getNowDict();                                  // 获取当前字库索引
long dm_addDict(const char* dict_info);                // 添加字库
long dm_saveDict(long index, const char* file);       // 保存字库
long dm_clearDict(long index);                         // 清除字库
long dm_enableShareDict(long enable);                  // 启用字库共享
```

---

### OCR 文字识别

```c
const char* dm_ocr(long x1, long y1, long x2, long y2, const char* color_format, long sim);
const char* dm_ocrEx(long x1, long y1, long x2, long y2, const char* color_format, long sim);
```

**示例 (Python)**:
```python
dm.set_dict(0, "ocr_dict.txt")  # 加载字库
text = dm.ocr(100, 100, 500, 200, "000000-000000", 0.9)
print(f"识别结果: {text}")
```

---

### 文字查找

```c
long dm_findStr(long x1, long y1, long x2, long y2, const char* str,
                const char* color_format, long sim, long* intX, long* intY);
long dm_findStrFast(long x1, long y1, long x2, long y2, const char* str,
                    const char* color_format, long sim, long* intX, long* intY);
```

---

### 字库参数设置

```c
long dm_setColGapNoDict(long col_gap);              // 设置列间距
long dm_setRowGapNoDict(long row_gap);              // 设置行间距
long dm_setWordGap(long word_gap);                  // 设置字间距
long dm_setWordLineHeight(long line_height);         // 设置行高
long dm_setExactOcr(long exact_ocr);                // 设置精确识别模式
```

---

## 9. 系统操作

### 延时

```c
long dm_delay(long mis);           // 延时 (毫秒)
long dm_delays(long mis_min, long mis_max);  // 随机延时
```

---

### 进程/应用

```c
long dm_runApp(const char* app_path, long mode);  // 运行程序
long dm_stop(long id);                              // 停止进程
```

**mode 取值**:
| 值 | 说明 |
|----|------|
| 0 | 常规模式 |
| 1 | 隐藏模式 |
| 2 | 最小化 |
| 3 | 最大化 |

---

### 系统信息

```c
long dm_getScreenWidth();         // 获取屏幕宽度
long dm_getScreenHeight();        // 获取屏幕高度
long dm_getScreenDepth();         // 获取屏幕色深
long dm_getDPI();                 // 获取 DPI
long dm_getTime();                // 获取当前时间戳 (毫秒)
long dm_getOsType();             // 获取操作系统类型
long dm_getOsBuildNumber();      // 获取系统构建号
long dm_is64Bit();               // 判断是否为 64 位系统
long dm_getCpuUsage();           // 获取 CPU 使用率
long dm_getMemoryUsage();        // 获取内存使用率
const char* dm_getMachineCode(); // 获取机器码
const char* dm_getDiskSerial();  // 获取磁盘序列号
const char* dm_getNetTime();     // 获取网络时间
```

**示例 (Python)**:
```python
w = dm.get_screen_width()
h = dm.get_screen_height()
print(f"屏幕分辨率: {w}x{h}")
print(f"操作系统: {dm.get_os_type()}")
print(f"机器码: {dm.get_machine_code()}")
```

---

### 杂项系统

```c
long dm_beep(long f, long duration);     // 蜂鸣
long dm_play(const char* media_file);    // 播放音频
long dm_exitOs(long type);               // 关机/重启
long dm_setScreen(long width, long height, long depth);  // 设置分辨率
long dm_getClipboard();                  // 获取剪贴板
long dm_setClipboard(const char* value); // 设置剪贴板
```

**exitOs type 取值**:
| 值 | 说明 |
|----|------|
| 0 | 注销 |
| 1 | 关机 |
| 2 | 重启 |

---

## 10. 杂项

```c
long dm_enterCri();              // 进入临界区
long dm_leaveCri();              // 离开临界区
long dm_releaseRef();            // 释放引用
long dm_setExitThread(long mode); // 设置线程退出模式
const char* dm_executeCmd(const char* cmd, long time_out);  // 执行命令行
```

---

## 11. 汇编

```c
long dm_asmAdd(const char* asm_ins);                              // 添加汇编指令
long dm_asmCall(intptr_t hwnd, long mode);                       // 执行汇编代码
long dm_asmCallEx(intptr_t hwnd, long mode, long base_addr, long call_addr);  // 在指定地址执行
long dm_asmClear();                                               // 清除汇编代码
long dm_asmSetTimeout(long timeout);                              // 设置超时
const char* dm_assemble(long base_addr, long is_asm_code);       // 汇编编译
const char* dm_disAssemble(long asm_code, long base_addr, long is_asm_code);  // 反汇编
```

**示例 (Python)**:
```python
dm.asm_clear()
dm.asm_add("push 0")
dm.asm_add("push 0")
dm.asm_add("call 0x401000")
dm.asm_add("add esp, 8")
dm.asm_call(hwnd, 0)
```

---

## 12. AI 功能

### AI 找图

```c
long dm_loadAi(const char* file);                                                     // 加载 AI 模型
long dm_aiFindPic(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir, long* intX, long* intY);
long dm_aiFindPicEx(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir);
```

---

### YOLO 目标检测

```c
long dm_aiYoloSetModel(const char* model_file, const char* model_type);  // 设置 YOLO 模型
long dm_aiYoloSetVersion(const char* version);                           // 设置 YOLO 版本
const char* dm_aiYoloDetectObjects(long x1, long y1, long x2, long y2, float prob, float iou);  // 目标检测
```

**示例 (Python)**:
```python
dm.ai_yolo_set_model("yolov5.onnx", "yolov5")
objects = dm.ai_yolo_detect_objects(0, 0, 1920, 1080, 0.5, 0.45)
print(f"检测到: {objects}")
```

---

## 13. Foobar 界面控件

Foobar 是在窗口上创建浮动图形控件的功能，用于绘制文字、矩形、图片等。

### 创建控件

```c
long dm_createFoobarRect(intptr_t hwnd, long x, long y, long w, long h);     // 矩形区域
long dm_createFoobarEllipse(intptr_t hwnd, long x, long y, long w, long h);  // 椭圆区域
long dm_createFoobarRoundRect(intptr_t hwnd, long x, long y, long w, long h, long rw, long rh);  // 圆角矩形
long dm_createFoobarCustom(intptr_t hwnd, long x, long y, long w, long h, const char* pic);  // 自定义形状
```

---

### 绘制操作

```c
long dm_foobarDrawText(intptr_t hwnd, long x, long y, long w, long h, const char* text, long color, long align);
long dm_foobarPrintText(intptr_t hwnd, const char* text, long color);
long dm_foobarDrawRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
long dm_foobarDrawLine(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
long dm_foobarFillRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color);
long dm_foobarDrawPic(intptr_t hwnd, long x, long y, const char* pic);
```

---

## 14. 答题

```c
long dm_faqCapture(long x1, long y1, long x2, long y2, long quality, long delay, long time);
long dm_faqPost(const char* server, long handle, long request_type, long time_out);
long dm_faqSend(long handle, long time_out);
long dm_faqFetch(long time_out);
long dm_faqCancel();
long dm_faqIsPosted();
```

---

## 15. 算法

```c
const char* dm_excludePos(const char* all_pos, long type, long x1, long y1, long x2, long y2);
const char* dm_findNearestPos(const char* all_pos, long type, long x, long y);
const char* dm_sortPosDistance(const char* all_pos, long type, long x, long y);
```

**说明**: 坐标处理工具，用于筛选/排序一组坐标点。

---

## 16. 防护盾

```c
long dm_dmGuard(long enable, const char* type);                    // 启用/禁用防护盾
long dm_dmGuardExtract(const char* type, const char* file);       // 提取驱动文件
long dm_dmGuardLoadCustom(const char* type, const char* data);    // 加载自定义数据
const char* dm_dmGuardParams(const char* cmd, const char* param); // 设置参数
long dm_unLoadDriver();                                            // 卸载驱动
```

---

## 附录：错误码

| 错误码 | 常量名 | 说明 |
|--------|--------|------|
| 0 | DMERR_SUCCESS | 成功 |
| -1 | DMERR_NEED_REG | 需要注册 |
| -2 | DMERR_WINDOW_PROTECTED | 窗口保护 |
| -3 | DMERR_WINDOW_ABNORMAL | 窗口异常 |
| -4 | DMERR_EXCEPTION | 异常 |
| -5 | DMERR_NEED_RESTART | 需要重启 |
| -6 | DMERR_BLOCKED_BY_AV | 被杀毒软件拦截 |
| -8 | DMERR_PROCESS_PROTECTED | 进程保护 |
| -13 | DMERR_BIND_NOT_RELEASED | 绑定未释放 |
| -14 | DMERR_MISSING_DLL | 缺少 DLL |

---

## 附录：跨语言调用示例

### C++ 调用

```cpp
#include "dm.h"
#include <iostream>

int main() {
    // 加载 DLL
    HMODULE hDll = LoadLibraryA("dm_hook.dll");
    if (!hDll) {
        std::cerr << "加载 dm_hook.dll 失败" << std::endl;
        return 1;
    }
    
    // 获取函数指针
    auto dm_ver = (decltype(::dm_ver)*)GetProcAddress(hDll, "dm_ver");
    auto dm_getID = (decltype(::dm_getID)*)GetProcAddress(hDll, "dm_getID");
    auto dm_getScreenWidth = (decltype(::dm_getScreenWidth)*)GetProcAddress(hDll, "dm_getScreenWidth");
    auto dm_getScreenHeight = (decltype(::dm_getScreenHeight)*)GetProcAddress(hDll, "dm_getScreenHeight");
    
    // 调用
    std::cout << "版本: " << dm_ver() << std::endl;
    std::cout << "对象ID: " << dm_getID() << std::endl;
    std::cout << "屏幕: " << dm_getScreenWidth() << "x" << dm_getScreenHeight() << std::endl;
    
    FreeLibrary(hDll);
    return 0;
}
```

### Python 调用 (使用 dm_hook.py)

```python
from dm_hook import DmHook

dm = DmHook()

# 基本信息
print(f"版本: {dm.ver()}")
print(f"对象ID: {dm.get_id()}")

# 屏幕信息
print(f"屏幕: {dm.get_screen_width()}x{dm.get_screen_height()}")

# 窗口操作
hwnd = dm.get_foreground_window()
print(f"前台窗口: {dm.get_window_title(hwnd)}")

# 键鼠操作
dm.move_to(500, 300)
dm.left_click()

# 截图
dm.capture(0, 0, 800, 600, "capture.bmp")
```

### AHK 调用 (AutoHotkey)

```autohotkey
; 加载 DLL
hModule := DllCall("LoadLibrary", "Str", "dm_hook.dll", "Ptr")

; 调用函数
ver := DllCall("dm_hook.dll\dm_ver", "AStr")
MsgBox, 版本: %ver%

id := DllCall("dm_hook.dll\dm_getID")
MsgBox, 对象ID: %id%

; 获取屏幕尺寸
width := DllCall("dm_hook.dll\dm_getScreenWidth")
height := DllCall("dm_hook.dll\dm_getScreenHeight")
MsgBox, 屏幕: %width%x%height%

; 获取前台窗口标题
hwnd := DllCall("dm_hook.dll\dm_getForegroundWindow")
title := DllCall("dm_hook.dll\dm_getWindowTitle", "Ptr", hwnd, "AStr")
MsgBox, 前台窗口: %title%

; 释放
DllCall("FreeLibrary", "Ptr", hModule)
```

### VBS / VBA 调用

```vbs
Set dm = CreateObject("dm_hook.dmsoft")
MsgBox "版本: " & dm.Ver()
MsgBox "屏幕: " & dm.GetScreenWidth() & "x" & dm.GetScreenHeight()
```

---

> **文档版本**: 1.0  
> **最后更新**: 2026-08-13  
> **项目地址**: [https://github.com/Pkk233/dm_hook](https://github.com/Pkk233/dm_hook)