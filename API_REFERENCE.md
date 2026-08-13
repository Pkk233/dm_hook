# dm_hook.dll 完整函数使用说明

> **大漠插件 (dm.dll) 开源重新实现** — 接口完全兼容原版大漠插件
> 采用 C 风格导出（`__stdcall`），支持跨语言调用（C++、Python、AutoHotkey 等）
>
> **文档版本**: 1.1 | **最后更新**: 2026-08-13

---

## 目录

- [模块 1：基本设置](#模块-1基本设置)
- [模块 2：窗口操作](#模块-2窗口操作)
- [模块 3：键鼠操作](#模块-3键鼠操作)
- [模块 4：图色操作](#模块-4图色操作)
- [模块 5：后台设置](#模块-5后台设置)
- [模块 6：内存操作](#模块-6内存操作)
- [模块 7：文件操作](#模块-7文件操作)
- [模块 8：文字识别](#模块-8文字识别)
- [模块 9：系统操作](#模块-9系统操作)
- [模块 10：杂项](#模块-10杂项)
- [模块 11：汇编](#模块-11汇编)
- [模块 12：AI 功能](#模块-12ai-功能)
- [模块 13：Foobar 界面控件](#模块-13foobar-界面控件)
- [模块 14：答题](#模块-14答题)
- [模块 15：算法](#模块-15算法)
- [模块 16：防护盾](#模块-16防护盾)
- [附录 A：错误码表](#附录-a错误码表)
- [附录 B：虚拟键码表](#附录-b虚拟键码表)
- [附录 C：跨语言调用示例](#附录-c跨语言调用示例)

---

## 函数列表（按模块）

### 模块 1：基本设置（13 个）
| # | 函数 | 说明 |
|---|------|------|
| 1.1 | `dm_reg` | 注册插件（基于机器码） |
| 1.2 | `dm_regEx` | 注册插件（基于 IP） |
| 1.3 | `dm_regNoMac` | 注册插件（不绑定机器码） |
| 1.4 | `dm_regExNoMac` | 注册插件（IP + 不绑定机器码） |
| 1.5 | `dm_ver` | 获取插件版本号 |
| 1.6 | `dm_getID` | 获取当前对象 ID |
| 1.7 | `dm_getDmCount` | 获取进程中的 dm 对象数量 |
| 1.8 | `dm_getLastError` | 获取最后错误码 |
| 1.9 | `dm_getPath` | 获取全局路径 |
| 1.10 | `dm_setPath` | 设置全局路径 |
| 1.11 | `dm_getBasePath` | 获取 DLL 基础路径 |
| 1.12 | `dm_setShowErrorMsg` | 设置错误消息框显示 |
| 1.13 | `dm_enablePicCache` | 启用/禁用图片缓存 |

### 模块 2：窗口操作（40 个）
| # | 函数 | 说明 |
|---|------|------|
| 2.1 | `dm_findWindow` | 查找窗口（类名/标题） |
| 2.2 | `dm_findWindowEx` | 查找子窗口 |
| 2.3 | `dm_findWindowByProcess` | 按进程名查找窗口 |
| 2.4 | `dm_findWindowByProcessId` | 按进程 ID 查找窗口 |
| 2.5 | `dm_findWindowSuper` | 高级窗口查找（支持正则） |
| 2.6 | `dm_enumWindow` | 枚举窗口 |
| 2.7 | `dm_enumWindowByProcess` | 枚举进程窗口 |
| 2.8 | `dm_enumWindowByProcessId` | 按 PID 枚举窗口 |
| 2.9 | `dm_enumWindowSuper` | 高级窗口枚举 |
| 2.10 | `dm_enumProcess` | 枚举进程 |
| 2.11 | `dm_getWindow` | 获取关联窗口 |
| 2.12 | `dm_getForegroundWindow` | 获取前台窗口 |
| 2.13 | `dm_getForegroundFocus` | 获取焦点窗口 |
| 2.14 | `dm_getMousePointWindow` | 获取鼠标下方窗口 |
| 2.15 | `dm_getPointWindow` | 获取指定坐标处窗口 |
| 2.16 | `dm_getSpecialWindow` | 获取特殊窗口（桌面/任务栏） |
| 2.17 | `dm_getWindowClass` | 获取窗口类名 |
| 2.18 | `dm_getWindowProcessId` | 获取窗口进程 ID |
| 2.19 | `dm_getWindowProcessPath` | 获取窗口进程路径 |
| 2.20 | `dm_getWindowThreadId` | 获取窗口线程 ID |
| 2.21 | `dm_getWindowTitle` | 获取窗口标题 |
| 2.22 | `dm_getWindowRect` | 获取窗口矩形 |
| 2.23 | `dm_getClientRect` | 获取客户区矩形 |
| 2.24 | `dm_getClientSize` | 获取客户区大小 |
| 2.25 | `dm_getWindowState` | 获取窗口状态 |
| 2.26 | `dm_getProcessInfo` | 获取进程信息 |
| 2.27 | `dm_moveWindow` | 移动窗口 |
| 2.28 | `dm_setWindowSize` | 调整窗口大小 |
| 2.29 | `dm_setWindowState` | 设置窗口状态 |
| 2.30 | `dm_setWindowText` | 修改窗口标题 |
| 2.31 | `dm_setWindowTransparent` | 设置窗口透明度 |
| 2.32 | `dm_setClientSize` | 设置客户区大小 |
| 2.33 | `dm_setSendStringDelay` | 设置发送字符串延迟 |
| 2.34 | `dm_sendPaste` | 发送粘贴命令 |
| 2.35 | `dm_sendString` | 发送文本字符串 |
| 2.36 | `dm_sendString2` | 发送文本字符串（备选方式） |
| 2.37 | `dm_sendStringIme` | 通过 IME 发送字符串 |
| 2.38 | `dm_sendStringIme2` | 通过 IME 发送字符串（备选） |
| 2.39 | `dm_clientToScreen` | 客户区坐标转屏幕坐标 |
| 2.40 | `dm_screenToClient` | 屏幕坐标转客户区坐标 |

### 模块 3：键鼠操作（34 个）
| # | 函数 | 说明 |
|---|------|------|
| 3.1 | `dm_keyDown` | 按下键（虚拟键码） |
| 3.2 | `dm_keyDownChar` | 按下字符键 |
| 3.3 | `dm_keyPress` | 按键（按下+释放） |
| 3.4 | `dm_keyPressChar` | 按字符键 |
| 3.5 | `dm_keyPressStr` | 按字符串序列 |
| 3.6 | `dm_keyUp` | 释放键 |
| 3.7 | `dm_keyUpChar` | 释放字符键 |
| 3.8 | `dm_waitKey` | 等待按键 |
| 3.9 | `dm_getKeyState` | 获取按键状态 |
| 3.10 | `dm_leftClick` | 左键单击 |
| 3.11 | `dm_leftDoubleClick` | 左键双击 |
| 3.12 | `dm_leftDown` | 左键按下 |
| 3.13 | `dm_leftUp` | 左键释放 |
| 3.14 | `dm_rightClick` | 右键单击 |
| 3.15 | `dm_rightDown` | 右键按下 |
| 3.16 | `dm_rightUp` | 右键释放 |
| 3.17 | `dm_middleClick` | 中键单击 |
| 3.18 | `dm_middleDown` | 中键按下 |
| 3.19 | `dm_middleUp` | 中键释放 |
| 3.20 | `dm_moveTo` | 移动鼠标到绝对坐标 |
| 3.21 | `dm_moveToEx` | 移动鼠标到区域随机点 |
| 3.22 | `dm_moveR` | 相对移动鼠标 |
| 3.23 | `dm_wheelDown` | 滚轮向下 |
| 3.24 | `dm_wheelUp` | 滚轮向上 |
| 3.25 | `dm_getCursorPos` | 获取鼠标位置 |
| 3.26 | `dm_getCursorShape` | 获取光标形状 |
| 3.27 | `dm_getCursorShapeEx` | 获取光标形状详细信息 |
| 3.28 | `dm_getCursorSpot` | 获取光标热点 |
| 3.29 | `dm_setMouseDelay` | 设置鼠标延迟 |
| 3.30 | `dm_setKeypadDelay` | 设置键盘延迟 |
| 3.31 | `dm_setMouseSpeed` | 设置鼠标速度 |
| 3.32 | `dm_getMouseSpeed` | 获取鼠标速度 |
| 3.33 | `dm_enableMouseAccuracy` | 启用/禁用鼠标加速 |
| 3.34 | `dm_setSimMode` | 设置模拟模式 |

### 模块 4：图色操作（57 个）
| # | 函数 | 说明 |
|---|------|------|
| 4.1 | `dm_capture` | 截图保存为 BMP |
| 4.2 | `dm_captureGif` | 截图保存为 GIF 动画 |
| 4.3 | `dm_captureJpg` | 截图保存为 JPEG |
| 4.4 | `dm_capturePng` | 截图保存为 PNG |
| 4.5 | `dm_capturePre` | 保存上次截图结果 |
| 4.6 | `dm_getColor` | 获取颜色（RGB） |
| 4.7 | `dm_getColorBGR` | 获取颜色（BGR） |
| 4.8 | `dm_getColorHSV` | 获取颜色（HSV） |
| 4.9 | `dm_getColorNum` | 统计颜色像素数量 |
| 4.10 | `dm_getAveRGB` | 获取区域平均颜色（RGB） |
| 4.11 | `dm_getAveHSV` | 获取区域平均颜色（HSV） |
| 4.12 | `dm_cmpColor` | 比较颜色 |
| 4.13 | `dm_findColor` | 找色（单点） |
| 4.14 | `dm_findColorEx` | 找色（返回所有坐标） |
| 4.15 | `dm_findColorBlock` | 找色块 |
| 4.16 | `dm_findColorBlockEx` | 找色块（返回所有坐标） |
| 4.17 | `dm_findColorE` | 找色（返回坐标字符串） |
| 4.18 | `dm_findMulColor` | 多色查找 |
| 4.19 | `dm_findMultiColor` | 多点找色 |
| 4.20 | `dm_findMultiColorEx` | 多点找色（返回所有坐标） |
| 4.21 | `dm_findMultiColorE` | 多点找色（返回坐标字符串） |
| 4.22 | `dm_findPic` | 找图 |
| 4.23 | `dm_findPicE` | 找图（返回坐标字符串） |
| 4.24 | `dm_findPicEx` | 找图（返回所有坐标） |
| 4.25 | `dm_findPicExS` | 智能找图（返回所有坐标） |
| 4.26 | `dm_findPicS` | 智能找图 |
| 4.27 | `dm_findPicMem` | 内存找图 |
| 4.28 | `dm_findPicMemE` | 内存找图（返回坐标字符串） |
| 4.29 | `dm_findPicMemEx` | 内存找图（返回所有坐标） |
| 4.30 | `dm_findPicSim` | 智能找图（支持透明色） |
| 4.31 | `dm_findPicSimE` | 智能找图（返回坐标字符串） |
| 4.32 | `dm_findPicSimEx` | 智能找图（返回所有坐标） |
| 4.33 | `dm_findPicSimMem` | 智能找图（内存模式） |
| 4.34 | `dm_findPicSimMemE` | 智能找图内存版（返回坐标字符串） |
| 4.35 | `dm_findPicSimMemEx` | 智能找图内存版（返回所有坐标） |
| 4.36 | `dm_findShape` | 找形状 |
| 4.37 | `dm_findShapeE` | 找形状（返回坐标字符串） |
| 4.38 | `dm_findShapeEx` | 找形状（返回所有坐标） |
| 4.39 | `dm_getPicSize` | 获取图片尺寸 |
| 4.40 | `dm_freePic` | 释放图片缓存 |
| 4.41 | `dm_loadPic` | 加载图片到内存 |
| 4.42 | `dm_loadPicByte` | 从内存加载图片 |
| 4.43 | `dm_appendPicAddr` | 追加内存图片地址 |
| 4.44 | `dm_matchPicName` | 匹配图片路径 |
| 4.45 | `dm_imageToBmp` | 转换图片为 BMP 格式 |
| 4.46 | `dm_setPicPwd` | 设置图片密码 |
| 4.47 | `dm_setExcludeRegion` | 设置排除区域 |
| 4.48 | `dm_getScreenData` | 获取屏幕原始数据 |
| 4.49 | `dm_getScreenDataBmp` | 获取屏幕 BMP 数据 |
| 4.50 | `dm_enableDisplayDebug` | 启用显示调试 |
| 4.51 | `dm_enableFindPicMultithread` | 启用多线程找图 |
| 4.52 | `dm_enableGetColorByCapture` | 通过截图获取颜色 |
| 4.53 | `dm_setFindPicMultithreadCount` | 设置多线程找图线程数 |
| 4.54 | `dm_setFindPicMultithreadLimit` | 设置多线程找图限制 |
| 4.55 | `dm_bgr2RGB` | BGR 转 RGB |
| 4.56 | `dm_rgb2BGR` | RGB 转 BGR |
| 4.57 | `dm_isDisplayDead` | 检测画面是否静止 |

### 模块 5：后台设置（28 个）
| # | 函数 | 说明 |
|---|------|------|
| 5.1 | `dm_bindWindow` | 绑定窗口 |
| 5.2 | `dm_bindWindowEx` | 绑定窗口（扩展版） |
| 5.3 | `dm_unBindWindow` | 解绑窗口 |
| 5.4 | `dm_getBindWindow` | 获取绑定窗口 |
| 5.5 | `dm_isBind` | 检查窗口是否已绑定 |
| 5.6 | `dm_forceUnBindWindow` | 强制解绑窗口 |
| 5.7 | `dm_setAero` | 设置 Aero 效果 |
| 5.8 | `dm_lockInput` | 锁定输入 |
| 5.9 | `dm_lockMouseRect` | 锁定鼠标区域 |
| 5.10 | `dm_lockDisplay` | 锁定显示 |
| 5.11 | `dm_enableBind` | 启用绑定模式 |
| 5.12 | `dm_enableFakeActive` | 启用虚假激活 |
| 5.13 | `dm_enableIme` | 启用输入法 |
| 5.14 | `dm_enableKeypadMsg` | 启用键盘消息同步 |
| 5.15 | `dm_enableKeypadPatch` | 启用键盘补丁 |
| 5.16 | `dm_enableKeypadSync` | 启用键盘同步 |
| 5.17 | `dm_enableMouseMsg` | 启用鼠标消息同步 |
| 5.18 | `dm_enableMouseSync` | 启用鼠标同步 |
| 5.19 | `dm_enableRealKeypad` | 启用真实键盘 |
| 5.20 | `dm_enableRealMouse` | 启用真实鼠标 |
| 5.21 | `dm_enableSpeedDx` | 启用 DX 加速 |
| 5.22 | `dm_getFps` | 获取帧率 |
| 5.23 | `dm_setDisplayDelay` | 设置显示延迟 |
| 5.24 | `dm_setDisplayRefreshDelay` | 设置刷新延迟 |
| 5.25 | `dm_setInputDm` | 设置 DM 输入模式 |
| 5.26 | `dm_hackSpeed` | 设置游戏速度 |
| 5.27 | `dm_downCpu` | 降低 CPU 占用 |
| 5.28 | `dm_switchBindWindow` | 切换绑定窗口 |

### 模块 6：内存操作（54 个）
| # | 函数 | 说明 |
|---|------|------|
| 6.1 | `dm_openProcess` | 打开进程 |
| 6.2 | `dm_getModuleBaseAddr` | 获取模块基址 |
| 6.3 | `dm_getModuleSize` | 获取模块大小 |
| 6.4 | `dm_getRemoteApiAddress` | 获取远程 API 地址 |
| 6.5 | `dm_readInt` | 读整数 |
| 6.6 | `dm_readIntAddr` | 读整数（多级指针） |
| 6.7 | `dm_readFloat` | 读浮点数 |
| 6.8 | `dm_readFloatAddr` | 读浮点数（多级指针） |
| 6.9 | `dm_readDouble` | 读双精度浮点数 |
| 6.10 | `dm_readDoubleAddr` | 读双精度浮点数（多级指针） |
| 6.11 | `dm_readString` | 读字符串 |
| 6.12 | `dm_readStringAddr` | 读字符串（多级指针） |
| 6.13 | `dm_readData` | 读字节数据 |
| 6.14 | `dm_readDataAddr` | 读字节数据（多级指针） |
| 6.15 | `dm_readDataToBin` | 读数据到二进制缓冲区 |
| 6.16 | `dm_readDataAddrToBin` | 读数据到二进制缓冲区（多级指针） |
| 6.17 | `dm_findInt` | 搜索整数 |
| 6.18 | `dm_findIntEx` | 高级整数搜索 |
| 6.19 | `dm_findFloat` | 搜索浮点数 |
| 6.20 | `dm_findFloatEx` | 高级浮点数搜索 |
| 6.21 | `dm_findDouble` | 搜索双精度浮点数 |
| 6.22 | `dm_findDoubleEx` | 高级双精度搜索 |
| 6.23 | `dm_findString` | 搜索字符串 |
| 6.24 | `dm_findStringEx` | 高级字符串搜索 |
| 6.25 | `dm_findData` | 搜索字节数据 |
| 6.26 | `dm_findDataEx` | 高级字节数据搜索 |
| 6.27 | `dm_writeInt` | 写整数 |
| 6.28 | `dm_writeIntAddr` | 写整数（多级指针） |
| 6.29 | `dm_writeFloat` | 写浮点数 |
| 6.30 | `dm_writeFloatAddr` | 写浮点数（多级指针） |
| 6.31 | `dm_writeDouble` | 写双精度浮点数 |
| 6.32 | `dm_writeDoubleAddr` | 写双精度浮点数（多级指针） |
| 6.33 | `dm_writeString` | 写字符串 |
| 6.34 | `dm_writeStringAddr` | 写字符串（多级指针） |
| 6.35 | `dm_writeData` | 写字节数据 |
| 6.36 | `dm_writeDataAddr` | 写字节数据（多级指针） |
| 6.37 | `dm_writeDataFromBin` | 从缓冲区写数据 |
| 6.38 | `dm_writeDataAddrFromBin` | 从缓冲区写数据（多级指针） |
| 6.39 | `dm_virtualAllocEx` | 分配虚拟内存 |
| 6.40 | `dm_virtualFreeEx` | 释放虚拟内存 |
| 6.41 | `dm_virtualProtectEx` | 修改内存保护属性 |
| 6.42 | `dm_virtualQueryEx` | 查询虚拟内存信息 |
| 6.43 | `dm_freeProcessMemory` | 释放进程内存资源 |
| 6.44 | `dm_getCommandLine` | 获取进程命令行 |
| 6.45 | `dm_terminateProcess` | 终止进程 |
| 6.46 | `dm_terminateProcessTree` | 终止进程树 |
| 6.47 | `dm_setMemoryFindResultToFile` | 设置搜索结果保存到文件 |
| 6.48 | `dm_setMemoryHwndAsProcessId` | 设置 hwnd 作为进程 ID |
| 6.49 | `dm_setParam64ToPointer` | 设置 64 位参数为指针 |
| 6.50 | `dm_int64ToInt32` | 64 位整数转 32 位地址 |
| 6.51 | `dm_intToData` | 整数转十六进制数据 |
| 6.52 | `dm_floatToData` | 浮点数转十六进制数据 |
| 6.53 | `dm_doubleToData` | 双精度转十六进制数据 |
| 6.54 | `dm_stringToData` | 字符串转十六进制数据 |

### 模块 7：文件操作（26 个）
| # | 函数 | 说明 |
|---|------|------|
| 7.1 | `dm_writeFile` | 写文件 |
| 7.2 | `dm_readFile` | 读文件 |
| 7.3 | `dm_deleteFile` | 删除文件 |
| 7.4 | `dm_copyFile` | 复制文件 |
| 7.5 | `dm_moveFile` | 移动文件 |
| 7.6 | `dm_createFolder` | 创建文件夹 |
| 7.7 | `dm_deleteFolder` | 删除文件夹 |
| 7.8 | `dm_isFileExist` | 判断文件是否存在 |
| 7.9 | `dm_isFolderExist` | 判断文件夹是否存在 |
| 7.10 | `dm_getFileLength` | 获取文件大小 |
| 7.11 | `dm_getRealPath` | 获取绝对路径 |
| 7.12 | `dm_selectFile` | 文件选择对话框 |
| 7.13 | `dm_selectDirectory` | 目录选择对话框 |
| 7.14 | `dm_downloadFile` | 下载文件 |
| 7.15 | `dm_encodeFile` | 加密文件 |
| 7.16 | `dm_decodeFile` | 解密文件 |
| 7.17 | `dm_writeIni` | 写 INI 文件 |
| 7.18 | `dm_readIni` | 读 INI 文件 |
| 7.19 | `dm_deleteIni` | 删除 INI 键值 |
| 7.20 | `dm_enumIniKey` | 枚举 INI 键名 |
| 7.21 | `dm_enumIniSection` | 枚举 INI 节名 |
| 7.22 | `dm_writeIniPwd` | 写加密 INI 文件 |
| 7.23 | `dm_readIniPwd` | 读加密 INI 文件 |
| 7.24 | `dm_deleteIniPwd` | 删除加密 INI 键值 |
| 7.25 | `dm_enumIniKeyPwd` | 枚举加密 INI 键名 |
| 7.26 | `dm_enumIniSectionPwd` | 枚举加密 INI 节名 |

### 模块 8：文字识别（46 个）
| # | 函数 | 说明 |
|---|------|------|
| 8.1 | `dm_setDict` | 加载字库文件 |
| 8.2 | `dm_setDictMem` | 从内存加载字库 |
| 8.3 | `dm_setDictPwd` | 设置字库密码 |
| 8.4 | `dm_useDict` | 切换当前字库 |
| 8.5 | `dm_getDict` | 获取字库信息 |
| 8.6 | `dm_getDictCount` | 获取字库字体数量 |
| 8.7 | `dm_getDictInfo` | 获取字库文件信息 |
| 8.8 | `dm_getNowDict` | 获取当前字库索引 |
| 8.9 | `dm_addDict` | 添加字体到字库 |
| 8.10 | `dm_saveDict` | 保存字库到文件 |
| 8.11 | `dm_clearDict` | 清除字库 |
| 8.12 | `dm_enableShareDict` | 启用字库共享 |
| 8.13 | `dm_ocr` | 文字识别 |
| 8.14 | `dm_ocrEx` | 增强文字识别 |
| 8.15 | `dm_ocrExOne` | 单行文字识别 |
| 8.16 | `dm_ocrInFile` | 识别结果保存到文件 |
| 8.17 | `dm_findStr` | 查找文字 |
| 8.18 | `dm_findStrE` | 查找文字（返回坐标字符串） |
| 8.19 | `dm_findStrEx` | 查找文字（返回所有坐标） |
| 8.20 | `dm_findStrS` | 智能查找文字 |
| 8.21 | `dm_findStrExS` | 智能查找文字（返回所有坐标） |
| 8.22 | `dm_findStrFast` | 快速查找文字 |
| 8.23 | `dm_findStrFastE` | 快速查找文字（返回坐标字符串） |
| 8.24 | `dm_findStrFastEx` | 快速查找文字（返回所有坐标） |
| 8.25 | `dm_findStrFastS` | 快速智能查找文字 |
| 8.26 | `dm_findStrFastExS` | 快速智能查找文字（返回所有坐标） |
| 8.27 | `dm_findStrWithFont` | 按字体查找文字 |
| 8.28 | `dm_findStrWithFontE` | 按字体查找文字（返回坐标字符串） |
| 8.29 | `dm_findStrWithFontEx` | 按字体查找文字（返回所有坐标） |
| 8.30 | `dm_getWords` | 提取文字 |
| 8.31 | `dm_getWordsNoDict` | 无字库提取文字 |
| 8.32 | `dm_fetchWord` | 提取文字样本到字库 |
| 8.33 | `dm_getResultCount` | 获取结果匹配数量 |
| 8.34 | `dm_getResultPos` | 获取结果坐标 |
| 8.35 | `dm_getWordResultCount` | 获取字符数量 |
| 8.36 | `dm_getWordResultPos` | 获取字符坐标 |
| 8.37 | `dm_getWordResultStr` | 获取字符识别结果 |
| 8.38 | `dm_setColGapNoDict` | 设置无字库列间距 |
| 8.39 | `dm_setRowGapNoDict` | 设置无字库行间距 |
| 8.40 | `dm_setWordGapNoDict` | 设置无字库字间距 |
| 8.41 | `dm_setWordLineHeightNoDict` | 设置无字库行高 |
| 8.42 | `dm_setExactOcr` | 设置精确 OCR 模式 |
| 8.43 | `dm_setMinColGap` | 设置最小列间距 |
| 8.44 | `dm_setMinRowGap` | 设置最小行间距 |
| 8.45 | `dm_setWordGap` | 设置字间距 |
| 8.46 | `dm_setWordLineHeight` | 设置行高 |

### 模块 9：系统操作（44 个）
| # | 函数 | 说明 |
|---|------|------|
| 9.1 | `dm_beep` | 蜂鸣 |
| 9.2 | `dm_delay` | 延时 |
| 9.3 | `dm_delays` | 随机延时 |
| 9.4 | `dm_runApp` | 运行程序 |
| 9.5 | `dm_stop` | 停止进程 |
| 9.6 | `dm_play` | 播放音频 |
| 9.7 | `dm_exitOs` | 关机/重启/注销 |
| 9.8 | `dm_setScreen` | 设置分辨率 |
| 9.9 | `dm_getScreenWidth` | 获取屏幕宽度 |
| 9.10 | `dm_getScreenHeight` | 获取屏幕高度 |
| 9.11 | `dm_getScreenDepth` | 获取屏幕色深 |
| 9.12 | `dm_getDPI` | 获取 DPI |
| 9.13 | `dm_getTime` | 获取时间戳 |
| 9.14 | `dm_getOsType` | 获取操作系统类型 |
| 9.15 | `dm_getOsBuildNumber` | 获取系统构建号 |
| 9.16 | `dm_is64Bit` | 判断是否为 64 位 |
| 9.17 | `dm_isSurrpotVt` | 判断是否支持虚拟化 |
| 9.18 | `dm_getSystemInfo` | 获取系统信息 |
| 9.19 | `dm_getDir` | 获取系统目录 |
| 9.20 | `dm_getLocale` | 获取区域设置 |
| 9.21 | `dm_getCpuType` | 获取 CPU 类型 |
| 9.22 | `dm_getCpuUsage` | 获取 CPU 使用率 |
| 9.23 | `dm_getMemoryUsage` | 获取内存使用率 |
| 9.24 | `dm_getDiskSerial` | 获取磁盘序列号 |
| 9.25 | `dm_getDiskModel` | 获取磁盘型号 |
| 9.26 | `dm_getDiskReversion` | 获取磁盘固件版本 |
| 9.27 | `dm_getMachineCode` | 获取机器码 |
| 9.28 | `dm_getMachineCodeNoMac` | 获取无 MAC 机器码 |
| 9.29 | `dm_getNetTime` | 获取网络时间 |
| 9.30 | `dm_getNetTimeByIp` | 按 IP 获取网络时间 |
| 9.31 | `dm_getNetTimeSafe` | 安全获取网络时间 |
| 9.32 | `dm_getClipboard` | 获取剪贴板 |
| 9.33 | `dm_setClipboard` | 设置剪贴板 |
| 9.34 | `dm_getDisplayInfo` | 获取显示器信息 |
| 9.35 | `dm_disableCloseDisplayAndSleep` | 禁用关闭显示器和睡眠 |
| 9.36 | `dm_disablePowerSave` | 禁用节能模式 |
| 9.37 | `dm_disableScreenSave` | 禁用屏保 |
| 9.38 | `dm_disableFontSmooth` | 禁用字体平滑 |
| 9.39 | `dm_enableFontSmooth` | 启用字体平滑 |
| 9.40 | `dm_checkFontSmooth` | 检查字体平滑状态 |
| 9.41 | `dm_checkUAC` | 检查 UAC 状态 |
| 9.42 | `dm_setUAC` | 设置 UAC |
| 9.43 | `dm_setDisplayAcceler` | 设置显示加速 |
| 9.44 | `dm_showTaskBarIcon` | 显示/隐藏任务栏图标 |

### 模块 10：杂项（9 个）
| # | 函数 | 说明 |
|---|------|------|
| 10.1 | `dm_enterCri` | 进入临界区 |
| 10.2 | `dm_leaveCri` | 离开临界区 |
| 10.3 | `dm_initCri` | 初始化临界区 |
| 10.4 | `dm_releaseRef` | 释放引用计数 |
| 10.5 | `dm_setExitThread` | 设置线程退出模式 |
| 10.6 | `dm_executeCmd` | 执行命令行 |
| 10.7 | `dm_activeInputMethod` | 激活输入法 |
| 10.8 | `dm_checkInputMethod` | 检查输入法 |
| 10.9 | `dm_findInputMethod` | 查找输入法 |

### 模块 11：汇编（9 个）
| # | 函数 | 说明 |
|---|------|------|
| 11.1 | `dm_asmAdd` | 添加汇编指令 |
| 11.2 | `dm_asmCall` | 执行汇编代码 |
| 11.3 | `dm_asmCallEx` | 指定地址执行汇编 |
| 11.4 | `dm_asmClear` | 清除汇编代码 |
| 11.5 | `dm_asmSetTimeout` | 设置汇编超时 |
| 11.6 | `dm_assemble` | 编译汇编为机器码 |
| 11.7 | `dm_disAssemble` | 反汇编机器码 |
| 11.8 | `dm_setAsmHwndAsProcessId` | 设置 hwnd 为进程 ID |
| 11.9 | `dm_setShowAsmErrorMsg` | 显示汇编错误消息 |

### 模块 12：AI 功能（17 个）
| # | 函数 | 说明 |
|---|------|------|
| 12.1 | `dm_loadAi` | 加载 AI 模型 |
| 12.2 | `dm_loadAiMemory` | 从内存加载 AI 模型 |
| 12.3 | `dm_aiEnableFindPicWindow` | 启用 AI 找图窗口模式 |
| 12.4 | `dm_aiFindPic` | AI 找图 |
| 12.5 | `dm_aiFindPicEx` | AI 找图（返回所有坐标） |
| 12.6 | `dm_aiFindPicMem` | AI 找图（内存模式） |
| 12.7 | `dm_aiFindPicMemEx` | AI 找图内存版（返回所有坐标） |
| 12.8 | `dm_aiYoloDetectObjects` | YOLO 目标检测 |
| 12.9 | `dm_aiYoloDetectObjectsToDataBmp` | YOLO 检测结果绘制到 BMP |
| 12.10 | `dm_aiYoloDetectObjectsToFile` | YOLO 检测结果保存到文件 |
| 12.11 | `dm_aiYoloSetModel` | 设置 YOLO 模型 |
| 12.12 | `dm_aiYoloSetModelMemory` | 从内存加载 YOLO 模型 |
| 12.13 | `dm_aiYoloFreeModel` | 释放 YOLO 模型 |
| 12.14 | `dm_aiYoloUseModel` | 切换 YOLO 模型 |
| 12.15 | `dm_aiYoloSetVersion` | 设置 YOLO 版本 |
| 12.16 | `dm_aiYoloObjectsToString` | 转换检测结果 |
| 12.17 | `dm_aiYoloSortsObjects` | 排序检测结果 |

### 模块 13：Foobar 界面控件（23 个）
| # | 函数 | 说明 |
|---|------|------|
| 13.1 | `dm_createFoobarRect` | 创建矩形 Foobar |
| 13.2 | `dm_createFoobarEllipse` | 创建椭圆 Foobar |
| 13.3 | `dm_createFoobarRoundRect` | 创建圆角矩形 Foobar |
| 13.4 | `dm_createFoobarCustom` | 创建自定义形状 Foobar |
| 13.5 | `dm_foobarClose` | 关闭 Foobar |
| 13.6 | `dm_foobarLock` | 锁定 Foobar |
| 13.7 | `dm_foobarUnlock` | 解锁 Foobar |
| 13.8 | `dm_foobarUpdate` | 更新 Foobar |
| 13.9 | `dm_foobarSetFont` | 设置 Foobar 字体 |
| 13.10 | `dm_foobarSetSave` | 设置 Foobar 自动保存 |
| 13.11 | `dm_foobarSetTrans` | 设置 Foobar 透明度 |
| 13.12 | `dm_foobarDrawText` | 绘制文本 |
| 13.13 | `dm_foobarPrintText` | 打印文本 |
| 13.14 | `dm_foobarDrawRect` | 绘制矩形 |
| 13.15 | `dm_foobarDrawLine` | 绘制直线 |
| 13.16 | `dm_foobarFillRect` | 填充矩形 |
| 13.17 | `dm_foobarDrawPic` | 绘制图片 |
| 13.18 | `dm_foobarClearText` | 清除文本 |
| 13.19 | `dm_foobarTextRect` | 设置文本区域 |
| 13.20 | `dm_foobarTextLineGap` | 设置行间距 |
| 13.21 | `dm_foobarTextPrintDir` | 设置打印方向 |
| 13.22 | `dm_foobarStartGif` | 播放 GIF |
| 13.23 | `dm_foobarStopGif` | 停止 GIF |

### 模块 14：答题（9 个）
| # | 函数 | 说明 |
|---|------|------|
| 14.1 | `dm_faqCapture` | 截取答题区域 |
| 14.2 | `dm_faqCaptureFromFile` | 从文件加载答题图片 |
| 14.3 | `dm_faqCaptureString` | 截取答题区域为字符串 |
| 14.4 | `dm_faqGetSize` | 获取答题数据大小 |
| 14.5 | `dm_faqPost` | 发送答题图片到服务器 |
| 14.6 | `dm_faqSend` | 发送答题请求 |
| 14.7 | `dm_faqFetch` | 获取答案 |
| 14.8 | `dm_faqCancel` | 取消答题请求 |
| 14.9 | `dm_faqIsPosted` | 检查是否已发送 |

### 模块 15：算法（3 个）
| # | 函数 | 说明 |
|---|------|------|
| 15.1 | `dm_excludePos` | 排除区域坐标 |
| 15.2 | `dm_findNearestPos` | 查找最近坐标 |
| 15.3 | `dm_sortPosDistance` | 按距离排序坐标 |

### 模块 16：防护盾（5 个）
| # | 函数 | 说明 |
|---|------|------|
| 16.1 | `dm_dmGuard` | 启用/禁用防护盾 |
| 16.2 | `dm_dmGuardExtract` | 提取驱动文件 |
| 16.3 | `dm_dmGuardLoadCustom` | 加载自定义防护数据 |
| 16.4 | `dm_dmGuardParams` | 设置防护参数 |
| 16.5 | `dm_unLoadDriver` | 卸载驱动 |

### 总函数数：约 417 个（含同类变体）

---

## 模块 1：基本设置

基本设置模块提供插件的注册、版本查询、路径管理和错误处理等基础功能。

---

### 1.1 dm_reg

**函数签名**：
```c
long DM_CALL dm_reg(const char* reg_code, const char* ver_info);
```

**功能说明**：注册插件（基于机器码）。使用本机机器码与注册码进行绑定验证，注册成功后才能在非演示模式下使用全部功能。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| reg_code | const char* | 是 | 注册码字符串 |
| ver_info | const char* | 否（可传空串） | 版本信息，如 `"1.0.0"` |

**返回值**：

| 返回值 | 含义 |
|--------|------|
| 0 | 注册成功 |
| -1 | 需要注册（DMERR_NEED_REG） |

**异常情况**：无 C++ 异常，通过返回值指示错误。

**调用示例**：

```python
# Python
dm = DmHook()
ret = dm.reg("my_reg_code_12345", "1.0.0")
if ret == 0:
    print("注册成功")
else:
    print(f"注册失败，错误码: {ret}")
```

```c
// C++
dm_reg("my_reg_code_12345", "1.0.0");
```

---

### 1.2 dm_regEx

**函数签名**：
```c
long DM_CALL dm_regEx(const char* reg_code, const char* ver_info, const char* ip);
```

**功能说明**：基于 IP 地址的注册方式。与 `dm_reg` 不同，此函数使用指定 IP 而非本机机器码进行验证。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| reg_code | const char* | 是 | 注册码 |
| ver_info | const char* | 否 | 版本信息 |
| ip | const char* | 是 | IP 地址字符串 |

**返回值**：0=成功，非0=失败（见错误码表）。

**调用示例**：

```python
# Python
ret = dm.reg_ex("reg_code", "1.0.0", "192.168.1.100")
```

---

### 1.3 dm_regNoMac

**函数签名**：
```c
long DM_CALL dm_regNoMac(const char* reg_code, const char* ver_info);
```

**功能说明**：注册插件（不绑定机器码）。与 `dm_reg` 的区别在于不验证机器码，适用于无需绑定的场景。

**参数列表**：同 `dm_reg`

**返回值**：0=成功，非0=失败。

---

### 1.4 dm_regExNoMac

**函数签名**：
```c
long DM_CALL dm_regExNoMac(const char* reg_code, const char* ver_info, const char* ip);
```

**功能说明**：注册插件（不绑定机器码，基于 IP）。结合 `dm_regEx` 和 `dm_regNoMac` 的特点。

**参数列表**：同 `dm_regEx`

**返回值**：0=成功，非0=失败。

---

### 1.5 dm_ver

**函数签名**：
```c
const char* DM_CALL dm_ver();
```

**功能说明**：获取当前插件的版本号字符串。

**参数列表**：无

**返回值**：C 风格字符串指针，指向版本信息，如 `"1.0.0.1"`。

**异常情况**：无。

**调用示例**：

```python
# Python
ver = dm.ver()
print(f"大漠插件版本: {ver}")
```

```c
// C++
const char* ver = dm_ver();
printf("版本: %s\n", ver);
```

```autohotkey
; AHK
ver := DllCall("dm_hook.dll\dm_ver", "AStr")
MsgBox, 版本: %ver%
```

---

### 1.6 dm_getID

**函数签名**：
```c
long DM_CALL dm_getID();
```

**功能说明**：获取当前 dm 对象的唯一标识 ID。每个 dm 对象实例在创建时分配一个递增的 ID，可用于区分不同的对象。

**参数列表**：无

**返回值**：当前对象的 ID（正整数）。

**异常情况**：无。

**调用示例**：

```python
# Python
obj_id = dm.get_id()
print(f"当前对象 ID: {obj_id}")
```

---

### 1.7 dm_getDmCount

**函数签名**：
```c
long DM_CALL dm_getDmCount();
```

**功能说明**：获取当前进程中所有 dm 对象的总数量。用于调试和资源管理。

**参数列表**：无

**返回值**：当前进程中的 dm 对象数量（整数）。

**异常情况**：无。

---

### 1.8 dm_getLastError

**函数签名**：
```c
long DM_CALL dm_getLastError();
```

**功能说明**：获取最近一次 API 调用产生的错误码。每次调用 API 后，内部错误码会被更新，建议在 API 返回非 0 值时调用此函数获取详细错误信息。

**参数列表**：无

**返回值**：错误码整数值，0 表示无错误，负值表示具体错误类型（见错误码表）。

**异常情况**：无。

**调用示例**：

```python
# Python
ret = dm.find_window("", "非存在窗口")
if ret == 0:
    err = dm.get_last_error()
    print(f"查找失败，错误码: {err}")
```

---

### 1.9 dm_getPath

**函数签名**：
```c
const char* DM_CALL dm_getPath();
```

**功能说明**：获取当前设置的全局路径。此路径是插件查找图片、字库等文件的默认目录。

**参数列表**：无

**返回值**：当前全局路径字符串。

**异常情况**：无。

**调用示例**：

```python
# Python
path = dm.get_path()
print(f"当前全局路径: {path}")
```

---

### 1.10 dm_setPath

**函数签名**：
```c
long DM_CALL dm_setPath(const char* path);
```

**功能说明**：设置全局路径。设置后，所有涉及文件操作的 API 将以此路径为基准查找文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| path | const char* | 是 | 要设置的路径字符串，支持绝对路径和相对路径 |

**返回值**：0=成功，非0=失败。

**异常情况**：路径不存在时返回错误码。

**调用示例**：

```python
# Python
dm.set_path("C:\\dm_project\\resources")
# 或使用相对路径
dm.set_path(".\\resources")
```

---

### 1.11 dm_getBasePath

**函数签名**：
```c
const char* DM_CALL dm_getBasePath();
```

**功能说明**：获取 dm_hook.dll 文件所在的目录路径。通常用于定位与 DLL 同目录的资源文件。

**参数列表**：无

**返回值**：DLL 所在目录的路径字符串。

**异常情况**：无。

---

### 1.12 dm_setShowErrorMsg

**函数签名**：
```c
long DM_CALL dm_setShowErrorMsg(long show);
```

**功能说明**：设置是否在发生错误时弹出消息框提示。适用于调试阶段开启，发布阶段关闭。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| show | long | 是 | 0=隐藏错误消息框，1=显示错误消息框 |

**返回值**：0=成功。

**异常情况**：无。

**调用示例**：

```python
# Python
dm.set_show_error_msg(0)  # 发布时关闭错误弹窗
```

---

### 1.13 dm_enablePicCache

**函数签名**：
```c
long DM_CALL dm_enablePicCache(long enable);
```

**功能说明**：启用或禁用图片缓存。启用后，`loadPic` 加载的图片会缓存在内存中，后续找图操作更快，但会占用更多内存。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用缓存，1=启用缓存 |

**返回值**：0=成功。

**调用示例**：

```python
# Python
dm.enable_pic_cache(1)  # 启用图片缓存提升性能
```

---

## 模块 2：窗口操作

窗口操作模块提供窗口的查找、枚举、信息获取、状态控制等功能。

---

### 2.1 dm_findWindow

**函数签名**：
```c
long DM_CALL dm_findWindow(const char* cls, const char* title);
```

**功能说明**：根据类名和/或标题查找顶层窗口。两个参数均支持空字符串（表示忽略该条件）和模糊匹配。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| cls | const char* | 否（可传空串） | 窗口类名，如 `"Notepad"`、`"#32770"`（对话框） |
| title | const char* | 否（可传空串） | 窗口标题，如 `"计算器"` |

**返回值**：找到的窗口句柄（HWND），0 表示未找到。

**异常情况**：无。

**调用示例**：

```python
# Python
# 按类名查找
hwnd = dm.find_window("Notepad", "")
if hwnd:
    print(f"找到记事本窗口: {hwnd}")

# 按标题查找
hwnd = dm.find_window("", "计算器")

# 组合查找（类名+标题）
hwnd = dm.find_window("Chrome_WidgetWin_1", "GitHub")
```

---

### 2.2 dm_findWindowEx

**函数签名**：
```c
long DM_CALL dm_findWindowEx(intptr_t parent, long cls, const char* title);
```

**功能说明**：在指定父窗口中查找子窗口。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| parent | intptr_t | 是 | 父窗口句柄，0 表示从桌面窗口开始查找 |
| cls | long | 否（传 ""） | 子窗口类名 |
| title | const char* | 否 | 子窗口标题 |

**返回值**：子窗口句柄，0=未找到。

**调用示例**：

```python
# Python
# 在记事本中查找编辑区域
hwnd = dm.find_window("Notepad", "")
if hwnd:
    edit = dm.find_window_ex(hwnd, "Edit", "")
    print(f"编辑框句柄: {edit}")
```

---

### 2.3 dm_findWindowByProcess

**函数签名**：
```c
long DM_CALL dm_findWindowByProcess(const char* process_name, const char* cls, const char* title);
```

**功能说明**：根据进程名查找窗口。先按进程名匹配，再按类名和标题筛选。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| process_name | const char* | 是 | 进程名称，如 `"notepad.exe"`、`"chrome.exe"` |
| cls | const char* | 否 | 窗口类名 |
| title | const char* | 否 | 窗口标题 |

**返回值**：窗口句柄，0=未找到。

**调用示例**：

```python
# Python
hwnd = dm.find_window_by_process("notepad.exe", "", "")
print(f"记事本窗口: {hwnd}")
```

---

### 2.4 dm_findWindowByProcessId

**函数签名**：
```c
long DM_CALL dm_findWindowByProcessId(long process_id, const char* cls, const char* title);
```

**功能说明**：根据进程 ID 查找窗口。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| process_id | long | 是 | 进程 ID |
| cls | const char* | 否 | 窗口类名 |
| title | const char* | 否 | 窗口标题 |

**返回值**：窗口句柄，0=未找到。

---

### 2.5 dm_findWindowSuper

**函数签名**：
```c
long DM_CALL dm_findWindowSuper(const char* cls, const char* title, long flag);
```

**功能说明**：高级窗口查找，支持多种匹配模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| cls | const char* | 否 | 窗口类名 |
| title | const char* | 否 | 窗口标题 |
| flag | long | 是 | 匹配模式（见下表） |

**flag 取值**：

| 值 | 说明 |
|----|------|
| 0 | 标题完全匹配 |
| 1 | 标题模糊匹配（包含） |
| 2 | 类名匹配 |
| 4 | 标题正则匹配 |
| 8 | 类名正则匹配 |

**返回值**：窗口句柄，0=未找到。

**调用示例**：

```python
# Python
# 正则匹配标题
hwnd = dm.find_window_super("", ".*记事本.*", 4)
```

---

### 2.6 dm_enumWindow

**函数签名**：
```c
long DM_CALL dm_enumWindow(intptr_t parent, const char* title, long filter);
```

**功能说明**：枚举指定父窗口下的所有子窗口，返回句柄字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| parent | intptr_t | 是 | 父窗口句柄，0 表示枚举桌面所有顶层窗口 |
| title | const char* | 否 | 标题过滤 |
| filter | long | 是 | 过滤方式，1=匹配标题，2=排除标题，其他值=不过滤 |

**返回值**：窗口句柄列表，以 `"|"` 分隔的字符串。如 `"123456|789012|345678"`。

**调用示例**：

```python
# Python
hwnds = dm.enum_window(0, "", 0)
print(f"所有窗口句柄: {hwnds}")
```

---

### 2.7 dm_enumWindowByProcess

**函数签名**：
```c
long DM_CALL dm_enumWindowByProcess(const char* process_name, const char* title, long filter);
```

**功能说明**：枚举指定进程的所有窗口。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| process_name | const char* | 是 | 进程名称 |
| title | const char* | 否 | 标题过滤 |
| filter | long | 是 | 过滤方式（同 dm_enumWindow） |

**返回值**：窗口句柄列表，以 `"|"` 分隔。

---

### 2.8 dm_enumWindowByProcessId

**函数签名**：
```c
long DM_CALL dm_enumWindowByProcessId(long pid, const char* title, long filter);
```

**功能说明**：根据进程 ID 枚举窗口。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pid | long | 是 | 进程 ID |
| title | const char* | 否 | 标题过滤 |
| filter | long | 是 | 过滤方式 |

**返回值**：窗口句柄列表，以 `"|"` 分隔。

---

### 2.9 dm_enumWindowSuper

**函数签名**：
```c
long DM_CALL dm_enumWindowSuper(const char* spec1, long flag1, const char* spec2, long flag2, long filter);
```

**功能说明**：高级窗口枚举，支持两个条件的任意组合匹配。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| spec1 | const char* | 是 | 第一个匹配条件（类名或标题） |
| flag1 | long | 是 | 第一个条件的匹配模式 |
| spec2 | const char* | 是 | 第二个匹配条件 |
| flag2 | long | 是 | 第二个条件的匹配模式 |
| filter | long | 是 | 返回格式过滤 |

**flag 取值**：同 `dm_findWindowSuper` 的 flag。

**返回值**：窗口句柄列表，以 `"|"` 分隔。

---

### 2.10 dm_enumProcess

**函数签名**：
```c
long DM_CALL dm_enumProcess(const char* name);
```

**功能说明**：枚举系统中所有匹配指定名称的进程，返回进程 ID 列表。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| name | const char* | 是 | 进程名称，支持通配符 `*`，如 `"*.exe"` 表示所有进程 |

**返回值**：进程 ID 列表，以 `","` 分隔的字符串。

**调用示例**：

```python
# Python
pids = dm.enum_process("notepad.exe")
print(f"记事本进程 PID: {pids}")
```

---

### 2.11 dm_getWindow

**函数签名**：
```c
long DM_CALL dm_getWindow(intptr_t hwnd, long flag);
```

**功能说明**：获取与指定窗口有特定关系的其他窗口句柄。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 源窗口句柄 |
| flag | long | 是 | 关系类型（见下表） |

**flag 取值**：

| 值 | 说明 |
|----|------|
| 0 | 获取父窗口 |
| 1 | 获取第一个子窗口 |
| 2 | 获取上一个兄弟窗口 |
| 3 | 获取下一个兄弟窗口 |
| 4 | 获取窗口所属的顶层窗口（所有者） |

**返回值**：目标窗口句柄，0=未找到。

---

### 2.12 dm_getForegroundWindow

**函数签名**：
```c
long DM_CALL dm_getForegroundWindow();
```

**功能说明**：获取当前系统中前台（活动）窗口的句柄。

**参数列表**：无

**返回值**：前台窗口句柄，0=获取失败。

**调用示例**：

```python
# Python
hwnd = dm.get_foreground_window()
title = dm.get_window_title(hwnd)
print(f"当前前台窗口: {title}")
```

---

### 2.13 dm_getForegroundFocus

**函数签名**：
```c
long DM_CALL dm_getForegroundFocus();
```

**功能说明**：获取当前拥有键盘焦点的窗口句柄。与 `getForegroundWindow` 不同，焦点窗口可能不是前台窗口（如前台窗口中的子控件）。

**参数列表**：无

**返回值**：焦点窗口句柄，0=获取失败。

---

### 2.14 dm_getMousePointWindow

**函数签名**：
```c
long DM_CALL dm_getMousePointWindow();
```

**功能说明**：获取当前鼠标位置下方的窗口句柄。

**参数列表**：无

**返回值**：鼠标所在位置的窗口句柄，0=获取失败。

**调用示例**：

```python
# Python
hwnd = dm.get_mouse_point_window()
print(f"鼠标下方窗口: {dm.get_window_title(hwnd)}")
```

---

### 2.15 dm_getPointWindow

**函数签名**：
```c
long DM_CALL dm_getPointWindow(long x, long y);
```

**功能说明**：获取指定屏幕坐标点下方的窗口句柄。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long | 是 | 屏幕 X 坐标 |
| y | long | 是 | 屏幕 Y 坐标 |

**返回值**：该坐标点下的窗口句柄，0=未找到。

---

### 2.16 dm_getSpecialWindow

**函数签名**：
```c
long DM_CALL dm_getSpecialWindow(long flag);
```

**功能说明**：获取特殊窗口句柄。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| flag | long | 是 | 特殊窗口类型（见下表） |

**flag 取值**：

| 值 | 说明 |
|----|------|
| 0 | 桌面窗口 |
| 1 | 任务栏窗口 |
| 2 | 开始菜单按钮 |

**返回值**：特殊窗口句柄，0=获取失败。

---

### 2.17 dm_getWindowClass

**函数签名**：
```c
const char* DM_CALL dm_getWindowClass(intptr_t hwnd);
```

**功能说明**：获取指定窗口的类名。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：窗口类名字符串。

**异常情况**：hwnd 无效时返回空字符串。

**调用示例**：

```python
# Python
hwnd = dm.get_foreground_window()
cls = dm.get_window_class(hwnd)
print(f"窗口类名: {cls}")
```

---

### 2.18 dm_getWindowProcessId

**函数签名**：
```c
long DM_CALL dm_getWindowProcessId(intptr_t hwnd);
```

**功能说明**：获取指定窗口所属进程的 PID。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：进程 ID，0=获取失败。

**调用示例**：

```python
# Python
hwnd = dm.get_foreground_window()
pid = dm.get_window_process_id(hwnd)
print(f"进程 PID: {pid}")
```

---

### 2.19 dm_getWindowProcessPath

**函数签名**：
```c
const char* DM_CALL dm_getWindowProcessPath(intptr_t hwnd);
```

**功能说明**：获取指定窗口所属进程的完整可执行文件路径。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：进程完整路径字符串。

**异常情况**：hwnd 无效时返回空字符串。

**调用示例**：

```python
# Python
hwnd = dm.get_foreground_window()
path = dm.get_window_process_path(hwnd)
print(f"进程路径: {path}")
```

---

### 2.20 dm_getWindowThreadId

**函数签名**：
```c
long DM_CALL dm_getWindowThreadId(intptr_t hwnd);
```

**功能说明**：获取指定窗口所属线程的 ID。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：线程 ID，0=获取失败。

---

### 2.21 dm_getWindowTitle

**函数签名**：
```c
const char* DM_CALL dm_getWindowTitle(intptr_t hwnd);
```

**功能说明**：获取指定窗口的标题文本。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：窗口标题字符串。

**异常情况**：hwnd 无效时返回空字符串。

**调用示例**：

```python
# Python
hwnd = dm.get_foreground_window()
title = dm.get_window_title(hwnd)
print(f"窗口标题: {title}")
```

---

### 2.22 dm_getWindowRect

**函数签名**：
```c
long DM_CALL dm_getWindowRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
```

**功能说明**：获取窗口在屏幕上的矩形区域（包括标题栏、边框等）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| x1 | long* | 输出 | 左上角 X 坐标 |
| y1 | long* | 输出 | 左上角 Y 坐标 |
| x2 | long* | 输出 | 右下角 X 坐标 |
| y2 | long* | 输出 | 右下角 Y 坐标 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
import ctypes
x1, y1, x2, y2 = ctypes.c_long(), ctypes.c_long(), ctypes.c_long(), ctypes.c_long()
ret = dm.get_window_rect(hwnd, x1, y1, x2, y2)
if ret == 0:
    w = x2.value - x1.value
    h = y2.value - y1.value
    print(f"窗口位置: ({x1.value},{y1.value}), 大小: {w}x{h}")
```

---

### 2.23 dm_getClientRect

**函数签名**：
```c
long DM_CALL dm_getClientRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2);
```

**功能说明**：获取窗口客户区（不包括标题栏、边框等）的矩形区域。

**参数列表**：同 `dm_getWindowRect`

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
x1, y1, x2, y2 = ctypes.c_long(), ctypes.c_long(), ctypes.c_long(), ctypes.c_long()
dm.get_client_rect(hwnd, x1, y1, x2, y2)
```

---

### 2.24 dm_getClientSize

**函数签名**：
```c
long DM_CALL dm_getClientSize(intptr_t hwnd, long* width, long* height);
```

**功能说明**：获取窗口客户区的宽度和高度。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| width | long* | 输出 | 客户区宽度（像素） |
| height | long* | 输出 | 客户区高度（像素） |

**返回值**：0=成功，非0=失败。

---

### 2.25 dm_getWindowState

**函数签名**：
```c
long DM_CALL dm_getWindowState(intptr_t hwnd, long flag);
```

**功能说明**：获取窗口的当前状态。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| flag | long | 是 | 查询类型（见下表） |

**flag 取值**：

| 值 | 说明 |
|----|------|
| 0 | 是否可见 |
| 1 | 是否最小化 |
| 2 | 是否最大化 |
| 3 | 是否激活 |

**返回值**：0=否，1=是。

---

### 2.26 dm_getProcessInfo

**函数签名**：
```c
long DM_CALL dm_getProcessInfo(long pid, const char* type);
```

**功能说明**：获取指定进程的详细信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pid | long | 是 | 进程 ID |
| type | const char* | 是 | 信息类型，如 `"time"`（创建时间） |

**返回值**：信息值，具体含义取决于 type 参数。

---

### 2.27 dm_moveWindow

**函数签名**：
```c
long DM_CALL dm_moveWindow(intptr_t hwnd, long x, long y);
```

**功能说明**：移动窗口到指定屏幕坐标位置。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| x | long | 是 | 目标 X 坐标 |
| y | long | 是 | 目标 Y 坐标 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.move_window(hwnd, 100, 100)  # 将窗口移动到 (100, 100)
```

---

### 2.28 dm_setWindowSize

**函数签名**：
```c
long DM_CALL dm_setWindowSize(intptr_t hwnd, long width, long height);
```

**功能说明**：调整窗口的大小（包括标题栏和边框的完整窗口尺寸）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| width | long | 是 | 新的窗口宽度（像素） |
| height | long | 是 | 新的窗口高度（像素） |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.set_window_size(hwnd, 1024, 768)  # 调整为 1024x768
```

---

### 2.29 dm_setWindowState

**函数签名**：
```c
long DM_CALL dm_setWindowState(intptr_t hwnd, long flag);
```

**功能说明**：设置窗口的状态（显示、隐藏、最小化、最大化、还原等）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| flag | long | 是 | 状态值（见下表） |

**flag 取值**：

| 值 | 说明 |
|----|------|
| 0 | 关闭窗口 |
| 1 | 激活窗口（置前并聚焦） |
| 2 | 最小化 |
| 3 | 最大化 |
| 4 | 还原（从最小化/最大化恢复） |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.set_window_state(hwnd, 1)  # 激活窗口
dm.set_window_state(hwnd, 2)  # 最小化
dm.set_window_state(hwnd, 3)  # 最大化
dm.set_window_state(hwnd, 4)  # 还原
```

---

### 2.30 dm_setWindowText

**函数签名**：
```c
long DM_CALL dm_setWindowText(intptr_t hwnd, const char* title);
```

**功能说明**：修改窗口的标题文本。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| title | const char* | 是 | 新的标题文本 |

**返回值**：0=成功，非0=失败。

---

### 2.31 dm_setWindowTransparent

**函数签名**：
```c
long DM_CALL dm_setWindowTransparent(intptr_t hwnd, long trans);
```

**功能说明**：设置窗口的透明度。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| trans | long | 是 | 透明度值，0=完全透明，255=完全不透明 |

**返回值**：0=成功，非0=失败。

---

### 2.32 dm_setClientSize

**函数签名**：
```c
long DM_CALL dm_setClientSize(intptr_t hwnd, long width, long height);
```

**功能说明**：调整窗口客户区的大小（不包括标题栏和边框）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| width | long | 是 | 目标客户区宽度 |
| height | long | 是 | 目标客户区高度 |

**返回值**：0=成功，非0=失败。

---

### 2.33 dm_setSendStringDelay

**函数签名**：
```c
long DM_CALL dm_setSendStringDelay(long delay);
```

**功能说明**：设置发送字符串时的字符间延迟时间。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| delay | long | 是 | 延迟时间（毫秒） |

**返回值**：0=成功。

---

### 2.34 dm_sendPaste

**函数签名**：
```c
long DM_CALL dm_sendPaste(intptr_t hwnd);
```

**功能说明**：向指定窗口发送粘贴命令（Ctrl+V）。需要先将文本复制到剪贴板。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 目标窗口句柄 |

**返回值**：0=成功，非0=失败。

---

### 2.35 dm_sendString

**函数签名**：
```c
long DM_CALL dm_sendString(intptr_t hwnd, const char* str);
```

**功能说明**：向指定窗口发送文本字符串（后台模式）。适用于标准 Windows 文本框控件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 目标窗口句柄 |
| str | const char* | 是 | 要发送的文本内容 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.send_string(hwnd, "Hello, World!")
```

---

### 2.36 dm_sendString2

**函数签名**：
```c
long DM_CALL dm_sendString2(intptr_t hwnd, const char* str);
```

**功能说明**：发送字符串的另一种实现方式，适用于某些 `sendString` 无法正常输入的场景。

**参数列表**：同 `dm_sendString`

**返回值**：0=成功，非0=失败。

---

### 2.37 dm_sendStringIme

**函数签名**：
```c
long DM_CALL dm_sendStringIme(intptr_t hwnd, const char* str);
```

**功能说明**：通过 IME（输入法）发送字符串，适用于需要输入中文等非 ASCII 字符的场景。

**参数列表**：同 `dm_sendString`

**返回值**：0=成功，非0=失败。

---

### 2.38 dm_sendStringIme2

**函数签名**：
```c
long DM_CALL dm_sendStringIme2(intptr_t hwnd, const char* str);
```

**功能说明**：IME 发送字符串的备选方式。

**参数列表**：同 `dm_sendString`

**返回值**：0=成功，非0=失败。

---

### 2.39 dm_clientToScreen

**函数签名**：
```c
long DM_CALL dm_clientToScreen(intptr_t hwnd, long* x, long* y);
```

**功能说明**：将窗口客户区坐标转换为屏幕坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| x | long* | 输入/输出 | 输入时为客户区 X，输出时为屏幕 X |
| y | long* | 输入/输出 | 输入时为客户区 Y，输出时为屏幕 Y |

**返回值**：0=成功，非0=失败。

---

### 2.40 dm_screenToClient

**函数签名**：
```c
long DM_CALL dm_screenToClient(intptr_t hwnd, long* x, long* y);
```

**功能说明**：将屏幕坐标转换为窗口客户区坐标。

**参数列表**：同 `dm_clientToScreen`（方向相反）

**返回值**：0=成功，非0=失败。

---

## 模块 3：键鼠操作

键鼠操作模块提供键盘和鼠标的模拟输入功能，支持前台和后台模式。

---

### 3.1 dm_keyDown

**函数签名**：
```c
long DM_CALL dm_keyDown(long vk_code);
```

**功能说明**：模拟按下指定虚拟键码的按键（不释放）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| vk_code | long | 是 | 虚拟键码（Virtual Key Code），如 0x0D=回车 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.key_down(0x11)  # 按下 Ctrl
dm.key_down(0x43)  # 按下 C
dm.key_up(0x43)    # 松开 C
dm.key_up(0x11)    # 松开 Ctrl
```

---

### 3.2 dm_keyDownChar

**函数签名**：
```c
long DM_CALL dm_keyDownChar(const char* key_str);
```

**功能说明**：模拟按下指定字符键（通过字符表示，而非虚拟键码）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| key_str | const char* | 是 | 字符，如 `"a"`、`"1"`、`"+"` |

**返回值**：0=成功，非0=失败。

---

### 3.3 dm_keyPress

**函数签名**：
```c
long DM_CALL dm_keyPress(long vk_code);
```

**功能说明**：模拟按下并释放指定虚拟键码的按键（一次完整的按键操作）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| vk_code | long | 是 | 虚拟键码 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.key_press(0x0D)     # 按回车
dm.key_press(0x1B)     # 按 ESC
dm.key_press(0x70)     # 按 F1
dm.key_press(ord('A')) # 按 A 键
```

---

### 3.4 dm_keyPressChar

**函数签名**：
```c
long DM_CALL dm_keyPressChar(const char* key_str);
```

**功能说明**：模拟按下并释放指定字符键。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| key_str | const char* | 是 | 字符，如 `"A"`、`"b"`、`"@"` |

**返回值**：0=成功，非0=失败。

---

### 3.5 dm_keyPressStr

**函数签名**：
```c
long DM_CALL dm_keyPressStr(const char* key_str, long delay);
```

**功能说明**：模拟依次按下并释放一串按键序列。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| key_str | const char* | 是 | 要输入的文本字符串 |
| delay | long | 是 | 每个字符之间的延迟（毫秒） |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.key_press_str("Hello World!", 50)  # 每个字符间隔 50ms
```

---

### 3.6 dm_keyUp

**函数签名**：
```c
long DM_CALL dm_keyUp(long vk_code);
```

**功能说明**：释放指定虚拟键码的按键（与 `keyDown` 配对使用）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| vk_code | long | 是 | 虚拟键码 |

**返回值**：0=成功，非0=失败。

---

### 3.7 dm_keyUpChar

**函数签名**：
```c
long DM_CALL dm_keyUpChar(const char* key_str);
```

**功能说明**：释放指定字符键。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| key_str | const char* | 是 | 字符 |

**返回值**：0=成功，非0=失败。

---

### 3.8 dm_waitKey

**函数签名**：
```c
long DM_CALL dm_waitKey(long vk_code, long time_out);
```

**功能说明**：等待指定按键被按下，支持超时。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| vk_code | long | 是 | 要等待的虚拟键码，0 表示等待任意键 |
| time_out | long | 是 | 超时时间（毫秒），0 表示无限等待 |

**返回值**：按下的虚拟键码，超时返回 0。

---

### 3.9 dm_getKeyState

**函数签名**：
```c
long DM_CALL dm_getKeyState(long vk_code);
```

**功能说明**：查询指定按键的当前状态（按下或释放）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| vk_code | long | 是 | 虚拟键码 |

**返回值**：0=未按下，1=已按下。

---

### 3.10 dm_leftClick

**函数签名**：
```c
long DM_CALL dm_leftClick();
```

**功能说明**：模拟鼠标左键单击（按下+释放）。

**参数列表**：无

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.move_to(500, 300)  # 先移动鼠标
dm.left_click()        # 左键单击
```

---

### 3.11 dm_leftDoubleClick

**函数签名**：
```c
long DM_CALL dm_leftDoubleClick();
```

**功能说明**：模拟鼠标左键双击。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.12 dm_leftDown

**函数签名**：
```c
long DM_CALL dm_leftDown();
```

**功能说明**：模拟鼠标左键按下（不释放，需与 `leftUp` 配对使用）。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.13 dm_leftUp

**函数签名**：
```c
long DM_CALL dm_leftUp();
```

**功能说明**：模拟鼠标左键释放。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.14 dm_rightClick

**函数签名**：
```c
long DM_CALL dm_rightClick();
```

**功能说明**：模拟鼠标右键单击。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.15 dm_rightDown

**函数签名**：
```c
long DM_CALL dm_rightDown();
```

**功能说明**：模拟鼠标右键按下。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.16 dm_rightUp

**函数签名**：
```c
long DM_CALL dm_rightUp();
```

**功能说明**：模拟鼠标右键释放。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.17 dm_middleClick

**函数签名**：
```c
long DM_CALL dm_middleClick();
```

**功能说明**：模拟鼠标中键单击。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.18 dm_middleDown

**函数签名**：
```c
long DM_CALL dm_middleDown();
```

**功能说明**：模拟鼠标中键按下。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.19 dm_middleUp

**函数签名**：
```c
long DM_CALL dm_middleUp();
```

**功能说明**：模拟鼠标中键释放。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 3.20 dm_moveTo

**函数签名**：
```c
long DM_CALL dm_moveTo(long x, long y);
```

**功能说明**：将鼠标移动到指定屏幕坐标位置。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long | 是 | 目标 X 坐标（屏幕像素） |
| y | long | 是 | 目标 Y 坐标（屏幕像素） |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.move_to(800, 600)   # 移动到屏幕中心
dm.left_click()         # 单击

# 组合操作：移动到目标位置并点击
dm.move_to(100, 200)
dm.left_click()
```

---

### 3.21 dm_moveToEx

**函数签名**：
```c
long DM_CALL dm_moveToEx(long x, long y, long w, long h);
```

**功能说明**：将鼠标移动到指定矩形区域内的随机位置。用于模拟更自然的鼠标行为，避免每次都移动到同一个精确坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long | 是 | 区域左上角 X 坐标 |
| y | long | 是 | 区域左上角 Y 坐标 |
| w | long | 是 | 区域宽度 |
| h | long | 是 | 区域高度 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
# 在 (100,100) 到 (200,150) 的矩形区域内随机点击
dm.move_to_ex(100, 100, 100, 50)
dm.left_click()
```

---

### 3.22 dm_moveR

**函数签名**：
```c
long DM_CALL dm_moveR(long rx, long ry);
```

**功能说明**：将鼠标从当前位置相对移动指定的偏移量。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| rx | long | 是 | X 方向偏移量（正数向右，负数向左） |
| ry | long | 是 | Y 方向偏移量（正数向下，负数向上） |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.move_to(500, 500)
dm.move_r(50, -30)  # 向右 50 像素，向上 30 像素
```

---

### 3.23 dm_wheelDown

**函数签名**：
```c
long DM_CALL dm_wheelDown(long repeat);
```

**功能说明**：模拟鼠标滚轮向下滚动。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| repeat | long | 是 | 滚动次数 |

**返回值**：0=成功，非0=失败。

---

### 3.24 dm_wheelUp

**函数签名**：
```c
long DM_CALL dm_wheelUp(long repeat);
```

**功能说明**：模拟鼠标滚轮向上滚动。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| repeat | long | 是 | 滚动次数 |

**返回值**：0=成功，非0=失败。

---

### 3.25 dm_getCursorPos

**函数签名**：
```c
long DM_CALL dm_getCursorPos(long* x, long* y);
```

**功能说明**：获取当前鼠标指针的屏幕坐标位置。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long* | 输出 | 当前鼠标 X 坐标 |
| y | long* | 输出 | 当前鼠标 Y 坐标 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
x, y = ctypes.c_long(), ctypes.c_long()
dm.get_cursor_pos(x, y)
print(f"鼠标位置: ({x.value}, {y.value})")
```

---

### 3.26 dm_getCursorShape

**函数签名**：
```c
long DM_CALL dm_getCursorShape();
```

**功能说明**：获取当前鼠标光标的形状 ID。

**参数列表**：无

**返回值**：光标形状 ID，可用于判断当前鼠标状态（如箭头、手型、沙漏等）。

---

### 3.27 dm_getCursorShapeEx

**函数签名**：
```c
const char* DM_CALL dm_getCursorShapeEx(long type);
```

**功能说明**：获取当前鼠标光标形状的详细描述信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | long | 是 | 0=返回光标形状索引，1=返回光标详细数据 |

**返回值**：光标形状信息字符串。

---

### 3.28 dm_getCursorSpot

**函数签名**：
```c
long DM_CALL dm_getCursorSpot();
```

**功能说明**：获取当前鼠标光标的热点（点击有效点）位置。

**参数列表**：无

**返回值**：热点位置编码，低16位为 X 偏移，高16位为 Y 偏移。

---

### 3.29 dm_setMouseDelay

**函数签名**：
```c
long DM_CALL dm_setMouseDelay(long type, long delay);
```

**功能说明**：设置鼠标操作之间的延迟时间。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | long | 是 | 延迟类型：0=普通延迟，1=Windows 延迟 |
| delay | long | 是 | 延迟时间（毫秒） |

**返回值**：0=成功。

---

### 3.30 dm_setKeypadDelay

**函数签名**：
```c
long DM_CALL dm_setKeypadDelay(long type, long delay);
```

**功能说明**：设置键盘操作之间的延迟时间。

**参数列表**：同 `dm_setMouseDelay`

**返回值**：0=成功。

---

### 3.31 dm_setMouseSpeed

**函数签名**：
```c
long DM_CALL dm_setMouseSpeed(long speed);
```

**功能说明**：设置鼠标移动速度。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| speed | long | 是 | 速度值（1-20），值越大移动越快 |

**返回值**：0=成功。

---

### 3.32 dm_getMouseSpeed

**函数签名**：
```c
long DM_CALL dm_getMouseSpeed();
```

**功能说明**：获取当前鼠标移动速度设置。

**参数列表**：无

**返回值**：当前速度值（1-20）。

---

### 3.33 dm_enableMouseAccuracy

**函数签名**：
```c
long DM_CALL dm_enableMouseAccuracy(long enable);
```

**功能说明**：启用或禁用鼠标加速（提高指针精确度）。关闭后可获得更精确的鼠标移动控制。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用鼠标加速，1=启用鼠标加速 |

**返回值**：0=成功。

---

### 3.34 dm_setSimMode

**函数签名**：
```c
long DM_CALL dm_setSimMode(long mode);
```

**功能说明**：设置键鼠模拟的模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| mode | long | 是 | 0=普通模拟（前台），1=硬件模拟（驱动级，需管理员权限） |

**返回值**：0=成功。

---

## 模块 4：图色操作

图色操作模块提供屏幕截图、颜色获取、颜色比较、找色、找图、图片处理等功能。

---

### 4.1 dm_capture

**函数签名**：
```c
long DM_CALL dm_capture(long x1, long y1, long x2, long y2, const char* file);
```

**功能说明**：截取屏幕指定区域并保存为 BMP 格式图片文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 截图区域左上角 X 坐标 |
| y1 | long | 是 | 截图区域左上角 Y 坐标 |
| x2 | long | 是 | 截图区域右下角 X 坐标 |
| y2 | long | 是 | 截图区域右下角 Y 坐标 |
| file | const char* | 是 | 保存路径，需包含 `.bmp` 扩展名 |

**返回值**：0=成功，非0=失败。

**异常情况**：路径不可写时返回错误码。

**调用示例**：

```python
# Python
# 截取全屏
dm.capture(0, 0, 1920, 1080, "fullscreen.bmp")

# 截取指定区域
dm.capture(100, 100, 500, 400, "region.bmp")
```

---

### 4.2 dm_captureGif

**函数签名**：
```c
long DM_CALL dm_captureGif(long x1, long y1, long x2, long y2, const char* file, long delay, long time);
```

**功能说明**：截取屏幕指定区域并保存为动态 GIF 图片。用于录制屏幕区域的变化。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 区域左上角 X |
| y1 | long | 是 | 区域左上角 Y |
| x2 | long | 是 | 区域右下角 X |
| y2 | long | 是 | 区域右下角 Y |
| file | const char* | 是 | 保存路径，需包含 `.gif` 扩展名 |
| delay | long | 是 | 帧间隔（毫秒） |
| time | long | 是 | 总录制时长（毫秒），0 表示只截取一帧 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
# 录制 5 秒 GIF，每秒 10 帧
dm.capture_gif(0, 0, 800, 600, "animation.gif", 100, 5000)
```

---

### 4.3 dm_captureJpg

**函数签名**：
```c
long DM_CALL dm_captureJpg(long x1, long y1, long x2, long y2, const char* file, long quality);
```

**功能说明**：截取屏幕指定区域并保存为 JPEG 格式图片，支持压缩质量设置。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 区域左上角 X |
| y1 | long | 是 | 区域左上角 Y |
| x2 | long | 是 | 区域右下角 X |
| y2 | long | 是 | 区域右下角 Y |
| file | const char* | 是 | 保存路径，需包含 `.jpg` 或 `.jpeg` 扩展名 |
| quality | long | 是 | 压缩质量（0-100），值越高画质越好、文件越大 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.capture_jpg(0, 0, 1920, 1080, "screenshot.jpg", 85)  # 85% 质量
```

---

### 4.4 dm_capturePng

**函数签名**：
```c
long DM_CALL dm_capturePng(long x1, long y1, long x2, long y2, const char* file);
```

**功能说明**：截取屏幕指定区域并保存为 PNG 格式图片（无损压缩）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 区域左上角 X |
| y1 | long | 是 | 区域左上角 Y |
| x2 | long | 是 | 区域右下角 X |
| y2 | long | 是 | 区域右下角 Y |
| file | const char* | 是 | 保存路径，需包含 `.png` 扩展名 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.capture_png(100, 100, 500, 400, "region.png")
```

---

### 4.5 dm_capturePre

**函数签名**：
```c
long DM_CALL dm_capturePre(const char* file);
```

**功能说明**：将上一次截图操作的结果保存到指定文件。用于连续截图场景中避免重复截取。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 保存路径 |

**返回值**：0=成功，非0=失败。

---

### 4.6 dm_getColor

**函数签名**：
```c
const char* DM_CALL dm_getColor(long x, long y);
```

**功能说明**：获取屏幕指定点的颜色值，返回 RGB 格式（RRGGBB）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long | 是 | X 坐标 |
| y | long | 是 | Y 坐标 |

**返回值**：颜色字符串，格式为 `"RRGGBB"`，如 `"FF0000"` 表示红色。

**异常情况**：坐标超出屏幕范围时返回空字符串。

**调用示例**：

```python
# Python
color = dm.get_color(500, 300)
print(f"(500,300) 处的颜色: #{color}")

# 解析 RGB 分量
r = int(color[0:2], 16)
g = int(color[2:4], 16)
b = int(color[4:6], 16)
print(f"R={r} G={g} B={b}")
```

---

### 4.7 dm_getColorBGR

**函数签名**：
```c
const char* DM_CALL dm_getColorBGR(long x, long y);
```

**功能说明**：获取屏幕指定点的颜色值，返回 BGR 格式（BBGGRR）。与 Windows 内部颜色格式一致。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long | 是 | X 坐标 |
| y | long | 是 | Y 坐标 |

**返回值**：BGR 格式颜色字符串，如 `"0000FF"` 表示红色（BGR 下蓝色在低位）。

---

### 4.8 dm_getColorHSV

**函数签名**：
```c
const char* DM_CALL dm_getColorHSV(long x, long y);
```

**功能说明**：获取屏幕指定点的颜色，返回 HSV（色相、饱和度、明度）格式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long | 是 | X 坐标 |
| y | long | 是 | Y 坐标 |

**返回值**：HSV 格式字符串，格式为 `"H.S.V"`，如 `"0.100.50"`。

---

### 4.9 dm_getColorNum

**函数签名**：
```c
long DM_CALL dm_getColorNum(long x1, long y1, long x2, long y2, const char* color, long sim);
```

**功能说明**：统计指定区域内符合指定颜色的像素数量。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 区域左上角 X |
| y1 | long | 是 | 区域左上角 Y |
| x2 | long | 是 | 区域右下角 X |
| y2 | long | 是 | 区域右下角 Y |
| color | const char* | 是 | 目标颜色，格式 `"RRGGBB-偏色"` |
| sim | long | 是 | 相似度，0.0-1.0 |

**返回值**：匹配的像素数量。

**调用示例**：

```python
# Python
# 统计屏幕左上角区域有多少红色像素
count = dm.get_color_num(0, 0, 200, 200, "FF0000-101010", 0.9)
print(f"红色像素数量: {count}")
```

---

### 4.10 dm_getAveRGB

**函数签名**：
```c
const char* DM_CALL dm_getAveRGB(long x1, long y1, long x2, long y2);
```

**功能说明**：获取指定区域的平均颜色（RGB格式）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 区域左上角 X |
| y1 | long | 是 | 区域左上角 Y |
| x2 | long | 是 | 区域右下角 X |
| y2 | long | 是 | 区域右下角 Y |

**返回值**：平均颜色 RGB 字符串，格式 `"RRGGBB"`。

---

### 4.11 dm_getAveHSV

**函数签名**：
```c
const char* DM_CALL dm_getAveHSV(long x1, long y1, long x2, long y2);
```

**功能说明**：获取指定区域的平均颜色（HSV格式）。

**参数列表**：同 `dm_getAveRGB`

**返回值**：平均颜色 HSV 字符串。

---

### 4.12 dm_cmpColor

**函数签名**：
```c
long DM_CALL dm_cmpColor(long x, long y, const char* color, long sim);
```

**功能说明**：比较屏幕指定点的颜色是否与目标颜色匹配（在指定相似度范围内）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x | long | 是 | X 坐标 |
| y | long | 是 | Y 坐标 |
| color | const char* | 是 | 目标颜色，格式 `"RRGGBB"` 或 `"RRGGBB-偏色"` |
| sim | long | 是 | 相似度（0.0-1.0），如 0.9 表示 90% 相似 |

**返回值**：0=颜色匹配，1=颜色不匹配。

**调用示例**：

```python
# Python
# 检查 (500, 300) 是否为红色
if dm.cmp_color(500, 300, "FF0000", 0.9) == 0:
    print("颜色匹配：红色")
    
# 检查是否接近某个颜色（带偏色容差）
if dm.cmp_color(500, 300, "FF0000-202020", 0.8) == 0:
    print("颜色在容差范围内")
```

---

### 4.13 dm_findColor

**函数签名**：
```c
long DM_CALL dm_findColor(long x1, long y1, long x2, long y2, const char* color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：在指定屏幕区域内查找指定颜色，返回第一个匹配的坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 查找区域左上角 X |
| y1 | long | 是 | 查找区域左上角 Y |
| x2 | long | 是 | 查找区域右下角 X |
| y2 | long | 是 | 查找区域右下角 Y |
| color | const char* | 是 | 目标颜色，格式 `"RRGGBB-偏色"` |
| sim | long | 是 | 相似度（0.0-1.0） |
| dir | long | 是 | 查找方向（见下表） |
| intX | long* | 输出 | 找到的 X 坐标 |
| intY | long* | 输出 | 找到的 Y 坐标 |

**dir 查找方向**：

| 值 | 说明 |
|----|------|
| 0 | 从左到右，从上到下 |
| 1 | 从左到右，从下到上 |
| 2 | 从右到左，从上到下 |
| 3 | 从右到左，从下到上 |
| 4 | 从中心向外 |
| 5 | 从左上角开始 |

**返回值**：0=找到颜色，1=未找到。

**调用示例**：

```python
# Python
import ctypes

x, y = ctypes.c_long(), ctypes.c_long()
# 在全屏范围查找红色
ret = dm.find_color(0, 0, 1920, 1080, "FF0000-050505", 0.9, 0, x, y)
if ret == 0:
    print(f"找到红色在 ({x.value}, {y.value})")
else:
    print("未找到红色")
```

---

### 4.14 dm_findColorEx

**函数签名**：
```c
const char* DM_CALL dm_findColorEx(long x1, long y1, long x2, long y2, const char* color, long sim, long dir);
```

**功能说明**：在指定区域查找颜色，返回所有匹配位置的坐标字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 查找区域左上角 X |
| y1 | long | 是 | 查找区域左上角 Y |
| x2 | long | 是 | 查找区域右下角 X |
| y2 | long | 是 | 查找区域右下角 Y |
| color | const char* | 是 | 目标颜色 |
| sim | long | 是 | 相似度 |
| dir | long | 是 | 查找方向 |

**返回值**：坐标字符串，格式 `"x1|y1,x2|y2,..."`，未找到时返回空字符串 `""`。

**调用示例**：

```python
# Python
result = dm.find_color_ex(0, 0, 1920, 1080, "FF0000", 0.9, 0)
if result:
    print(f"找到红色位置: {result}")
    # 解析第一个坐标
    parts = result.split(",")
    first_pos = parts[0]
    x, y = first_pos.split("|")
    print(f"第一个红色在 ({x}, {y})")
```

---

### 4.15 dm_findColorBlock

**函数签名**：
```c
long DM_CALL dm_findColorBlock(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height, long* intX, long* intY);
```

**功能说明**：在指定区域查找指定颜色的色块（连续满足颜色条件的像素区域）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 查找区域左上角 X |
| y1 | long | 是 | 查找区域左上角 Y |
| x2 | long | 是 | 查找区域右下角 X |
| y2 | long | 是 | 查找区域右下角 Y |
| color | const char* | 是 | 目标颜色 |
| sim | long | 是 | 相似度 |
| count | long | 是 | 色块中至少需要的像素数量 |
| width | long | 是 | 色块的最大宽度 |
| height | long | 是 | 色块的最大高度 |
| intX | long* | 输出 | 找到的 X 坐标 |
| intY | long* | 输出 | 找到的 Y 坐标 |

**返回值**：0=找到，1=未找到。

---

### 4.16 dm_findColorBlockEx

**函数签名**：
```c
const char* DM_CALL dm_findColorBlockEx(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height);
```

**功能说明**：在指定区域查找色块，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findColorBlock`（无输出参数）

**返回值**：坐标字符串，格式 `"x1|y1,x2|y2,..."`。

---

### 4.17 dm_findColorE

**函数签名**：
```c
const char* DM_CALL dm_findColorE(long x1, long y1, long x2, long y2, const char* color, long sim, long dir);
```

**功能说明**：查找颜色，返回第一个匹配位置的坐标字符串（包括偏色信息）。

**参数列表**：同 `dm_findColorEx`

**返回值**：坐标字符串，格式 `"x|y"`，未找到返回空字符串。

---

### 4.18 dm_findMulColor

**函数签名**：
```c
long DM_CALL dm_findMulColor(long x1, long y1, long x2, long y2, const char* color, long sim);
```

**功能说明**：在指定区域内查找多种颜色。多颜色用 `|` 分隔，只要找到其中任意一种即返回成功。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 区域左上角 X |
| y1 | long | 是 | 区域左上角 Y |
| x2 | long | 是 | 区域右下角 X |
| y2 | long | 是 | 区域右下角 Y |
| color | const char* | 是 | 多颜色，用 `|` 分隔，如 `"FF0000|00FF00|0000FF"` |
| sim | long | 是 | 相似度 |

**返回值**：0=找到任意一种颜色，1=全未找到。

**调用示例**：

```python
# Python
# 查找红、绿、蓝中任意一种颜色
ret = dm.find_mul_color(0, 0, 1920, 1080, "FF0000|00FF00|0000FF", 0.9)
if ret == 0:
    print("找到红色、绿色或蓝色")
```

---

### 4.19 dm_findMultiColor

**函数签名**：
```c
long DM_CALL dm_findMultiColor(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：多点找色。先找到指定主色，再按偏移检查周围的颜色，全部匹配才算找到。比单点找色更精确。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 查找区域左上角 X |
| y1 | long | 是 | 查找区域左上角 Y |
| x2 | long | 是 | 查找区域右下角 X |
| y2 | long | 是 | 查找区域右下角 Y |
| first_color | const char* | 是 | 主色，格式 `"RRGGBB-偏色"` |
| offset_color | const char* | 是 | 偏移颜色描述，格式 `"x1\|y1\|RRGGBB-偏色,x2\|y2\|RRGGBB-偏色,..."` |
| sim | long | 是 | 相似度 |
| dir | long | 是 | 查找方向 |
| intX | long* | 输出 | 找到的 X 坐标 |
| intY | long* | 输出 | 找到的 Y 坐标 |

**返回值**：0=找到，1=未找到。

**调用示例**：

```python
# Python
# 找一个红色点，其右方10像素为绿色，下方10像素为蓝色
x, y = ctypes.c_long(), ctypes.c_long()
ret = dm.find_multi_color(
    0, 0, 1920, 1080,
    "FF0000-000000",           # 主色：红色
    "10|0|00FF00-000000,0|10|0000FF-000000",  # 偏移颜色
    0.9, 0, x, y
)
if ret == 0:
    print(f"找到多点颜色在 ({x.value}, {y.value})")
```

---

### 4.20 dm_findMultiColorEx

**函数签名**：
```c
const char* DM_CALL dm_findMultiColorEx(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir);
```

**功能说明**：多点找色，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findMultiColor`（无输出参数）

**返回值**：坐标字符串，格式 `"x1|y1,x2|y2,..."`。

---

### 4.21 dm_findMultiColorE

**函数签名**：
```c
const char* DM_CALL dm_findMultiColorE(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir);
```

**功能说明**：多点找色，返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findMultiColorEx`

**返回值**：坐标字符串，格式 `"x|y"`。

---

### 4.22 dm_findPic

**函数签名**：
```c
long DM_CALL dm_findPic(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：在指定屏幕区域内查找指定的图片，返回第一个匹配的坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 查找区域左上角 X |
| y1 | long | 是 | 查找区域左上角 Y |
| x2 | long | 是 | 查找区域右下角 X |
| y2 | long | 是 | 查找区域右下角 Y |
| pic_name | const char* | 是 | 图片文件名，多个图片用 `|` 分隔（逐一查找，找到即返回） |
| delta_color | const char* | 是 | 偏色值，如 `"202020-000000"`，格式 `"偏色-偏色模式"` |
| sim | long | 是 | 相似度（0.0-1.0） |
| dir | long | 是 | 查找方向 |
| intX | long* | 输出 | 找到的 X 坐标 |
| intY | long* | 输出 | 找到的 Y 坐标 |

**返回值**：0=找到图片，1=未找到。

**调用示例**：

```python
# Python
# 在屏幕上找按钮图片
x, y = ctypes.c_long(), ctypes.c_long()
ret = dm.find_pic(0, 0, 1920, 1080, "button.bmp", "202020", 0.9, 0, x, y)
if ret == 0:
    print(f"找到按钮在 ({x.value}, {y.value})")
    dm.move_to(x.value, y.value)
    dm.left_click()
else:
    print("未找到按钮")

# 多个图片中找第一个匹配的
ret = dm.find_pic(0, 0, 1920, 1080, "btn_start.bmp|btn_ok.bmp|btn_confirm.bmp", "202020", 0.8, 0, x, y)
```

---

### 4.23 dm_findPicE

**函数签名**：
```c
const char* DM_CALL dm_findPicE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
```

**功能说明**：查找图片，返回第一个匹配位置的坐标字符串（包含图片索引）。

**参数列表**：同 `dm_findPic`（无输出参数）

**返回值**：坐标字符串，格式 `"图片索引|x|y"`，未找到返回空字符串。

---

### 4.24 dm_findPicEx

**函数签名**：
```c
const char* DM_CALL dm_findPicEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
```

**功能说明**：查找图片，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findPic`

**返回值**：坐标字符串，格式 `"索引|x1|y1,索引|x2|y2,..."`，未找到返回空字符串。

**调用示例**：

```python
# Python
results = dm.find_pic_ex(0, 0, 1920, 1080, "icon.bmp", "202020", 0.9, 0)
if results:
    # 解析结果: "0|100|200,0|300|400"
    for match in results.split(","):
        parts = match.split("|")
        idx, x, y = int(parts[0]), int(parts[1]), int(parts[2])
        print(f"图片 {idx} 在 ({x}, {y})")
```

---

### 4.25 dm_findPicExS

**函数签名**：
```c
const char* DM_CALL dm_findPicExS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
```

**功能说明**：查找图片（智能模式），返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findPicEx`

**返回值**：同 `dm_findPicEx`

---

### 4.26 dm_findPicS

**函数签名**：
```c
long DM_CALL dm_findPicS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：查找图片（智能模式），支持透明图片查找。

**参数列表**：同 `dm_findPic`

**返回值**：0=找到，1=未找到。

---

### 4.27 dm_findPicMem

**函数签名**：
```c
long DM_CALL dm_findPicMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：从内存中查找图片（图片数据已在内存中，无需从文件加载）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 查找区域 |
| pic_info | const char* | 是 | 图片信息（内存图片的描述字符串） |
| delta_color | const char* | 是 | 偏色值 |
| sim | long | 是 | 相似度 |
| dir | long | 是 | 查找方向 |
| intX/intY | long* | 输出 | 坐标 |

**返回值**：0=找到，1=未找到。

---

### 4.28 dm_findPicMemE

**函数签名**：
```c
const char* DM_CALL dm_findPicMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
```

**功能说明**：内存找图，返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findPicMem`（无输出参数）

**返回值**：坐标字符串。

---

### 4.29 dm_findPicMemEx

**函数签名**：
```c
const char* DM_CALL dm_findPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
```

**功能说明**：内存找图，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findPicMem`

**返回值**：坐标字符串，多个位置以 `,` 分隔。

---

### 4.30 dm_findPicSim

**函数签名**：
```c
long DM_CALL dm_findPicSim(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：智能找图，支持透明色和更灵活的匹配算法。

**参数列表**：同 `dm_findPic`

**返回值**：0=找到，1=未找到。

---

### 4.31 dm_findPicSimE

**函数签名**：
```c
const char* DM_CALL dm_findPicSimE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
```

**功能说明**：智能找图，返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findPicSim`（无输出参数）

**返回值**：坐标字符串。

---

### 4.32 dm_findPicSimEx

**函数签名**：
```c
const char* DM_CALL dm_findPicSimEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir);
```

**功能说明**：智能找图，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findPicSim`

**返回值**：坐标字符串。

---

### 4.33 dm_findPicSimMem

**函数签名**：
```c
long DM_CALL dm_findPicSimMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：智能找图（内存模式），从内存图片数据中查找。

**参数列表**：同 `dm_findPicMem`

**返回值**：0=找到，1=未找到。

---

### 4.34 dm_findPicSimMemE

**函数签名**：
```c
const char* DM_CALL dm_findPicSimMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
```

**功能说明**：智能找图（内存模式），返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findPicSimMem`（无输出参数）

**返回值**：坐标字符串。

---

### 4.35 dm_findPicSimMemEx

**函数签名**：
```c
const char* DM_CALL dm_findPicSimMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir);
```

**功能说明**：智能找图（内存模式），返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findPicSimMem`

**返回值**：坐标字符串。

---

### 4.36 dm_findShape

**函数签名**：
```c
long DM_CALL dm_findShape(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir, long* intX, long* intY);
```

**功能说明**：在指定区域查找指定的形状（颜色轮廓）。根据偏移颜色描述的形状特征进行匹配。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 查找区域 |
| offset_color | const char* | 是 | 形状描述（偏移颜色），格式同多点找色的偏移颜色 |
| sim | long | 是 | 相似度 |
| dir | long | 是 | 查找方向 |
| intX/intY | long* | 输出 | 坐标 |

**返回值**：0=找到，1=未找到。

---

### 4.37 dm_findShapeE

**函数签名**：
```c
const char* DM_CALL dm_findShapeE(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir);
```

**功能说明**：查找形状，返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findShape`（无输出参数）

**返回值**：坐标字符串。

---

### 4.38 dm_findShapeEx

**函数签名**：
```c
const char* DM_CALL dm_findShapeEx(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir);
```

**功能说明**：查找形状，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findShape`

**返回值**：坐标字符串。

---

### 4.39 dm_getPicSize

**函数签名**：
```c
const char* DM_CALL dm_getPicSize(const char* pic_name);
```

**功能说明**：获取指定图片文件的尺寸（宽度和高度）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pic_name | const char* | 是 | 图片文件名 |

**返回值**：尺寸字符串，格式 `"width|height"`。

**调用示例**：

```python
# Python
size = dm.get_pic_size("button.bmp")
if size:
    w, h = size.split("|")
    print(f"图片尺寸: {w}x{h}")
```

---

### 4.40 dm_freePic

**函数签名**：
```c
long DM_CALL dm_freePic(const char* pic_name);
```

**功能说明**：释放指定图片占用的内存缓存。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pic_name | const char* | 是 | 图片文件名，`"*"` 表示释放所有图片 |

**返回值**：0=成功。

---

### 4.41 dm_loadPic

**函数签名**：
```c
long DM_CALL dm_loadPic(const char* pic_name);
```

**功能说明**：将指定图片文件加载到内存缓存中，预先加载可提高后续找图速度。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pic_name | const char* | 是 | 图片文件名 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.load_pic("button.bmp")      # 预加载到内存
dm.load_pic("icon.bmp")
# 后续找图更快
dm.find_pic(0, 0, 1920, 1080, "button.bmp", "202020", 0.9, 0, x, y)
```

---

### 4.42 dm_loadPicByte

**函数签名**：
```c
long DM_CALL dm_loadPicByte(const char* pic_name, long data, long size);
```

**功能说明**：从内存字节数据加载图片，而非从文件加载。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pic_name | const char* | 是 | 图片名称标识 |
| data | long | 是 | 图片数据内存地址 |
| size | long | 是 | 数据大小（字节） |

**返回值**：0=成功，非0=失败。

---

### 4.43 dm_appendPicAddr

**函数签名**：
```c
const char* DM_CALL dm_appendPicAddr(const char* pic_info, long addr, long size);
```

**功能说明**：向图片信息字符串中追加内存图片数据地址，用于构建内存图片列表。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pic_info | const char* | 是 | 现有图片信息字符串 |
| addr | long | 是 | 图片数据内存地址 |
| size | long | 是 | 数据大小 |

**返回值**：更新后的图片信息字符串。

---

### 4.44 dm_matchPicName

**函数签名**：
```c
const char* DM_CALL dm_matchPicName(const char* pic_name);
```

**功能说明**：根据当前设置的路径，匹配图片文件的完整路径。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pic_name | const char* | 是 | 图片名称（支持通配符 `*` 和 `?`） |

**返回值**：匹配到的完整文件路径字符串。

---

### 4.45 dm_imageToBmp

**函数签名**：
```c
long DM_CALL dm_imageToBmp(const char* pic_name, const char* bmp_name);
```

**功能说明**：将指定图片文件转换为 BMP 格式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pic_name | const char* | 是 | 源图片文件（支持 PNG、JPG 等格式） |
| bmp_name | const char* | 是 | 输出 BMP 文件路径 |

**返回值**：0=成功，非0=失败。

---

### 4.46 dm_setPicPwd

**函数签名**：
```c
long DM_CALL dm_setPicPwd(const char* pwd);
```

**功能说明**：设置图片文件的加解密密码。用于加载加密的图片文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pwd | const char* | 是 | 密码字符串 |

**返回值**：0=成功。

---

### 4.47 dm_setExcludeRegion

**函数签名**：
```c
long DM_CALL dm_setExcludeRegion(long mode, const char* info);
```

**功能说明**：设置排除区域，在找图/找色时忽略指定区域。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| mode | long | 是 | 0=排除模式，1=仅保留模式 |
| info | const char* | 是 | 区域描述字符串 |

**返回值**：0=成功。

---

### 4.48 dm_getScreenData

**函数签名**：
```c
long DM_CALL dm_getScreenData(long x1, long y1, long x2, long y2);
```

**功能说明**：获取屏幕指定区域的原始图像数据（返回内存地址）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 区域左上角 X |
| y1 | long | 是 | 区域左上角 Y |
| x2 | long | 是 | 区域右下角 X |
| y2 | long | 是 | 区域右下角 Y |

**返回值**：图像数据内存地址（低32位），0=失败。需配合 `dm_getScreenDataBmp` 使用。

---

### 4.49 dm_getScreenDataBmp

**函数签名**：
```c
const char* DM_CALL dm_getScreenDataBmp(long x1, long y1, long x2, long y2, long* size);
```

**功能说明**：获取屏幕指定区域的 BMP 图像数据，返回数据指针。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 区域坐标 |
| size | long* | 输出 | 数据大小（字节） |

**返回值**：BMP 图像数据内存地址指针。

---

### 4.50 dm_enableDisplayDebug

**函数签名**：
```c
long DM_CALL dm_enableDisplayDebug(long enable_debug);
```

**功能说明**：启用或禁用显示调试模式。启用后会在屏幕上高亮显示找图/找色区域，方便调试。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable_debug | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 4.51 dm_enableFindPicMultithread

**函数签名**：
```c
long DM_CALL dm_enableFindPicMultithread(long enable);
```

**功能说明**：启用或禁用多线程找图。启用后可利用多核 CPU 加速找图操作。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 4.52 dm_enableGetColorByCapture

**函数签名**：
```c
long DM_CALL dm_enableGetColorByCapture(long enable);
```

**功能说明**：设置获取颜色时是否通过截图方式获取。启用后颜色获取更精确，但速度稍慢。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=直接获取，1=通过截图获取 |

**返回值**：0=成功。

---

### 4.53 dm_setFindPicMultithreadCount

**函数签名**：
```c
long DM_CALL dm_setFindPicMultithreadCount(long count);
```

**功能说明**：设置多线程找图的线程数量。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| count | long | 是 | 线程数（0=使用默认值） |

**返回值**：0=成功。

---

### 4.54 dm_setFindPicMultithreadLimit

**函数签名**：
```c
long DM_CALL dm_setFindPicMultithreadLimit(long limit);
```

**功能说明**：设置多线程找图时每个线程的最大处理区域限制。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| limit | long | 是 | 限制值（像素数） |

**返回值**：0=成功。

---

### 4.55 dm_bgr2RGB

**函数签名**：
```c
long DM_CALL dm_bgr2RGB(const char* bgr_color);
```

**功能说明**：将 BGR 格式颜色字符串转换为 RGB 格式颜色值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| bgr_color | const char* | 是 | BGR 颜色字符串，如 `"0000FF"` |

**返回值**：RGB 颜色值（整数），如 `0xFF0000`。

---

### 4.56 dm_rgb2BGR

**函数签名**：
```c
const char* DM_CALL dm_rgb2BGR(const char* rgb_color);
```

**功能说明**：将 RGB 格式颜色字符串转换为 BGR 格式字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| rgb_color | const char* | 是 | RGB 颜色字符串，如 `"FF0000"` |

**返回值**：BGR 格式颜色字符串，如 `"0000FF"`。

---

### 4.57 dm_isDisplayDead

**函数签名**：
```c
long DM_CALL dm_isDisplayDead(long x1, long y1, long x2, long y2, long time);
```

**功能说明**：检测指定区域是否在指定时间内画面静止（无变化）。用于判断界面是否卡死或加载完成。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 检测区域 |
| time | long | 是 | 检测时长（毫秒） |

**返回值**：0=画面有变化（未静止），1=画面静止。

**调用示例**：

```python
# Python
# 检测游戏画面是否卡死（3秒内无变化）
if dm.is_display_dead(0, 0, 800, 600, 3000) == 1:
    print("画面已静止3秒，可能卡死")
```

---

## 模块 5：后台设置

后台设置模块提供窗口绑定、后台键鼠操作、DX 加速等功能。

---

### 5.1 dm_bindWindow

**函数签名**：
```c
long DM_CALL dm_bindWindow(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, long mode);
```

**功能说明**：绑定窗口，使后续的图色、键鼠操作在后台（窗口最小化或遮挡时仍可操作）进行。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 要绑定的窗口句柄 |
| display | const char* | 是 | 显示模式（后台截图方式） |
| mouse | const char* | 是 | 鼠标模式（后台鼠标操作方式） |
| keypad | const char* | 是 | 键盘模式（后台键盘操作方式） |
| mode | long | 是 | 绑定模式，0=普通，1=增强 |

**显示/鼠标/键盘模式取值**：

| 值 | 说明 |
|----|------|
| `"normal"` | 普通模式（前台） |
| `"dx"` | DirectX 模式（适用于大多数游戏） |
| `"dx2"` | DirectX 2 模式 |
| `"dx3"` | DirectX 3 模式 |
| `"gdi"` | GDI 模式 |
| `"gdi2"` | GDI 2 模式 |
| `"opengl"` | OpenGL 模式 |

**返回值**：0=绑定成功，非0=绑定失败（见错误码表）。

**异常情况**：窗口句柄无效时返回 `DMERR_WINDOW_ABNORMAL`。

**调用示例**：

```python
# Python
# 绑定游戏窗口
hwnd = dm.find_window("", "游戏窗口标题")
ret = dm.bind_window(hwnd, "dx", "dx", "dx", 0)
if ret == 0:
    print("绑定成功，可以开始后台操作")
    
    # 后台操作...
    dm.move_to(500, 300)
    dm.left_click()
    color = dm.get_color(100, 100)
    
    # 完成后解绑
    dm.un_bind_window()
else:
    print(f"绑定失败，错误码: {ret}")
```

---

### 5.2 dm_bindWindowEx

**函数签名**：
```c
long DM_CALL dm_bindWindowEx(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, const char* pub, long mode);
```

**功能说明**：绑定窗口的扩展版本，支持额外的公共模式设置。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| display | const char* | 是 | 显示模式 |
| mouse | const char* | 是 | 鼠标模式 |
| keypad | const char* | 是 | 键盘模式 |
| pub | const char* | 否 | 公共模式（附加设置），如 `"dx"` |
| mode | long | 是 | 绑定模式 |

**返回值**：0=成功，非0=失败。

---

### 5.3 dm_unBindWindow

**函数签名**：
```c
long DM_CALL dm_unBindWindow();
```

**功能说明**：解绑当前绑定的窗口，释放后台操作资源。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

### 5.4 dm_getBindWindow

**函数签名**：
```c
long DM_CALL dm_getBindWindow();
```

**功能说明**：获取当前绑定的窗口句柄。

**参数列表**：无

**返回值**：当前绑定的窗口句柄，0=未绑定。

---

### 5.5 dm_isBind

**函数签名**：
```c
long DM_CALL dm_isBind(intptr_t hwnd);
```

**功能说明**：检查指定窗口是否已被绑定。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：0=未绑定，1=已绑定。

---

### 5.6 dm_forceUnBindWindow

**函数签名**：
```c
long DM_CALL dm_forceUnBindWindow();
```

**功能说明**：强制解绑窗口，即使解绑过程出现异常也继续执行。

**参数列表**：无

**返回值**：0=成功。

---

### 5.7 dm_setAero

**函数签名**：
```c
long DM_CALL dm_setAero(long enable);
```

**功能说明**：启用或禁用 Aero 毛玻璃效果（Windows 7/8/10）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.8 dm_lockInput

**函数签名**：
```c
long DM_CALL dm_lockInput(long enable);
```

**功能说明**：锁定或解锁鼠标键盘输入，锁定后用户无法操作鼠标键盘。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=解锁，1=锁定 |

**返回值**：0=成功。

---

### 5.9 dm_lockMouseRect

**函数签名**：
```c
long DM_CALL dm_lockMouseRect(long x1, long y1, long x2, long y2);
```

**功能说明**：将鼠标锁定在指定的矩形区域内，无法移出。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 锁定区域坐标 |

**返回值**：0=成功。

---

### 5.10 dm_lockDisplay

**函数签名**：
```c
long DM_CALL dm_lockDisplay(long enable);
```

**功能说明**：锁定或解锁显示。锁定后屏幕画面不会更新。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=解锁，1=锁定 |

**返回值**：0=成功。

---

### 5.11 dm_enableBind

**函数签名**：
```c
long DM_CALL dm_enableBind(long enable);
```

**功能说明**：启用或禁用绑定模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.12 dm_enableFakeActive

**函数签名**：
```c
long DM_CALL dm_enableFakeActive(long enable);
```

**功能说明**：启用或禁用虚假激活模式。启用后，后台绑定窗口时不会真正激活窗口，但模拟前台操作。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.13 dm_enableIme

**函数签名**：
```c
long DM_CALL dm_enableIme(long enable);
```

**功能说明**：启用或禁用输入法（IME）支持。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.14 dm_enableKeypadMsg

**函数签名**：
```c
long DM_CALL dm_enableKeypadMsg(long enable);
```

**功能说明**：启用或禁用键盘消息同步。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.15 dm_enableKeypadPatch

**函数签名**：
```c
long DM_CALL dm_enableKeypadPatch(long enable);
```

**功能说明**：启用或禁用键盘补丁模式。用于解决某些游戏后台键盘无效的问题。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.16 dm_enableKeypadSync

**函数签名**：
```c
long DM_CALL dm_enableKeypadSync(long enable);
```

**功能说明**：启用或禁用键盘同步模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.17 dm_enableMouseMsg

**函数签名**：
```c
long DM_CALL dm_enableMouseMsg(long enable);
```

**功能说明**：启用或禁用鼠标消息同步。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.18 dm_enableMouseSync

**函数签名**：
```c
long DM_CALL dm_enableMouseSync(long enable, long time_out);
```

**功能说明**：启用或禁用鼠标同步模式，并设置超时时间。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |
| time_out | long | 是 | 同步超时时间（毫秒） |

**返回值**：0=成功。

---

### 5.19 dm_enableRealKeypad

**函数签名**：
```c
long DM_CALL dm_enableRealKeypad(long enable);
```

**功能说明**：启用或禁用真实键盘模式。启用后，后台键盘操作使用硬件级模拟。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.20 dm_enableRealMouse

**函数签名**：
```c
long DM_CALL dm_enableRealMouse(long enable, long mousedelay, long mousestep);
```

**功能说明**：启用或禁用真实鼠标模式，并设置延迟和步长。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |
| mousedelay | long | 是 | 鼠标操作延迟（毫秒） |
| mousestep | long | 是 | 鼠标移动步长（像素） |

**返回值**：0=成功。

---

### 5.21 dm_enableSpeedDx

**函数签名**：
```c
long DM_CALL dm_enableSpeedDx(long enable);
```

**功能说明**：启用或禁用 DX 加速模式。可提升后台截图速度。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.22 dm_getFps

**函数签名**：
```c
long DM_CALL dm_getFps();
```

**功能说明**：获取当前后台刷新的帧率（FPS）。

**参数列表**：无

**返回值**：当前帧率（每秒帧数）。

---

### 5.23 dm_setDisplayDelay

**函数签名**：
```c
long DM_CALL dm_setDisplayDelay(long delay);
```

**功能说明**：设置后台显示延迟（截图间隔）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| delay | long | 是 | 延迟时间（毫秒） |

**返回值**：0=成功。

---

### 5.24 dm_setDisplayRefreshDelay

**函数签名**：
```c
long DM_CALL dm_setDisplayRefreshDelay(long delay);
```

**功能说明**：设置后台显示刷新延迟。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| delay | long | 是 | 刷新延迟（毫秒） |

**返回值**：0=成功。

---

### 5.25 dm_setInputDm

**函数签名**：
```c
long DM_CALL dm_setInputDm(long enable);
```

**功能说明**：设置输入模式为 DM 模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 5.26 dm_hackSpeed

**函数签名**：
```c
long DM_CALL dm_hackSpeed(long rate);
```

**功能说明**：设置游戏加速/减速倍率。用于控制游戏运行速度。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| rate | long | 是 | 速度倍率，100=正常速度，200=2倍速，50=半速 |

**返回值**：0=成功。

---

### 5.27 dm_downCpu

**函数签名**：
```c
long DM_CALL dm_downCpu(long enable, long rate);
```

**功能说明**：降低 CPU 占用率，通过限制插件刷新频率来减少 CPU 开销。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=关闭，1=开启 |
| rate | long | 是 | CPU 降低率（1-100） |

**返回值**：0=成功。

---

### 5.28 dm_switchBindWindow

**函数签名**：
```c
long DM_CALL dm_switchBindWindow(intptr_t hwnd);
```

**功能说明**：在不解绑的情况下切换绑定的窗口句柄。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 新的窗口句柄 |

**返回值**：0=成功，非0=失败。

---

## 模块 6：内存操作

内存操作模块提供进程内存的读写、搜索、分配等功能。

---

### 6.1 dm_openProcess

**函数签名**：
```c
long DM_CALL dm_openProcess(long pid);
```

**功能说明**：打开指定 PID 的进程，获取操作权限。后续的内存操作需要先调用此函数。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pid | long | 是 | 进程 ID |

**返回值**：0=成功，非0=失败。

**异常情况**：权限不足时返回错误码。

**调用示例**：

```python
# Python
pid = dm.get_window_process_id(hwnd)
ret = dm.open_process(pid)
if ret == 0:
    print(f"成功打开进程 {pid}")
    # 执行内存操作...
else:
    print(f"打开进程失败，可能需要管理员权限")
```

---

### 6.2 dm_getModuleBaseAddr

**函数签名**：
```c
long DM_CALL dm_getModuleBaseAddr(intptr_t hwnd, const char* module);
```

**功能说明**：获取指定进程（通过窗口句柄）中指定模块的基址。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 目标进程的窗口句柄 |
| module | const char* | 是 | 模块名称，如 `"dm_hook.dll"`、`"kernel32.dll"` |

**返回值**：模块基址（内存地址），0=获取失败。

**调用示例**：

```python
# Python
base = dm.get_module_base_addr(hwnd, "game.exe")
print(f"game.exe 基址: 0x{base:08X}")

# 获取 DLL 基址
dll_base = dm.get_module_base_addr(hwnd, "user32.dll")
```

---

### 6.3 dm_getModuleSize

**函数签名**：
```c
long DM_CALL dm_getModuleSize(intptr_t hwnd, const char* module);
```

**功能说明**：获取指定模块的大小（字节数）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| module | const char* | 是 | 模块名称 |

**返回值**：模块大小（字节），0=获取失败。

---

### 6.4 dm_getRemoteApiAddress

**函数签名**：
```c
long DM_CALL dm_getRemoteApiAddress(intptr_t hwnd, const char* module, const char* api);
```

**功能说明**：获取远程进程中指定 API 函数的地址。用于远程调用或 Hook。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| module | const char* | 是 | 模块名称，如 `"kernel32.dll"` |
| api | const char* | 是 | API 函数名，如 `"MessageBoxA"` |

**返回值**：函数地址，0=获取失败。

---

### 6.5 dm_readInt

**函数签名**：
```c
long DM_CALL dm_readInt(intptr_t hwnd, long addr, long* ret);
```

**功能说明**：从指定进程的指定内存地址读取一个 4 字节整数。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| ret | long* | 输出 | 读取到的整数值 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
# 读取游戏中的血量值
addr = 0x00A1B2C3  # 假设地址
val = ctypes.c_long()
ret = dm.read_int(hwnd, addr, val)
if ret == 0:
    print(f"地址 0x{addr:08X} 的值: {val.value}")
```

---

### 6.6 dm_readIntAddr

**函数签名**：
```c
long DM_CALL dm_readIntAddr(intptr_t hwnd, long addr, long type, long* ret);
```

**功能说明**：从指定地址读取指定类型的整数值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| type | long | 是 | 数据类型：0=字节，1=字(2字节)，2=整数(4字节) |
| ret | long* | 输出 | 读取的值 |

**返回值**：0=成功，非0=失败。

---

### 6.7 dm_readFloat

**函数签名**：
```c
long DM_CALL dm_readFloat(intptr_t hwnd, long addr, float* ret);
```

**功能说明**：从指定内存地址读取一个 float（4 字节浮点数）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| ret | float* | 输出 | 读取到的浮点数值 |

**返回值**：0=成功，非0=失败。

---

### 6.8 dm_readFloatAddr

**函数签名**：
```c
long DM_CALL dm_readFloatAddr(intptr_t hwnd, long addr, float* ret);
```

**功能说明**：从指定地址读取 float 值（支持多级指针）。

**参数列表**：同 `dm_readFloat`

**返回值**：0=成功，非0=失败。

---

### 6.9 dm_readDouble

**函数签名**：
```c
long DM_CALL dm_readDouble(intptr_t hwnd, long addr, double* ret);
```

**功能说明**：从指定内存地址读取一个 double（8 字节双精度浮点数）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| ret | double* | 输出 | 读取到的双精度浮点数值 |

**返回值**：0=成功，非0=失败。

---

### 6.10 dm_readDoubleAddr

**函数签名**：
```c
long DM_CALL dm_readDoubleAddr(intptr_t hwnd, long addr, double* ret);
```

**功能说明**：从指定地址读取 double 值（支持多级指针）。

**参数列表**：同 `dm_readDouble`

**返回值**：0=成功，非0=失败。

---

### 6.11 dm_readString

**函数签名**：
```c
const char* DM_CALL dm_readString(intptr_t hwnd, long addr, long type, long len);
```

**功能说明**：从指定内存地址读取字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| type | long | 是 | 字符串类型：0=ASCII，1=Unicode（UTF-16） |
| len | long | 是 | 读取的最大长度（字节数） |

**返回值**：读取到的字符串内容。

**调用示例**：

```python
# Python
# 读取 ASCII 字符串
name = dm.read_string(hwnd, 0x00A1B2C3, 0, 64)
print(f"读取的字符串: {name}")

# 读取 Unicode 字符串
w_name = dm.read_string(hwnd, 0x00A1B2C3, 1, 128)
```

---

### 6.12 dm_readStringAddr

**函数签名**：
```c
const char* DM_CALL dm_readStringAddr(intptr_t hwnd, long addr, long type, long len);
```

**功能说明**：从指定地址读取字符串（支持多级指针）。

**参数列表**：同 `dm_readString`

**返回值**：读取到的字符串。

---

### 6.13 dm_readData

**函数签名**：
```c
const char* DM_CALL dm_readData(intptr_t hwnd, long addr, long len);
```

**功能说明**：从指定内存地址读取原始字节数据。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| len | long | 是 | 读取的字节数 |

**返回值**：十六进制编码的数据字符串，如 `"FF00AABB"`。

---

### 6.14 dm_readDataAddr

**函数签名**：
```c
const char* DM_CALL dm_readDataAddr(intptr_t hwnd, long addr, long len);
```

**功能说明**：从指定地址读取原始字节数据（支持多级指针）。

**参数列表**：同 `dm_readData`

**返回值**：十六进制编码的数据字符串。

---

### 6.15 dm_readDataToBin

**函数签名**：
```c
long DM_CALL dm_readDataToBin(intptr_t hwnd, long addr, long len);
```

**功能说明**：读取内存数据并保存到二进制缓冲区，返回数据的内存地址。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| len | long | 是 | 读取的字节数 |

**返回值**：二进制数据的内存地址（低32位），0=失败。

---

### 6.16 dm_readDataAddrToBin

**函数签名**：
```c
long DM_CALL dm_readDataAddrToBin(intptr_t hwnd, long addr, long len);
```

**功能说明**：从指定地址读取数据到二进制缓冲区（支持多级指针）。

**参数列表**：同 `dm_readDataToBin`

**返回值**：二进制数据的内存地址。

---

### 6.17 dm_findInt

**函数签名**：
```c
const char* DM_CALL dm_findInt(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max);
```

**功能说明**：在指定地址范围内搜索指定范围的整数值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr_range | const char* | 是 | 搜索范围，格式 `"起始地址-结束地址"`，如 `"00400000-004FFFFF"` |
| int_value_min | long | 是 | 最小值（包含） |
| int_value_max | long | 是 | 最大值（包含） |

**返回值**：匹配的地址字符串，格式 `"地址1,地址2,..."`。

**调用示例**：

```python
# Python
# 在游戏主模块中搜索血量值
result = dm.find_int(hwnd, "00400000-00A00000", 100, 100)
if result:
    print(f"找到可能的地址: {result}")
```

---

### 6.18 dm_findIntEx

**函数签名**：
```c
const char* DM_CALL dm_findIntEx(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max, long step, long multi_thread, long mode);
```

**功能说明**：高级整数搜索，支持步进、多线程和多种搜索模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr_range | const char* | 是 | 搜索范围 |
| int_value_min | long | 是 | 最小值 |
| int_value_max | long | 是 | 最大值 |
| step | long | 是 | 搜索步进（字节），通常为 4 |
| multi_thread | long | 是 | 0=单线程，1=多线程 |
| mode | long | 是 | 搜索模式：0=精确搜索，1=模糊搜索 |

**返回值**：匹配的地址字符串。

---

### 6.19 dm_findFloat

**函数签名**：
```c
const char* DM_CALL dm_findFloat(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max);
```

**功能说明**：在指定地址范围内搜索指定范围的 float 浮点数值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr_range | const char* | 是 | 搜索范围 |
| float_value_min | float | 是 | 浮点数最小值 |
| float_value_max | float | 是 | 浮点数最大值 |

**返回值**：匹配的地址字符串。

---

### 6.20 dm_findFloatEx

**函数签名**：
```c
const char* DM_CALL dm_findFloatEx(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max, long step, long multi_thread, long mode);
```

**功能说明**：高级浮点数搜索，支持步进、多线程和多种搜索模式。

**参数列表**：同 `dm_findIntEx` 的额外参数

**返回值**：匹配的地址字符串。

---

### 6.21 dm_findDouble

**函数签名**：
```c
const char* DM_CALL dm_findDouble(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max);
```

**功能说明**：在指定地址范围内搜索指定范围的 double 双精度浮点数值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr_range | const char* | 是 | 搜索范围 |
| double_value_min | double | 是 | 最小值 |
| double_value_max | double | 是 | 最大值 |

**返回值**：匹配的地址字符串。

---

### 6.22 dm_findDoubleEx

**函数签名**：
```c
const char* DM_CALL dm_findDoubleEx(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max, long step, long multi_thread, long mode);
```

**功能说明**：高级双精度浮点数搜索。

**参数列表**：同 `dm_findIntEx`

**返回值**：匹配的地址字符串。

---

### 6.23 dm_findString

**函数签名**：
```c
const char* DM_CALL dm_findString(intptr_t hwnd, const char* addr_range, const char* string_value, long type);
```

**功能说明**：在指定地址范围内搜索字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr_range | const char* | 是 | 搜索范围 |
| string_value | const char* | 是 | 要搜索的字符串内容 |
| type | long | 是 | 0=ASCII，1=Unicode |

**返回值**：匹配的地址字符串。

---

### 6.24 dm_findStringEx

**函数签名**：
```c
const char* DM_CALL dm_findStringEx(intptr_t hwnd, const char* addr_range, const char* string_value, long type, long step, long multi_thread, long mode);
```

**功能说明**：高级字符串搜索。

**参数列表**：同 `dm_findIntEx`

**返回值**：匹配的地址字符串。

---

### 6.25 dm_findData

**函数签名**：
```c
const char* DM_CALL dm_findData(intptr_t hwnd, const char* addr_range, const char* data);
```

**功能说明**：在指定地址范围内搜索指定的字节数据。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr_range | const char* | 是 | 搜索范围 |
| data | const char* | 是 | 十六进制字节数据，如 `"FF00AABB"` |

**返回值**：匹配的地址字符串。

---

### 6.26 dm_findDataEx

**函数签名**：
```c
const char* DM_CALL dm_findDataEx(intptr_t hwnd, const char* addr_range, const char* data, long step, long multi_thread, long mode);
```

**功能说明**：高级字节数据搜索。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr_range | const char* | 是 | 搜索范围 |
| data | const char* | 是 | 十六进制数据 |
| step | long | 是 | 步进 |
| multi_thread | long | 是 | 多线程 |
| mode | long | 是 | 搜索模式 |

**返回值**：匹配的地址字符串。

---

### 6.27 dm_writeInt

**函数签名**：
```c
long DM_CALL dm_writeInt(intptr_t hwnd, long addr, long type, long v);
```

**功能说明**：向指定内存地址写入整数值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| type | long | 是 | 写入类型：0=字节，1=字(2字节)，2=整数(4字节) |
| v | long | 是 | 要写入的值 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
# 修改游戏中的血量值
addr = 0x00A1B2C3
ret = dm.write_int(hwnd, addr, 2, 9999)  # 写入 9999
if ret == 0:
    print(f"成功写入 9999 到 0x{addr:08X}")
```

---

### 6.28 dm_writeIntAddr

**函数签名**：
```c
long DM_CALL dm_writeIntAddr(intptr_t hwnd, long addr, long type, long v);
```

**功能说明**：向指定地址写入整数值（支持多级指针）。

**参数列表**：同 `dm_writeInt`

**返回值**：0=成功，非0=失败。

---

### 6.29 dm_writeFloat

**函数签名**：
```c
long DM_CALL dm_writeFloat(intptr_t hwnd, long addr, float v);
```

**功能说明**：向指定内存地址写入 float 浮点数值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| v | float | 是 | 要写入的浮点数值 |

**返回值**：0=成功，非0=失败。

---

### 6.30 dm_writeFloatAddr

**函数签名**：
```c
long DM_CALL dm_writeFloatAddr(intptr_t hwnd, long addr, float v);
```

**功能说明**：向指定地址写入 float 值（支持多级指针）。

**参数列表**：同 `dm_writeFloat`

**返回值**：0=成功，非0=失败。

---

### 6.31 dm_writeDouble

**函数签名**：
```c
long DM_CALL dm_writeDouble(intptr_t hwnd, long addr, double v);
```

**功能说明**：向指定内存地址写入 double 双精度浮点数值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| v | double | 是 | 要写入的双精度浮点数值 |

**返回值**：0=成功，非0=失败。

---

### 6.32 dm_writeDoubleAddr

**函数签名**：
```c
long DM_CALL dm_writeDoubleAddr(intptr_t hwnd, long addr, double v);
```

**功能说明**：向指定地址写入 double 值（支持多级指针）。

**参数列表**：同 `dm_writeDouble`

**返回值**：0=成功，非0=失败。

---

### 6.33 dm_writeString

**函数签名**：
```c
long DM_CALL dm_writeString(intptr_t hwnd, long addr, long type, const char* v);
```

**功能说明**：向指定内存地址写入字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| type | long | 是 | 0=ASCII，1=Unicode |
| v | const char* | 是 | 要写入的字符串 |

**返回值**：0=成功，非0=失败。

---

### 6.34 dm_writeStringAddr

**函数签名**：
```c
long DM_CALL dm_writeStringAddr(intptr_t hwnd, long addr, long type, const char* v);
```

**功能说明**：向指定地址写入字符串（支持多级指针）。

**参数列表**：同 `dm_writeString`

**返回值**：0=成功，非0=失败。

---

### 6.35 dm_writeData

**函数签名**：
```c
long DM_CALL dm_writeData(intptr_t hwnd, long addr, const char* data);
```

**功能说明**：向指定内存地址写入原始字节数据。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| data | const char* | 是 | 十六进制数据字符串，如 `"FF00AABB"` |

**返回值**：0=成功，非0=失败。

---

### 6.36 dm_writeDataAddr

**函数签名**：
```c
long DM_CALL dm_writeDataAddr(intptr_t hwnd, long addr, const char* data);
```

**功能说明**：向指定地址写入字节数据（支持多级指针）。

**参数列表**：同 `dm_writeData`

**返回值**：0=成功，非0=失败。

---

### 6.37 dm_writeDataFromBin

**函数签名**：
```c
long DM_CALL dm_writeDataFromBin(intptr_t hwnd, long addr, long data, long len);
```

**功能说明**：从二进制缓冲区向指定内存地址写入数据。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| data | long | 是 | 二进制数据的内存地址 |
| len | long | 是 | 数据长度（字节） |

**返回值**：0=成功，非0=失败。

---

### 6.38 dm_writeDataAddrFromBin

**函数签名**：
```c
long DM_CALL dm_writeDataAddrFromBin(intptr_t hwnd, long addr, long data, long len);
```

**功能说明**：从二进制缓冲区向指定地址写入数据（支持多级指针）。

**参数列表**：同 `dm_writeDataFromBin`

**返回值**：0=成功，非0=失败。

---

### 6.39 dm_virtualAllocEx

**函数签名**：
```c
long DM_CALL dm_virtualAllocEx(intptr_t hwnd, long size, long type, long protect);
```

**功能说明**：在远程进程中分配虚拟内存。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| size | long | 是 | 分配的字节数 |
| type | long | 是 | 分配类型：0=MEM_COMMIT，1=MEM_RESERVE |
| protect | long | 是 | 内存保护常量：0=PAGE_READWRITE，1=PAGE_EXECUTE_READWRITE |

**返回值**：分配的内存地址，0=失败。

---

### 6.40 dm_virtualFreeEx

**函数签名**：
```c
long DM_CALL dm_virtualFreeEx(intptr_t hwnd, long addr);
```

**功能说明**：释放远程进程中分配的虚拟内存。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 要释放的内存地址 |

**返回值**：0=成功，非0=失败。

---

### 6.41 dm_virtualProtectEx

**函数签名**：
```c
long DM_CALL dm_virtualProtectEx(intptr_t hwnd, long addr, long size, long type, long* old_protect);
```

**功能说明**：修改远程进程中指定内存区域的保护属性。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 内存地址 |
| size | long | 是 | 区域大小 |
| type | long | 是 | 新的保护类型 |
| old_protect | long* | 输出 | 原来的保护类型 |

**返回值**：0=成功，非0=失败。

---

### 6.42 dm_virtualQueryEx

**函数签名**：
```c
const char* DM_CALL dm_virtualQueryEx(intptr_t hwnd, long addr, long pmbi);
```

**功能说明**：查询远程进程中指定内存区域的信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| addr | long | 是 | 要查询的内存地址 |
| pmbi | long | 是 | 查询信息类型 |

**返回值**：内存信息字符串。

---

### 6.43 dm_freeProcessMemory

**函数签名**：
```c
long DM_CALL dm_freeProcessMemory(intptr_t hwnd);
```

**功能说明**：释放进程内存操作相关的内部资源。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：0=成功。

---

### 6.44 dm_getCommandLine

**函数签名**：
```c
const char* DM_CALL dm_getCommandLine(intptr_t hwnd);
```

**功能说明**：获取指定进程的命令行参数。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |

**返回值**：命令行字符串。

---

### 6.45 dm_terminateProcess

**函数签名**：
```c
long DM_CALL dm_terminateProcess(long pid);
```

**功能说明**：终止指定 PID 的进程。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pid | long | 是 | 进程 ID |

**返回值**：0=成功，非0=失败。

---

### 6.46 dm_terminateProcessTree

**函数签名**：
```c
long DM_CALL dm_terminateProcessTree(long pid);
```

**功能说明**：终止指定进程及其所有子进程。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pid | long | 是 | 进程 ID |

**返回值**：0=成功，非0=失败。

---

### 6.47 dm_setMemoryFindResultToFile

**函数签名**：
```c
long DM_CALL dm_setMemoryFindResultToFile(long enable);
```

**功能说明**：设置内存搜索的结果是否保存到文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=不保存，1=保存到文件 |

**返回值**：0=成功。

---

### 6.48 dm_setMemoryHwndAsProcessId

**函数签名**：
```c
long DM_CALL dm_setMemoryHwndAsProcessId(long enable);
```

**功能说明**：设置内存操作时是否将 hwnd 参数作为进程 ID 使用（而非窗口句柄）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=作为窗口句柄，1=作为进程 ID |

**返回值**：0=成功。

---

### 6.49 dm_setParam64ToPointer

**函数签名**：
```c
long DM_CALL dm_setParam64ToPointer(long enable);
```

**功能说明**：设置 64 位参数是否作为指针处理。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=普通参数，1=指针参数 |

**返回值**：0=成功。

---

### 6.50 dm_int64ToInt32

**函数签名**：
```c
const char* DM_CALL dm_int64ToInt32(long v_high, long v_low, long flag);
```

**功能说明**：将 64 位整数转换为 32 位地址。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| v_high | long | 是 | 64 位值的高 32 位 |
| v_low | long | 是 | 64 位值的低 32 位 |
| flag | long | 是 | 转换标志 |

**返回值**：转换后的地址字符串。

---

### 6.51 dm_intToData

**函数签名**：
```c
const char* DM_CALL dm_intToData(long v, long type);
```

**功能说明**：将整数值转换为十六进制数据字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| v | long | 是 | 整数值 |
| type | long | 是 | 数据类型：0=字节，1=字，2=整数 |

**返回值**：十六进制数据字符串。

---

### 6.52 dm_floatToData

**函数签名**：
```c
const char* DM_CALL dm_floatToData(float v);
```

**功能说明**：将 float 浮点数值转换为十六进制数据字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| v | float | 是 | 浮点数值 |

**返回值**：十六进制数据字符串。

---

### 6.53 dm_doubleToData

**函数签名**：
```c
const char* DM_CALL dm_doubleToData(double v);
```

**功能说明**：将 double 双精度浮点数值转换为十六进制数据字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| v | double | 是 | 双精度浮点数值 |

**返回值**：十六进制数据字符串。

---

### 6.54 dm_stringToData

**函数签名**：
```c
const char* DM_CALL dm_stringToData(const char* v, long type);
```

**功能说明**：将字符串转换为十六进制数据字符串。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| v | const char* | 是 | 字符串内容 |
| type | long | 是 | 0=ASCII，1=Unicode |

**返回值**：十六进制数据字符串。

---

## 模块 7：文件操作

文件操作模块提供文件的读写、复制、删除、移动，以及 INI 文件操作等功能。

---

### 7.1 dm_writeFile

**函数签名**：
```c
long DM_CALL dm_writeFile(const char* file, const char* content);
```

**功能说明**：将文本内容写入文件（覆盖模式）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 文件路径 |
| content | const char* | 是 | 要写入的文本内容 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.write_file("log.txt", "操作日志：开始执行任务")
```

---

### 7.2 dm_readFile

**函数签名**：
```c
const char* DM_CALL dm_readFile(const char* file);
```

**功能说明**：读取文件内容。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 文件路径 |

**返回值**：文件内容字符串，失败返回空字符串。

---

### 7.3 dm_deleteFile

**函数签名**：
```c
long DM_CALL dm_deleteFile(const char* file);
```

**功能说明**：删除指定文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 文件路径 |

**返回值**：0=成功，非0=失败。

---

### 7.4 dm_copyFile

**函数签名**：
```c
long DM_CALL dm_copyFile(const char* src, const char* dest, long over);
```

**功能说明**：复制文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| src | const char* | 是 | 源文件路径 |
| dest | const char* | 是 | 目标文件路径 |
| over | long | 是 | 0=目标存在时询问，1=覆盖 |

**返回值**：0=成功，非0=失败。

---

### 7.5 dm_moveFile

**函数签名**：
```c
long DM_CALL dm_moveFile(const char* src, const char* dest);
```

**功能说明**：移动或重命名文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| src | const char* | 是 | 源文件路径 |
| dest | const char* | 是 | 目标文件路径 |

**返回值**：0=成功，非0=失败。

---

### 7.6 dm_createFolder

**函数签名**：
```c
long DM_CALL dm_createFolder(const char* folder);
```

**功能说明**：创建文件夹。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| folder | const char* | 是 | 文件夹路径 |

**返回值**：0=成功，非0=失败。

---

### 7.7 dm_deleteFolder

**函数签名**：
```c
long DM_CALL dm_deleteFolder(const char* folder);
```

**功能说明**：删除文件夹及其所有内容。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| folder | const char* | 是 | 文件夹路径 |

**返回值**：0=成功，非0=失败。

---

### 7.8 dm_isFileExist

**函数签名**：
```c
long DM_CALL dm_isFileExist(const char* file);
```

**功能说明**：判断文件是否存在。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 文件路径 |

**返回值**：0=不存在，1=存在。

---

### 7.9 dm_isFolderExist

**函数签名**：
```c
long DM_CALL dm_isFolderExist(const char* folder);
```

**功能说明**：判断文件夹是否存在。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| folder | const char* | 是 | 文件夹路径 |

**返回值**：0=不存在，1=存在。

---

### 7.10 dm_getFileLength

**函数签名**：
```c
long DM_CALL dm_getFileLength(const char* file);
```

**功能说明**：获取文件大小（字节数）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 文件路径 |

**返回值**：文件大小（字节），-1=文件不存在。

---

### 7.11 dm_getRealPath

**函数签名**：
```c
const char* DM_CALL dm_getRealPath(const char* path);
```

**功能说明**：获取相对路径的绝对路径（基于当前全局路径）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| path | const char* | 是 | 相对路径 |

**返回值**：绝对路径字符串。

---

### 7.12 dm_selectFile

**函数签名**：
```c
const char* DM_CALL dm_selectFile();
```

**功能说明**：打开文件选择对话框，让用户选择一个文件。

**参数列表**：无

**返回值**：用户选择的文件路径，取消则返回空字符串。

---

### 7.13 dm_selectDirectory

**函数签名**：
```c
const char* DM_CALL dm_selectDirectory();
```

**功能说明**：打开文件夹选择对话框，让用户选择一个目录。

**参数列表**：无

**返回值**：用户选择的文件夹路径，取消则返回空字符串。

---

### 7.14 dm_downloadFile

**函数签名**：
```c
long DM_CALL dm_downloadFile(const char* url, const char* save_file, long timeout);
```

**功能说明**：从指定 URL 下载文件到本地。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| url | const char* | 是 | 文件下载 URL |
| save_file | const char* | 是 | 本地保存路径 |
| timeout | long | 是 | 超时时间（毫秒） |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
ret = dm.download_file("https://example.com/update.zip", "update.zip", 30000)
if ret == 0:
    print("下载成功")
```

---

### 7.15 dm_encodeFile

**函数签名**：
```c
long DM_CALL dm_encodeFile(const char* file, const char* pwd);
```

**功能说明**：使用密码加密文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 文件路径 |
| pwd | const char* | 是 | 加密密码 |

**返回值**：0=成功，非0=失败。

---

### 7.16 dm_decodeFile

**函数签名**：
```c
long DM_CALL dm_decodeFile(const char* file, const char* pwd);
```

**功能说明**：使用密码解密由 `dm_encodeFile` 加密的文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 文件路径 |
| pwd | const char* | 是 | 解密密码（需与加密时一致） |

**返回值**：0=成功，非0=失败。

---

### 7.17 dm_writeIni

**函数签名**：
```c
long DM_CALL dm_writeIni(const char* section, const char* key, const char* value, const char* file);
```

**功能说明**：向 INI 文件写入键值对。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| section | const char* | 是 | 节名称（如 `"[config]"` -> `"config"`） |
| key | const char* | 是 | 键名 |
| value | const char* | 是 | 值 |
| file | const char* | 是 | INI 文件路径 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.write_ini("config", "username", "admin", "settings.ini")
dm.write_ini("config", "volume", "80", "settings.ini")
```

---

### 7.18 dm_readIni

**函数签名**：
```c
const char* DM_CALL dm_readIni(const char* section, const char* key, const char* file);
```

**功能说明**：从 INI 文件读取指定键的值。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| section | const char* | 是 | 节名称 |
| key | const char* | 是 | 键名 |
| file | const char* | 是 | INI 文件路径 |

**返回值**：键对应的值字符串，键不存在时返回空字符串。

**调用示例**：

```python
# Python
username = dm.read_ini("config", "username", "settings.ini")
print(f"用户名: {username}")
```

---

### 7.19 dm_deleteIni

**函数签名**：
```c
long DM_CALL dm_deleteIni(const char* section, const char* key, const char* file);
```

**功能说明**：删除 INI 文件中的指定键或整个节。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| section | const char* | 是 | 节名称 |
| key | const char* | 是 | 键名（传空字符串表示删除整个节） |
| file | const char* | 是 | INI 文件路径 |

**返回值**：0=成功，非0=失败。

---

### 7.20 dm_enumIniKey

**函数签名**：
```c
const char* DM_CALL dm_enumIniKey(const char* section, const char* file);
```

**功能说明**：枚举 INI 文件中指定节的所有键名。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| section | const char* | 是 | 节名称 |
| file | const char* | 是 | INI 文件路径 |

**返回值**：键名列表，以 `"|"` 分隔。

---

### 7.21 dm_enumIniSection

**函数签名**：
```c
const char* DM_CALL dm_enumIniSection(const char* file);
```

**功能说明**：枚举 INI 文件中所有节名称。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | INI 文件路径 |

**返回值**：节名称列表，以 `"|"` 分隔。

---

### 7.22 dm_writeIniPwd

**函数签名**：
```c
long DM_CALL dm_writeIniPwd(const char* section, const char* key, const char* value, const char* file, const char* pwd);
```

**功能说明**：向加密的 INI 文件写入键值对。

**参数列表**：多一个 `pwd` 参数（加解密密码）

**返回值**：0=成功，非0=失败。

---

### 7.23 dm_readIniPwd

**函数签名**：
```c
const char* DM_CALL dm_readIniPwd(const char* section, const char* key, const char* file, const char* pwd);
```

**功能说明**：从加密的 INI 文件读取键值。

**参数列表**：同 `dm_writeIniPwd` 参数

**返回值**：键对应的值字符串。

---

### 7.24 dm_deleteIniPwd

**函数签名**：
```c
long DM_CALL dm_deleteIniPwd(const char* section, const char* key, const char* file, const char* pwd);
```

**功能说明**：删除加密 INI 文件中的键或节。

**参数列表**：同 `dm_deleteIni` + `pwd`

**返回值**：0=成功，非0=失败。

---

### 7.25 dm_enumIniKeyPwd

**函数签名**：
```c
const char* DM_CALL dm_enumIniKeyPwd(const char* section, const char* file, const char* pwd);
```

**功能说明**：枚举加密 INI 文件中指定节的所有键名。

**参数列表**：同 `dm_enumIniKey` + `pwd`

**返回值**：键名列表。

---

### 7.26 dm_enumIniSectionPwd

**函数签名**：
```c
const char* DM_CALL dm_enumIniSectionPwd(const char* file, const char* pwd);
```

**功能说明**：枚举加密 INI 文件中所有节名称。

**参数列表**：同 `dm_enumIniSection` + `pwd`

**返回值**：节名称列表。

---

## 模块 8：文字识别

文字识别模块提供 OCR（光学字符识别）功能，支持字库管理、文字识别和文字查找。

---

### 8.1 dm_setDict

**函数签名**：
```c
long DM_CALL dm_setDict(long index, const char* file);
```

**功能说明**：从文件加载字库到指定索引位置。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 字库索引（0-255） |
| file | const char* | 是 | 字库文件路径 |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.set_dict(0, "ocr_dict.txt")   # 加载字库到索引0
dm.set_dict(1, "font_dict.txt")  # 加载字库到索引1
```

---

### 8.2 dm_setDictMem

**函数签名**：
```c
long DM_CALL dm_setDictMem(long index, const char* data, long size);
```

**功能说明**：从内存数据加载字库到指定索引。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 字库索引 |
| data | const char* | 是 | 字库数据内存地址 |
| size | long | 是 | 数据大小 |

**返回值**：0=成功，非0=失败。

---

### 8.3 dm_setDictPwd

**函数签名**：
```c
long DM_CALL dm_setDictPwd(const char* pwd);
```

**功能说明**：设置字库文件的加解密密码。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| pwd | const char* | 是 | 密码字符串 |

**返回值**：0=成功。

---

### 8.4 dm_useDict

**函数签名**：
```c
long DM_CALL dm_useDict(long index);
```

**功能说明**：切换到指定索引的字库作为当前使用的字库。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 字库索引 |

**返回值**：0=成功。

---

### 8.5 dm_getDict

**函数签名**：
```c
const char* DM_CALL dm_getDict(long index, long font_index);
```

**功能说明**：获取指定字库中指定字体的字库信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 字库索引 |
| font_index | long | 是 | 字体索引（0=所有字体） |

**返回值**：字库信息字符串。

---

### 8.6 dm_getDictCount

**函数签名**：
```c
long DM_CALL dm_getDictCount(long index);
```

**功能说明**：获取指定字库中的字体数量。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 字库索引 |

**返回值**：字体数量。

---

### 8.7 dm_getDictInfo

**函数签名**：
```c
const char* DM_CALL dm_getDictInfo(const char* file, long index);
```

**功能说明**：获取字库文件的详细信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 字库文件路径 |
| index | long | 是 | 字体索引 |

**返回值**：字库信息字符串。

---

### 8.8 dm_getNowDict

**函数签名**：
```c
long DM_CALL dm_getNowDict();
```

**功能说明**：获取当前正在使用的字库索引。

**参数列表**：无

**返回值**：当前字库索引。

---

### 8.9 dm_addDict

**函数签名**：
```c
long DM_CALL dm_addDict(const char* dict_info);
```

**功能说明**：向当前字库中添加字体信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| dict_info | const char* | 是 | 字体信息字符串 |

**返回值**：0=成功，非0=失败。

---

### 8.10 dm_saveDict

**函数签名**：
```c
long DM_CALL dm_saveDict(long index, const char* file);
```

**功能说明**：将指定索引的字库保存到文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 字库索引 |
| file | const char* | 是 | 保存路径 |

**返回值**：0=成功，非0=失败。

---

### 8.11 dm_clearDict

**函数签名**：
```c
long DM_CALL dm_clearDict(long index);
```

**功能说明**：清除指定索引的字库内容。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 字库索引 |

**返回值**：0=成功。

---

### 8.12 dm_enableShareDict

**函数签名**：
```c
long DM_CALL dm_enableShareDict(long enable);
```

**功能说明**：启用或禁用字库共享。启用后，同一进程中的多个 dm 对象可以共享字库数据。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 8.13 dm_ocr

**函数签名**：
```c
const char* DM_CALL dm_ocr(long x1, long y1, long x2, long y2, const char* color_format, long sim);
```

**功能说明**：对指定区域进行文字识别（OCR），返回识别到的文本内容。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1 | long | 是 | 识别区域左上角 X |
| y1 | long | 是 | 识别区域左上角 Y |
| x2 | long | 是 | 识别区域右下角 X |
| y2 | long | 是 | 识别区域右下角 Y |
| color_format | const char* | 是 | 颜色格式，如 `"000000-000000"`，表示文字颜色-偏色 |
| sim | long | 是 | 相似度（0.0-1.0） |

**返回值**：识别到的文本内容字符串。

**调用示例**：

```python
# Python
# 先加载字库
dm.set_dict(0, "ocr_dict.txt")
dm.use_dict(0)

# 识别屏幕区域中的文字
text = dm.ocr(100, 100, 500, 200, "000000-000000", 0.9)
print(f"识别结果: {text}")

# 识别白色文字（带偏色）
text = dm.ocr(100, 100, 500, 200, "FFFFFF-101010", 0.8)
```

---

### 8.14 dm_ocrEx

**函数签名**：
```c
const char* DM_CALL dm_ocrEx(long x1, long y1, long x2, long y2, const char* color_format, long sim);
```

**功能说明**：增强版 OCR，返回更详细的识别结果（包括每个字符的位置信息）。

**参数列表**：同 `dm_ocr`

**返回值**：识别结果字符串，包含文本和位置信息。

---

### 8.15 dm_ocrExOne

**函数签名**：
```c
const char* DM_CALL dm_ocrExOne(long x1, long y1, long x2, long y2, const char* color_format, long sim);
```

**功能说明**：OCR 识别单行文字（优化了单行场景的识别精度）。

**参数列表**：同 `dm_ocr`

**返回值**：识别到的文本内容。

---

### 8.16 dm_ocrInFile

**函数签名**：
```c
long DM_CALL dm_ocrInFile(long x1, long y1, long x2, long y2, const char* color_format, long sim, const char* file);
```

**功能说明**：OCR 识别结果保存到文件。

**参数列表**：`file` 为保存路径

**返回值**：0=成功，非0=失败。

---

### 8.17 dm_findStr

**函数签名**：
```c
long DM_CALL dm_findStr(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
```

**功能说明**：在指定区域内查找指定文字，返回第一个匹配的位置。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 查找区域 |
| str | const char* | 是 | 要查找的文字内容 |
| color_format | const char* | 是 | 颜色格式 |
| sim | long | 是 | 相似度 |
| intX | long* | 输出 | 找到的 X 坐标 |
| intY | long* | 输出 | 找到的 Y 坐标 |

**返回值**：0=找到，1=未找到。

**调用示例**：

```python
# Python
x, y = ctypes.c_long(), ctypes.c_long()
ret = dm.find_str(0, 0, 1920, 1080, "确定", "000000-000000", 0.9, x, y)
if ret == 0:
    print(f"找到'确定'按钮在 ({x.value}, {y.value})")
    dm.move_to(x.value, y.value)
    dm.left_click()
```

---

### 8.18 dm_findStrE

**函数签名**：
```c
const char* DM_CALL dm_findStrE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
```

**功能说明**：查找文字，返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findStr`（无输出参数）

**返回值**：坐标字符串 `"x|y"`，未找到返回空字符串。

---

### 8.19 dm_findStrEx

**函数签名**：
```c
const char* DM_CALL dm_findStrEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
```

**功能说明**：查找文字，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findStr`

**返回值**：坐标字符串，多个位置以 `,` 分隔。

---

### 8.20 dm_findStrS

**函数签名**：
```c
long DM_CALL dm_findStrS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
```

**功能说明**：查找文字（智能模式），支持更灵活的匹配。

**参数列表**：同 `dm_findStr`

**返回值**：0=找到，1=未找到。

---

### 8.21 dm_findStrExS

**函数签名**：
```c
const char* DM_CALL dm_findStrExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
```

**功能说明**：查找文字（智能模式），返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findStr`

**返回值**：坐标字符串。

---

### 8.22 dm_findStrFast

**函数签名**：
```c
long DM_CALL dm_findStrFast(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
```

**功能说明**：快速查找文字（速度优先，精度略低）。

**参数列表**：同 `dm_findStr`

**返回值**：0=找到，1=未找到。

---

### 8.23 dm_findStrFastE

**函数签名**：
```c
const char* DM_CALL dm_findStrFastE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
```

**功能说明**：快速查找文字，返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findStrFast`

**返回值**：坐标字符串。

---

### 8.24 dm_findStrFastEx

**函数签名**：
```c
const char* DM_CALL dm_findStrFastEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
```

**功能说明**：快速查找文字，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findStrFast`

**返回值**：坐标字符串。

---

### 8.25 dm_findStrFastS

**函数签名**：
```c
long DM_CALL dm_findStrFastS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY);
```

**功能说明**：快速查找文字（智能模式），返回第一个匹配位置。

**参数列表**：同 `dm_findStr`

**返回值**：0=找到，1=未找到。

---

### 8.26 dm_findStrFastExS

**函数签名**：
```c
const char* DM_CALL dm_findStrFastExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim);
```

**功能说明**：快速查找文字（智能模式），返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findStrFast`

**返回值**：坐标字符串。

---

### 8.27 dm_findStrWithFont

**函数签名**：
```c
long DM_CALL dm_findStrWithFont(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size, long* intX, long* intY);
```

**功能说明**：根据指定字体信息查找文字。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 查找区域 |
| str | const char* | 是 | 要查找的文字 |
| color_format | const char* | 是 | 颜色格式 |
| sim | long | 是 | 相似度 |
| font_name | const char* | 是 | 字体名称，如 `"Arial"`、`"宋体"` |
| font_flag | long | 是 | 字体标志：0=普通，1=粗体，2=斜体 |
| font_size | long | 是 | 字体大小（像素或磅值） |
| intX/intY | long* | 输出 | 坐标 |

**返回值**：0=找到，1=未找到。

---

### 8.28 dm_findStrWithFontE

**函数签名**：
```c
const char* DM_CALL dm_findStrWithFontE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size);
```

**功能说明**：按字体查找文字，返回第一个匹配位置的坐标字符串。

**参数列表**：同 `dm_findStrWithFont`（无输出参数）

**返回值**：坐标字符串。

---

### 8.29 dm_findStrWithFontEx

**函数签名**：
```c
const char* DM_CALL dm_findStrWithFontEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size);
```

**功能说明**：按字体查找文字，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_findStrWithFont`

**返回值**：坐标字符串。

---

### 8.30 dm_getWords

**函数签名**：
```c
const char* DM_CALL dm_getWords(long x1, long y1, long x2, long y2, const char* color, const char* words);
```

**功能说明**：在指定区域中提取文字。用于手动制作字库时获取文字样本。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 区域坐标 |
| color | const char* | 是 | 颜色描述 |
| words | const char* | 是 | 已知的文字内容（用于辅助提取） |

**返回值**：提取的文字信息字符串。

---

### 8.31 dm_getWordsNoDict

**函数签名**：
```c
const char* DM_CALL dm_getWordsNoDict(long x1, long y1, long x2, long y2, const char* color);
```

**功能说明**：在无字库模式下提取文字（不依赖字库，通过颜色分割）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 区域坐标 |
| color | const char* | 是 | 颜色描述 |

**返回值**：提取的文字信息字符串。

---

### 8.32 dm_fetchWord

**函数签名**：
```c
long DM_CALL dm_fetchWord(long x1, long y1, long x2, long y2, const char* color, const char* words);
```

**功能说明**：提取文字样本并添加到当前字库中。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 区域坐标 |
| color | const char* | 是 | 颜色描述 |
| words | const char* | 是 | 对应的文字内容 |

**返回值**：0=成功，非0=失败。

---

### 8.33 dm_getResultCount

**函数签名**：
```c
long DM_CALL dm_getResultCount(const char* ret);
```

**功能说明**：获取查找结果字符串中的匹配数量。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| ret | const char* | 是 | 查找结果字符串（如 `findStrEx` 的返回值） |

**返回值**：匹配数量。

**调用示例**：

```python
# Python
results = dm.find_str_ex(0, 0, 1920, 1080, "确定", "000000-000000", 0.9)
count = dm.get_result_count(results)
print(f"找到 {count} 个'确定'")
```

---

### 8.34 dm_getResultPos

**函数签名**：
```c
long DM_CALL dm_getResultPos(const char* ret, long index, long* intX, long* intY);
```

**功能说明**：从查找结果字符串中获取指定索引的坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| ret | const char* | 是 | 查找结果字符串 |
| index | long | 是 | 索引（从0开始） |
| intX | long* | 输出 | X 坐标 |
| intY | long* | 输出 | Y 坐标 |

**返回值**：0=成功，1=索引无效。

---

### 8.35 dm_getWordResultCount

**函数签名**：
```c
long DM_CALL dm_getWordResultCount(const char* str);
```

**功能说明**：获取文字识别结果中的字符数量。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| str | const char* | 是 | OCR 识别结果字符串 |

**返回值**：字符数量。

---

### 8.36 dm_getWordResultPos

**函数签名**：
```c
long DM_CALL dm_getWordResultPos(const char* str, long index, long* intX, long* intY);
```

**功能说明**：从 OCR 识别结果中获取指定索引字符的坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| str | const char* | 是 | OCR 结果字符串 |
| index | long | 是 | 字符索引 |
| intX/intY | long* | 输出 | 坐标 |

**返回值**：0=成功。

---

### 8.37 dm_getWordResultStr

**函数签名**：
```c
const char* DM_CALL dm_getWordResultStr(const char* str, long index);
```

**功能说明**：从 OCR 识别结果中获取指定索引字符的识别结果。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| str | const char* | 是 | OCR 结果字符串 |
| index | long | 是 | 字符索引 |

**返回值**：指定索引处的字符识别结果。

---

### 8.38 dm_setColGapNoDict

**函数签名**：
```c
long DM_CALL dm_setColGapNoDict(long col_gap);
```

**功能说明**：设置无字库模式下的列间距（用于文字分割）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| col_gap | long | 是 | 列间距（像素） |

**返回值**：0=成功。

---

### 8.39 dm_setRowGapNoDict

**函数签名**：
```c
long DM_CALL dm_setRowGapNoDict(long row_gap);
```

**功能说明**：设置无字库模式下的行间距。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| row_gap | long | 是 | 行间距（像素） |

**返回值**：0=成功。

---

### 8.40 dm_setWordGapNoDict

**函数签名**：
```c
long DM_CALL dm_setWordGapNoDict(long word_gap);
```

**功能说明**：设置无字库模式下的字间距。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| word_gap | long | 是 | 字间距（像素） |

**返回值**：0=成功。

---

### 8.41 dm_setWordLineHeightNoDict

**函数签名**：
```c
long DM_CALL dm_setWordLineHeightNoDict(long line_height);
```

**功能说明**：设置无字库模式下的行高。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| line_height | long | 是 | 行高（像素） |

**返回值**：0=成功。

---

### 8.42 dm_setExactOcr

**函数签名**：
```c
long DM_CALL dm_setExactOcr(long exact_ocr);
```

**功能说明**：设置精确 OCR 模式。启用后识别精度更高，但速度会变慢。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| exact_ocr | long | 是 | 0=普通模式，1=精确模式 |

**返回值**：0=成功。

---

### 8.43 dm_setMinColGap

**函数签名**：
```c
long DM_CALL dm_setMinColGap(long min_col_gap);
```

**功能说明**：设置最小列间距。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| min_col_gap | long | 是 | 最小列间距（像素） |

**返回值**：0=成功。

---

### 8.44 dm_setMinRowGap

**函数签名**：
```c
long DM_CALL dm_setMinRowGap(long min_row_gap);
```

**功能说明**：设置最小行间距。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| min_row_gap | long | 是 | 最小行间距（像素） |

**返回值**：0=成功。

---

### 8.45 dm_setWordGap

**函数签名**：
```c
long DM_CALL dm_setWordGap(long word_gap);
```

**功能说明**：设置字间距（有字库模式）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| word_gap | long | 是 | 字间距（像素） |

**返回值**：0=成功。

---

### 8.46 dm_setWordLineHeight

**函数签名**：
```c
long DM_CALL dm_setWordLineHeight(long line_height);
```

**功能说明**：设置行高（有字库模式）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| line_height | long | 是 | 行高（像素） |

**返回值**：0=成功。

---

## 模块 9：系统操作

系统操作模块提供延时、程序运行、系统信息获取、屏幕设置等功能。

---

### 9.1 dm_beep

**函数签名**：
```c
long DM_CALL dm_beep(long f, long duration);
```

**功能说明**：发出蜂鸣声。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| f | long | 是 | 频率（Hz），如 1000=1kHz |
| duration | long | 是 | 持续时间（毫秒） |

**返回值**：0=成功。

---

### 9.2 dm_delay

**函数签名**：
```c
long DM_CALL dm_delay(long mis);
```

**功能说明**：延时等待指定的毫秒数。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| mis | long | 是 | 延时毫秒数 |

**返回值**：0=成功。

**调用示例**：

```python
# Python
dm.delay(1000)     # 等待 1 秒
dm.delay(500)      # 等待 0.5 秒
```

---

### 9.3 dm_delays

**函数签名**：
```c
long DM_CALL dm_delays(long mis_min, long mis_max);
```

**功能说明**：随机延时，在指定范围内随机等待一个时间。用于模拟更自然的操作行为。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| mis_min | long | 是 | 最小延时（毫秒） |
| mis_max | long | 是 | 最大延时（毫秒） |

**返回值**：0=成功。

**调用示例**：

```python
# Python
dm.delays(500, 1500)  # 随机等待 0.5-1.5 秒
```

---

### 9.4 dm_runApp

**函数签名**：
```c
long DM_CALL dm_runApp(const char* app_path, long mode);
```

**功能说明**：运行指定的应用程序或文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| app_path | const char* | 是 | 可执行文件路径或文档路径 |
| mode | long | 是 | 显示模式（见下表） |

**mode 取值**：

| 值 | 说明 |
|----|------|
| 0 | 正常显示 |
| 1 | 隐藏 |
| 2 | 最小化 |
| 3 | 最大化 |

**返回值**：进程 ID，0=失败。

**调用示例**：

```python
# Python
pid = dm.run_app("notepad.exe", 0)     # 正常打开记事本
pid = dm.run_app("calc.exe", 3)        # 最大化打开计算器
pid = dm.run_app("C:\\game.exe", 1)    # 隐藏运行游戏
```

---

### 9.5 dm_stop

**函数签名**：
```c
long DM_CALL dm_stop(long id);
```

**功能说明**：停止指定进程。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| id | long | 是 | 进程 ID（由 `dm_runApp` 返回） |

**返回值**：0=成功，非0=失败。

---

### 9.6 dm_play

**函数签名**：
```c
long DM_CALL dm_play(const char* media_file);
```

**功能说明**：播放指定的音频文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| media_file | const char* | 是 | 音频文件路径（支持 WAV、MP3 等格式） |

**返回值**：0=成功，非0=失败。

---

### 9.7 dm_exitOs

**函数签名**：
```c
long DM_CALL dm_exitOs(long type);
```

**功能说明**：执行系统关机、重启或注销操作。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | long | 是 | 操作类型：0=注销，1=关机，2=重启 |

**返回值**：0=成功（通常不会返回，因为系统会关闭）。

---

### 9.8 dm_setScreen

**函数签名**：
```c
long DM_CALL dm_setScreen(long width, long height, long depth);
```

**功能说明**：设置屏幕分辨率。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| width | long | 是 | 屏幕宽度（像素） |
| height | long | 是 | 屏幕高度（像素） |
| depth | long | 是 | 颜色深度（位），如 32 |

**返回值**：0=成功，非0=失败。

---

### 9.9 dm_getScreenWidth

**函数签名**：
```c
long DM_CALL dm_getScreenWidth();
```

**功能说明**：获取屏幕宽度（像素）。

**参数列表**：无

**返回值**：屏幕宽度像素数。

**调用示例**：

```python
# Python
w = dm.get_screen_width()
h = dm.get_screen_height()
print(f"当前屏幕分辨率: {w}x{h}")
```

---

### 9.10 dm_getScreenHeight

**函数签名**：
```c
long DM_CALL dm_getScreenHeight();
```

**功能说明**：获取屏幕高度（像素）。

**参数列表**：无

**返回值**：屏幕高度像素数。

---

### 9.11 dm_getScreenDepth

**函数签名**：
```c
long DM_CALL dm_getScreenDepth();
```

**功能说明**：获取屏幕颜色深度（色位数）。

**参数列表**：无

**返回值**：颜色深度，如 32（32位真彩色）。

---

### 9.12 dm_getDPI

**函数签名**：
```c
long DM_CALL dm_getDPI();
```

**功能说明**：获取系统 DPI（每英寸点数）设置。

**参数列表**：无

**返回值**：DPI 值，如 96、120、144 等。

---

### 9.13 dm_getTime

**函数签名**：
```c
long DM_CALL dm_getTime();
```

**功能说明**：获取当前系统时间戳（毫秒级）。

**参数列表**：无

**返回值**：从 1970-01-01 00:00:00 UTC 到现在的毫秒数。

**调用示例**：

```python
# Python
start = dm.get_time()
# 执行操作...
elapsed = dm.get_time() - start
print(f"操作耗时: {elapsed} 毫秒")
```

---

### 9.14 dm_getOsType

**函数签名**：
```c
long DM_CALL dm_getOsType();
```

**功能说明**：获取操作系统类型。

**参数列表**：无

**返回值**：系统类型代码：

| 返回值 | 说明 |
|--------|------|
| 0 | 未知 |
| 1 | Windows 2000 |
| 2 | Windows XP |
| 3 | Windows Server 2003 |
| 4 | Windows Vista |
| 5 | Windows 7 |
| 6 | Windows 8 |
| 7 | Windows 10 |
| 8 | Windows 11 |

---

### 9.15 dm_getOsBuildNumber

**函数签名**：
```c
long DM_CALL dm_getOsBuildNumber();
```

**功能说明**：获取操作系统构建号。

**参数列表**：无

**返回值**：系统构建号，如 22000（Windows 11）。

---

### 9.16 dm_is64Bit

**函数签名**：
```c
long DM_CALL dm_is64Bit();
```

**功能说明**：判断当前操作系统是否为 64 位。

**参数列表**：无

**返回值**：0=32位，1=64位。

---

### 9.17 dm_isSurrpotVt

**函数签名**：
```c
long DM_CALL dm_isSurrpotVt();
```

**功能说明**：判断 CPU 是否支持虚拟化技术（VT）。

**参数列表**：无

**返回值**：0=不支持，1=支持。

---

### 9.18 dm_getSystemInfo

**函数签名**：
```c
const char* DM_CALL dm_getSystemInfo(long type, long method);
```

**功能说明**：获取系统信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | long | 是 | 信息类型 |
| method | long | 是 | 获取方法 |

**返回值**：系统信息字符串。

---

### 9.19 dm_getDir

**函数签名**：
```c
const char* DM_CALL dm_getDir(long type);
```

**功能说明**：获取系统特殊目录路径。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | long | 是 | 目录类型：0=Windows 目录，1=System 目录，2=Temp 目录 |

**返回值**：目录路径字符串。

---

### 9.20 dm_getLocale

**函数签名**：
```c
long DM_CALL dm_getLocale();
```

**功能说明**：获取系统区域设置（Locale）。

**参数列表**：无

**返回值**：区域设置代码。

---

### 9.21 dm_getCpuType

**函数签名**：
```c
long DM_CALL dm_getCpuType();
```

**功能说明**：获取 CPU 类型。

**参数列表**：无

**返回值**：CPU 类型代码。

---

### 9.22 dm_getCpuUsage

**函数签名**：
```c
long DM_CALL dm_getCpuUsage();
```

**功能说明**：获取当前 CPU 使用率。

**参数列表**：无

**返回值**：CPU 使用率百分比（0-100）。

---

### 9.23 dm_getMemoryUsage

**函数签名**：
```c
long DM_CALL dm_getMemoryUsage();
```

**功能说明**：获取当前内存使用率。

**参数列表**：无

**返回值**：内存使用率百分比（0-100）。

---

### 9.24 dm_getDiskSerial

**函数签名**：
```c
const char* DM_CALL dm_getDiskSerial();
```

**功能说明**：获取系统盘的序列号。

**参数列表**：无

**返回值**：磁盘序列号字符串。

---

### 9.25 dm_getDiskModel

**函数签名**：
```c
const char* DM_CALL dm_getDiskModel();
```

**功能说明**：获取系统盘的型号信息。

**参数列表**：无

**返回值**：磁盘型号字符串。

---

### 9.26 dm_getDiskReversion

**函数签名**：
```c
const char* DM_CALL dm_getDiskReversion();
```

**功能说明**：获取系统盘的固件版本号。

**参数列表**：无

**返回值**：固件版本字符串。

---

### 9.27 dm_getMachineCode

**函数签名**：
```c
const char* DM_CALL dm_getMachineCode();
```

**功能说明**：获取本机机器码（基于硬件信息生成的唯一标识）。用于插件注册验证。

**参数列表**：无

**返回值**：机器码字符串。

**调用示例**：

```python
# Python
code = dm.get_machine_code()
print(f"本机机器码: {code}")
```

---

### 9.28 dm_getMachineCodeNoMac

**函数签名**：
```c
const char* DM_CALL dm_getMachineCodeNoMac();
```

**功能说明**：获取不包含 MAC 地址的机器码。

**参数列表**：无

**返回值**：机器码字符串。

---

### 9.29 dm_getNetTime

**函数签名**：
```c
const char* DM_CALL dm_getNetTime();
```

**功能说明**：从网络时间服务器获取当前时间。

**参数列表**：无

**返回值**：网络时间字符串，格式 `"YYYY-MM-DD HH:MM:SS"`。

---

### 9.30 dm_getNetTimeByIp

**函数签名**：
```c
const char* DM_CALL dm_getNetTimeByIp(const char* ip);
```

**功能说明**：从指定的 IP 地址获取网络时间。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| ip | const char* | 是 | 时间服务器 IP 地址 |

**返回值**：网络时间字符串。

---

### 9.31 dm_getNetTimeSafe

**函数签名**：
```c
const char* DM_CALL dm_getNetTimeSafe(long time_out);
```

**功能说明**：安全获取网络时间（带超时处理）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| time_out | long | 是 | 超时时间（毫秒） |

**返回值**：网络时间字符串，超时返回空字符串。

---

### 9.32 dm_getClipboard

**函数签名**：
```c
const char* DM_CALL dm_getClipboard();
```

**功能说明**：获取系统剪贴板中的文本内容。

**参数列表**：无

**返回值**：剪贴板文本内容，空字符串表示无文本或获取失败。

**调用示例**：

```python
# Python
text = dm.get_clipboard()
print(f"剪贴板内容: {text}")
```

---

### 9.33 dm_setClipboard

**函数签名**：
```c
long DM_CALL dm_setClipboard(const char* value);
```

**功能说明**：设置系统剪贴板文本内容。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| value | const char* | 是 | 要设置的文本内容 |

**返回值**：0=成功，非0=失败。

---

### 9.34 dm_getDisplayInfo

**函数签名**：
```c
long DM_CALL dm_getDisplayInfo(long type);
```

**功能说明**：获取显示器信息。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | long | 是 | 信息类型 |

**返回值**：显示器信息值。

---

### 9.35 dm_disableCloseDisplayAndSleep

**函数签名**：
```c
long DM_CALL dm_disableCloseDisplayAndSleep(long enable);
```

**功能说明**：禁用或启用关闭显示器和进入睡眠状态。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 1=禁用（阻止关闭显示器和睡眠），0=恢复 |

**返回值**：0=成功。

---

### 9.36 dm_disablePowerSave

**函数签名**：
```c
long DM_CALL dm_disablePowerSave(long enable);
```

**功能说明**：禁用或启用节能模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 1=禁用节能，0=启用节能 |

**返回值**：0=成功。

---

### 9.37 dm_disableScreenSave

**函数签名**：
```c
long DM_CALL dm_disableScreenSave(long enable);
```

**功能说明**：禁用或启用屏幕保护程序。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 1=禁用屏保，0=启用屏保 |

**返回值**：0=成功。

---

### 9.38 dm_disableFontSmooth

**函数签名**：
```c
long DM_CALL dm_disableFontSmooth();
```

**功能说明**：禁用字体平滑（抗锯齿）效果。用于提高文字识别精度。

**参数列表**：无

**返回值**：0=成功。

---

### 9.39 dm_enableFontSmooth

**函数签名**：
```c
long DM_CALL dm_enableFontSmooth();
```

**功能说明**：启用字体平滑（抗锯齿）效果。

**参数列表**：无

**返回值**：0=成功。

---

### 9.40 dm_checkFontSmooth

**函数签名**：
```c
long DM_CALL dm_checkFontSmooth();
```

**功能说明**：检查当前字体平滑是否启用。

**参数列表**：无

**返回值**：0=未启用，1=已启用。

---

### 9.41 dm_checkUAC

**函数签名**：
```c
long DM_CALL dm_checkUAC();
```

**功能说明**：检查 UAC（用户账户控制）是否启用。

**参数列表**：无

**返回值**：0=未启用，1=已启用。

---

### 9.42 dm_setUAC

**函数签名**：
```c
long DM_CALL dm_setUAC(long enable);
```

**功能说明**：启用或禁用 UAC（需要管理员权限）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功，非0=失败。

---

### 9.43 dm_setDisplayAcceler

**函数签名**：
```c
long DM_CALL dm_setDisplayAcceler(long flag);
```

**功能说明**：设置显示加速标志。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| flag | long | 是 | 加速标志值 |

**返回值**：0=成功。

---

### 9.44 dm_showTaskBarIcon

**函数签名**：
```c
long DM_CALL dm_showTaskBarIcon(intptr_t hwnd, long show);
```

**功能说明**：显示或隐藏指定窗口在任务栏上的图标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| show | long | 是 | 0=隐藏，1=显示 |

**返回值**：0=成功。

---

## 模块 10：杂项

杂项模块提供临界区、引用计数、命令行执行等辅助功能。

---

### 10.1 dm_enterCri

**函数签名**：
```c
long DM_CALL dm_enterCri();
```

**功能说明**：进入临界区（线程同步）。用于多线程环境下保护共享资源。

**参数列表**：无

**返回值**：0=成功。

---

### 10.2 dm_leaveCri

**函数签名**：
```c
long DM_CALL dm_leaveCri();
```

**功能说明**：离开临界区。与 `dm_enterCri` 配对使用。

**参数列表**：无

**返回值**：0=成功。

---

### 10.3 dm_initCri

**函数签名**：
```c
long DM_CALL dm_initCri();
```

**功能说明**：初始化临界区。

**参数列表**：无

**返回值**：0=成功。

---

### 10.4 dm_releaseRef

**函数签名**：
```c
long DM_CALL dm_releaseRef();
```

**功能说明**：释放当前对象的引用计数。

**参数列表**：无

**返回值**：0=成功。

---

### 10.5 dm_setExitThread

**函数签名**：
```c
long DM_CALL dm_setExitThread(long mode);
```

**功能说明**：设置线程退出模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| mode | long | 是 | 0=正常退出，1=强制退出 |

**返回值**：0=成功。

---

### 10.6 dm_executeCmd

**函数签名**：
```c
const char* DM_CALL dm_executeCmd(const char* cmd, long time_out);
```

**功能说明**：执行命令行命令，并返回命令输出结果。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| cmd | const char* | 是 | 要执行的命令行 |
| time_out | long | 是 | 超时时间（毫秒），0=无限等待 |

**返回值**：命令执行输出结果字符串。

**调用示例**：

```python
# Python
# 执行系统命令
result = dm.execute_cmd("ipconfig", 5000)
print(f"IP 配置: {result}")

result = dm.execute_cmd("dir C:\\", 5000)
print(f"C盘文件列表: {result}")
```

---

### 10.7 dm_activeInputMethod

**函数签名**：
```c
long DM_CALL dm_activeInputMethod(intptr_t hwnd, const char* input_method);
```

**功能说明**：激活指定窗口的输入法。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| input_method | const char* | 是 | 输入法名称 |

**返回值**：0=成功，非0=失败。

---

### 10.8 dm_checkInputMethod

**函数签名**：
```c
long DM_CALL dm_checkInputMethod(intptr_t hwnd, const char* input_method);
```

**功能说明**：检查指定窗口当前是否使用指定的输入法。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 窗口句柄 |
| input_method | const char* | 是 | 输入法名称 |

**返回值**：0=不是，1=是。

---

### 10.9 dm_findInputMethod

**函数签名**：
```c
long DM_CALL dm_findInputMethod(const char* input_method);
```

**功能说明**：查找系统中是否存在指定的输入法。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| input_method | const char* | 是 | 输入法名称 |

**返回值**：0=未找到，1=找到。

---

## 模块 11：汇编

汇编模块提供汇编代码的编写、编译、执行和反汇编功能。

---

### 11.1 dm_asmAdd

**函数签名**：
```c
long DM_CALL dm_asmAdd(const char* asm_ins);
```

**功能说明**：向汇编代码缓冲区中添加一条汇编指令。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| asm_ins | const char* | 是 | 汇编指令，如 `"push 0"`、`"mov eax, 1"` |

**返回值**：0=成功，非0=失败。

**调用示例**：

```python
# Python
dm.asm_clear()
dm.asm_add("push 0")
dm.asm_add("push 0")
dm.asm_add("call 0x401000")
dm.asm_add("add esp, 8")
dm.asm_add("ret")
```

---

### 11.2 dm_asmCall

**函数签名**：
```c
long DM_CALL dm_asmCall(intptr_t hwnd, long mode);
```

**功能说明**：在远程进程中执行已添加的汇编代码。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 目标窗口句柄 |
| mode | long | 是 | 执行模式：0=直接执行，1=创建新线程执行 |

**返回值**：0=成功，非0=失败。

---

### 11.3 dm_asmCallEx

**函数签名**：
```c
long DM_CALL dm_asmCallEx(intptr_t hwnd, long mode, long base_addr, long call_addr);
```

**功能说明**：在远程进程中指定地址处执行汇编代码。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 目标窗口句柄 |
| mode | long | 是 | 执行模式 |
| base_addr | long | 是 | 代码基址 |
| call_addr | long | 是 | 调用地址 |

**返回值**：0=成功，非0=失败。

---

### 11.4 dm_asmClear

**函数签名**：
```c
long DM_CALL dm_asmClear();
```

**功能说明**：清除汇编代码缓冲区中的所有指令。

**参数列表**：无

**返回值**：0=成功。

---

### 11.5 dm_asmSetTimeout

**函数签名**：
```c
long DM_CALL dm_asmSetTimeout(long timeout);
```

**功能说明**：设置汇编代码执行的超时时间。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| timeout | long | 是 | 超时时间（毫秒） |

**返回值**：0=成功。

---

### 11.6 dm_assemble

**函数签名**：
```c
const char* DM_CALL dm_assemble(long base_addr, long is_asm_code);
```

**功能说明**：将汇编代码缓冲区中的指令编译为机器码。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| base_addr | long | 是 | 基址（用于计算相对跳转） |
| is_asm_code | long | 是 | 0=返回十六进制机器码，1=返回 asm 代码 |

**返回值**：编译后的机器码字符串。

---

### 11.7 dm_disAssemble

**函数签名**：
```c
const char* DM_CALL dm_disAssemble(long asm_code, long base_addr, long is_asm_code);
```

**功能说明**：反汇编机器码为汇编指令。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| asm_code | long | 是 | 机器码数据 |
| base_addr | long | 是 | 基址 |
| is_asm_code | long | 是 | 0=十六进制字符串，1=asm 代码 |

**返回值**：反汇编结果字符串。

---

### 11.8 dm_setAsmHwndAsProcessId

**函数签名**：
```c
long DM_CALL dm_setAsmHwndAsProcessId(long enable);
```

**功能说明**：设置汇编操作时是否将 hwnd 参数作为进程 ID 处理。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=作为窗口句柄，1=作为进程 ID |

**返回值**：0=成功。

---

### 11.9 dm_setShowAsmErrorMsg

**函数签名**：
```c
long DM_CALL dm_setShowAsmErrorMsg(long show);
```

**功能说明**：设置是否显示汇编错误消息框。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| show | long | 是 | 0=不显示，1=显示 |

**返回值**：0=成功。

---

## 模块 12：AI 功能

AI 功能模块提供基于 AI 模型的找图和 YOLO 目标检测功能。

---

### 12.1 dm_loadAi

**函数签名**：
```c
long DM_CALL dm_loadAi(const char* file);
```

**功能说明**：加载 AI 模型文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | AI 模型文件路径 |

**返回值**：0=成功，非0=失败。

---

### 12.2 dm_loadAiMemory

**函数签名**：
```c
long DM_CALL dm_loadAiMemory(long data, long size);
```

**功能说明**：从内存加载 AI 模型数据。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| data | long | 是 | 模型数据内存地址 |
| size | long | 是 | 数据大小 |

**返回值**：0=成功，非0=失败。

---

### 12.3 dm_aiEnableFindPicWindow

**函数签名**：
```c
long DM_CALL dm_aiEnableFindPicWindow(long enable);
```

**功能说明**：启用或禁用 AI 找图窗口模式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=禁用，1=启用 |

**返回值**：0=成功。

---

### 12.4 dm_aiFindPic

**函数签名**：
```c
long DM_CALL dm_aiFindPic(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir, long* intX, long* intY);
```

**功能说明**：使用 AI 模型查找图片，返回第一个匹配位置的坐标。

**参数列表**：同 `dm_findPic`（参数格式相同）

**返回值**：0=找到，1=未找到。

---

### 12.5 dm_aiFindPicEx

**函数签名**：
```c
const char* DM_CALL dm_aiFindPicEx(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir);
```

**功能说明**：AI 找图，返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_aiFindPic`（无输出参数）

**返回值**：坐标字符串。

---

### 12.6 dm_aiFindPicMem

**函数签名**：
```c
long DM_CALL dm_aiFindPicMem(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir, long* intX, long* intY);
```

**功能说明**：AI 找图（内存模式），从内存图片数据中查找。

**参数列表**：同 `dm_findPicMem`

**返回值**：0=找到，1=未找到。

---

### 12.7 dm_aiFindPicMemEx

**函数签名**：
```c
const char* DM_CALL dm_aiFindPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir);
```

**功能说明**：AI 找图（内存模式），返回所有匹配位置的坐标字符串。

**参数列表**：同 `dm_aiFindPicMem`

**返回值**：坐标字符串。

---

### 12.8 dm_aiYoloDetectObjects

**函数签名**：
```c
const char* DM_CALL dm_aiYoloDetectObjects(long x1, long y1, long x2, long y2, float prob, float iou);
```

**功能说明**：使用 YOLO 模型进行目标检测，识别指定区域内的所有目标对象。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 检测区域 |
| prob | float | 是 | 置信度阈值（0.0-1.0），低于此值的检测结果将被过滤 |
| iou | float | 是 | IOU 阈值（0.0-1.0），用于 NMS 去重 |

**返回值**：检测结果字符串，包含所有检测到的目标信息。

**调用示例**：

```python
# Python
# 设置 YOLO 模型
dm.ai_yolo_set_model("yolov5s.onnx", "yolov5")
# 进行目标检测
objects = dm.ai_yolo_detect_objects(0, 0, 1920, 1080, 0.5, 0.45)
print(f"检测结果: {objects}")
```

---

### 12.9 dm_aiYoloDetectObjectsToDataBmp

**函数签名**：
```c
long DM_CALL dm_aiYoloDetectObjectsToDataBmp(long x1, long y1, long x2, long y2, float prob, float iou, long data, long size, long mode);
```

**功能说明**：YOLO 检测并将结果绘制到内存 BMP 数据中。

**参数列表**：多出 `data`（内存地址）、`size`（数据大小）、`mode`（绘制模式）参数

**返回值**：0=成功，非0=失败。

---

### 12.10 dm_aiYoloDetectObjectsToFile

**函数签名**：
```c
long DM_CALL dm_aiYoloDetectObjectsToFile(long x1, long y1, long x2, long y2, float prob, float iou, const char* file, long mode);
```

**功能说明**：YOLO 检测并将结果绘制到图片文件。

**参数列表**：多出 `file`（保存路径）和 `mode`（绘制模式）参数

**返回值**：0=成功，非0=失败。

---

### 12.11 dm_aiYoloSetModel

**函数签名**：
```c
long DM_CALL dm_aiYoloSetModel(const char* model_file, const char* model_type);
```

**功能说明**：设置 YOLO 模型文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| model_file | const char* | 是 | 模型文件路径（如 `.onnx` 文件） |
| model_type | const char* | 是 | 模型类型，如 `"yolov5"`、`"yolov8"` |

**返回值**：0=成功，非0=失败。

---

### 12.12 dm_aiYoloSetModelMemory

**函数签名**：
```c
long DM_CALL dm_aiYoloSetModelMemory(long data, long size, const char* model_type);
```

**功能说明**：从内存加载 YOLO 模型。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| data | long | 是 | 模型数据内存地址 |
| size | long | 是 | 数据大小 |
| model_type | const char* | 是 | 模型类型 |

**返回值**：0=成功，非0=失败。

---

### 12.13 dm_aiYoloFreeModel

**函数签名**：
```c
long DM_CALL dm_aiYoloFreeModel();
```

**功能说明**：释放当前加载的 YOLO 模型资源。

**参数列表**：无

**返回值**：0=成功。

---

### 12.14 dm_aiYoloUseModel

**函数签名**：
```c
long DM_CALL dm_aiYoloUseModel(long index);
```

**功能说明**：切换到指定索引的 YOLO 模型（如果加载了多个模型）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| index | long | 是 | 模型索引 |

**返回值**：0=成功。

---

### 12.15 dm_aiYoloSetVersion

**函数签名**：
```c
long DM_CALL dm_aiYoloSetVersion(const char* version);
```

**功能说明**：设置 YOLO 版本。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| version | const char* | 是 | 版本号，如 `"v5"`、`"v8"` |

**返回值**：0=成功。

---

### 12.16 dm_aiYoloObjectsToString

**函数签名**：
```c
const char* DM_CALL dm_aiYoloObjectsToString(const char* objects);
```

**功能说明**：将 YOLO 检测结果对象转换为可读的字符串格式。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| objects | const char* | 是 | YOLO 检测结果 |

**返回值**：格式化后的字符串。

---

### 12.17 dm_aiYoloSortsObjects

**函数签名**：
```c
const char* DM_CALL dm_aiYoloSortsObjects(const char* objects, long sort_type);
```

**功能说明**：对 YOLO 检测结果进行排序。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| objects | const char* | 是 | 检测结果 |
| sort_type | long | 是 | 排序方式 |

**返回值**：排序后的结果字符串。

---

## 模块 13：Foobar 界面控件

Foobar 是窗口上的浮动图形控件系统，支持在窗口上绘制文字、形状、图片等。

---

### 13.1 dm_createFoobarRect

**函数签名**：
```c
long DM_CALL dm_createFoobarRect(intptr_t hwnd, long x, long y, long w, long h);
```

**功能说明**：在指定窗口上创建一个矩形 Foobar 控件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 父窗口句柄 |
| x | long | 是 | 控件左上角 X |
| y | long | 是 | 控件左上角 Y |
| w | long | 是 | 控件宽度 |
| h | long | 是 | 控件高度 |

**返回值**：Foobar 句柄，0=失败。

**调用示例**：

```python
# Python
# 在窗口上创建一个信息显示区域
fb = dm.create_foobar_rect(hwnd, 10, 10, 300, 200)
dm.foobar_set_font(fb, "宋体", 12, 0)
dm.foobar_draw_text(fb, 0, 0, 300, 30, "Hello, World!", 0x00FF00, 0)
```

---

### 13.2 dm_createFoobarEllipse

**函数签名**：
```c
long DM_CALL dm_createFoobarEllipse(intptr_t hwnd, long x, long y, long w, long h);
```

**功能说明**：在窗口上创建一个椭圆形状的 Foobar 控件。

**参数列表**：同 `dm_createFoobarRect`

**返回值**：Foobar 句柄，0=失败。

---

### 13.3 dm_createFoobarRoundRect

**函数签名**：
```c
long DM_CALL dm_createFoobarRoundRect(intptr_t hwnd, long x, long y, long w, long h, long rw, long rh);
```

**功能说明**：在窗口上创建一个圆角矩形 Foobar 控件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 父窗口句柄 |
| x-y | long | 是 | 位置和大小 |
| rw | long | 是 | 圆角宽度 |
| rh | long | 是 | 圆角高度 |

**返回值**：Foobar 句柄，0=失败。

---

### 13.4 dm_createFoobarCustom

**函数签名**：
```c
long DM_CALL dm_createFoobarCustom(intptr_t hwnd, long x, long y, long w, long h, const char* pic);
```

**功能说明**：使用图片作为遮罩创建自定义形状的 Foobar 控件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | 父窗口句柄 |
| x-y | long | 是 | 位置和大小 |
| pic | const char* | 是 | 遮罩图片路径 |

**返回值**：Foobar 句柄，0=失败。

---

### 13.5 dm_foobarClose

**函数签名**：
```c
long DM_CALL dm_foobarClose(intptr_t hwnd);
```

**功能说明**：关闭并销毁 Foobar 控件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 控件句柄 |

**返回值**：0=成功。

---

### 13.6 dm_foobarLock

**函数签名**：
```c
long DM_CALL dm_foobarLock(intptr_t hwnd);
```

**功能说明**：锁定 Foobar 控件，防止内容被刷新。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 控件句柄 |

**返回值**：0=成功。

---

### 13.7 dm_foobarUnlock

**函数签名**：
```c
long DM_CALL dm_foobarUnlock(intptr_t hwnd);
```

**功能说明**：解锁 Foobar 控件，恢复刷新。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 控件句柄 |

**返回值**：0=成功。

---

### 13.8 dm_foobarUpdate

**函数签名**：
```c
long DM_CALL dm_foobarUpdate(intptr_t hwnd);
```

**功能说明**：立即更新 Foobar 控件的显示内容。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 控件句柄 |

**返回值**：0=成功。

---

### 13.9 dm_foobarSetFont

**函数签名**：
```c
long DM_CALL dm_foobarSetFont(intptr_t hwnd, const char* font_name, long size, long flag);
```

**功能说明**：设置 Foobar 控件的字体。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| font_name | const char* | 是 | 字体名称，如 `"宋体"`、`"Arial"` |
| size | long | 是 | 字体大小（像素） |
| flag | long | 是 | 0=普通，1=粗体，2=斜体，3=粗体+斜体 |

**返回值**：0=成功。

---

### 13.10 dm_foobarSetSave

**函数签名**：
```c
long DM_CALL dm_foobarSetSave(intptr_t hwnd, const char* file, long enable);
```

**功能说明**：设置 Foobar 控件内容是否自动保存到文件。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| file | const char* | 是 | 保存文件路径 |
| enable | long | 是 | 0=不保存，1=保存 |

**返回值**：0=成功。

---

### 13.11 dm_foobarSetTrans

**函数签名**：
```c
long DM_CALL dm_foobarSetTrans(intptr_t hwnd, long trans, long color);
```

**功能说明**：设置 Foobar 控件的透明度和透明色。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| trans | long | 是 | 透明度（0-255），0=完全透明，255=不透明 |
| color | long | 是 | 透明色（RGB 颜色值），该颜色将被视为透明 |

**返回值**：0=成功。

---

### 13.12 dm_foobarDrawText

**函数签名**：
```c
long DM_CALL dm_foobarDrawText(intptr_t hwnd, long x, long y, long w, long h, const char* text, long color, long align);
```

**功能说明**：在 Foobar 控件上绘制文本。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| x-y | long | 是 | 文本区域位置 |
| w-h | long | 是 | 文本区域大小 |
| text | const char* | 是 | 要显示的文本 |
| color | long | 是 | 文本颜色（RGB 格式十六进制，如 0xFF0000=红色） |
| align | long | 是 | 对齐方式：0=左对齐，1=居中，2=右对齐 |

**返回值**：0=成功。

---

### 13.13 dm_foobarPrintText

**函数签名**：
```c
long DM_CALL dm_foobarPrintText(intptr_t hwnd, const char* text, long color);
```

**功能说明**：在 Foobar 控件上追加打印文本（自动换行）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| text | const char* | 是 | 要打印的文本 |
| color | long | 是 | 文本颜色 |

**返回值**：0=成功。

---

### 13.14 dm_foobarDrawRect

**函数签名**：
```c
long DM_CALL dm_foobarDrawRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
```

**功能说明**：在 Foobar 控件上绘制矩形边框。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| x1-y2 | long | 是 | 矩形坐标 |
| color | long | 是 | 边框颜色 |
| style | long | 是 | 边框样式：0=实线，1=虚线 |

**返回值**：0=成功。

---

### 13.15 dm_foobarDrawLine

**函数签名**：
```c
long DM_CALL dm_foobarDrawLine(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style);
```

**功能说明**：在 Foobar 控件上绘制直线。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| x1-y2 | long | 是 | 线段起点和终点坐标 |
| color | long | 是 | 线条颜色 |
| style | long | 是 | 线条样式：0=实线，1=虚线 |

**返回值**：0=成功。

---

### 13.16 dm_foobarFillRect

**函数签名**：
```c
long DM_CALL dm_foobarFillRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color);
```

**功能说明**：在 Foobar 控件上绘制填充矩形。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| x1-y2 | long | 是 | 矩形坐标 |
| color | long | 是 | 填充颜色 |

**返回值**：0=成功。

---

### 13.17 dm_foobarDrawPic

**函数签名**：
```c
long DM_CALL dm_foobarDrawPic(intptr_t hwnd, long x, long y, const char* pic);
```

**功能说明**：在 Foobar 控件上绘制图片。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| x | long | 是 | 图片左上角 X |
| y | long | 是 | 图片左上角 Y |
| pic | const char* | 是 | 图片文件路径 |

**返回值**：0=成功。

---

### 13.18 dm_foobarClearText

**函数签名**：
```c
long DM_CALL dm_foobarClearText(intptr_t hwnd);
```

**功能说明**：清除 Foobar 控件上的所有文本内容。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |

**返回值**：0=成功。

---

### 13.19 dm_foobarTextRect

**函数签名**：
```c
long DM_CALL dm_foobarTextRect(intptr_t hwnd, long x, long y, long w, long h);
```

**功能说明**：设置 Foobar 控件的文本显示区域。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| x-y | long | 是 | 文本区域位置和大小 |

**返回值**：0=成功。

---

### 13.20 dm_foobarTextLineGap

**函数签名**：
```c
long DM_CALL dm_foobarTextLineGap(intptr_t hwnd, long line_gap);
```

**功能说明**：设置 Foobar 控件的文本行间距。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| line_gap | long | 是 | 行间距（像素） |

**返回值**：0=成功。

---

### 13.21 dm_foobarTextPrintDir

**函数签名**：
```c
long DM_CALL dm_foobarTextPrintDir(intptr_t hwnd, long dir);
```

**功能说明**：设置 Foobar 控件的文本打印方向。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| dir | long | 是 | 0=从左到右，1=从右到左 |

**返回值**：0=成功。

---

### 13.22 dm_foobarStartGif

**函数签名**：
```c
long DM_CALL dm_foobarStartGif(intptr_t hwnd, const char* pic, long x, long y);
```

**功能说明**：在 Foobar 控件上播放 GIF 动画。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |
| pic | const char* | 是 | GIF 文件路径 |
| x | long | 是 | 显示位置 X |
| y | long | 是 | 显示位置 Y |

**返回值**：0=成功。

---

### 13.23 dm_foobarStopGif

**函数签名**：
```c
long DM_CALL dm_foobarStopGif(intptr_t hwnd);
```

**功能说明**：停止 Foobar 控件上的 GIF 动画播放。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| hwnd | intptr_t | 是 | Foobar 句柄 |

**返回值**：0=成功。

---

## 模块 14：答题

答题模块提供与答题服务端交互的截图、发送、获取答案等功能。

---

### 14.1 dm_faqCapture

**函数签名**：
```c
long DM_CALL dm_faqCapture(long x1, long y1, long x2, long y2, long quality, long delay, long time);
```

**功能说明**：截取答题区域图片，用于发送给答题服务器识别。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| x1-y2 | long | 是 | 截图区域 |
| quality | long | 是 | 图片质量（1-100） |
| delay | long | 是 | 截图延迟（毫秒） |
| time | long | 是 | 超时时间 |

**返回值**：答题句柄，0=失败。

---

### 14.2 dm_faqCaptureFromFile

**函数签名**：
```c
long DM_CALL dm_faqCaptureFromFile(const char* file, long quality, long delay, long time);
```

**功能说明**：从图片文件加载答题图片。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| file | const char* | 是 | 图片文件路径 |
| quality-long | long | 是 | 质量和延迟参数 |

**返回值**：答题句柄。

---

### 14.3 dm_faqCaptureString

**函数签名**：
```c
const char* DM_CALL dm_faqCaptureString(long x1, long y1, long x2, long y2, long quality, long delay, long time);
```

**功能说明**：截取答题区域并以字符串形式返回图片数据。

**参数列表**：同 `dm_faqCapture`

**返回值**：图片数据字符串。

---

### 14.4 dm_faqGetSize

**函数签名**：
```c
long DM_CALL dm_faqGetSize(long handle);
```

**功能说明**：获取答题句柄对应的图片数据大小。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| handle | long | 是 | 答题句柄 |

**返回值**：数据大小（字节）。

---

### 14.5 dm_faqPost

**函数签名**：
```c
long DM_CALL dm_faqPost(const char* server, long handle, long request_type, long time_out);
```

**功能说明**：将答题图片发送到指定的答题服务器。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| server | const char* | 是 | 服务器地址 |
| handle | long | 是 | 答题句柄 |
| request_type | long | 是 | 请求类型 |
| time_out | long | 是 | 超时时间（毫秒） |

**返回值**：0=成功，非0=失败。

---

### 14.6 dm_faqSend

**函数签名**：
```c
long DM_CALL dm_faqSend(long handle, long time_out);
```

**功能说明**：发送答题请求（使用默认服务器）。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| handle | long | 是 | 答题句柄 |
| time_out | long | 是 | 超时时间 |

**返回值**：0=成功，非0=失败。

---

### 14.7 dm_faqFetch

**函数签名**：
```c
long DM_CALL dm_faqFetch(long time_out);
```

**功能说明**：从答题服务器获取答案。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| time_out | long | 是 | 超时时间（毫秒） |

**返回值**：答案数据句柄，0=获取失败。

---

### 14.8 dm_faqCancel

**函数签名**：
```c
long DM_CALL dm_faqCancel();
```

**功能说明**：取消当前答题请求。

**参数列表**：无

**返回值**：0=成功。

---

### 14.9 dm_faqIsPosted

**函数签名**：
```c
long DM_CALL dm_faqIsPosted();
```

**功能说明**：检查答题请求是否已发送。

**参数列表**：无

**返回值**：0=未发送，1=已发送。

---

## 模块 15：算法

算法模块提供坐标处理工具函数。

---

### 15.1 dm_excludePos

**函数签名**：
```c
const char* DM_CALL dm_excludePos(const char* all_pos, long type, long x1, long y1, long x2, long y2);
```

**功能说明**：从坐标列表中排除指定区域内的坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| all_pos | const char* | 是 | 坐标列表字符串，格式 `"x1|y1,x2|y2,..."` |
| type | long | 是 | 0=排除矩形区域内的坐标，1=排除矩形区域外的坐标 |
| x1-y2 | long | 是 | 区域坐标 |

**返回值**：过滤后的坐标列表字符串。

---

### 15.2 dm_findNearestPos

**函数签名**：
```c
const char* DM_CALL dm_findNearestPos(const char* all_pos, long type, long x, long y);
```

**功能说明**：从坐标列表中找出距离指定点最近的坐标。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| all_pos | const char* | 是 | 坐标列表 |
| type | long | 是 | 0=返回最近的一个坐标，1=返回按距离排序的坐标列表 |
| x | long | 是 | 参考点 X |
| y | long | 是 | 参考点 Y |

**返回值**：最近的坐标或排序后的坐标列表。

---

### 15.3 dm_sortPosDistance

**函数签名**：
```c
const char* DM_CALL dm_sortPosDistance(const char* all_pos, long type, long x, long y);
```

**功能说明**：对坐标列表按距离指定点的远近进行排序。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| all_pos | const char* | 是 | 坐标列表 |
| type | long | 是 | 排序方式 |
| x | long | 是 | 参考点 X |
| y | long | 是 | 参考点 Y |

**返回值**：排序后的坐标列表字符串。

---

## 模块 16：防护盾

防护盾模块提供驱动级保护功能，用于保护插件进程。

---

### 16.1 dm_dmGuard

**函数签名**：
```c
long DM_CALL dm_dmGuard(long enable, const char* type);
```

**功能说明**：启用或禁用防护盾。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| enable | long | 是 | 0=卸载，1=加载 |
| type | const char* | 是 | 防护类型，如 `"np"` 等 |

**返回值**：0=成功，非0=失败。

---

### 16.2 dm_dmGuardExtract

**函数签名**：
```c
long DM_CALL dm_dmGuardExtract(const char* type, const char* file);
```

**功能说明**：提取防护盾驱动文件到指定路径。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | const char* | 是 | 防护类型 |
| file | const char* | 是 | 提取到的文件路径 |

**返回值**：0=成功，非0=失败。

---

### 16.3 dm_dmGuardLoadCustom

**函数签名**：
```c
long DM_CALL dm_dmGuardLoadCustom(const char* type, const char* data);
```

**功能说明**：加载自定义的防护盾数据。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| type | const char* | 是 | 防护类型 |
| data | const char* | 是 | 自定义数据 |

**返回值**：0=成功，非0=失败。

---

### 16.4 dm_dmGuardParams

**函数签名**：
```c
const char* DM_CALL dm_dmGuardParams(const char* cmd, const char* param);
```

**功能说明**：设置防护盾参数。

**参数列表**：

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| cmd | const char* | 是 | 命令 |
| param | const char* | 是 | 参数值 |

**返回值**：执行结果字符串。

---

### 16.5 dm_unLoadDriver

**函数签名**：
```c
long DM_CALL dm_unLoadDriver();
```

**功能说明**：卸载防护盾驱动。

**参数列表**：无

**返回值**：0=成功，非0=失败。

---

## 附录 A：错误码表

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

## 附录 B：虚拟键码表

| 键名 | 键码 | 说明 |
|------|------|------|
| VK_LBUTTON | 0x01 | 鼠标左键 |
| VK_RBUTTON | 0x02 | 鼠标右键 |
| VK_MBUTTON | 0x04 | 鼠标中键 |
| VK_BACK | 0x08 | Backspace |
| VK_TAB | 0x09 | Tab |
| VK_CLEAR | 0x0C | Clear |
| VK_RETURN | 0x0D | Enter |
| VK_SHIFT | 0x10 | Shift |
| VK_CONTROL | 0x11 | Ctrl |
| VK_MENU | 0x12 | Alt |
| VK_PAUSE | 0x13 | Pause |
| VK_CAPITAL | 0x14 | Caps Lock |
| VK_ESCAPE | 0x1B | ESC |
| VK_SPACE | 0x20 | 空格 |
| VK_PRIOR | 0x21 | Page Up |
| VK_NEXT | 0x22 | Page Down |
| VK_END | 0x23 | End |
| VK_HOME | 0x24 | Home |
| VK_LEFT | 0x25 | ← |
| VK_UP | 0x26 | ↑ |
| VK_RIGHT | 0x27 | → |
| VK_DOWN | 0x28 | ↓ |
| VK_SNAPSHOT | 0x2C | Print Screen |
| VK_INSERT | 0x2D | Insert |
| VK_DELETE | 0x2E | Delete |
| 0-9 | 0x30-0x39 | 数字键 |
| A-Z | 0x41-0x5A | 字母键 |
| VK_NUMPAD0-9 | 0x60-0x69 | 小键盘数字 |
| VK_F1-F12 | 0x70-0x7B | 功能键 |
| VK_NUMLOCK | 0x90 | Num Lock |
| VK_SCROLL | 0x91 | Scroll Lock |
| VK_LSHIFT | 0xA0 | 左 Shift |
| VK_RSHIFT | 0xA1 | 右 Shift |
| VK_LCONTROL | 0xA2 | 左 Ctrl |
| VK_RCONTROL | 0xA3 | 右 Ctrl |
| VK_LMENU | 0xA4 | 左 Alt |
| VK_RMENU | 0xA5 | 右 Alt |

---

## 附录 C：跨语言调用示例

### Python 调用

```python
from dm_hook import DmHook

# 加载 DLL
dm = DmHook()

# 版本信息
print(f"版本: {dm.ver()}")
print(f"对象 ID: {dm.get_id()}")

# 系统信息
print(f"屏幕: {dm.get_screen_width()}x{dm.get_screen_height()}")
print(f"机器码: {dm.get_machine_code()}")

# 窗口操作
hwnd = dm.get_foreground_window()
print(f"前台窗口: {dm.get_window_title(hwnd)}")

# 键鼠
dm.move_to(500, 300)
dm.left_click()

# 截图
dm.capture(0, 0, 800, 600, "test.bmp")

# 颜色
color = dm.get_color(100, 100)
print(f"颜色: {color}")

# 延时
dm.delay(1000)
```

### C++ 调用

```cpp
#include <windows.h>
#include <iostream>

typedef const char* (*DM_VER)();
typedef long (*DM_GETID)();
typedef long (*DM_GETSCREENWIDTH)();
typedef long (*DM_GETSCREENHEIGHT)();

int main() {
    HMODULE hDll = LoadLibraryA("dm_hook.dll");
    if (!hDll) { return 1; }
    
    auto ver = (DM_VER)GetProcAddress(hDll, "dm_ver");
    auto getID = (DM_GETID)GetProcAddress(hDll, "dm_getID");
    auto getSW = (DM_GETSCREENWIDTH)GetProcAddress(hDll, "dm_getScreenWidth");
    auto getSH = (DM_GETSCREENHEIGHT)GetProcAddress(hDll, "dm_getScreenHeight");
    
    std::cout << "版本: " << ver() << std::endl;
    std::cout << "ID: " << getID() << std::endl;
    std::cout << "屏幕: " << getSW() << "x" << getSH() << std::endl;
    
    FreeLibrary(hDll);
    return 0;
}
```

### AutoHotkey 调用

```autohotkey
hModule := DllCall("LoadLibrary", "Str", "dm_hook.dll", "Ptr")

; 获取版本
ver := DllCall("dm_hook.dll\dm_ver", "AStr")
MsgBox, 版本: %ver%

; 获取屏幕尺寸
w := DllCall("dm_hook.dll\dm_getScreenWidth")
h := DllCall("dm_hook.dll\dm_getScreenHeight")
MsgBox, 屏幕: %w%x%h%

; 获取前台窗口
hwnd := DllCall("dm_hook.dll\dm_getForegroundWindow")
title := DllCall("dm_hook.dll\dm_getWindowTitle", "Ptr", hwnd, "AStr")
MsgBox, 前台窗口: %title%

; 鼠标操作
DllCall("dm_hook.dll\dm_moveTo", "Int", 500, "Int", 300)
DllCall("dm_hook.dll\dm_leftClick")

; 释放
DllCall("FreeLibrary", "Ptr", hModule)
```

---

> **文档版本**: 1.1  
> **最后更新**: 2026-08-13  
> **项目地址**: [https://github.com/Pkk233/dm_hook](https://github.com/Pkk233/dm_hook)  
> **总函数数**: 约 260+ 个（涵盖 16 个模块）