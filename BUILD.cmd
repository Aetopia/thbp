@echo off

cd "%~dp0" & cd "src"
rd /q /s "bin" & md "bin"
rd /q /s "obj" & md "obj"

i686-w64-mingw32-windres.exe -i "resource.rc" -o "obj\main.o"

i686-w64-mingw32-gcc.exe -Wall -Werror -municode -DINITGUID -DWINVER=NTDDI_WIN10 ^
-s -Oz -shared -nostdlib -mstackrealign -Wl,--kill-at,--gc-sections,--exclude-all-symbols ^
-e "DllMain" "main.c" "obj\main.o" -ld3d9 -lonecore -o "bin\dinput8.dll" -Wno-dll-attribute-on-redeclaration