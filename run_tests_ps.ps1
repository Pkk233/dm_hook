$ErrorActionPreference = "Continue"
$log = "D:\dm\dm_hook\tests\run_tests.log"
"=== tests build+run $(Get-Date) ===" | Out-File -FilePath $log -Encoding utf8

try {
    Import-Module "D:\ProgramData\Microsoft VisualStudio\17\Common7\Tools\Microsoft.VisualStudio.DevShell.dll" -ErrorAction Stop
    "Module imported OK" | Out-File -FilePath $log -Append -Encoding utf8
} catch {
    "Module import FAILED: $($_.Exception.Message)" | Out-File -FilePath $log -Append -Encoding utf8
    exit 1
}
try {
    Enter-VsDevShell -VsInstallPath "D:\ProgramData\Microsoft VisualStudio\17" -SkipAutomaticLocation -DevCmdArguments "-arch=x64" -ErrorAction Stop
    "DevShell OK" | Out-File -FilePath $log -Append -Encoding utf8
} catch {
    "DevShell FAILED: $($_.Exception.Message)" | Out-File -FilePath $log -Append -Encoding utf8
    exit 1
}

$inc = "D:\dm\dm_hook"
$cl = "cl"

# 编译冒烟测试
& $cl /nologo /std:c++17 /EHa /utf-8 /I"$inc" /Fe:"D:\dm\dm_hook\tests\test_dm_all.exe" "D:\dm\dm_hook\tests\test_dm_all.cpp" 2>&1 | Out-File -FilePath $log -Append -Encoding utf8
"smoke compile exit: $LASTEXITCODE" | Out-File -FilePath $log -Append -Encoding utf8

# 编译单元测试
& $cl /nologo /std:c++17 /EHa /utf-8 /I"$inc" /Fe:"D:\dm\dm_hook\tests\test_dm_unit.exe" "D:\dm\dm_hook\tests\test_dm_unit.cpp" 2>&1 | Out-File -FilePath $log -Append -Encoding utf8
"unit compile exit: $LASTEXITCODE" | Out-File -FilePath $log -Append -Encoding utf8

if (Test-Path "D:\dm\dm_hook\tests\test_dm_all.exe") {
    "----- running test_dm_all.exe -----" | Out-File -FilePath $log -Append -Encoding utf8
    & "D:\dm\dm_hook\tests\test_dm_all.exe" 2>&1 | Out-File -FilePath $log -Append -Encoding utf8
    "smoke run exit: $LASTEXITCODE" | Out-File -FilePath $log -Append -Encoding utf8
} else {
    "SKIP smoke run (exe missing)" | Out-File -FilePath $log -Append -Encoding utf8
}

if (Test-Path "D:\dm\dm_hook\tests\test_dm_unit.exe") {
    "----- running test_dm_unit.exe -----" | Out-File -FilePath $log -Append -Encoding utf8
    & "D:\dm\dm_hook\tests\test_dm_unit.exe" 2>&1 | Out-File -FilePath $log -Append -Encoding utf8
    "unit run exit: $LASTEXITCODE" | Out-File -FilePath $log -Append -Encoding utf8
} else {
    "SKIP unit run (exe missing)" | Out-File -FilePath $log -Append -Encoding utf8
}

"=== done ===" | Out-File -FilePath $log -Append -Encoding utf8
