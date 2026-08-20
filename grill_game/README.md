# Tongs Are Mine! — playable prototype

Drag meat onto the grill with the left mouse button. Right-click it to flip.
When both tiny bars are green, drag it onto the plate. You have 60 seconds.

## Build (installed MSVC; no Windows SDK required)

```bat
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
lib /nologo /def:kernel32.def /machine:x64 /out:kernel32.lib
lib /nologo /def:user32.def /machine:x64 /out:user32.lib
lib /nologo /def:gdi32.def /machine:x64 /out:gdi32.lib
cl /nologo /O1 /GS- /GR- /EHs-c- /Zl main.cpp /link /NODEFAULTLIB /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OPT:REF /OPT:ICF /FILEALIGN:512 kernel32.lib user32.lib gdi32.lib /OUT:tongs.exe
```

Run `tongs.exe --self-test`; exit code `0` means the scoring rule passed.
