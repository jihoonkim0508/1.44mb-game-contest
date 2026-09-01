@echo off
setlocal
set "VSROOT=C:\Program Files\Microsoft Visual Studio\18\Community"
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
for %%L in (kernel32 user32 gdi32 msimg32 winmm) do lib /nologo /machine:x64 /def:%%L.def /out:%%L.lib >nul || exit /b 1
for %%F in (assets\room_background.bmp) do for %%S in (assets\sprite_sheet.bmp) do for %%C in (assets\audio_compact\founded.wav) do for %%G in (assets\audio_compact\game.wav) do for %%T in (assets\audio_compact\stomp.wav) do for %%D in (assets\audio_compact\door_open.wav) do cl /nologo /utf-8 /O1 /Os /GS- /W4 /Zl /DROOM_BMP_SIZE=%%~zF /DSHEET_BMP_SIZE=%%~zS /DFOUNDED_WAV_SIZE=%%~zC /DGAME_WAV_SIZE=%%~zG /DSTOMP_WAV_SIZE=%%~zT /DDOOR_WAV_SIZE=%%~zD main.c /c
if errorlevel 1 exit /b 1
link /nologo main.obj kernel32.lib user32.lib gdi32.lib msimg32.lib winmm.lib /out:dad_game_core.exe /subsystem:windows /machine:x64 /nodefaultlib /entry:mainCRTStartup /incremental:no /opt:ref /opt:icf /filealign:512
if errorlevel 1 exit /b 1
copy /b dad_game_core.exe+assets\room_background.bmp+assets\sprite_sheet.bmp+assets\audio_compact\founded.wav+assets\audio_compact\game.wav+assets\audio_compact\stomp.wav+assets\audio_compact\door_open.wav dad_game.exe >nul
if errorlevel 1 exit /b 1
dad_game.exe --self-test
if errorlevel 1 exit /b 1
for %%F in (dad_game.exe) do if %%~zF GTR 1474560 exit /b 2
powershell -NoProfile -Command "$n=[string][char]0xBAB0+[char]0xCEF4+[char]0xD558+[char]0xAE30+'.exe'; Move-Item -LiteralPath 'dad_game.exe' -Destination $n -Force"
if errorlevel 1 exit /b 1
exit /b 0
