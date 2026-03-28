# 🔴 CPP-Raycast

A first-person raycasting engine built in **C++ with SDL3**, implementing the DDA (Digital Differential Analysis) algorithm. This is a rewrite of the original [C raycasting engine](https://github.com/cdron9/raycast-engine-dda), migrating to C++ for better code organisation and as a stepping stone into deeper C++ development.

![C++](https://img.shields.io/badge/language-C%2B%2B17-blue) ![SDL3](https://img.shields.io/badge/library-SDL3-orange) ![License](https://img.shields.io/badge/license-MIT-green)

---

## What Changed from the C Version

| Feature | C Version | C++ Version |
|---------|-----------|-------------|
| Language | C | C++17 |
| Player | `struct` with bare fields | `class Player` with constructor + `handleInput()` method |
| Timing | `SDL_GetTicks()` (ms) | `std::chrono::high_resolution_clock` (sub-ms precision) |
| Constants | `#define` macros | `constexpr int` |
| Null pointer | `NULL` | `nullptr` |
| Build | manual gcc | makefile with auto-detected Homebrew prefix + `-O2` |
| Rotation | In main loop | Still in main loop (next step to encapsulate) |

The core DDA raycasting algorithm, map layout, rendering, minimap, and wall shading are identical — this is a structural refactor, not a feature rewrite.

---

## Features

- DDA raycasting with correct perspective projection (no fisheye)
- Distance-based wall shading with side-face darkening (70% brightness on Y-axis hits)
- Smooth movement and rotation via `std::chrono` delta time
- Collision detection against the tile map
- Minimap overlay in the top-left corner
- `Player` class encapsulating state and input handling
- `constexpr` constants, `nullptr`, and C++17 throughout

---

## Controls

| Key | Action |
|-----|--------|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Rotate left |
| `D` | Rotate right |

---

## Building

### Requirements

- A C++17 compiler (`g++` or `clang++`)
- [SDL3](https://github.com/libsdl-org/SDL) installed (via Homebrew on macOS, or system package manager on Linux)

### Build

```bash
make
```

The makefile auto-detects your Homebrew prefix (`brew --prefix`), so SDL3 paths are resolved automatically on macOS. The output binary is named `raycast`.

```bash
make clean   # remove the binary
```

### Linux

If you're on Linux (no Homebrew), update the makefile paths manually:

```makefile
SDL3_LIB     := /usr/local/lib
SDL3_INCLUDE := /usr/local/include
```

Or just compile directly:

```bash
g++ -std=c++17 -Wall -O2 main.cpp -o raycast -lSDL3 -lm
```

---

## Project Structure

```
.
├── main.cpp    # Everything: Player class, map, DDA raycaster, minimap, game loop
└── makefile    # g++/c++17 build with auto Homebrew detection
```

### Player Class

```cpp
class Player {
public:
    float x, y;           // world position
    float dirX, dirY;     // direction vector
    float planeX, planeY; // camera plane (controls FOV)
    float speed, rotSpeed;

    Player(float startX, float startY);
    void handleInput(const bool* keys, float deltaTime, const int map[][MAP_SIZE]);
};
```

Movement and collision detection live in `handleInput()`. Rotation is still handled directly in the game loop — a natural next step for further encapsulation.

---

## How It Works

The DDA algorithm steps rays through the tile grid one cell boundary at a time. For each screen column, a ray is cast and the perpendicular wall distance is used to compute slice height — avoiding the fisheye effect that Euclidean distance would cause.

```
perpendicularWallDist = (side == 0) ? (sDX - dDX) : (sDY - dDY)
sliceHeight = SCREEN_HEIGHT / perpendicularWallDist
```

Wall brightness falls off with distance:

```cpp
float intensity = 1.0f / (1.0f + pWD * pWD * 0.03f);
```

Y-axis (side) walls are rendered at 70% brightness to add depth. Timing uses `std::chrono::high_resolution_clock` for sub-millisecond delta time accuracy.

---

## Configuration

Constants at the top of `main.cpp`:

| Constant | Default | Description |
|----------|---------|-------------|
| `MAP_SIZE` | `20` | Grid dimensions (NxN) |
| `TILE_SIZE` | `64` | World units per tile |
| `SCREEN_WIDTH` | `1280` | Render width in pixels |
| `SCREEN_HEIGHT` | `960` | Render height in pixels |

Player properties set in the constructor:

```cpp
Player player(100.0f, 100.0f); // starting position

// Inside Player::Player():
speed    = 300.0f;  // pixels/sec
rotSpeed = 3.0f;    // radians/sec
planeY   = 0.66f;   // ~66° FOV
```

---

## Map Format

The map is a 20×20 grid — `1` = wall, `0` = open space. Edit the `map` array in `main.cpp` to design your own layout. Keep the outer border as walls to prevent out-of-bounds access.

---

## References

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html)
- [SDL3 Documentation](https://wiki.libsdl.org/SDL3/FrontPage)
- [Original C version](https://github.com/cdron9/raycast-engine-dda)

---

## License

MIT — do whatever you want with it.
