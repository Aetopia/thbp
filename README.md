# Touhou Borderless Patch

Adds borderless fullscreen support to Touhou games.

## Games

> [!TIP]
> This patch might work with other entires!
> - This list shows games that are confirmed to work.
> - Feel free to test it with other games & report back!

> [!CAUTION]
> All games prior to Touhou 10 aren't supported due to D3D8.
> - To fix this, install [`d3d8to9`](https://github.com/crosire/d3d8to9/releases/latest) to promote D3D9.
> - Any games that use D3D8 will now be compatible. 

- Touhou 9
- Touhou 9.5
- Touhou 10
- Touhou 12
- Touhou 15
- Touhou 20

<div align="center">

https://github.com/user-attachments/assets/9d57d8a5-3f28-4521-9ebc-fdd175e5f498

</div>

### Tweaks
- The presentation rate is constrained to 60 FPS on purpose.
- Desktop Window Manager opts in for [MMCSS scheduling](https://learn.microsoft.com/windows/win32/direct3darticles/direct3d-9ex-improvements#summary-of-programming-recommendations-for-frame-synchronization).
- DPI awareness is now enabled to avoid [bitmap stretching](https://learn.microsoft.com/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows#dpi-unaware).
- DirectInput no longer [blocks the Windows key](https://learn.microsoft.com/previous-versions/windows/desktop/ee417921(v=vs.85)) when in use.

## Usage

> [!IMPORTANT]
> - The patch will promote everything to borderless.
> - Use the highest available resolution when playing.

- [Download](https://github.com/Aetopia/thbp/releases/latest) the latest release of `thbp`.
- Locate a supported game on your system.
- Place `dinput8.dll` in the game's install folder.

## Build
1. Install [MinGW (x86)](https://www.mingw-w64.org) on your system.
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.

## References
- https://github.com/astral4/neopatch
- https://github.com/GensokyoClub/th06
- https://github.com/GensokyoClub/th08
- https://github.com/khang06/OpenInputLagPatch
