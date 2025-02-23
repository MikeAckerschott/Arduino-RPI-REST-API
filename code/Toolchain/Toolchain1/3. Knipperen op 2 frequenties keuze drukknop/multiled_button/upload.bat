@echo off
setlocal

if "%~1"=="" (
    echo Usage: %~nx0 source_file.ino
    exit /b 1
)

set SRC_FILE=%~1
set BASE_NAME=%~n1
set C_FILE=build\%BASE_NAME%.c
set OBJ_FILE=build\%BASE_NAME%.o
set OUT_FILE=build\%BASE_NAME%

if not exist build (
    mkdir build
)

@REM Preprocess the .ino file into a .c file
type %SRC_FILE% > %C_FILE%

"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-gcc" -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o %OBJ_FILE% %C_FILE%

"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-gcc" -mmcu=atmega328p %OBJ_FILE% -o %OUT_FILE%

"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objdump" -D -m avr5 %OUT_FILE% > %OUT_FILE%.asm

"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objcopy" -O ihex -R .eeprom %OUT_FILE% %OUT_FILE%.hex

"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avrdude" -C "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -F -V -c arduino -p ATMEGA328P -P COM3 -b 115200 -U flash:w:%OUT_FILE%.hex