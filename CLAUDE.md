# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a monorepo for the BAG Programmierrunde ESP32 microcontroller project. The main focus is developing firmware for an ESP32-based handheld gaming device with multiple hardware components (displays, buttons, joysticks, buzzers, LEDs, potentiometers, etc.). The primary active project is **DinoGame**, a T-Rex runner game implementation.

## Repository Structure

- **DinoGame/**: Main active firmware project for ESP32-S3
  - `main/`: Application entry point and game-specific code
    - `main.cpp`: Entry point that initializes System and DinoGame
    - `game/`: Game-specific implementation (DinoGame, GameScene)
  - `components/engine/`: Reusable engine subsystems (component library)
    - `display/`: ST7789 display driver
    - `gal/`: Graphics abstraction layer
    - `input/`: Button input handling
    - `system/`: Core system (Scene, Game abstractions, FreeRTOS tasks)
    - `game/`: Game interface definitions
  - `scripts/`: Python utilities (font generation tools)
  - `docs/`: Documentation
  - `build/`: Generated build artifacts (not tracked)
  - `sdkconfig`: ESP-IDF configuration (tracked)

- **main/**: Separate older project using different structure
- **Hardware/**: PCB designs and schematics
  - `kicad/`: KiCad project files for the console PCB
  - `case/`: Physical case designs
- **OLD/**: Archived code
- **DevDocs/**: Development documentation and command references

## Build System and Commands

The project uses ESP-IDF (Espressif IoT Development Framework) with CMake.

### Setup Requirements
- ESP-IDF v5.4+ (currently v5.5.1 recommended)
- Target chip: **ESP32-S3**
- Development in Visual Studio Code with ESP-IDF extension

### Essential Commands

All commands should be run from the `DinoGame/` directory after activating ESP-IDF environment.

**Windows:**
```cmd
cd DinoGame
idf.bat  # Activates ESP-IDF environment (custom wrapper)
idf.py build
idf.py flash
idf.py monitor
idf.py flash monitor  # Combined flash and monitor
```

**Mac/Linux:**
```bash
source ~/esp/esp-idf/export.sh  # Activate ESP-IDF
cd DinoGame
idf.py build
idf.py flash
idf.py monitor
idf.py flash monitor
```

### Other Useful Commands
- `idf.py set-target esp32s3` - Set target chip (once per environment)
- `idf.py clean` - Clean build artifacts when builds get stale
- `idf.py menuconfig` - Configure project settings (modifies sdkconfig)

### Troubleshooting
If builds fail unexpectedly, delete the `build/` directory and rebuild. This often resolves caching issues.

## Architecture

### Game Engine Architecture

The codebase follows a component-based architecture with separation between engine and game logic:

1. **System Layer** (`components/engine/system/`):
   - `System`: Manages FreeRTOS tasks, initializes hardware, runs the main game loop
   - `Scene`: Abstract base class for game states (menu, gameplay, game over, etc.)
   - `Game`: Abstract base class for game implementations

2. **Game Implementation** (`main/game/`):
   - `DinoGame`: Concrete Game class that initializes and starts the T-Rex runner
   - `GameScene`: Concrete Scene that handles gameplay update loop (delta time, button input)

3. **Hardware Abstraction** (`components/engine/`):
   - `display/`: ST7789 LCD driver (SPI communication)
   - `gal/`: Graphics primitives and rendering
   - `input/`: Button debouncing and input handling

### Control Flow
1. `app_main()` in `main.cpp` creates System and DinoGame instances
2. `System::init(game)` sets up hardware and FreeRTOS tasks
3. `System::run()` calls `game->start()` to get initial Scene, then loops calling `scene->update(deltaTime, buttonPressed)`
4. Scenes can return new Scenes to transition between game states

### Adding Engine Modules

When adding new source files to `components/engine/`, update `components/engine/CMakeLists.txt`:
```cmake
idf_component_register(
    SRCS
    "display/st7789.cpp"
    "your_new_module/new_file.cpp"  # Add here
    ...
    REQUIRES spi_flash esp_driver_spi esp_driver_gpio esp_timer
    INCLUDE_DIRS "."
)
```

## Code Style

The project uses `.clang-format` configuration:
- 4-space indentation
- 140-character line limit
- Left-aligned pointers (`int* ptr`)
- PascalCase for classes/types
- camelCase for functions/methods
- snake_case for files

Run `clang-format -i <files>` before committing.

## Hardware Configuration

- **Target board**: ESP32-S3
- **Display**: ST7789 LCD (SPI interface)
- **Communication protocol**: UART (bottom bar in VS Code should show "UART", not JTAG)
- Pin mappings and peripheral configurations are in `sdkconfig`

When introducing new peripherals, document wiring in `docs/` and use feature flags to keep default builds safe.

## Asset Management

Graphics assets are stored as byte arrays in `main/game/assets/`. The `assets.md` file contains sprite data (e.g., cactus sprites) as `uint8_t` arrays with bit-packed pixel data.

Python scripts in `scripts/font_generation/` generate font bitmaps for text rendering.

## Development Workflow

1. Make changes in `DinoGame/` directory
2. Build with `idf.py build`
3. Flash to connected ESP32 with `idf.py flash`
4. Monitor serial output with `idf.py monitor` (Ctrl+] to exit)
5. Watch boot logs and gameplay behavior for validation

No formal unit test suite exists yet. Validate on hardware using serial logs and visual inspection.

## Git Workflow

- Main branch: `main`
- Keep commit messages imperative and concise
- The `.vscode/` directory in DinoGame can be regenerated using ESP-IDF extension ("Add .vscode subdirectory files")
- `build/` directories are gitignored
