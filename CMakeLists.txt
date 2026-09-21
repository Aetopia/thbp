cmake_minimum_required(VERSION 3.10)
project("" LANGUAGES C)

set(CMAKE_STANDARD_LIBRARIES "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/obj")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/obj")

add_compile_options(-Oz -Wno-dll-attribute-on-redeclaration)
add_compile_definitions(UNICODE INITGUID WIN32_LEAN_AND_MEAN WINVER=NTDDI_WIN10)
add_link_options(-Wl,--kill-at,--gc-sections,--exclude-all-symbols -nostdlib -s)

add_library(dinput8 SHARED "src/main.c" "src/res.rc")

target_link_options(dinput8 PRIVATE -e DllMain)
target_link_libraries(dinput8 PRIVATE kernel32 d3d9 user32 gdi32 dwmapi shlwapi)