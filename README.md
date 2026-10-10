# Touhou Borderless Patch

Adds resizable window & borderless fullscreen support to Touhou games with aspect ratio preservation.

## Games

> [!TIP]
> This patch might work with other entires!
> - This list shows games that are confirmed to work.
> - Feel free to test it with other games & report back!

> [!CAUTION]
> All games prior to Touhou 10 aren't supported due to D3D8.
> - To fix this, install [`d3d8to9`](https://github.com/crosire/d3d8to9) to promote D3D9.
> - Any games that use D3D8 will now be compatible. 

- Touhou 9
- Touhou 9.5
- Touhou 10
- Touhou 12
- Touhou 15
- Touhou 20

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
- Place `dinput8.dll` in the install folder.

## Build
1. Install [MinGW (x86)](https://www.mingw-w64.org) on your system.
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.

## References
- https://github.com/astral4/neopatch
- https://github.com/GensokyoClub/th06
- https://github.com/GensokyoClub/th08
- https://github.com/khang06/OpenInputLagPatch