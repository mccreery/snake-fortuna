# Snake Fortuna
This repository is a partial fork/mirror of a private playground repository for AVR/LaFortuna code. I have included all the necessary files for building and uploading Snake Fortuna, a clone of classic Snake.

The game features a 32x32 game grid, and should work correctly all the way up to the maximum score of 1023 or 32^2 - 1 (though I haven't played that long). The game stores the top ten highest scores along with allowing high scorers to input their 3-letter arcade name. When you play the high score is shown on the right side of the screen with your score on the left.

## Using the Makefile
Use `make [task...]` to run one or more tasks. `-s` ("silent") is useful.

These instructions are borrowed from my private repo, for the purposes of Snake Fortuna you can just run `make -s clean upload-snake` with your LaFortuna connected in DFU mode (by pressing the small button on the top). If your screen blinks white when you first flash the game, just power cycle it.

### Tasks
- `all` (default): generates `build/*.hex` for all programs (in this repo only Snake)
- `clean`: cleans all generated files
- `flash-*`: generates, flashes, disconnects and SW resets `build/*.hex`
- `flash`: shorthand for `flash-*`, using the most recent hex file
- `eeprom-*`: generates and flashes the EEPROM with `build/*.eep`. You have to flash the EEPROM between erasing and flashing the hex.
- `eeprom`: shorthand for `eeprom-*`, using the most recent eep file

### Adding Programs
To add a new program (and corresponding build tasks), define a new variable `*_OBJECTS` (case-sensitive) in [`programs.mk`](programs.mk) containing each object (source file, excluding the `src/` prefix and `.c` suffix) needed by the program, separated by spaces.

## Required Programs
- GNU tools `make` ~~and `find`~~
- `avr-gcc`/`avr-g++`
- `avr-objcopy`
- `dfu-programmer` (v0.7.0+)
