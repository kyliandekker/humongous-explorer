@ECHO OFF
SETLOCAL

SET "SOURCE_DIR=%~dp0"
IF "%SOURCE_DIR:~-1%"=="\" SET "SOURCE_DIR=%SOURCE_DIR:~0,-1%"

:: Output to build folder
SET "OUTPUT_DIR=%SOURCE_DIR%\build"

PUSHD "%SOURCE_DIR%"

:: Create build directory if it doesn't exist
IF NOT EXIST "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

:: Generate common shaders and HLSL
CALL "%SOURCE_DIR%\setup\generate-common-shader-and-hlsl\run.bat"

:: Generate in build/
cmake -G "Visual Studio 17 2022" -A x64 -Wno-dev -S "%SOURCE_DIR%" -B "%OUTPUT_DIR%"

IF %ERRORLEVEL% NEQ 0 (
    PAUSE
) ELSE (
    tasklist /FI "IMAGENAME eq devenv.exe" | find /I "devenv.exe" >nul
    IF ERRORLEVEL 1 (
        FOR %%f IN ("%OUTPUT_DIR%\*.sln") DO START "" "%%f"
    ) ELSE (
        ECHO Visual Studio already running; skipping open.
    )
)

POPD
PAUSE