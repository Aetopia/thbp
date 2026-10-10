@echo off

cd "%~dp0"
cd "src"

rd /q /s "bin"
rd /q /s "obj"

md "bin"
md "obj"

i686-w64-mingw32-windres.exe -i "res.rc" -o "obj\res.o"

i686-w64-mingw32-gcc.exe -std=c23 ^
-DINITGUID -DWINVER=NTDDI_WIN10 -DWIN32_LEAN_AND_MEAN ^
-e "DllMain" -s -Oz -shared -municode -nostdlib -mstackrealign ^
-Wl,--kill-at,--gc-sections,--exclude-all-symbols -Wno-dll-attribute-on-redeclaration ^
"main.c" "obj\res.o" -ld3d9 -lgdi32 -ldwmapi -lmfplat -luser32 -lkernel32 -o "bin\dinput8.dll"