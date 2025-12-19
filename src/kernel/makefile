BUILD_DIR ?= $(abspath ../../../build)
CC ?= gcc

SRC := kernel.c
OUT := $(BUILD_DIR)/kernel.bin

.PHONY: all clean

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@

clean:
	rm -f $(OUT)
