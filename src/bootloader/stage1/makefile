# Stage1 Makefile
BUILD_DIR ?= $(abspath ../../../build)
ASM       ?= nasm
ASM_SRC   := boot.asm
OUT       := $(BUILD_DIR)/stage1.bin

.PHONY: all clean

all: $(OUT)

$(OUT): $(ASM_SRC)
	$(ASM) $< -f bin -o $@

clean:
	rm -f $(OUT)
