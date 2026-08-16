# HighGrow-420

Source code of the original game, developed by my close friend

<img width="800" height="600" alt="image" src="https://github.com/user-attachments/assets/8f1bd1b3-46b7-4c1e-910f-471ea746a03d" />

Now people smarter than me could help me compile this XD

**Until we did.** It now builds on modern Windows with MinGW — including the room
background images that older attempts failed to show.

## Why it didn't compile before (and what changed)

The original (1998) project targets Win32 + **MFC**, which is not freely buildable
in modern toolchains. The image code (`JPGView.cpp` / `Picture.cpp`) was the only
MFC-dependent part. A previous build attempt worked around this with
`JPGViewStub.c` — a no-op stub — which is why those builds started fine but showed
**no images**.

This tree removes the MFC dependency properly:

| File | Change |
| --- | --- |
| `HighGrow 420/StdAfx.h` | MFC precompiled header → minimal non-MFC stub (only `JPGView.cpp`/`Picture.cpp` include it) |
| `HighGrow 420/Picture.h` / `Picture.cpp` | MFC/ATL `CPicture` → pure Win32 + **GDI+** port (same public API; images load via COM `IStream`, render to an `HDC`) |
| `HighGrow 420/JPGView.cpp` | 3 `CDC Attach/Render/Detach` call sites → direct `Render(hdc, …)` |
| `HighGrow 420/highgrow.RC` | Main menu resource renamed `MENU` → `MAINMENURESID` (windres rejects the name `MENU`) |
| `HighGrow 420/highgrow.C` | `lpszMenuName` updated to match the rename |
| `HighGrow 420/Internet.c` | Commented-out `#include<iostream.h>` (that 1990s header doesn't exist in MinGW; nothing needs it) |
| `HighGrow 420/DIBAPI.H` | Dead `PrintWindow` declaration renamed to `DIBPrintWindow` (conflicted with the real Win32 one) |

`JPGViewStub.c` is **not compiled** — the real `JPGView.cpp` is. The 4 room
backgrounds (`Empty room.jpg`, `Attic.jpg`, `Basement.jpg`, `Bedroom.jpg`) and all
other art/sound assets are tracked in this repo, so a fresh clone is self-contained:
nothing else to download.

## Building (Windows, 32-bit)

Requirements:

- **MSYS2** (<https://www.msys2.org>), with the 32-bit toolchain in the `mingw32`
  environment: `pacman -S mingw-w64-i686-gcc`
- **Windows 10/11 SDK** (for `mt.exe` — used to embed the original manifest).
  The build script expects it at `C:\hgkit\bin\10.0.19041.0\x86\mt.exe`; edit the
  `MT=` line in `build_mingw32.bat` if yours lives elsewhere.

Then:

```bat
build_mingw32.bat
```

Output lands in `_build\`:

- `HighGrow32.exe` — the 32-bit game (140 resources, 4 embedded room images)
- `libgcc_s_dw2-1.dll`, `libstdc++-6.dll`, `libwinpthread-1.dll` — GCC runtime,
  copied next to the exe so it runs without MSYS2 on PATH

### Why 32-bit?

HighGrow is a 1998 Win32 game and its `LoadLibrary()` loads **32-bit** `ROBBIE.DLL` and
`COMMENTS.DLL` at runtime (the Rasta Robbie comments and the growroom animations). 
A 64-bit exe can't load those, and the original manifest declares
`processorArchitecture="X86"`. Build 32-bit or the game can't play.

### Build notes / gotchas

- The script keeps `C:\msys64\mingw32\bin` **first on PATH on purpose**. If a
  64-bit MinGW (e.g. Git's own `mingw64`) comes earlier, the 32-bit compiler dies
  with `0xC000007B` (it loads a 64-bit `zlib1.dll`).
- Sources compile with `-fcommon` — this is 1998 C, with tentative global
  definitions in several files.
- IDE/LSP diagnostics (clangd) may show thousands of phantom errors
  (`strcpy`/`sprintf`/`GdiplusStartup` undeclared, `excpt.h` missing). That is a
  broken MSVC header installation on the build machine, **not** a source problem —
  if `build_mingw32.bat` prints `BUILD OK`, trust the build.
- File list follows `HIGHGROW.dsp`: 35 C files + `highgrow.C` + `highgrow.RC` +
  `JPGView.cpp` + `Picture.cpp`. `SEEDVALS.C` / `ccscode.c` are not part of the
  retail build; `JPGViewStub.c` is superseded.
