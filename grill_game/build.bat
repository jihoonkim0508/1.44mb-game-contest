@echo off
setlocal
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
lib /nologo /def:kernel32.def /machine:x64 /out:kernel32.lib >nul || exit /b 1
lib /nologo /def:user32.def /machine:x64 /out:user32.lib >nul || exit /b 1
lib /nologo /def:gdi32.def /machine:x64 /out:gdi32.lib >nul || exit /b 1
cl /nologo /utf-8 /O1 /GS- /GR- /EHs-c- /Zl main.cpp /link /NODEFAULTLIB /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OPT:REF /OPT:ICF /FILEALIGN:512 kernel32.lib user32.lib gdi32.lib /OUT:tongs.exe
if errorlevel 1 exit /b 1
for %%F in (tongs.exe) do if %%~zF GTR 1474560 exit /b 2
