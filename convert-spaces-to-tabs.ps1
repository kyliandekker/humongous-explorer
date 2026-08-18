$files = Get-ChildItem -Recurse -Include *.h,*.hpp,*.cpp

foreach ($file in $files) {
    $content = Get-Content -Raw $file.FullName
    $newContent = $content -replace '    ', "`t"
    if ($content -ne $newContent) {
        [System.IO.File]::WriteAllText($file.FullName, $newContent)
        Write-Host "Converted: $($file.FullName)"
    }
}
