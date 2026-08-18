# Code Style Instructions

## Naming

| Element | Convention | Example |
|---|---|---|
| Classes/Structs | PascalCase | `ScrInstruction`, `DataStream` |
| Free functions | PascalCase | `GetLogger()`, `LoadFile()` |
| Local variables | camelCase | `size`, `childPos`, `isContainer` |
| Member variables | `m_` + type prefix + PascalCase | `m_iByteCode`, `m_pData`, `m_aArguments` |
| Static variables | `s_` + type prefix + PascalCase | `s_pConsole`, `s_sDroppedFile` |
| Parameters | `a_` + type prefix + PascalCase | `a_iSize`, `a_pData`, `a_sName` |
| Enum class values | PascalCase | `ScrArgumentType::Byte` |
| Plain enum values | UPPER_SNAKE_CASE with prefix | `LOGSEVERITY_ERROR` |
| Constants | UPPER_SNAKE_CASE | `CHUNK_ID_SIZE`, `COLOR_YELLOW` |
| Namespaces | lowercase | `humongousexplorer::script` |
| Template params | PascalCase | `typename T`, `typename... Args` |

Hungarian prefixes: `i` = integer, `p` = pointer, `s` = string, `b` = bool, `e` = enum, `c` = char, `a` = array or std::vector, `v` = vector3, `fn` = function, `h` = handle, `m` = map, `evnt` = event

## Formatting

- Indentation: tabs (width 4)
- Braces: Allman style (opening brace on new line for everything: namespaces, classes, functions, control flow)
- Pointer/ref alignment: attached to the type (`const void* a_pData`, not `const void *a_pData`)
- const placement: west-const (`const int`, not `int const`)
- Spacing: space after keywords (`if (`, `for (`), around operators, after commas
- Spaces after casts
- Use static_cast, reinterpret_cast over c casts.
- Multi-line formatting: break at commas in argument lists, after operators

## Structure

- Header guards: `#pragma once` exclusively
- Include order: corresponding header first (relative `./`), then grouped with section comments (`// standard`, `// core`, `// external`, etc.)
- Access specifier order: public, protected, private (no extra indentation, aligned with class brace)
- Constructor init lists: colon on signature line, members on separate lines with leading comma
- Forward declarations: separate namespace blocks after `#pragma once`, before main namespace
- Section separators: `//---------------------------------------------------------------------`
- File pairs: .h and .cpp in same directory, same name
- Even with single line if statements, brackets will need to surround it
- Cases inside of switch should be surrounded by brackets

## Patterns

- Error handling: return `bool`, early return on failure, `assert()` for programmer errors.
- Singleton access: Meyer's singleton via file-scope static, declared `extern` in header
- Doc comments: Doxygen XML (`/// <summary>`, `/// <param name="">`)
- Inline keyword on non-template header-defined functions
- Deleted copy constructors for non-copyable types
- `constexpr` for compile-time constants and functions
- `#pragma pack(push, 1)` for binary structs
- `= default` for trivial constructors/destructors
- `emplace_back` over `push_back` when constructing in-place
- `std::unique_ptr` for polymorphic types and factory pattern ownership transfer

## Project Structure

- `helib` = core library (parsing, data, audio, file I/O)
- `heapp` = application (dx11, imgui, editor, resources, helib)
- `heconsole` = application (helib)
- Namespaces mirror directory structure: `helib/src/core/` = `humongousexplorer::core`