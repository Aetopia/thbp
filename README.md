# Touhou Borderless Patch

Adds resizable window & borderless fullscreen support to Touhou games with aspect ratio preservation.

## Games

> [!TIP]
> This patch might work with other entires!
> - This list shows games that are confirmed to work.
> - Feel free to test it with other games & report back!

- Touhou 10
- Touhou 12
- Touhou 15

<div align="center">

[![](https://img.youtube.com/vi/19nlS8QVjpo/maxresdefault.jpg)](https://youtu.be/19nlS8QVjpo)

</div>

### Tweaks
- A spin-wait framerate limiter is asserted to ensure 60 FPS.
- [High DPI Awareness is enabled](https://learn.microsoft.com/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows) to avoid bitmap stretching.
- Desktop Window Manager opts in for [MMCSS scheduling](https://github.com/djdallmann/GamingPCSetup/blob/master/CONTENT/RESEARCH/WINSERVICES/README.md#q-can-you-take-advantage-of-the-mmcss-boosted-csrss-and-dwm-thread-priorities-dwmenablemmcss-while-using-a-fullscreen-exclusive-application).
- DirectInput no longer [blocks the Windows key](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417921(v=vs.85)) when in use.

## Usage

> [!TIP]
> The patch will promote screen modes as follows:
> |Game|→|Patch|
> |:-:|:-:|:-:|
> |Windowed|→|Resizable Window|
> |Fullscreen|→|Borderless Fullscreen|
>
> Use the highest available resolution when playing.

- [Download](https://github.com/Aetopia/thbp/releases/latest) the latest release of `thbp`.
- Locate a supported game on your system.
- Place `dinput8.dll` in the install directory.

## FAQ
#### Is the patch compatible with other mods or patches?
The patch has been only tested against [`thcrap`](https://github.com/thpatch/thcrap) & works flawless.
- Other mods or patches might require testing.
- Feel free to consult & report any issues that occur.

#### Why not use vpatch for "borderless fullscreen"?
Though vpatch allows one to [run the games at any resolution](https://maribelhearn.com/faq/graphics).

- It requires the user to intervene & configure it, 
- Additionally you must somewhat compromise on fullscreen.

This patch solely serves as a plug & play solution for borderless fullscreen.

## Build
1. Install [MinGW (x86)](https://www.mingw-w64.org).
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.