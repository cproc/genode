TARGET = mprime

SRC_C = prime.c \
        cpuid.c \
        gwnum.c \
        gwtables.c \
        gwthread.c \
        gwutil.c \
        giants.c \
        ecmstag1.c

SRC_CC = gwdbldbl.cc

LIBS = pthread libc libm stdcxx

ifeq ($(filter-out $(SPECS),x86_32),)
CC_OPT += -march=i486 -malign-double
EXT_OBJECTS = $(PRG_DIR)/x86_32/gwnum.a
else ifeq ($(filter-out $(SPECS),x86_64),)
CC_OPT += -DX86_64
EXT_OBJECTS = $(PRG_DIR)/x86_64/gwnum.a
endif
