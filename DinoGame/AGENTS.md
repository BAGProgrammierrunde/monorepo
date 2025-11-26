# Repository Guidelines

## Project Structure & Module Organization
- Firmware entry point lives in `main/main.cpp`; gameplay logic is under `main/game/`.
- Reusable subsystems live in `components/engine/` (`display/`, `input/`, `system/`, etc.). Update `components/engine/CMakeLists.txt` when adding modules.
- Build artifacts land in `build/` or `cmake-build-debug/` (generated); configuration is tracked in `sdkconfig`. Docs and assets belong in `docs/`.

## Build, Flash, and Development Commands
- `idf.py set-target esp32s3` once per environment to align the toolchain.
- `idf.py build` compiles the project; `idf.py flash` uploads to the board; `idf.py monitor` attaches a serial console. Combine as `idf.py flash monitor` while iterating.
- `idf.py clean` clears generated outputs if builds get stale. On Windows, run these via `idf.bat` after activating the ESP-IDF environment.

## Coding Style & Naming Conventions
- C++ follows `.clang-format`: 4-space indent, 140-column limit, left-aligned pointers, and no compressed one-line control blocks.
- Prefer PascalCase for types/classes, camelCase for methods/functions, and snake_case for files and constants. Keep headers minimal and avoid unused includes.
- Run `clang-format -i <files>` before committing to keep diffs tidy.

## Testing Guidelines
- No dedicated unit test suite yet; validate changes on hardware using `idf.py flash monitor` and watch for boot logs and gameplay regressions.
- Add small, focused tests when introducing new subsystems (e.g., input debouncing, timing utilities). Name test files `*_test.cpp` and place near the code under test.

## Commit & Pull Request Guidelines
- Use short, imperative commit subjects (e.g., "Add dino jump animation"); keep body lines under 72 chars when needed.
- PRs should describe behavior changes, reference related issues/tasks, and include screenshots or serial logs when UI/output changes.
- Call out build/test steps you ran (e.g., `idf.py build`, `idf.py flash monitor`) so reviewers can reproduce quickly.

## Configuration & Hardware Notes
- Keep `sdkconfig` changes intentional; document key pin mappings and clock settings in PR descriptions.
- If you introduce new peripherals, add brief wiring notes in `docs/` and guard feature flags so the default build remains board-safe.
