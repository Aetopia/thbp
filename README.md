# Touhou Borderless Patch

Adds resizable window & borderless fullscreen support to Touhou games with aspect ratio preservation.

## Games

> [!TIP]
> This patch might work with other entires!
> - This list shows games that are confirmed to work.
> - Feel free to test it with other games & report back!

<div align='center'>

|Game|Preview|
|:-:|:-:|
|Touhou 10|<img width="320" height="180" src="img/th10.gif"/>|
|Touhou 12|<img width="320" height="180" src="img/th12.gif"/>|

</div>

### Tweaks

- The game has Desktop Window Manager opt in for [MMCSS scheduling](https://learn.microsoft.com/windows/win32/api/dwmapi/nf-dwmapi-dwmenablemmcss).
- The game will no longer [block the Windows key](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417921(v=vs.85)) when DirectInput is being used.
- The game now uses a higher timer resolution which provides [better V-Sync quality](https://learn.microsoft.com/windows/win32/direct3d9/d3dpresent#remarks).

## Usage

- [Download](https://github.com/Aetopia/thbp/releases/latest) the latest release of `thbp`.
- Locate a supported game on your system.
- Place `dinput8.dll` in the install directory.

## Build
1. Install [CMake](https://cmake.org) & [MinGW (x86)](https://www.mingw-w64.org).
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.