# d3d8-fps-limiter

A lightweight frame rate limiter for Direct3D 8 games.

Many old D3D8 games have broken VSync implementation on modern Windows. This wrapper DLL provides accurate frame rate limiting without relying on VSync.

## Features

- Works with any D3D8 game
- High-precision timing using QueryPerformanceCounter
- Minimal CPU usage (Sleep + short busy-wait)
- Frame time accumulation to prevent drift
- Automatic catch-up prevention to avoid stutter
- Single-file configuration

## Usage

1. Copy `d3d8.dll` to your game folder (next to the game .exe)
2. Create `d3d8_limiter.ini` in the same folder:

```ini
[limiter]
fps=60
```

3. Launch the game

Set `fps=0` to disable the limiter.

## Building

Requires 32-bit MinGW-w64.

```cmd
build.bat
```

Or manually:

```cmd
gcc -c main.c -o main.o -O2
gcc -shared -o d3d8.dll main.o d3d8.def -lwinmm
```

## How It Works

The wrapper intercepts `Direct3DCreate8` and hooks `IDirect3DDevice8::Present` using inline patching. After each Present call, it waits until the target frame time has elapsed.

Timing strategy:
1. `timeBeginPeriod(1)` for 1ms Sleep resolution
2. Sleep for most of the wait time
3. Busy-wait for the final ~1.5ms for accuracy
4. Accumulate frame times to prevent long-term drift

## License

Public Domain / CC0
