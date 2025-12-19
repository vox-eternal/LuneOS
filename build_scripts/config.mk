export CFLAGS = -std=c99 -g
export CC = gcc
export CXX = g++
export LD = gcc
export ASM = nasm
export ASMFLAGS=
export LINKFlAGS = 
export LIBS = 

export TARGET = i686-elf
export TARGET_CFLAGS = -std=c99 -g
export TARGET_CC = $(TARGET)-gcc
export TARGET_CCX = $(TARGET)-g++
export TARGET_LD = $(TARGET)-ld
export TARGET_ASM = nasm
export TARGET_NASMFLAGS = 
export TARGET_LINKFLAGS = 
export TARGET_LIBS = 


export BUILD_DIR=$(abspath build)

export SRC_DIR=$(abspath src)


BINUTILS_VERSION=2.7
GCC_VERSION=13.2.0
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/binutils-$(BUNUTILS_VERSION).tar.gz"
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION).tar.gz"
BINUTILS_BUILD = toolchain/binutils-build-$(BUNUTILS_VERSION)
GCC_BUILD = toolchain/gcc-build-$(GCC_VERSION)
