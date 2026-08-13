# dm_hook 函数参考文档

> 大漠插件 (dm_hook.dll) 全部导出函数，按模块归类。
> 调用约定：`extern "C" __stdcall`；返回 `const char*` 的函数使用线程安全静态缓冲区。

共 **417** 个函数，**16** 个模块。

## 目录
1. [1. 基本设置](#1-基本设置) — 13 个
2. [2. 窗口](#2-窗口) — 40 个
3. [3. 键鼠](#3-键鼠) — 34 个
4. [4. 图色](#4-图色) — 57 个
5. [5. 后台设置](#5-后台设置) — 28 个
6. [6. 内存](#6-内存) — 54 个
7. [7. 文件](#7-文件) — 26 个
8. [8. 文字识别](#8-文字识别) — 46 个
9. [9. 系统](#9-系统) — 44 个
10. [10. 杂项](#10-杂项) — 9 个
11. [11. 汇编](#11-汇编) — 9 个
12. [12. AI](#12-AI) — 17 个
13. [13. Foobar (界面控件)](#13-Foobar (界面控件)) — 23 个
14. [14. 答题](#14-答题) — 9 个
15. [15. 算法](#15-算法) — 3 个
16. [16. 防护盾](#16-防护盾) — 5 个

## 1. 基本设置

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_reg` | `long` | `const char* reg_code, const char* ver_info` |
| `dm_regEx` | `long` | `const char* reg_code, const char* ver_info, const char* ip` |
| `dm_regNoMac` | `long` | `const char* reg_code, const char* ver_info` |
| `dm_regExNoMac` | `long` | `const char* reg_code, const char* ver_info, const char* ip` |
| `dm_ver` | `const char*` | `` |
| `dm_getID` | `long` | `` |
| `dm_getDmCount` | `long` | `` |
| `dm_getLastError` | `long` | `` |
| `dm_getPath` | `const char*` | `` |
| `dm_setPath` | `long` | `const char* path` |
| `dm_getBasePath` | `const char*` | `` |
| `dm_setShowErrorMsg` | `long` | `long show` |
| `dm_enablePicCache` | `long` | `long enable` |

## 2. 窗口

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_findWindow` | `long` | `const char* cls, const char* title` |
| `dm_findWindowEx` | `long` | `intptr_t parent, long cls, const char* title` |
| `dm_findWindowByProcess` | `long` | `const char* process_name, const char* cls, const char* title` |
| `dm_findWindowByProcessId` | `long` | `long process_id, const char* cls, const char* title` |
| `dm_findWindowSuper` | `long` | `const char* cls, const char* title, long flag` |
| `dm_enumWindow` | `long` | `intptr_t parent, const char* title, long filter` |
| `dm_enumWindowByProcess` | `long` | `const char* process_name, const char* title, long filter` |
| `dm_enumWindowByProcessId` | `long` | `long pid, const char* title, long filter` |
| `dm_enumWindowSuper` | `long` | `const char* spec1, long flag1, const char* spec2, long flag2, long filter` |
| `dm_enumProcess` | `long` | `const char* name` |
| `dm_getWindow` | `long` | `intptr_t hwnd, long flag` |
| `dm_getForegroundWindow` | `long` | `` |
| `dm_getForegroundFocus` | `long` | `` |
| `dm_getMousePointWindow` | `long` | `` |
| `dm_getPointWindow` | `long` | `long x, long y` |
| `dm_getSpecialWindow` | `long` | `long flag` |
| `dm_getWindowClass` | `const char*` | `intptr_t hwnd` |
| `dm_getWindowProcessId` | `long` | `intptr_t hwnd` |
| `dm_getWindowProcessPath` | `const char*` | `intptr_t hwnd` |
| `dm_getWindowThreadId` | `long` | `intptr_t hwnd` |
| `dm_getWindowTitle` | `const char*` | `intptr_t hwnd` |
| `dm_getWindowRect` | `long` | `intptr_t hwnd, long* x1, long* y1, long* x2, long* y2` |
| `dm_getClientRect` | `long` | `intptr_t hwnd, long* x1, long* y1, long* x2, long* y2` |
| `dm_getClientSize` | `long` | `intptr_t hwnd, long* width, long* height` |
| `dm_getWindowState` | `long` | `intptr_t hwnd, long flag` |
| `dm_getProcessInfo` | `long` | `long pid, const char* type` |
| `dm_moveWindow` | `long` | `intptr_t hwnd, long x, long y` |
| `dm_setWindowSize` | `long` | `intptr_t hwnd, long width, long height` |
| `dm_setWindowState` | `long` | `intptr_t hwnd, long flag` |
| `dm_setWindowText` | `long` | `intptr_t hwnd, const char* title` |
| `dm_setWindowTransparent` | `long` | `intptr_t hwnd, long trans` |
| `dm_setClientSize` | `long` | `intptr_t hwnd, long width, long height` |
| `dm_setSendStringDelay` | `long` | `long delay` |
| `dm_sendPaste` | `long` | `intptr_t hwnd` |
| `dm_sendString` | `long` | `intptr_t hwnd, const char* str` |
| `dm_sendString2` | `long` | `intptr_t hwnd, const char* str` |
| `dm_sendStringIme` | `long` | `intptr_t hwnd, const char* str` |
| `dm_sendStringIme2` | `long` | `intptr_t hwnd, const char* str` |
| `dm_clientToScreen` | `long` | `intptr_t hwnd, long* x, long* y` |
| `dm_screenToClient` | `long` | `intptr_t hwnd, long* x, long* y` |

## 3. 键鼠

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_keyDown` | `long` | `long vk_code` |
| `dm_keyDownChar` | `long` | `const char* key_str` |
| `dm_keyPress` | `long` | `long vk_code` |
| `dm_keyPressChar` | `long` | `const char* key_str` |
| `dm_keyPressStr` | `long` | `const char* key_str, long delay` |
| `dm_keyUp` | `long` | `long vk_code` |
| `dm_keyUpChar` | `long` | `const char* key_str` |
| `dm_waitKey` | `long` | `long vk_code, long time_out` |
| `dm_getKeyState` | `long` | `long vk_code` |
| `dm_leftClick` | `long` | `` |
| `dm_leftDoubleClick` | `long` | `` |
| `dm_leftDown` | `long` | `` |
| `dm_leftUp` | `long` | `` |
| `dm_rightClick` | `long` | `` |
| `dm_rightDown` | `long` | `` |
| `dm_rightUp` | `long` | `` |
| `dm_middleClick` | `long` | `` |
| `dm_middleDown` | `long` | `` |
| `dm_middleUp` | `long` | `` |
| `dm_moveTo` | `long` | `long x, long y` |
| `dm_moveToEx` | `long` | `long x, long y, long w, long h` |
| `dm_moveR` | `long` | `long rx, long ry` |
| `dm_wheelDown` | `long` | `long repeat` |
| `dm_wheelUp` | `long` | `long repeat` |
| `dm_getCursorPos` | `long` | `long* x, long* y` |
| `dm_getCursorShape` | `long` | `` |
| `dm_getCursorShapeEx` | `const char*` | `long type` |
| `dm_getCursorSpot` | `long` | `` |
| `dm_setMouseDelay` | `long` | `long type, long delay` |
| `dm_setKeypadDelay` | `long` | `long type, long delay` |
| `dm_setMouseSpeed` | `long` | `long speed` |
| `dm_getMouseSpeed` | `long` | `` |
| `dm_enableMouseAccuracy` | `long` | `long enable` |
| `dm_setSimMode` | `long` | `long mode` |

## 4. 图色

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_capture` | `long` | `long x1, long y1, long x2, long y2, const char* file` |
| `dm_captureGif` | `long` | `long x1, long y1, long x2, long y2, const char* file, long delay, long time` |
| `dm_captureJpg` | `long` | `long x1, long y1, long x2, long y2, const char* file, long quality` |
| `dm_capturePng` | `long` | `long x1, long y1, long x2, long y2, const char* file` |
| `dm_capturePre` | `long` | `const char* file` |
| `dm_getColor` | `const char*` | `long x, long y` |
| `dm_getColorBGR` | `const char*` | `long x, long y` |
| `dm_getColorHSV` | `const char*` | `long x, long y` |
| `dm_getColorNum` | `long` | `long x1, long y1, long x2, long y2, const char* color, long sim` |
| `dm_getAveRGB` | `const char*` | `long x1, long y1, long x2, long y2` |
| `dm_getAveHSV` | `const char*` | `long x1, long y1, long x2, long y2` |
| `dm_cmpColor` | `long` | `long x, long y, const char* color, long sim` |
| `dm_findColor` | `long` | `long x1, long y1, long x2, long y2, const char* color, long sim, long dir, long* intX, long* intY` |
| `dm_findColorEx` | `long` | `long x1, long y1, long x2, long y2, const char* color, long sim, long dir` |
| `dm_findColorBlock` | `long` | `long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height, long* intX, long* intY` |
| `dm_findColorBlockEx` | `long` | `long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height` |
| `dm_findColorE` | `long` | `long x1, long y1, long x2, long y2, const char* color, long sim, long dir` |
| `dm_findMulColor` | `long` | `long x1, long y1, long x2, long y2, const char* color, long sim` |
| `dm_findMultiColor` | `long` | `long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir, long* intX, long* intY` |
| `dm_findMultiColorEx` | `long` | `long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir` |
| `dm_findMultiColorE` | `long` | `long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir` |
| `dm_findPic` | `long` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY` |
| `dm_findPicE` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir` |
| `dm_findPicEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir` |
| `dm_findPicExS` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir` |
| `dm_findPicS` | `long` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY` |
| `dm_findPicMem` | `long` | `long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY` |
| `dm_findPicMemE` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir` |
| `dm_findPicMemEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir` |
| `dm_findPicSim` | `long` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY` |
| `dm_findPicSimE` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir` |
| `dm_findPicSimEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir` |
| `dm_findPicSimMem` | `long` | `long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY` |
| `dm_findPicSimMemE` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir` |
| `dm_findPicSimMemEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir` |
| `dm_findShape` | `long` | `long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir, long* intX, long* intY` |
| `dm_findShapeE` | `const char*` | `long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir` |
| `dm_findShapeEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir` |
| `dm_getPicSize` | `const char*` | `const char* pic_name` |
| `dm_freePic` | `long` | `const char* pic_name` |
| `dm_loadPic` | `long` | `const char* pic_name` |
| `dm_loadPicByte` | `long` | `const char* pic_name, long data, long size` |
| `dm_appendPicAddr` | `const char*` | `const char* pic_info, long addr, long size` |
| `dm_matchPicName` | `const char*` | `const char* pic_name` |
| `dm_imageToBmp` | `long` | `const char* pic_name, const char* bmp_name` |
| `dm_setPicPwd` | `long` | `const char* pwd` |
| `dm_setExcludeRegion` | `long` | `long mode, const char* info` |
| `dm_getScreenData` | `long` | `long x1, long y1, long x2, long y2` |
| `dm_getScreenDataBmp` | `const char*` | `long x1, long y1, long x2, long y2, long* size` |
| `dm_enableDisplayDebug` | `long` | `long enable_debug` |
| `dm_enableFindPicMultithread` | `long` | `long enable` |
| `dm_enableGetColorByCapture` | `long` | `long enable` |
| `dm_setFindPicMultithreadCount` | `long` | `long count` |
| `dm_setFindPicMultithreadLimit` | `long` | `long limit` |
| `dm_bgr2RGB` | `long` | `const char* bgr_color` |
| `dm_rgb2BGR` | `const char*` | `const char* rgb_color` |
| `dm_isDisplayDead` | `long` | `long x1, long y1, long x2, long y2, long time` |

## 5. 后台设置

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_bindWindow` | `long` | `intptr_t hwnd, const char* display, const char* mouse, const char* keypad, long mode` |
| `dm_bindWindowEx` | `long` | `intptr_t hwnd, const char* display, const char* mouse, const char* keypad, const char* pub, long mode` |
| `dm_unBindWindow` | `long` | `` |
| `dm_getBindWindow` | `long` | `` |
| `dm_isBind` | `long` | `intptr_t hwnd` |
| `dm_forceUnBindWindow` | `long` | `` |
| `dm_setAero` | `long` | `long enable` |
| `dm_lockInput` | `long` | `long enable` |
| `dm_lockMouseRect` | `long` | `long x1, long y1, long x2, long y2` |
| `dm_lockDisplay` | `long` | `long enable` |
| `dm_enableBind` | `long` | `long enable` |
| `dm_enableFakeActive` | `long` | `long enable` |
| `dm_enableIme` | `long` | `long enable` |
| `dm_enableKeypadMsg` | `long` | `long enable` |
| `dm_enableKeypadPatch` | `long` | `long enable` |
| `dm_enableKeypadSync` | `long` | `long enable` |
| `dm_enableMouseMsg` | `long` | `long enable` |
| `dm_enableMouseSync` | `long` | `long enable, long time_out` |
| `dm_enableRealKeypad` | `long` | `long enable` |
| `dm_enableRealMouse` | `long` | `long enable, long mousedelay, long mousestep` |
| `dm_enableSpeedDx` | `long` | `long enable` |
| `dm_getFps` | `long` | `` |
| `dm_setDisplayDelay` | `long` | `long delay` |
| `dm_setDisplayRefreshDelay` | `long` | `long delay` |
| `dm_setInputDm` | `long` | `long enable` |
| `dm_hackSpeed` | `long` | `long rate` |
| `dm_downCpu` | `long` | `long enable, long rate` |
| `dm_switchBindWindow` | `long` | `intptr_t hwnd` |

## 6. 内存

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_openProcess` | `long` | `long pid` |
| `dm_getModuleBaseAddr` | `long` | `intptr_t hwnd, const char* module` |
| `dm_getModuleSize` | `long` | `intptr_t hwnd, const char* module` |
| `dm_getRemoteApiAddress` | `long` | `intptr_t hwnd, const char* module, const char* api` |
| `dm_readInt` | `long` | `intptr_t hwnd, long addr, long* ret` |
| `dm_readIntAddr` | `long` | `intptr_t hwnd, long addr, long type, long* ret` |
| `dm_readFloat` | `long` | `intptr_t hwnd, long addr, float* ret` |
| `dm_readFloatAddr` | `long` | `intptr_t hwnd, long addr, float* ret` |
| `dm_readDouble` | `long` | `intptr_t hwnd, long addr, double* ret` |
| `dm_readDoubleAddr` | `long` | `intptr_t hwnd, long addr, double* ret` |
| `dm_readString` | `const char*` | `intptr_t hwnd, long addr, long type, long len` |
| `dm_readStringAddr` | `const char*` | `intptr_t hwnd, long addr, long type, long len` |
| `dm_readData` | `const char*` | `intptr_t hwnd, long addr, long len` |
| `dm_readDataAddr` | `const char*` | `intptr_t hwnd, long addr, long len` |
| `dm_readDataToBin` | `long` | `intptr_t hwnd, long addr, long len` |
| `dm_readDataAddrToBin` | `long` | `intptr_t hwnd, long addr, long len` |
| `dm_findInt` | `const char*` | `intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max` |
| `dm_findIntEx` | `const char*` | `intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max, long step, long multi_thread, long mode` |
| `dm_findFloat` | `const char*` | `intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max` |
| `dm_findFloatEx` | `const char*` | `intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max, long step, long multi_thread, long mode` |
| `dm_findDouble` | `const char*` | `intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max` |
| `dm_findDoubleEx` | `const char*` | `intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max, long step, long multi_thread, long mode` |
| `dm_findString` | `const char*` | `intptr_t hwnd, const char* addr_range, const char* string_value, long type` |
| `dm_findStringEx` | `const char*` | `intptr_t hwnd, const char* addr_range, const char* string_value, long type, long step, long multi_thread, long mode` |
| `dm_findData` | `const char*` | `intptr_t hwnd, const char* addr_range, const char* data` |
| `dm_findDataEx` | `const char*` | `intptr_t hwnd, const char* addr_range, const char* data, long step, long multi_thread, long mode` |
| `dm_writeInt` | `long` | `intptr_t hwnd, long addr, long type, long v` |
| `dm_writeIntAddr` | `long` | `intptr_t hwnd, long addr, long type, long v` |
| `dm_writeFloat` | `long` | `intptr_t hwnd, long addr, float v` |
| `dm_writeFloatAddr` | `long` | `intptr_t hwnd, long addr, float v` |
| `dm_writeDouble` | `long` | `intptr_t hwnd, long addr, double v` |
| `dm_writeDoubleAddr` | `long` | `intptr_t hwnd, long addr, double v` |
| `dm_writeString` | `long` | `intptr_t hwnd, long addr, long type, const char* v` |
| `dm_writeStringAddr` | `long` | `intptr_t hwnd, long addr, long type, const char* v` |
| `dm_writeData` | `long` | `intptr_t hwnd, long addr, const char* data` |
| `dm_writeDataAddr` | `long` | `intptr_t hwnd, long addr, const char* data` |
| `dm_writeDataFromBin` | `long` | `intptr_t hwnd, long addr, long data, long len` |
| `dm_writeDataAddrFromBin` | `long` | `intptr_t hwnd, long addr, long data, long len` |
| `dm_virtualAllocEx` | `long` | `intptr_t hwnd, long size, long type, long protect` |
| `dm_virtualFreeEx` | `long` | `intptr_t hwnd, long addr` |
| `dm_virtualProtectEx` | `long` | `intptr_t hwnd, long addr, long size, long type, long* old_protect` |
| `dm_virtualQueryEx` | `const char*` | `intptr_t hwnd, long addr, long pmbi` |
| `dm_freeProcessMemory` | `long` | `intptr_t hwnd` |
| `dm_getCommandLine` | `const char*` | `intptr_t hwnd` |
| `dm_terminateProcess` | `long` | `long pid` |
| `dm_terminateProcessTree` | `long` | `long pid` |
| `dm_setMemoryFindResultToFile` | `long` | `long enable` |
| `dm_setMemoryHwndAsProcessId` | `long` | `long enable` |
| `dm_setParam64ToPointer` | `long` | `long enable` |
| `dm_int64ToInt32` | `const char*` | `long v_high, long v_low, long flag` |
| `dm_intToData` | `const char*` | `long v, long type` |
| `dm_floatToData` | `const char*` | `float v` |
| `dm_doubleToData` | `const char*` | `double v` |
| `dm_stringToData` | `const char*` | `const char* v, long type` |

## 7. 文件

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_writeFile` | `long` | `const char* file, const char* content` |
| `dm_readFile` | `const char*` | `const char* file` |
| `dm_deleteFile` | `long` | `const char* file` |
| `dm_copyFile` | `long` | `const char* src, const char* dest, long over` |
| `dm_moveFile` | `long` | `const char* src, const char* dest` |
| `dm_createFolder` | `long` | `const char* folder` |
| `dm_deleteFolder` | `long` | `const char* folder` |
| `dm_isFileExist` | `long` | `const char* file` |
| `dm_isFolderExist` | `long` | `const char* folder` |
| `dm_getFileLength` | `long` | `const char* file` |
| `dm_getRealPath` | `const char*` | `const char* path` |
| `dm_selectFile` | `const char*` | `` |
| `dm_selectDirectory` | `const char*` | `` |
| `dm_downloadFile` | `long` | `const char* url, const char* save_file, long timeout` |
| `dm_encodeFile` | `long` | `const char* file, const char* pwd` |
| `dm_decodeFile` | `long` | `const char* file, const char* pwd` |
| `dm_writeIni` | `long` | `const char* section, const char* key, const char* value, const char* file` |
| `dm_readIni` | `const char*` | `const char* section, const char* key, const char* file` |
| `dm_deleteIni` | `long` | `const char* section, const char* key, const char* file` |
| `dm_enumIniKey` | `const char*` | `const char* section, const char* file` |
| `dm_enumIniSection` | `const char*` | `const char* file` |
| `dm_writeIniPwd` | `long` | `const char* section, const char* key, const char* value, const char* file, const char* pwd` |
| `dm_readIniPwd` | `const char*` | `const char* section, const char* key, const char* file, const char* pwd` |
| `dm_deleteIniPwd` | `long` | `const char* section, const char* key, const char* file, const char* pwd` |
| `dm_enumIniKeyPwd` | `const char*` | `const char* section, const char* file, const char* pwd` |
| `dm_enumIniSectionPwd` | `const char*` | `const char* file, const char* pwd` |

## 8. 文字识别

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_setDict` | `long` | `long index, const char* file` |
| `dm_setDictMem` | `long` | `long index, const char* data, long size` |
| `dm_setDictPwd` | `long` | `const char* pwd` |
| `dm_useDict` | `long` | `long index` |
| `dm_getDict` | `const char*` | `long index, long font_index` |
| `dm_getDictCount` | `long` | `long index` |
| `dm_getDictInfo` | `const char*` | `const char* file, long index` |
| `dm_getNowDict` | `long` | `` |
| `dm_addDict` | `long` | `const char* dict_info` |
| `dm_saveDict` | `long` | `long index, const char* file` |
| `dm_clearDict` | `long` | `long index` |
| `dm_enableShareDict` | `long` | `long enable` |
| `dm_ocr` | `const char*` | `long x1, long y1, long x2, long y2, const char* color_format, long sim` |
| `dm_ocrEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* color_format, long sim` |
| `dm_ocrExOne` | `const char*` | `long x1, long y1, long x2, long y2, const char* color_format, long sim` |
| `dm_ocrInFile` | `long` | `long x1, long y1, long x2, long y2, const char* color_format, long sim, const char* file` |
| `dm_findStr` | `long` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY` |
| `dm_findStrE` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim` |
| `dm_findStrEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim` |
| `dm_findStrS` | `long` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY` |
| `dm_findStrExS` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim` |
| `dm_findStrFast` | `long` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY` |
| `dm_findStrFastE` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim` |
| `dm_findStrFastEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim` |
| `dm_findStrFastS` | `long` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY` |
| `dm_findStrFastExS` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim` |
| `dm_findStrWithFont` | `long` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size, long* intX, long* intY` |
| `dm_findStrWithFontE` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size` |
| `dm_findStrWithFontEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size` |
| `dm_getWords` | `const char*` | `long x1, long y1, long x2, long y2, const char* color, const char* words` |
| `dm_getWordsNoDict` | `const char*` | `long x1, long y1, long x2, long y2, const char* color` |
| `dm_fetchWord` | `long` | `long x1, long y1, long x2, long y2, const char* color, const char* words` |
| `dm_getResultCount` | `long` | `const char* ret` |
| `dm_getResultPos` | `long` | `const char* ret, long index, long* intX, long* intY` |
| `dm_getWordResultCount` | `long` | `const char* str` |
| `dm_getWordResultPos` | `long` | `const char* str, long index, long* intX, long* intY` |
| `dm_getWordResultStr` | `const char*` | `const char* str, long index` |
| `dm_setColGapNoDict` | `long` | `long col_gap` |
| `dm_setRowGapNoDict` | `long` | `long row_gap` |
| `dm_setWordGapNoDict` | `long` | `long word_gap` |
| `dm_setWordLineHeightNoDict` | `long` | `long line_height` |
| `dm_setExactOcr` | `long` | `long exact_ocr` |
| `dm_setMinColGap` | `long` | `long min_col_gap` |
| `dm_setMinRowGap` | `long` | `long min_row_gap` |
| `dm_setWordGap` | `long` | `long word_gap` |
| `dm_setWordLineHeight` | `long` | `long line_height` |

## 9. 系统

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_beep` | `long` | `long f, long duration` |
| `dm_delay` | `long` | `long mis` |
| `dm_delays` | `long` | `long mis_min, long mis_max` |
| `dm_runApp` | `long` | `const char* app_path, long mode` |
| `dm_stop` | `long` | `long id` |
| `dm_play` | `long` | `const char* media_file` |
| `dm_exitOs` | `long` | `long type` |
| `dm_setScreen` | `long` | `long width, long height, long depth` |
| `dm_getScreenWidth` | `long` | `` |
| `dm_getScreenHeight` | `long` | `` |
| `dm_getScreenDepth` | `long` | `` |
| `dm_getDPI` | `long` | `` |
| `dm_getTime` | `long` | `` |
| `dm_getOsType` | `long` | `` |
| `dm_getOsBuildNumber` | `long` | `` |
| `dm_is64Bit` | `long` | `` |
| `dm_isSurrpotVt` | `long` | `` |
| `dm_getSystemInfo` | `const char*` | `long type, long method` |
| `dm_getDir` | `const char*` | `long type` |
| `dm_getLocale` | `long` | `` |
| `dm_getCpuType` | `long` | `` |
| `dm_getCpuUsage` | `long` | `` |
| `dm_getMemoryUsage` | `long` | `` |
| `dm_getDiskSerial` | `const char*` | `` |
| `dm_getDiskModel` | `const char*` | `` |
| `dm_getDiskReversion` | `const char*` | `` |
| `dm_getMachineCode` | `const char*` | `` |
| `dm_getMachineCodeNoMac` | `const char*` | `` |
| `dm_getNetTime` | `const char*` | `` |
| `dm_getNetTimeByIp` | `const char*` | `const char* ip` |
| `dm_getNetTimeSafe` | `const char*` | `long time_out` |
| `dm_getClipboard` | `const char*` | `` |
| `dm_setClipboard` | `long` | `const char* value` |
| `dm_getDisplayInfo` | `long` | `long type` |
| `dm_disableCloseDisplayAndSleep` | `long` | `long enable` |
| `dm_disablePowerSave` | `long` | `long enable` |
| `dm_disableScreenSave` | `long` | `long enable` |
| `dm_disableFontSmooth` | `long` | `` |
| `dm_enableFontSmooth` | `long` | `` |
| `dm_checkFontSmooth` | `long` | `` |
| `dm_checkUAC` | `long` | `` |
| `dm_setUAC` | `long` | `long enable` |
| `dm_setDisplayAcceler` | `long` | `long flag` |
| `dm_showTaskBarIcon` | `long` | `intptr_t hwnd, long show` |

## 10. 杂项

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_enterCri` | `long` | `` |
| `dm_leaveCri` | `long` | `` |
| `dm_initCri` | `long` | `` |
| `dm_releaseRef` | `long` | `` |
| `dm_setExitThread` | `long` | `long mode` |
| `dm_executeCmd` | `const char*` | `const char* cmd, long time_out` |
| `dm_activeInputMethod` | `long` | `intptr_t hwnd, const char* input_method` |
| `dm_checkInputMethod` | `long` | `intptr_t hwnd, const char* input_method` |
| `dm_findInputMethod` | `long` | `const char* input_method` |

## 11. 汇编

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_asmAdd` | `long` | `const char* asm_ins` |
| `dm_asmCall` | `long` | `intptr_t hwnd, long mode` |
| `dm_asmCallEx` | `long` | `intptr_t hwnd, long mode, long base_addr, long call_addr` |
| `dm_asmClear` | `long` | `` |
| `dm_asmSetTimeout` | `long` | `long timeout` |
| `dm_assemble` | `const char*` | `long base_addr, long is_asm_code` |
| `dm_disAssemble` | `const char*` | `long asm_code, long base_addr, long is_asm_code` |
| `dm_setAsmHwndAsProcessId` | `long` | `long enable` |
| `dm_setShowAsmErrorMsg` | `long` | `long show` |

## 12. AI

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_loadAi` | `long` | `const char* file` |
| `dm_loadAiMemory` | `long` | `long data, long size` |
| `dm_aiEnableFindPicWindow` | `long` | `long enable` |
| `dm_aiFindPic` | `long` | `long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir, long* intX, long* intY` |
| `dm_aiFindPicEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir` |
| `dm_aiFindPicMem` | `long` | `long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir, long* intX, long* intY` |
| `dm_aiFindPicMemEx` | `const char*` | `long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir` |
| `dm_aiYoloDetectObjects` | `const char*` | `long x1, long y1, long x2, long y2, float prob, float iou` |
| `dm_aiYoloDetectObjectsToDataBmp` | `long` | `long x1, long y1, long x2, long y2, float prob, float iou, long data, long size, long mode` |
| `dm_aiYoloDetectObjectsToFile` | `long` | `long x1, long y1, long x2, long y2, float prob, float iou, const char* file, long mode` |
| `dm_aiYoloSetModel` | `long` | `const char* model_file, const char* model_type` |
| `dm_aiYoloSetModelMemory` | `long` | `long data, long size, const char* model_type` |
| `dm_aiYoloFreeModel` | `long` | `` |
| `dm_aiYoloUseModel` | `long` | `long index` |
| `dm_aiYoloSetVersion` | `long` | `const char* version` |
| `dm_aiYoloObjectsToString` | `const char*` | `const char* objects` |
| `dm_aiYoloSortsObjects` | `const char*` | `const char* objects, long sort_type` |

## 13. Foobar (界面控件)

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_createFoobarRect` | `long` | `intptr_t hwnd, long x, long y, long w, long h` |
| `dm_createFoobarEllipse` | `long` | `intptr_t hwnd, long x, long y, long w, long h` |
| `dm_createFoobarRoundRect` | `long` | `intptr_t hwnd, long x, long y, long w, long h, long rw, long rh` |
| `dm_createFoobarCustom` | `long` | `intptr_t hwnd, long x, long y, long w, long h, const char* pic` |
| `dm_foobarClose` | `long` | `intptr_t hwnd` |
| `dm_foobarLock` | `long` | `intptr_t hwnd` |
| `dm_foobarUnlock` | `long` | `intptr_t hwnd` |
| `dm_foobarUpdate` | `long` | `intptr_t hwnd` |
| `dm_foobarSetFont` | `long` | `intptr_t hwnd, const char* font_name, long size, long flag` |
| `dm_foobarSetSave` | `long` | `intptr_t hwnd, const char* file, long enable` |
| `dm_foobarSetTrans` | `long` | `intptr_t hwnd, long trans, long color` |
| `dm_foobarDrawText` | `long` | `intptr_t hwnd, long x, long y, long w, long h, const char* text, long color, long align` |
| `dm_foobarPrintText` | `long` | `intptr_t hwnd, const char* text, long color` |
| `dm_foobarDrawRect` | `long` | `intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style` |
| `dm_foobarDrawLine` | `long` | `intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style` |
| `dm_foobarFillRect` | `long` | `intptr_t hwnd, long x1, long y1, long x2, long y2, long color` |
| `dm_foobarDrawPic` | `long` | `intptr_t hwnd, long x, long y, const char* pic` |
| `dm_foobarClearText` | `long` | `intptr_t hwnd` |
| `dm_foobarTextRect` | `long` | `intptr_t hwnd, long x, long y, long w, long h` |
| `dm_foobarTextLineGap` | `long` | `intptr_t hwnd, long line_gap` |
| `dm_foobarTextPrintDir` | `long` | `intptr_t hwnd, long dir` |
| `dm_foobarStartGif` | `long` | `intptr_t hwnd, const char* pic, long x, long y` |
| `dm_foobarStopGif` | `long` | `intptr_t hwnd` |

## 14. 答题

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_faqCapture` | `long` | `long x1, long y1, long x2, long y2, long quality, long delay, long time` |
| `dm_faqCaptureFromFile` | `long` | `const char* file, long quality, long delay, long time` |
| `dm_faqCaptureString` | `const char*` | `long x1, long y1, long x2, long y2, long quality, long delay, long time` |
| `dm_faqGetSize` | `long` | `long handle` |
| `dm_faqPost` | `long` | `const char* server, long handle, long request_type, long time_out` |
| `dm_faqSend` | `long` | `long handle, long time_out` |
| `dm_faqFetch` | `long` | `long time_out` |
| `dm_faqCancel` | `long` | `` |
| `dm_faqIsPosted` | `long` | `` |

## 15. 算法

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_excludePos` | `const char*` | `const char* all_pos, long type, long x1, long y1, long x2, long y2` |
| `dm_findNearestPos` | `const char*` | `const char* all_pos, long type, long x, long y` |
| `dm_sortPosDistance` | `const char*` | `const char* all_pos, long type, long x, long y` |

## 16. 防护盾

| 函数 | 返回类型 | 参数 |
|:-----|:---------|:-----|
| `dm_dmGuard` | `long` | `long enable, const char* type` |
| `dm_dmGuardExtract` | `long` | `const char* type, const char* file` |
| `dm_dmGuardLoadCustom` | `long` | `const char* type, const char* data` |
| `dm_dmGuardParams` | `const char*` | `const char* cmd, const char* param` |
| `dm_unLoadDriver` | `long` | `` |
