cmake_minimum_required(VERSION 3.10)
project("" LANGUAGES C)

set(CMAKE_STANDARD_LIBRARIES "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/obj")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/obj")

include(FetchContent)
FetchContent_Declare(minhook GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git GIT_TAG v1.3.4)
FetchContent_MakeAvailable(minhook)

add_compile_definitions(INITGUID WIN32_LEAN_AND_MEAN WINVER=NTDDI_WIN10)
add_compile_options(-municode -mstackrealign -Oz -Wno-dll-attribute-on-redeclaration)
add_link_options(-Wl,--kill-at,--gc-sections,--exclude-all-symbols,--wrap=memcpy,--wrap=memset -nostdlib -s)

add_library(dinput8 SHARED "src/main.c" "src/res.rc")

target_link_options(dinput8 PRIVATE -e DllMain)
target_link_libraries(dinput8 PRIVATE minhook kernel32 d3d9 user32 gdi32 dwmapi mfplat)