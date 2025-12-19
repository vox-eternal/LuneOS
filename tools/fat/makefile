BUILD_DIR?=build
CC?=gcc
CFLAGS?=-ggdb

SOURCES_C=$(wildcard *.c)
OBJECTS_C=$(patsubst %.c, build/tools/fat/%.o, $(SOURCES_C))

.PHONY: all fat clean always

all: fat

fat: build/tools/fat.out

build/tools/fat.out: $(OBJECTS_ASM) $(OBJECTS_C)
	$(CC) $(CFLAGS) -o build/tools/fat.out $(OBJECTS_C)

build/tools/fat/%.o: %.c always
	$(CC) $(CFLAGS) -c -o $@ $<

always:
	mkdir -p build/tools/fat

clean:
	rm -f build/tools/fat.out
	rm -rf build/tools/fat