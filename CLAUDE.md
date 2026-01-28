# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a monorepo for the BAG Programmierrunde ESP32 microcontroller project. The project is developing firmware for an ESP32-S3 based handheld gaming device with an ST7789 LCD display and button inputs. The current implementation is a T-Rex runner game (Dino Game).

## Repository Structure

The repository root contains the active project:

- **components/engine/**: Reusable game engine component library
  - `core/`: System initialization, scene management, FreeRTOS task orchestration
  - `display/`: ST7789 LCD driver and display abstraction
  - `gal/`: Graphics abstraction layer (GAL) for rendering primitives
  - `input/`: Button input handling with debouncing
  - `engine.h`: Single include header that exposes all engine APIs

- **main/**: Application entry point and game implementation
  - `main.cpp`: Entry point (`app_main()`) that initializes System and starts GameScene
  - `game/`: Game-specific code (GameScene, Dino, assets)
  - `CMakeLists.txt`: Registers main component with engine dependency

- **build/**: Generated build artifacts (gitignored)
- **sdkconfig**: ESP-IDF configuration (tracked in git)
- **docs/**: Documentation and hardware specs
- **DinoGame/**: Legacy directory structure (no longer active)
- **.clang-format**: Code style configuration

## Build System and Commands

The project uses ESP-IDF (Espressif IoT Development Framework) v5.5.1 with CMake and the ESP32-S3 target chip. Development is done in Visual Studio Code with the ESP-IDF extension.

### Essential Commands

All commands run from the **repository root** (not DinoGame/):

**Build the project:**
```bash
idf.py build
```

**Flash to ESP32:**
```bash
idf.py flash
```

**Monitor serial output:**
```bash
idf.py monitor          # Ctrl+] to exit
```

**Combined build, flash, and monitor:**
```bash
idf.py build flash monitor
```

**VS Code shortcuts:** The ESP-IDF extension provides toolbar buttons at the bottom of VS Code:
- Wrench icon: Build Project
- Lightning icon: Flash Device
- Monitor icon: Monitor Device
- Flame icon: Build, Flash and Monitor (recommended workflow)

### Configuration Commands

```bash
idf.py set-target esp32s3    # Set target chip (run once per environment)
idf.py menuconfig            # Configure project settings (modifies sdkconfig)
idf.py clean                 # Clean build artifacts
```

### Troubleshooting

If builds fail unexpectedly, delete the `build/` directory and rebuild. This often resolves CMake caching issues.

## Architecture

The codebase uses a scene-based game engine architecture with clear separation between reusable engine code and game-specific logic.

### Core Architecture Components

**System (components/engine/core/system.h):**
- Singleton that manages the entire application lifecycle
- Initializes hardware (display, GAL, input)
- Creates two FreeRTOS tasks: main task (rendering) and game task (logic/input)
- Owns the current scene via `PolyValue<Scene>` (type-erased storage)
- Provides `System::setScene<SceneT>(...)` for scene transitions using delayed execution

**Scene (components/engine/core/scene.h):**
- Abstract base class for game states (gameplay, menu, game over, etc.)
- Pure virtual `update(float deltaTime, bool buttonPressed)` called every frame
- Virtual `start()` called when scene becomes active
- Scenes are created/destroyed dynamically during transitions

**GAL (Graphics Abstraction Layer):**
- Double-buffered rendering system
- Provides drawing primitives: `draw_bytes_at()`, `fill_background()`, `switch_frame_buffers()`
- Abstracts the ST7789 display hardware

### Control Flow

1. **Initialization:** `app_main()` in `main.cpp` calls `System::init()` to set up hardware
2. **Scene Start:** `System::start<GameScene>()` creates the initial scene and enters main loop
3. **Dual-Task Architecture:**
   - **Main task:** Sends frame buffer to display, waits for game task, swaps buffers (render loop)
   - **Game task:** Updates scene logic with delta time and button state, notifies main task
4. **Scene Transitions:** Call `System::setScene<NewScene>(args...)` from within any scene's `update()` method. The new scene is constructed at the start of the next frame via `delayedSceneSwitchFunc`

### Current Game Implementation

**GameScene (main/game/game_scene.h):**
- Concrete scene implementing the Dino runner game
- Manages scrolling ground textures, dino character, and start screen
- Uses button input for jumping and starting the game

### Adding New Engine Modules

When adding new `.cpp` files to `components/engine/`, register them in `components/engine/CMakeLists.txt`:

```cmake
idf_component_register(
    SRCS
    "display/driver.cpp"
    "display/st7789.cpp"
    "your_new_module/new_file.cpp"  # Add here
    ...
    REQUIRES spi_flash esp_driver_spi esp_driver_gpio esp_timer
    INCLUDE_DIRS "."
)
```

Similarly, when adding game source files to `main/`, register them in `main/CMakeLists.txt`:

```cmake
idf_component_register(SRCS
    "main.cpp"
    "game/game_scene.cpp"
    "game/your_new_file.cpp"  # Add here
    REQUIRES esp_timer esp_driver_gpio engine
    INCLUDE_DIRS "."
)
```

## Code Style

The project uses `.clang-format` configuration:
- 4-space indentation
- 140-character line limit
- Left-aligned pointers (`int* ptr`)
- PascalCase for classes/types (`GameScene`, `System`)
- camelCase for functions/methods (`updatePlayer`, `getSurvivalSecs`)
- snake_case for files (`game_scene.cpp`)

Format code before committing:
```bash
clang-format -i path/to/file.cpp
```

## Hardware Configuration

- **Target board**: ESP32-S3-DevKitC-1 N16R8
- **Display**: 2.8" TFT LCD 240x320 with ST7789 controller (SPI interface)
- **Buttons**: GPIO 13 and GPIO 14 (see `system.cpp:69-70`)
- **Display orientation**: Landscape mode (320x240 after rotation)
- **Communication protocol**: UART for flashing/monitoring (VS Code bottom bar must show "UART", not "JTAG")

Pin mappings are configured in `sdkconfig`. The complete pinout diagram is in `docs/media/esp32-s3_devkitc-1_pinlayout_v1.1.jpg`.

## Asset Management

Graphics assets are stored as byte arrays in `main/game/assets/`:
- `font.h`: Bitmap font for text rendering
- `dino.h`: Dino character sprites
- `ground.h`: Ground obstacle sprites (cactus variants)
- `color.h`: Color definitions (FOREGROUND_COLOR, BACKGROUND_COLOR)

Assets use bit-packed pixel format compatible with `GAL::draw_bytes_at()`.

## Development Workflow

Testing is done directly on hardware:

1. Make code changes
2. Build and flash: `idf.py build flash monitor`
3. Observe serial logs (`ESP_LOGI`, `ESP_LOGW`) and visual output on the LCD
4. Iterate

The system logs frametime statistics every second for performance monitoring.
