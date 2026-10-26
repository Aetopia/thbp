@echo off

cd "%~dp0"
cd "src"

rd /q /s "bin"
rd /q /s "obj"

md "bin"
md "obj"

i686-w64-mingw32-windres.exe -i "resource.rc" -o "obj\main.o"

i686-w64-mingw32-gcc.exe -std=gnu99 -Wall -Werror ^
-DINITGUID -DWINVER=NTDDI_WIN10 -DWIN32_LEAN_AND_MEAN ^
-e "DllMain" -s -Oz -shared -municode -nostdlib -mstackrealign ^
-Wl,--kill-at,--gc-sections,--exclude-all-symbols -Wno-dll-attribute-on-redeclaration ^
"main.c" "obj\main.o" -ld3d9 -lonecore -o "bin\dinput8.dll"