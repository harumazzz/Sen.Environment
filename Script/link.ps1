$source = "$PSScriptRoot".Replace('\', '/')
$target = Join-Path $source "./build"

# Delete build folder
Remove-Item -LiteralPath $target -Force -Recurse

# Copy JSON files
Get-ChildItem $source -recurse -Filter "*.json" | ForEach-Object {
    $item = $_.FullName.Replace('\', '/')
    $relativePath = $_.FullName.Substring($source.Length).Replace('\', '/')
    $destination = Join-Path $target $relativePath
    if (-not ($relativePath -eq "/tsconfig.json")) {
        Write-Host "Copying: $relativePath"
        $destinationDir = Split-Path -Path $destination
        if (!(Test-Path -Path $destinationDir)) {
            New-Item -ItemType Directory -Path $destinationDir -Force
        }
        Copy-Item -Path $item -Destination $destination
    }
}

# Run tsc
Write-Host "Transpiling TS to JS"
& npx tsc

# Finish

Write-Host "All commands executed!"
