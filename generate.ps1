$SourceDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$OutputDir = Join-Path $SourceDir "build"

Set-Location $SourceDir

# Create build directory if it doesn't exist
if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir | Out-Null
}

# Generate Visual Studio solution
cmake `
    -G "Visual Studio 17 2022" `
    -A x64 `
    -Wno-dev `
    -S $SourceDir `
    -B $OutputDir

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed." -ForegroundColor Red
    Pause
    exit $LASTEXITCODE
}

# Copy everything from setup/ into the solution/build folder
$ConfigDir = Join-Path $SourceDir "setup"

if (Test-Path $ConfigDir) {
    Write-Host "Copying setup files to build folder..."

    Copy-Item `
        -Path "$ConfigDir\*" `
        -Destination $OutputDir `
        -Recurse `
        -Force
}

# Generate command JSON
$JsonPath = Join-Path $OutputDir "heconsole.args.json"
$OutDir = Join-Path $OutputDir "out"

$CommandJson = @{
    FileVersion = 2
    Id          = "c6f3fb3b-aa78-3332-84f5-c162d5dd17bf"
    Items       = @(
        @{
            Id      = "930677a7-d3cd-4d1f-91eb-a74ad670d936"
            Command = "-outputPath=`"$OutDir`""
        }
    )
}

$CommandJson | ConvertTo-Json -Depth 10 | Set-Content -Path $JsonPath -Encoding UTF8

Write-Host "Generated $JsonPath"

# Open solution if Visual Studio isn't already running
$VisualStudioRunning = Get-Process -Name "devenv" -ErrorAction SilentlyContinue

if (-not $VisualStudioRunning) {
    $Solution = Get-ChildItem -Path $OutputDir -Filter "*.sln" | Select-Object -First 1

    if ($Solution) {
        Start-Process $Solution.FullName
    }
}
else {
    Write-Host "Visual Studio already running; skipping open."
}