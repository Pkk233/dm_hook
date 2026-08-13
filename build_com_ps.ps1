$ErrorActionPreference = "Continue"
$log = "D:\dm\dm_hook\build_com.log"
"=== build start $(Get-Date) ===" | Out-File -FilePath $log -Encoding utf8

try {
    Import-Module "D:\ProgramData\Microsoft VisualStudio\17\Common7\Tools\Microsoft.VisualStudio.DevShell.dll" -ErrorAction Stop
    "Module imported OK" | Out-File -FilePath $log -Append -Encoding utf8
} catch {
    "Module import FAILED: $($_.Exception.Message)" | Out-File -FilePath $log -Append -Encoding utf8
    exit 1
}

try {
    Enter-VsDevShell -VsInstallPath "D:\ProgramData\Microsoft VisualStudio\17" -SkipAutomaticLocation -DevCmdArguments "-arch=x64" -ErrorAction Stop
    "DevShell entered OK" | Out-File -FilePath $log -Append -Encoding utf8
} catch {
    "DevShell FAILED: $($_.Exception.Message)" | Out-File -FilePath $log -Append -Encoding utf8
    exit 1
}

"cl.exe: $((Get-Command cl.exe -ErrorAction SilentlyContinue).Source)" | Out-File -FilePath $log -Append -Encoding utf8
"ninja: $((Get-Command ninja.exe -ErrorAction SilentlyContinue).Source)" | Out-File -FilePath $log -Append -Encoding utf8

$cmake = "D:\ProgramData\Microsoft VisualStudio\17\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
& $cmake --build D:\dm\dm_hook\build --config Release 2>&1 | Out-File -FilePath $log -Append -Encoding utf8

"=== build exit code: $LASTEXITCODE ===" | Out-File -FilePath $log -Append -Encoding utf8
exit $LASTEXITCODE
