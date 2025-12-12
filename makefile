ASM = nasm
SRC_DIR = src
BUILD_DIR = build
CC = gcc
TOOLS_DIR = tools

.PHONY: all floppy_image kernel bootloader clean always tools_fat

all: floppy_image


# Floppy image (raw, no FAT12)

floppy_image: $(BUILD_DIR)/main_floppy.img

$(BUILD_DIR)/main_floppy.img: bootloader kernel
	dd if=/dev/zero of=$(BUILD_DIR)/main_floppy.img bs=512 count=2880
	# Create a FAT12 filesystem on the blank floppy image
	mformat -i $(BUILD_DIR)/main_floppy.img -f 1440 ::
	# Copy files into the FAT image (kernel/test) - test.txt is optional
	if [ -f test.txt ]; then mcopy -i $(BUILD_DIR)/main_floppy.img test.txt ::/test.txt; fi
	mcopy -i $(BUILD_DIR)/main_floppy.img $(BUILD_DIR)/kernel.bin ::/kernel.bin
	# Write bootloader and kernel to sectors (bootloader last to preserve boot code)
	dd if=$(BUILD_DIR)/kernel.bin of=$(BUILD_DIR)/main_floppy.img bs=512 seek=1 conv=notrunc
	dd if=$(BUILD_DIR)/bootloader.bin of=$(BUILD_DIR)/main_floppy.img conv=notrunc

# Bootloader

bootloader: $(BUILD_DIR)/bootloader.bin

$(BUILD_DIR)/bootloader.bin: always
	$(ASM) $(SRC_DIR)/bootloader/boot.asm -f bin -o $(BUILD_DIR)/bootloader.bin


# Kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: always
	$(ASM) $(SRC_DIR)/kernel/main.asm -f bin -o $(BUILD_DIR)/kernel.bin



# Tools FAT
tools_fat: $(BUILD_DIR)/tools/fat
$(BUILD_DIR)/tools/fat: always $(TOOLS_DIR)/FAT/fat.c
	mkdir -p $(BUILD_DIR)/tools
	$(CC) -o $(BUILD_DIR)/tools/fat $(TOOLS_DIR)/FAT/fat.c



# Always

always:
	mkdir -p $(BUILD_DIR)


# Clean

clean:
	rm -rf $(BUILD_DIR)/*
