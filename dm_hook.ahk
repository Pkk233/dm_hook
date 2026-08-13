; ============================================================================
; dm_hook.ahk �?大漠插件 (dm_hook.dll) AutoHotkey v1 绑定示例
; ============================================================================
; 用法: 安装 AutoHotkey v1 (https://www.autohotkey.com/), 双击运行此文�?;
; 本示例演示如何通过 DllCall 调用 dm_hook.dll 的导出函�?; 所有函数使�?__stdcall 调用约定，可直接�?DllCall 调用
; ============================================================================

#NoEnv
#Persistent
#SingleInstance Force
SetWorkingDir %A_ScriptDir%

; ============================================================================
; 初始�? 加载 DLL
; ============================================================================

; 尝试加载 DLL
DllPath := A_ScriptDir . "\dm_hook.dll"
if !FileExist(DllPath)
    DllPath := A_ScriptDir . "\build\Release\dm_hook.dll"
if !FileExist(DllPath)
    DllPath := A_ScriptDir . "\build\dm_hook.dll"

hModule := DllCall("LoadLibrary", "Str", DllPath, "Ptr")
if (hModule = 0) {
    MsgBox, 16, 错误, 无法加载 dm_hook.dll`n请先编译项目: `ncmake -B build && cmake --build build --config Release
    ExitApp
}

; ============================================================================
; 辅助函数: 简�?DllCall
; ============================================================================

; 调用返回 long 的函�?dm_call(FuncName, args*) {
    global hModule
    ; 构建 DllCall 参数
    fn := FuncName
    result := DllCall(fn, args*)
    return result
}

; 调用返回 string (const char*) 的函�?dm_call_str(FuncName, args*) {
    global hModule
    ptr := DllCall(FuncName, args*, "CDecl Str")
    return ptr
}

; ============================================================================
; 基本设置
; ============================================================================

dm_Ver() {
    return DllCall("dm_ver", "AStr")
}

dm_GetID() {
    return DllCall("dm_getID")
}

dm_GetLastError() {
    return DllCall("dm_getLastError")
}

dm_GetPath() {
    return DllCall("dm_getPath", "AStr")
}

dm_SetPath(path) {
    return DllCall("dm_setPath", "AStr", path)
}

dm_GetBasePath() {
    return DllCall("dm_getBasePath", "AStr")
}

; ============================================================================
; 窗口
; ============================================================================

dm_FindWindow(cls, title) {
    return DllCall("dm_findWindow", "AStr", cls, "AStr", title)
}

dm_GetForegroundWindow() {
    return DllCall("dm_getForegroundWindow")
}

dm_GetWindowTitle(hwnd) {
    return DllCall("dm_getWindowTitle", "Ptr", hwnd, "AStr")
}

dm_GetWindowClass(hwnd) {
    return DllCall("dm_getWindowClass", "Ptr", hwnd, "AStr")
}

dm_GetWindowProcessPath(hwnd) {
    return DllCall("dm_getWindowProcessPath", "Ptr", hwnd, "AStr")
}

dm_GetWindowRect(hwnd, ByRef x1, ByRef y1, ByRef x2, ByRef y2) {
    return DllCall("dm_getWindowRect", "Ptr", hwnd, "Int*", x1, "Int*", y1, "Int*", x2, "Int*", y2)
}

dm_MoveWindow(hwnd, x, y) {
    return DllCall("dm_moveWindow", "Ptr", hwnd, "Int", x, "Int", y)
}

dm_SetWindowText(hwnd, title) {
    return DllCall("dm_setWindowText", "Ptr", hwnd, "AStr", title)
}

; ============================================================================
; 键鼠
; ============================================================================

dm_MoveTo(x, y) {
    return DllCall("dm_moveTo", "Int", x, "Int", y)
}

dm_LeftClick() {
    return DllCall("dm_leftClick")
}

dm_LeftDoubleClick() {
    return DllCall("dm_leftDoubleClick")
}

dm_RightClick() {
    return DllCall("dm_rightClick")
}

dm_KeyPress(vk) {
    return DllCall("dm_keyPress", "Int", vk)
}

dm_KeyPressChar(key) {
    return DllCall("dm_keyPressChar", "AStr", key)
}

dm_KeyDown(vk) {
    return DllCall("dm_keyDown", "Int", vk)
}

dm_KeyUp(vk) {
    return DllCall("dm_keyUp", "Int", vk)
}

dm_GetCursorPos(ByRef x, ByRef y) {
    return DllCall("dm_getCursorPos", "Int*", x, "Int*", y)
}

; ============================================================================
; 图色
; ============================================================================

dm_GetColor(x, y) {
    return DllCall("dm_getColor", "Int", x, "Int", y, "AStr")
}

dm_GetColorBGR(x, y) {
    return DllCall("dm_getColorBGR", "Int", x, "Int", y, "AStr")
}

dm_Capture(x1, y1, x2, y2, file) {
    return DllCall("dm_capture", "Int", x1, "Int", y1, "Int", x2, "Int", y2, "AStr", file)
}

dm_CapturePng(x1, y1, x2, y2, file) {
    return DllCall("dm_capturePng", "Int", x1, "Int", y1, "Int", x2, "Int", y2, "AStr", file)
}

dm_FindColor(x1, y1, x2, y2, color, sim, dir, ByRef intX, ByRef intY) {
    return DllCall("dm_findColor", "Int", x1, "Int", y1, "Int", x2, "Int", y2,
                   "AStr", color, "Int", sim, "Int", dir, "Int*", intX, "Int*", intY)
}

dm_FindPic(x1, y1, x2, y2, pic_name, delta_color, sim, dir, ByRef intX, ByRef intY) {
    return DllCall("dm_findPic", "Int", x1, "Int", y1, "Int", x2, "Int", y2,
                   "AStr", pic_name, "AStr", delta_color, "Int", sim, "Int", dir,
                   "Int*", intX, "Int*", intY)
}

dm_CmpColor(x, y, color, sim) {
    return DllCall("dm_cmpColor", "Int", x, "Int", y, "AStr", color, "Int", sim)
}

; ============================================================================
; 系统
; ============================================================================

dm_Delay(ms) {
    DllCall("dm_delay", "Int", ms)
}

dm_Beep(freq, duration) {
    return DllCall("dm_beep", "Int", freq, "Int", duration)
}

dm_GetScreenWidth() {
    return DllCall("dm_getScreenWidth")
}

dm_GetScreenHeight() {
    return DllCall("dm_getScreenHeight")
}

dm_GetTime() {
    return DllCall("dm_getTime")
}

dm_GetMachineCode() {
    return DllCall("dm_getMachineCode", "AStr")
}

dm_RunApp(app_path, mode) {
    return DllCall("dm_runApp", "AStr", app_path, "Int", mode)
}

dm_ExecuteCmd(cmd, timeout) {
    return DllCall("dm_executeCmd", "AStr", cmd, "Int", timeout, "AStr")
}

; ============================================================================
; 文件
; ============================================================================

dm_WriteFile(file, content) {
    return DllCall("dm_writeFile", "AStr", file, "AStr", content)
}

dm_ReadFile(file) {
    return DllCall("dm_readFile", "AStr", file, "AStr")
}

dm_DeleteFile(file) {
    return DllCall("dm_deleteFile", "AStr", file)
}

dm_IsFileExist(file) {
    return DllCall("dm_isFileExist", "AStr", file)
}

dm_WriteIni(section, key, value, file) {
    return DllCall("dm_writeIni", "AStr", section, "AStr", key, "AStr", value, "AStr", file)
}

dm_ReadIni(section, key, file) {
    return DllCall("dm_readIni", "AStr", section, "AStr", key, "AStr", file, "AStr")
}

; ============================================================================
; 示例
; ============================================================================

; 显示信息窗口
InfoStr =
(
大漠插件 (dm_hook) AutoHotkey v1 示例
=====================================
版本: %dm_Ver()%
对象ID: %dm_GetID()%
屏幕: %dm_GetScreenWidth()% x %dm_GetScreenHeight()%
全局路径: %dm_GetPath()%
基础路径: %dm_GetBasePath()%
机器�? %dm_GetMachineCode()%

详细 API 文档请参�?dm.h 头文�?)

MsgBox, 0, 大漠插件 dm_hook, %InfoStr%

; 获取前台窗口信息
fg_hwnd := dm_GetForegroundWindow()
if (fg_hwnd) {
    WinGetTitle, fg_title, ahk_id %fg_hwnd%
    WinGetClass, fg_class, ahk_id %fg_hwnd%
    WinGet, fg_pid, PID, ahk_id %fg_hwnd%
    
    WinGetPos, wx, wy, ww, wh, ahk_id %fg_hwnd%
    
    InfoStr2 =
    (
前台窗口信息:
句柄: %fg_hwnd%
标题: %fg_title%
类名: %fg_class%
PID: %fg_pid%
位置: %wx%, %wy%, %ww%, %wh%
    )
    MsgBox, 0, 前台窗口, %InfoStr2%
}

; 获取鼠标位置颜色
MouseGetPos, mx, my
color := dm_GetColor(mx, my)
MsgBox, 0, 鼠标位置颜色, 鼠标位置: (%mx%, %my%)`n颜色�? %color%

; 卸载 DLL
DllCall("FreeLibrary", "Ptr", hModule)
MsgBox, 已完�?
ExitApp
