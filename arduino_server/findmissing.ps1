$missingLibs = arduino-cli.exe compile -b arduino:avr:uno --verbose . 2>&1 | Select-String "fatal error: (.*).h: No such file or directory"

foreach ($lib in $missingLibs) {
    if ($lib -match "fatal error: (\w+).h: No such file or directory") {
        $libraryName = $matches[1]
        Write-Host "Installing missing library: $libraryName"
        arduino-cli.exe lib install $libraryName
    }
}

arduino-cli.exe compile -b arduino:avr:uno .
