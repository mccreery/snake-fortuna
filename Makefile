THIS_FILE := $(lastword $(MAKEFILE_LIST))
# Points to the magick or "convert" tool from legacy ImageMagick
MAGICK := magick

objs = $(patsubst src/%.c,%,$(wildcard src/$(1).c)) \
	$(patsubst src/%.s,%,$(wildcard src/$(1).s)) \
	$(patsubst src/%.cpp,%,$(wildcard src/$(1).cpp))
-include programs.mk

CC := avr-gcc
CXX := avr-g++

SRC   := src
BUILD := build
IMG   := img
BLOB  := blob

.PHONY: all clean \
	erase flash flash-% eeprom eeprom-% \
	build/flash.mk build/eeprom.mk
.SECONDARY:

BOARD := LaFortuna
MCU   := at90usb1286
F_CPU := 8000000UL

# Preprocessor flags, not C++
CPPFLAGS    := -DF_CPU=$(F_CPU) -iquotesrc $(DEFINES)
WARNINGS    := -Wall -Wextra -pedantic \
               -Wno-main -fno-strict-aliasing \
               -Wstrict-overflow=5 -fstrict-overflow -Winline
CWARNINGS   := -Wstrict-prototypes -Wmissing-prototypes \
               -Wold-style-definition -Wold-style-declaration
LINKFLAGS   := -mmcu=$(MCU) -O3 -Wl,--gc-sections

COMMONFLAGS := $(WARNINGS) $(LINKFLAGS) -fdata-sections -ffunction-sections
CFLAGS      := $(COMMONFLAGS) $(CWARNINGS) -std=gnu99
CXXFLAGS    := $(COMMONFLAGS) -std=gnu++98

# everything past this point is pattern-matched and automagic
PROGRAM_OBJECTS := $(filter %_OBJECTS,$(.VARIABLES))
PROGRAM_NAMES   := $(PROGRAM_OBJECTS:_OBJECTS=)
PROGRAM_HEXES  := $(foreach name,$(PROGRAM_NAMES),$(BUILD)/$(name).hex)

# default target
all: $(PROGRAM_HEXES)
clean:
	rm -rf $(BUILD)

# include generated upload rule
-include $(BUILD)/flash.mk
-include $(BUILD)/eeprom.mk

$(BUILD)/%.hex: $(BUILD)/%.elf
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex $< $@
	echo $(patsubst $(BUILD)/%.hex,flash: flash-%,$@) > $(BUILD)/flash.mk

$(BUILD)/%.eep: $(BUILD)/%.elf
	avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@
	echo $(patsubst $(BUILD)/%.eep,eeprom: eeprom-%,$@) > $(BUILD)/eeprom.mk

.SECONDEXPANSION:
$(BUILD)/%.elf: $$(foreach o,$$(%_OBJECTS),$$(BUILD)/$$(o).o)
	$(CXX) $(LINKFLAGS) $^ -o $@

# override default C/C++ rules to use the build folder
$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(BUILD)/%.o: $(SRC)/%.s
	mkdir -p $(dir $@)
	$(CC) -x assembler-with-cpp -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(BUILD)/%.o: $(SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# Generated blobs
$(BUILD)/%.o: $(BUILD)/%
	mkdir -p $(dir $@)
	avr-objcopy \
		--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
		--redefine-sym _binary_$(BUILD)_$*_start=$* \
		--redefine-sym _binary_$(BUILD)_$*_end=$*_end \
		--redefine-sym _binary_$(BUILD)_$*_size=$*_size_addr \
		-I binary -O elf32-avr $< $@

# Source blobs
$(BUILD)/%.o: $(BLOB)/%
	mkdir -p $(dir $@)
	avr-objcopy \
		--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
		--redefine-sym _binary_$(BLOB)_$*_start=$* \
		--redefine-sym _binary_$(BLOB)_$*_end=$*_end \
		--redefine-sym _binary_$(BLOB)_$*_size=$*_size_addr \
		-I binary -O elf32-avr $< $@

$(BUILD)/%_4: $(IMG)/%.png
	mkdir -p $(dir $@)
	$(MAGICK) $< -depth 4 -colorspace gray Y:$@

$(BUILD)/%_2: $(IMG)/%.png
	mkdir -p $(dir $@)
	$(MAGICK) $< -depth 2 -colorspace gray Y:$@

$(BUILD)/%_1: $(IMG)/%.png
	mkdir -p $(dir $@)
	$(MAGICK) $< -depth 1 -colorspace gray Y:$@

erase:
	dfu-programmer $(MCU) erase

eeprom-%: $(BUILD)/%.eep
	dfu-programmer $(MCU) flash --eeprom --suppress-validation $<

flash-%: $(BUILD)/%.hex
	dfu-programmer $(MCU) flash $<; \
	dfu-programmer $(MCU) launch --no-reset
