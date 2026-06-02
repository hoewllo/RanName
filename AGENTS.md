# AGENTS.md - RandomNamePicker

## Build & Versioning
- `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release` then `cmake --build build`
- Output binaries at `build/bin/`
- GUI target requires Qt6 (optional). On Ubuntu: `apt install qt6-base-dev qt6-tools-dev`
- Version auto-detected from git: tag `vX.Y.Z` → `X.Y.Z`, else commit msg `X.Y.Z`, else `git log`, else `0.0.0`
- `VERSION` file removed — never create one

## Code Quality
- Format: `clang-format -i src/*.cpp src/**/*.cpp src/**/*.h`
- Cppcheck: `cppcheck --std=c++17 -I src src/`

## CI/CD (`.github/workflows/ci.yml`)
- Triggers: push to `main`/`master`, or any tag matching `v*`
- Platforms: Linux (GCC), Linux ARM64, macOS (Clang), Windows (MinGW)
- All checkout steps use `fetch-depth: 0` (needed for version auto-detection from tags)
- `.qm` files not tracked in git; CI regenerates with `lrelease trans/*.ts`
- On Linux: `lrelease` at `/usr/lib/qt6/bin/lrelease` (not in PATH)
- Qt6 packages: `qt6-base-dev` + `qt6-tools-dev` (Ubuntu)
- 2 targets: RandomNamePicker (TUI), RandomNamePickerGUI
- Release created only on tagged pushes (`softprops/action-gh-release`)

## Commit Conventions
- Bare version number (e.g. `2.0.1`) — no `v` prefix, no descriptions
- Tag name = `v` + version (e.g. `v2.0.1`)
- Tags and releases are **never** pushed unless explicitly asked

## Internationalization
- **Console/TUI**: uses `i18n::Localizer` with `--lang en-US|zh-CN` flag. Default: en-US
- **GUI**: uses Qt `tr()` / `.ts` / `.qm`. Language from `data/config.conf` or `--lang` flag
- Config stores language as `[language]= en_US` or `zh_CN`
- To update: edit `.ts` in `trans/`, run `lrelease trans/*.ts` to regenerate `.qm`

## Source Structure
- `src/main.cpp` — TUI entrypoint
- `src/gui/main.cpp` — GUI entrypoint
- `src/core/` — shared: `randomizer`, `name_list`, `config_manager`
- `src/ui/` — TUI screens: `tui` (interactive picker), `console` (menu), `setup_tui` (config)
- `src/gui/` — Qt GUI: `main_window` (+ auto-generated `ui_main_window.h` from `.ui`)
- `src/i18n/` — `localizer` (console i18n string tables)
- `src/utils/` — `platform` (cross-platform: UTF-8, directory creation)
- `ui_files/` — Qt Designer `.ui` files
- `trans/` — `.ts`/`.qm` translation files
- `tools/setup/` — orphaned source (not built; config merged into TUI)

## Key Architecture Details
- `data/config.conf` and `data/namelist.txt` created at runtime (not tracked in git)
- `Randomizer::peekNextIndex()` added for "Hide Next" preview feature
- Hide Next hides the **next** name (Small2 preview), not the current name (Large)
- SetupTool removed as standalone binary — config accessible via `setup`/`s` command or `--setup` flag in TUI
- No tests exist (`BUILD_TESTS=OFF`); no test framework configured
