include $(REP_DIR)/lib/mk/mesa10.inc

# sources and compiler options extracted from the Linux build log for libglapi.so

SRC_C = entry.c \
        mapi_glapi.c \
        stub.c \
        table.c \
        u_current.c \
        u_execmem.c

CC_OPT += -DMAPI_MODE_GLAPI \
          -DMAPI_ABI_HEADER=\"shared-glapi/glapi_mapi_tmp.h\" \

#          -std=c99 -Werror=implicit-function-declaration -Werror=missing-prototypes -fno-strict-aliasing -fno-builtin-memcmp -MT entry.lo -MD -MP -MF .deps/entry.Tpo -c ../../../src/mapi/entry.c  -fPIC -DPIC -o .libs/entry.o

INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/mapi

LIBS = libc

vpath %.c $(REP_DIR)/contrib/$(MESA_DIR)/src/mapi

SHARED_LIB = yes
