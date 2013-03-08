GMP_VER=5.0.5
GMP_URL=http://ftp.gnu.org/gnu/gmp/gmp-$(GMP_VER).tar.bz2
GMP_TAR=gmp-$(GMP_VER).tar.bz2

MPFR_VER=3.1.1
MPFR_URL=http://ftp.gnu.org/gnu/mpfr/mpfr-$(MPFR_VER).tar.bz2
MPFR_TAR=mpfr-$(MPFR_VER).tar.bz2

MPC_VER=1.0.1
MPC_URL=http://ftp.gnu.org/gnu/mpc/mpc-$(MPC_VER).tar.gz
MPC_TAR=mpc-$(MPC_VER).tar.gz

BINUTILS_VER=2.23.1
BINUTILS_URL=http://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VER).tar.bz2
BINUTILS_TAR=binutils-$(BINUTILS_VER).tar.bz2

GCC_VER=4.7.2
GCC_URL=http://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VER)/gcc-$(GCC_VER).tar.bz2
GCC_TAR=gcc-$(GCC_VER).tar.bz2

BASEDIR=$(shell pwd)
TOOLCHAIN_DIR=$(BASEDIR)/toolchain
TARGET=xtensa-elf

all: toolchain

gmp-$(GMP_VER):
	wget -N -P $(TOOLCHAIN_DIR)/dl $(GMP_URL)
	tar -C $(TOOLCHAIN_DIR)/dl -xjf $(TOOLCHAIN_DIR)/dl/$(GMP_TAR)

gmp: gmp-$(GMP_VER)
	mkdir -p $(TOOLCHAIN_DIR)/build/gmp
	cd $(TOOLCHAIN_DIR)/build/gmp; \
	$(TOOLCHAIN_DIR)/dl/$</configure --disable-shared --enable-static --prefix=$(TOOLCHAIN_DIR)/inst; \
	$(MAKE); $(MAKE) check;$(MAKE) install

mpfr-$(MPFR_VER):
	wget -N -P $(TOOLCHAIN_DIR)/dl $(MPFR_URL)
	tar -C $(TOOLCHAIN_DIR)/dl -xjf $(TOOLCHAIN_DIR)/dl/$(MPFR_TAR)

mpfr: mpfr-$(MPFR_VER)
	mkdir -p $(TOOLCHAIN_DIR)/build/mpfr
	cd $(TOOLCHAIN_DIR)/build/mpfr; \
	$(TOOLCHAIN_DIR)/dl/$</configure --disable-shared --enable-static --with-gmp=$(TOOLCHAIN_DIR)/inst \
	--prefix=$(TOOLCHAIN_DIR)/inst; \
	$(MAKE); $(MAKE) check;$(MAKE) install

mpc-$(MPC_VER):
	wget -N -P $(TOOLCHAIN_DIR)/dl $(MPC_URL)
	tar -C $(TOOLCHAIN_DIR)/dl -zxvf $(TOOLCHAIN_DIR)/dl/$(MPC_TAR)

mpc: mpc-$(MPC_VER)
	mkdir -p $(TOOLCHAIN_DIR)/build/mpfc
	cd $(TOOLCHAIN_DIR)/build/mpfc; \
	$(TOOLCHAIN_DIR)/dl/$</configure --disable-shared --enable-static --with-gmp=$(TOOLCHAIN_DIR)/inst \
	--with-mpfr=$(TOOLCHAIN_DIR)/inst --prefix=$(TOOLCHAIN_DIR)/inst; \
	$(MAKE); $(MAKE) check;$(MAKE) install

binutils-$(BINUTILS_VER):
	wget -N -P $(TOOLCHAIN_DIR)/dl $(BINUTILS_URL)
	tar -C $(TOOLCHAIN_DIR)/dl -xjf $(TOOLCHAIN_DIR)/dl/$(BINUTILS_TAR)
	patch -d $(TOOLCHAIN_DIR)/dl/$@ -p1 < local/patches/binutils.patch

binutils: binutils-$(BINUTILS_VER)
	mkdir -p $(TOOLCHAIN_DIR)/build/binutils
	cd $(TOOLCHAIN_DIR)/build/binutils; \
	$(TOOLCHAIN_DIR)/dl/$</configure --target=$(TARGET) --prefix=$(TOOLCHAIN_DIR)/inst; \
	$(MAKE) all; $(MAKE) install

gcc-$(GCC_VER):
	wget -N -P $(TOOLCHAIN_DIR)/dl $(GCC_URL)
	tar -C $(TOOLCHAIN_DIR)/dl -xjf $(TOOLCHAIN_DIR)/dl/$(GCC_TAR)
	patch -d $(TOOLCHAIN_DIR)/dl/$@ -p1 < local/patches/gcc.patch

gcc: gcc-$(GCC_VER)
	mkdir -p $(TOOLCHAIN_DIR)/build/gcc
	cd $(TOOLCHAIN_DIR)/build/gcc; \
	export PATH=$(TOOLCHAIN_DIR)/inst/bin:$(PATH); \
	$(TOOLCHAIN_DIR)/dl/$</configure --target=$(TARGET) --prefix=$(TOOLCHAIN_DIR)/inst \
	--enable-languages=c --disable-libssp --disable-shared --disable-libquadmath \
	--with-gmp=$(TOOLCHAIN_DIR)/inst \
	--with-mpfr=$(TOOLCHAIN_DIR)/inst \
	--with-mpc=$(TOOLCHAIN_DIR)/inst --with-newlib; \
	$(MAKE) all; $(MAKE) install

toolchain: gmp mpfr mpc binutils gcc

clean:
	rm -rf $(TOOLCHAIN_DIR)/build $(TOOLCHAIN_DIR)/inst
