@echo off

cd "%~dp0"
cd "src"

rd /q /s "bin"
rd /q /s "obj"

md "bin"
md "obj"

i686-w64-mingw32-windres.exe -i "res.rc" -o "obj\res.o"

i686-w64-mingw32-gcc.exe ^
-nostdlib -s -Oz -shared -e "DllMain" ^
-DUNICODE -DINITGUID -DWIN32_LEAN_AND_MEAN -DWINVER=NTDDI_WIN10 ^
-Wl,--kill-at,--gc-sections,--exclude-all-symbols -Wno-dll-attribute-on-redeclaration ^
"main.c" "obj\res.o" -lkernel32 -luser32 -lgdi32 -ld3d9 -lshlwapi -ldwmapi -o "bin/dinput8.dll"