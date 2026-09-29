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

- The game has Desktop Window Manager opt in for [MMCSS scheduling](https://github.com/djdallmann/GamingPCSetup/blob/master/CONTENT/RESEARCH/WINSERVICES/README.md#q-can-you-take-advantage-of-the-mmcss-boosted-csrss-and-dwm-thread-priorities-dwmenablemmcss-while-using-a-fullscreen-exclusive-application).
- The game will no longer [block the Windows key](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417921(v=vs.85)) when DirectInput is being used.

## Usage

> [!TIP]
> The patch will promote each screen mode as follows:
> - Windowed → Resizable Window
> - Fullscreen → Borderless Fullscreen
>
> Use the highest available resolution when playing.

- [Download](https://github.com/Aetopia/thbp/releases/latest) the latest release of `thbp`.
- Locate a supported game on your system.
- Place `dinput8.dll` in the install directory.

## FAQ
#### Why do my games have high CPU usage?
The patch asserts its own framerate limiter when active.

- A game might disengage its framerate limiter.
- A busy-wait framerate limiter is used for accuracy.

This ensures the games run at 60 FPS regardless of screen mode.

#### Why not use vpatch for "borderless fullscreen"?
Though vpatch allows one to [run the games at any resolution](https://maribelhearn.com/faq/graphics).

- It requires the user to intervene & configure it, 
- Additionally you must somewhat compromise on fullscreen.

This patch solely serves as a plug & play solution for borderless fullscreen.

## Build
1. Install [MinGW (x86)](https://www.mingw-w64.org).
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.