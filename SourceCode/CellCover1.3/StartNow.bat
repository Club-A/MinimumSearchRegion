@echo off
set rr="HKCU\Console\%%SystemRoot%%_system32_cmd.exe"
reg delete %rr% /f>nul
reg add %rr% /v "WindowPosition" /t REG_DWORD /d 0x00240309 /f>nul
color 17
echo "RedisStart Now..."
start cmd /k RedisStart.bat
echo "Success"
timeout /t 2
echo "ProStart Now..."
start cmd /k ProStart.bat
echo "Success"