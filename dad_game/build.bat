@echo off
setlocal
set "VSROOT=C:\Program Files\Microsoft Visual Studio\18\Community"
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
for %%L in (kernel32 user32 gdi32) do lib /nologo /machine:x64 /def:%%L.def /out:%%L.lib >nul || exit /b 1
cl /nologo /utf-8 /O1 /Os /GS- /W4 /Zl main.c /c
if errorlevel 1 exit /b 1
link /nologo main.obj kernel32.lib user32.lib gdi32.lib /out:dad_game.exe /subsystem:windows /machine:x64 /nodefaultlib /entry:mainCRTStartup /incremental:no /opt:ref /opt:icf /filealign:512
if errorlevel 1 exit /b 1
dad_game.exe --self-test
if errorlevel 1 exit /b 1
for %%F in (dad_game.exe) do if %%~zF GTR 1474560 exit /b 2
exit /b 0
