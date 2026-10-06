@echo off

cd "%~dp0"
cd "src"

rd /q /s "bin"
rd /q /s "obj"

md "bin"
md "obj"

i686-w64-mingw32-windres.exe -i "res.rc" -o "obj\res.o"

i686-w64-mingw32-gcc.exe ^
-DINITGUID -DWIN32_LEAN_AND_MEAN -DWINVER=NTDDI_WIN10 ^
-municode -mstackrealign -nostdlib -s -Oz -shared -e "DllMain" ^
-Wl,--kill-at,--gc-sections,--exclude-all-symbols -Wno-dll-attribute-on-redeclaration ^
"main.c" "obj\res.o" -lkernel32 -luser32 -lgdi32 -ld3d9 -ldwmapi -lmfplat -o "bin\dinput8.dll"