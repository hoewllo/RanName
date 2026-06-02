# RandomNamePicker

## Build
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
Output: `build/bin/RandomNamePicker` (TUI) and optionally `build/bin/RandomNamePickerGUI` (Qt6).

TUI includes config mode — run `setup`/`s` at prompt or pass `--setup`.

## Version (auto-detected)
No `VERSION` file. CMake derives from (in order):
1. Latest git tag matching `v*` → strips `v`
2. HEAD commit message matching `X.Y.Z`
3. Walk commit history (max 100) for `X.Y.Z` pattern
4. Fallback to `0.0.0`

Tags must be pushed for CI to pick them up. CI checkouts use `fetch-depth: 0` so tags are available.

## CI/CD (`.github/workflows/ci.yml`)
- Triggers: push to `main`/`master`, or tag `v*`
- Builds: Linux GCC, Linux ARM64, macOS Clang, Windows MinGW
- All build jobs have `continue-on-error: true` — partial failures don't block
- Release created only when all 4 builds pass AND push is a `v*` tag
- `.qm` files are **not tracked** — CI runs `lrelease` before build

## Qt6 quirks
- **Ubuntu**: `lrelease` at `/usr/lib/qt6/bin/lrelease`, package `qt6-tools-dev`
- **macOS**: `brew install qt@6`, set `Qt6_DIR="$(brew --prefix qt@6)/lib/cmake/Qt6"`
- **Windows (CI)**: uses `jurplel/install-qt-action@v4` with MSVC or MinGW

## i18n — two systems
- **Console** (`RandomNamePicker`): `i18n::Localizer` with `--lang en-US|zh-CN`. Default from `data/config.conf`.
- **GUI** (`RandomNamePickerGUI`): Qt `tr()` with `.ts`/`.qm`. Same `--lang` flag or config file.
- To update: edit `.ts` files under `trans/`, then `lrelease trans/*.ts` to regenerate `.qm`.

## Commit format
```
x.x.x

<optional description body on later lines>
```
First line is the bare version number. No inline description on the first line. Tag name = same as version (e.g. `v2.0.2`).

## Code quality
- Format: `clang-format -i src/*.cpp src/**/*.cpp src/**/*.h`
- Cppcheck: `cppcheck --std=c++17 -I src src/`

## Runtime data
`data/` directory (with `config.conf`, `namelist.txt`) is created at runtime by both apps. Not in git.
