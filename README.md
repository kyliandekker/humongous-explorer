# HumongousExplorer

A file archive viewer and editor for Humongous Entertainment game data files. Supports parsing, browsing, and previewing assets from games built on the Humongous Engine (SCUMM variant) — titles such as *Putt-Putt*, *Freddi Fish*, *Pajama Sam*, and *SpyFox*.

## Features

- Preview room backgrounds, voice clips, sound effects, and music
- Dockable window layout with search and filtering

## Modules

| Module | Type | Description |
|---|---|---|
| `helib` | Static library | Core parsing library — file I/O, data streams, chunk tree parsing, wave loading |
| `heapp` | Win32 executable | GUI application — DirectX 11 rendering, ImGui interface, resource preview |
| `heconsole` | Console executable | CLI tool for batch operations |

## Building

### Requirements

- Visual Studio 2022
- CMake 3.20+

### Build

```bat
generate.bat
```

This generates a Visual Studio solution in `build/` and opens it.

## Known Supported Formats

| Extension | Type | Contents |
|---|---|---|
| `.HE0` | Index file | Room names, object names, file directory tables |
| `.HE2` | Talk bank | Voice clips |
| `.HE4` | Song file | Music tracks |
| `.(A)` | Resource archive | Rooms, scripts, images, audio, costumes |

Other formats might be supported but have not been tested.

## Third-Party Libraries

- [Dear ImGui](https://github.com/ocornut/imgui) — Immediate-mode GUI (with docking)
- [ImPlot](https://github.com/epezent/implot) — Plotting extension for ImGui
- [NanoSVG](https://github.com/memononen/nanosvg) — SVG parser and rasterizer
- [stb_image](https://github.com/nothings/stb) — Image loading and saving
- [RapidJSON](https://github.com/Tencent/rapidjson) — JSON parser/serializer

## License

See individual third-party libraries for their respective licenses.
