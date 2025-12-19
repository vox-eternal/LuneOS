TARGET_ASMFLAGS? += -f elf 
TARGET_CFLAGS += -ffreestanding -nostdlib
TARGET_LIBS += -lgcc
SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
BUILD_DIR ?= $(abspath ../../../build)
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(SOURCES_C))


.PHONY: all stage2 clean always

all: stage2

stage2: $(BUILD_DIR)/stage2.bin


$(BUILD_DIR)/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(TARGET_LD) $(TARGET_LINKFlAGS) -Map=$(BUILD_DIR)/stage2.map -o $@ $^

$(BUILD_DIR)/stage2/c/%.obj: %.c always
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/stage2/asm/%.obj: %.asm always
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -f elf32 -o $@ $<


always:
	mkdir -p $(BUILD_DIR)/stage2/c
	mkdir -p $(BUILD_DIR)/stage2/asm

clean:
	rm -f $(BUILD_DIR)/stage2.bin