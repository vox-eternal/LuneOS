toolchain: toolchain_binutils toolchain_gcc

TOOLCHAIN_PREFIX=$(abspath toolchain/$(TARGET))
BINUTILS_BUILD = toolchain/binutils-built-$(BINUTILS_VERSION)
BINUTILS_SRC = toolchain/binutils/$(BINUTILS_VERSION)

export PATH :=$(TOOLCHAIN_PREFIX)/bin:$(PATH)

toolchain_binutils:
	mkdir toolchain
	cd toolchain && wget $(BINUTILS_URL)
	cd toolchain && tar -xf binutils-$(BINUTILS_VERSION).tar.gz
	mkdir -p toolchain/binutils-built-$(BINUTILS_VERSION)
	cd toolchain/binutils-built-$(BINUTILS_VERSION) && ../binutils-$(BINUTILS_VERSION)/configure \
        --prefix="$(TOOLCHAIN_PREFIX)" \
        --target=$(TARGET) \
        --with-sysroot \
        --disable-nls \
        --disable-werror
	$(MAKE) -j8 -C $(BINUTILS_BUILD)
	$(MAKE) -C $(BINUTILS_BUILD) install

	$(BINUTILS_SRC).tar.xz:
		mkdir -p toolchain
		cd toolchain && wget $(BINUTILS_URL)

GCC_SRC = toolchain/gcc-$(GCC_VERSION)
GCC_BUILD = toolchain/gcc-build-$(GCC_VERSION)

toolchain_gcc: toolchain_binutils
	cd toolchain && wget $(GCC_URL)
	cd toolchain && tar -xf gcc-$(GCC_VERSION).tar.gz
	mkdir $(GCC_BUILD)
	cd $(GCC_BUILD) && ../gcc-$(GCC_VERSION)/configure \
		--prefix="$(TOOLCHAIN_PREFIX)"	\
		--target=$(TARGET)				\
		--disable-nls					\
		--without-headers				\
		--enable-languages=c,c++		\
		--with-newlib					

	$(MAKE) -j8 -C $(GCC_BUILD) all-gcc all-target-libgcc
	$(MAKE) -C $(GCC_BUILD) install-gcc install-target-libgcc

$(GCC_SRC).tar.xz:
	mkdir -p toolchain
	cd toolchain && wget $(GCC_URL)

# for cleaning

clean-toolchain:
	rm -rf $(GCC_BUILD) $(GCC_SRC) $(BINUTILS_SRC) $(BINUTILS_BUILD)

clean-toolchain-all:
	rm -rf toolchain/*

.PHONY: toolchain_binutils toolchain_gcc toolchain_gcc clean-toolchain clain-toolchain-all
