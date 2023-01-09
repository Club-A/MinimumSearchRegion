@echo off&setlocal enabledelayedexpansion
set time1=%time%
set /a time1_second=1%time1:~-5,2%-100
set /a time1_millisec=1%time1:~-2,2%-100
call 测算消耗时间的程序或者将该行改为某程序命令
set time2=%time%
set /a time2_second=1%time2:~-5,2%-100
set /a time2_millisec=1%time2:~-2,2%-100
if %time2_millisec% lss %time1_millisec% set /a time2_millisec+=100&set /a time2_second-=1
set /a second=time2_second-time1_second
set /a millisec=time2_millisec-time1_millisec
echo 程序运行开始时间:%time1%  结束时间:%time2%
echo 程序运行时间为%second%秒%millisec%0毫秒
pause & exit