BUILD_DIR ?= $(abspath ../../build)
CC ?= gcc
SRC := kernel.c
OUT := $(BUILD_DIR)/kernel.bin

.PHONY: all clean

all: $(OUT)

$(OUT): $(SRC)
	$(CC) -o $@ $<

clean:
	rm -f $(OUT)
