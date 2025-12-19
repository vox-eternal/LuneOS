BUILD_DIR ?= $(abspath ../../../build)
ASM       ?= nasm
ASM_SRC   := boot.asm
OUT       := $(BUILD_DIR)/stage1.bin

.PHONY: all clean

all: $(OUT)

$(OUT): $(ASM_SRC)
	mkdir -p $(BUILD_DIR)
	$(ASM) $< -f bin -o $@

clean:
	rm -f $(OUT)
