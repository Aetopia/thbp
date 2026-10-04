# Touhou Borderless Patch

Adds resizable window & borderless fullscreen support to Touhou games with aspect ratio preservation.

## Games

> [!TIP]
> This patch might work with other entires!
> - This list shows games that are confirmed to work.
> - Feel free to test it with other games & report back!

- Touhou 9.5 <sup>[*](#how-to-use-the-patch-with-games-prior-to-touhou-10)</sup>
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
- Place `dinput8.dll` in the install folder.

## FAQ
#### Is the patch compatible with other mods or patches?
The patch has been only tested against [`thcrap`](https://github.com/thpatch/thcrap).
- Other mods or patches might require testing.
- Feel free to consult & report any issues that occur.

#### How to use the patch with games prior to Touhou 10?
All games prior to Touhou 10 use D3D8 which isn't supported.
- The patch intercepts D3D9 to provide its features.

To fix this, the games must use D3D9 via a wrapper:
|Wrapper|Usage|
|:-:|:-:|
|[`d3d8to9`](https://github.com/crosire/d3d8to9)|D3D8 → D3D9|
|[`dxvk`](https://github.com/doitsujin/dxvk)|D3D8 → D3D9 → Vulkan|

> [!TIP]
> - For modern systems, use `dxvk`.
> - For compatibility, use `d3d8to9`.

#### Why not use vpatch for "borderless fullscreen"?
Though vpatch allows one to [run the games at any resolution](https://maribelhearn.com/faq/graphics).

- It requires the user to intervene & configure it, 
- Additionally you must somewhat compromise on fullscreen.

## Build
1. Install [MinGW (x86)](https://www.mingw-w64.org).
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.