@echo off
chcp 65001 > nul
echo Registering dm_hook.dll...
regsvr32 /s "D:\dm\dm_hook\dm_hook.dll"
if %ERRORLEVEL% EQU 0 (
    echo Registration successful!
) else (
    echo Registration failed with error code: %ERRORLEVEL%
)
pause