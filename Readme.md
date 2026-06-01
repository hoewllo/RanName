# RandomNamePicker

A cross-platform classroom roll-call application with both terminal (TUI) and Qt GUI interfaces.

## Features

- **Two picking modes**: All Random & One by One
- **Dual interfaces**: Interactive TUI (Unicode box-drawing) and Qt6 GUI
- **Internationalization**: English and Chinese (en-US, zh-CN)
- **Cross-platform**: Linux, macOS, Windows

## Build

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Requires C++17, CMake 3.16+, and optionally Qt6 for the GUI.

## Usage

| Binary | Description |
|--------|-------------|
| `RandomNamePicker` | TUI console app (run `setup` command or `--setup` for config) |
| `RandomNamePickerGUI` | Qt GUI (requires Qt6) |
| `SetupTool` | Standalone configuration tool |

All binaries accept `--lang zh-CN` for Chinese.

## License

MIT © 2025 Jianyin Li
