# AGENTS.md - RandomNamePicker

## Building
- `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release` then `cmake --build build`
- Output binaries at `build/bin/`
- GUI target requires Qt6 (optional for console apps)

## Code Quality
- Format: `clang-format -i src/*.cpp src/**/*.cpp src/**/*.h`
- Cppcheck: `cppcheck --std=c++17 -I src src/`

## CI/CD
- Workflow: `.github/workflows/ci.yml`
- Triggers: push to `main`/`master`, or any tag matching `v*`
- Builds: Linux (GCC), Linux ARM64, macOS (Clang), Windows (MinGW)
- 2 targets: RandomNamePicker (TUI + config tool merged), RandomNamePickerGUI
- Config tool built into TUI — run `setup` command or pass `--setup` flag
- Release: created only on tagged pushes

## Commit Conventions
- Release: bare version number (e.g. `2.1.0`)
- Feature/fix: descriptive imperative sentence
- Tag name = same as version number for releases

## Source Structure
- `src/main.cpp` — TUI/console entrypoint
- `src/gui/main.cpp` — GUI entrypoint
- `src/core/` — shared business logic: `randomizer`, `name_list`, `config_manager`
- `src/ui/` — terminal UIs: `tui` (interactive picker), `console` (menu-based, used by SetupTool), `setup_tui` (config mode UI)
- `src/gui/` — Qt GUI code: `main_window`, `ui_main_window.h`
- `src/i18n/` — `localizer` for console i18n string tables (en-US/zh-CN)
- `src/utils/` — `platform` for cross-platform abstractions
- `tools/setup/` — setup tool standalone source
- `ui_files/` — Qt Designer .ui files
- `trans/` — Qt `.ts`/`.qm` translation files for GUI
- `resources.qrc` — embeds `.qm` files for the GUI target
- `VERSION` file at repo root

## Internationalization
- **Console apps** (RandomNamePicker, SetupTool): use `i18n::Localizer` with `--lang en-US|zh-CN` flag. Default: en-US.
- **GUI app** (RandomNamePickerGUI): uses Qt `tr()` / `.ts` / `.qm`. Loads language from `data/config.conf` or `--lang` flag.
- Config file stores language as `[language]= en_US` or `[language]= zh_CN`.
- To update translations: edit `.ts` files in `trans/`, then run `lrelease trans/*.ts` to regenerate `.qm`.
