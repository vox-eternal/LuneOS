
TOOLS_DIR=tools

include build_scripts/config.mk


.PHONY: all floppy_image kernel bootloader clean always tools_fat

all: floppy_image tools_fat

include build_scripts/toolchain.mk

# Floppy image

floppy_image: build/main_floppy.img

build/main_floppy.img: bootloader kernel
	dd if=/dev/zero of=build/main_floppy.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" build/main_floppy.img
	dd if=build/stage1.bin of=build/main_floppy.img conv=notrunc
	mcopy -i build/main_floppy.img build/stage2.bin "::stage2.bin"
	mcopy -i build/main_floppy.img build/kernel.bin "::kernel.bin"
	mcopy -i build/main_floppy.img test.txt "::test.txt"
# Bootloader

bootloader: stage1 stage2

stage1: build/stage1.bin

build/stage1.bin: always
	$(MAKE) -C src/bootloader/stage1 BUILD_DIR=$(abspath build)

stage2: build/stage2.bin

build/stage2.bin: always
	$(MAKE) -C src/bootloader/stage2 BUILD_DIR=$(abspath build)


# Kernel

kernel: build/kernel.bin

build/kernel.bin: always
	$(MAKE) -C src/kernel BUILD_DIR=$(abspath build)

# Tools

tools_fat: build/tools/fat
build/tools/fat: always $(TOOLS_DIR)/fat/fat.c
	mkdir -p build/tools
	$(CC) -g -o build/tools/fat $(TOOLS_DIR)/fat/fat.c


# Always

always:
	mkdir -p build


# Clean

clean:
	$(MAKE) -C src/bootloader/stage1 BUILD_DIR=$(abspath build) clean
	$(MAKE) -C src/bootloader/stage2 BUILD_DIR=$(abspath build) clean
	$(MAKE) -C src/kernel BUILD_DIR=$(abspath build) clean
	rm -rf build/*
