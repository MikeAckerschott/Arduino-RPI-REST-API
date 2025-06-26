@echo off
setlocal

:: Check for arguments
set verify=false
set upload=false
set monitor=false

for %%i in (%*) do (
    if /i "%%i"=="v" set verify=true
    if /i "%%i"=="u" set upload=true
    if /i "%%i"=="m" set monitor=true
)

:: Verify the build if 'v' is passed
if "%verify%"=="true" (
    echo Verifying the build...
    arduino-cli.exe compile --fqbn arduino:avr:uno --build-property "compiler.cpp.extra_flags=-Wall"

)

:: Upload if 'u' is passed
if "%upload%"=="true" (
    echo Uploading to the board...
    :: Run PowerShell script
    powershell -ExecutionPolicy Bypass -File findmissing.ps1
    arduino-cli.exe upload -p COM3 -b arduino:avr:uno --verbose
)

:: Open serial monitor if 'm' is passed
if "%monitor%"=="true" (
    echo Opening serial monitor at 9600 baud...
    arduino-cli.exe monitor -p COM3 -c baudrate=9600
)

endlocal
