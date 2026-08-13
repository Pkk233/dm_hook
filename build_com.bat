@echo off
chcp 65001 > nul
set "CMAKE=D:\ProgramData\Microsoft VisualStudio\17\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
cd /d D:\dm\dm_hook
call "D:\ProgramData\Microsoft VisualStudio\17\Common7\Tools\VsDevCmd.bat" -arch=x64
echo INCLUDE path: %INCLUDE:~0,100%...
echo Building...
"%CMAKE%" --build build --config Release
echo Exit code: %ERRORLEVEL%