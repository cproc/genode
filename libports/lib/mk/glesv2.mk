include $(REP_DIR)/lib/mk/mesa10.inc

# sources and compiler options extracted from the Linux build log for libGLESv2.so

SRC_C = entry.c

CC_OPT += -DMAPI_MODE_BRIDGE \
          -DMAPI_ABI_HEADER=\"glapi_mapi_tmp.h\"

# -fvisibility=hidden -g -O2 -Wall -std=c99 -Werror=implicit-function-declaration -Werror=missing-prototypes -fno-strict-aliasing -fno-builtin-memcmp

INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/mapi \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/mapi/es2api \

LIBS = glapi libc

vpath %.c $(REP_DIR)/contrib/$(MESA_DIR)/src/mapi

SHARED_LIB = yes
