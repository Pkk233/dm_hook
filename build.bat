@echo off
REM ============================================================================
REM build.bat — 大漠插件 (dm_hook) 构建脚本
REM ============================================================================
chcp 65001 > nul
echo ============================================
echo  大漠插件 (dm_hook) 构建脚本
echo ============================================

REM 检查 Visual Studio
where cl > nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [错误] 未找到 Visual Studio 编译器 (cl.exe)
    echo 请先运行 vcvarsall.bat 或使用"适用于 VS 的开发人员命令提示符"
    echo.
    echo 示例: 
    echo   call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    goto :end
)

REM 检查 CMake
where cmake > nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [错误] 未找到 CMake. 请安装 CMake 并添加到 PATH
    echo 下载地址: https://cmake.org/download/
    goto :end
)

REM 创建构建目录
if not exist build mkdir build
cd build

REM 配置
echo.
echo [1/3] 配置项目...
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release 2>nul
if %ERRORLEVEL% NEQ 0 (
    REM 如果没有 Ninja, 使用默认生成器
    cmake .. -DCMAKE_BUILD_TYPE=Release
)

REM 构建
echo.
echo [2/3] 编译项目...
cmake --build . --config Release --parallel

REM 检查结果
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [3/3] 构建成功!
    echo.
    echo 输出文件:
    dir /b Release\dm_hook.dll 2>nul || dir /b dm_hook.dll 2>nul
    echo.
    echo DLL 路径: %CD%\Release\dm_hook.dll
) else (
    echo.
    echo [错误] 构建失败，请检查错误信息
)

cd ..
:end
echo.
pause