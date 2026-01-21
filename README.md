# d3d8-fps-limiter

A lightweight frame rate limiter for Direct3D 8 games.

Many old D3D8 games have broken VSync implementation on modern Windows. This wrapper DLL provides accurate frame rate limiting without relying on VSync.

## Features

- Works with any D3D8 game
- High-resolution waitable timer on Windows 10 1803+ (automatic fallback for older OS)
- High-precision timing using QueryPerformanceCounter
- Minimal CPU usage
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
gcc -c main.c -o main.o -O2
gcc -shared -o d3d8.dll main.o d3d8.def -lwinmm
```

## How It Works

The wrapper intercepts `Direct3DCreate8` and hooks `IDirect3DDevice8::Present` using inline patching. After each Present call, it waits until the target frame time has elapsed.

Timing strategy (Windows 10 1803+):
1. Use high-resolution waitable timer (`CREATE_WAITABLE_TIMER_HIGH_RESOLUTION`)
2. Accurate sub-millisecond sleep with minimal CPU usage

Timing strategy (older Windows):
1. `timeBeginPeriod(1)` for 1ms Sleep resolution
2. Sleep for most of the wait time
3. Busy-wait for the final ~2ms for accuracy

Common:
- Accumulate frame times to prevent long-term drift
- Reset timing when falling behind to prevent stutter

