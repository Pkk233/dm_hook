$ErrorActionPreference = "Continue"
$log = "D:\dm\dm_hook\tests\compile_smoke.log"
"=== compile tests $(Get-Date) ===" | Out-File -FilePath $log -Encoding utf8

$vc     = "D:\ProgramData\Microsoft VisualStudio\17\VC\Tools\MSVC\14.44.35207"
$cl     = "$vc\bin\Hostx64\x64\cl.exe"
$sdk    = "D:\Windows Kits\10"
$sdkver = "10.0.26100.0"
$inc = @(
    "$vc\include",
    "$sdk\Include\$sdkver\um",
    "$sdk\Include\$sdkver\shared",
    "$sdk\Include\$sdkver\ucrt",
    "$sdk\Include\$sdkver\winrt",
    "$sdk\Include\$sdkver\cppwinrt"
)
$lib = @(
    "$vc\lib\x64",
    "$sdk\Lib\$sdkver\um\x64",
    "$sdk\Lib\$sdkver\ucrt\x64"
)
$env:INCLUDE = $inc -join ";"
$env:LIB     = $lib -join ";"
$env:PATH    = "$vc\bin\Hostx64\x64;" + $env:PATH

$inc | ForEach-Object { "INCLUDE: $_" | Out-File -FilePath $log -Append -Encoding utf8 }
"cl exists: $(Test-Path $cl)" | Out-File -FilePath $log -Append -Encoding utf8

$src = "D:\dm\dm_hook\tests\test_dm_all.cpp"
$out = "D:\dm\dm_hook\tests\test_dm_all.exe"
& $cl /nologo /std:c++17 /EHa /utf-8 /I"D:\dm\dm_hook" /Fe:"$out" "$src" 2>&1 | Out-File -FilePath $log -Append -Encoding utf8
"smoke compile exit: $LASTEXITCODE" | Out-File -FilePath $log -Append -Encoding utf8

"=== done ===" | Out-File -FilePath $log -Append -Encoding utf8
