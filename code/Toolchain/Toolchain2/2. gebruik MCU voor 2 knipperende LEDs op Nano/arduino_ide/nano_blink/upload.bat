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

"C:\Users\mikea\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin\avr-gcc" -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p -o "%OUT_FILE%.elf" "%C_FILE%"

"C:\Users\mikea\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin\avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 "%OUT_FILE%.elf" "%OUT_FILE%.eep"

"C:\Users\mikea\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin\avr-objcopy" -O ihex -R .eeprom "%OUT_FILE%.elf" "%OUT_FILE%.hex"

"C:\Users\mikea\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin\avr-size" -A "%OUT_FILE%.elf"

"C:\Users\mikea\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin\avr-objdump" -d "%OUT_FILE%.elf" > "%OUT_FILE%.asm"

"C:\Users\mikea\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\bin\avrdude" -C "C:\Users\mikea\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf" -v -V -patmega328p -cstk500v1 -PCOM3 -b19200 -Uflash:w:%OUT_FILE%.hex:i