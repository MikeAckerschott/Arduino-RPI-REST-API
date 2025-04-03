@echo off
set SKETCH_PATH=C:\Users\mikea\Documents\IoT-techniek_svn\Code IoT-software\eindopdracht\arduino_server\arduino_server.ino
set BOARD_FQBN=arduino:avr:uno
set PORT=COM3

echo Verifying Sketch...
arduino-cli compile -b %BOARD_FQBN% %SKETCH_PATH%
if %errorlevel% neq 0 (
    echo Compilation failed!
    exit /b %errorlevel%
)

echo Uploading Sketch...
arduino-cli upload -p %PORT% --fqbn %BOARD_FQBN% %SKETCH_PATH%
if %errorlevel% neq 0 (
    echo Upload failed!
    exit /b %errorlevel%
)

echo Done!
