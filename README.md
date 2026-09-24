# Touhou Borderless Patch

Adds borderless fullscreen support to Touhou games with aspect ratio preservation.

## Games

> [!TIP]
> This patch might work with other entires!
> - This list shows games that are confirmed to work.
> - Feel free to test it with other games & report back!

- [Touhou 10](img/th10.gif)
- [Touhou 12](img/th12.gif)
- [Touhou 15](img/th15.gif)

### Tweaks

- The game has Desktop Window Manager opt in for [MMCSS scheduling](https://github.com/djdallmann/GamingPCSetup/blob/master/CONTENT/RESEARCH/WINSERVICES/README.md#q-can-you-take-advantage-of-the-mmcss-boosted-csrss-and-dwm-thread-priorities-dwmenablemmcss-while-using-a-fullscreen-exclusive-application).
- The game will no longer [block the Windows key](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417921(v=vs.85)) when DirectInput is being used.

## Usage

> [!CAUTION]
> The patch promotes windowed mode to borderless fullscreen.
> - Open a supported game's configuration tool.
> - Select windowed mode & the highest available resolution.
> 
> **Using fullscreen isn't supported & the patch will refuse to work.**

- [Download](https://github.com/Aetopia/thbp/releases/latest) the latest release of `thbp`.
- Locate a supported game on your system.
- Place `dinput8.dll` in the install directory.

## FAQ
#### Why does the patch require windowed mode?
The patch avoids intercepting any game functionality.
- Makes it compatible with the Disk & Steam versions.
- Removes the need to handle game specific edge cases.

Enforcing windowed mode allows the patch to safely promote borderless fullscreen.
- Depending on the game, fullscreen might disengage critical features.
- For example, a game might rely on V-Sync instead of its framerate limiter.

#### Why not use vpatch for "borderless fullscreen"?
Though vpatch allows one to [run the games at any resolution](https://maribelhearn.com/faq/graphics).

- It requires the user to intervene & configure it, 
- Additionally you must somewhat compromise on fullscreen.

This patch solely serves as a plug & play solution for borderless fullscreen.

## Build
1. Install [MinGW (x86)](https://www.mingw-w64.org).
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.