# Touhou Borderless Patch

Adds resizable window & borderless fullscreen support to Touhou games.

## Games

> [!TIP]
> This patch might work with other entires!
> - This list shows games that are confirmed to work.
> - Feel free to test it with other games & report back!

|Entry|Name|API|
|:-|:-|:-|
|9|Phantasmagoria of Flower View|D3D8|
|9.5|Shoot the Bullet|D3D8|
|10|Mountain of Faith|D3D9|
|12|Undefined Fantastic Object|D3D9|
|15|Legacy of Lunatic Kingdom|D3D9|
|18|Unconnected Marketeers|D3D9|
|20|Fossilized Wonders|D3D9|

> [!CAUTION]
> For games that use D3D8, please install [d3d8to9](https://github.com/crosire/d3d8to9/releases/latest).
> - This will improve compatibility on modern systems.
> - This will allow the patch to function with these games.

### Showcase
https://github.com/user-attachments/assets/9d57d8a5-3f28-4521-9ebc-fdd175e5f498

### Tweaks
- The presentation rate is constrained to 60 FPS on purpose.
- DPI awareness is now enabled to avoid [bitmap stretching](https://learn.microsoft.com/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows#dpi-unaware).
- DirectInput no longer [blocks the Windows key](https://learn.microsoft.com/previous-versions/windows/desktop/ee417921(v=vs.85)) when in use.

## Usage

> [!IMPORTANT]
> The patch will promote screen modes as follows:
> |Game|→|Patch|
> |:-:|:-:|:-:|
> |Windowed|→|Resizable Window|
> |Fullscreen|→|Borderless Fullscreen|
>
> Use the highest available resolution when playing.

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
