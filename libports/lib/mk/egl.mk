include $(REP_DIR)/lib/mk/mesa10.inc

include $(REP_DIR)/lib/import/import-egl.mk

# sources and compiler options extracted from the Linux build log for libEGL.so

SRC_C = eglapi.c \
        eglarray.c \
        eglconfig.c \
        eglcontext.c \
        eglcurrent.c \
        egldisplay.c \
        egldriver.c \
        eglfallbacks.c \
        eglglobals.c \
        eglimage.c \
        egllog.c \
        eglmisc.c \
        eglmode.c \
        eglscreen.c \
        eglstring.c \
        eglsurface.c \
        eglsync.c \

CC_OPT += -D_EGL_NATIVE_PLATFORM=_EGL_PLATFORM_GENODE \
          -D_EGL_DRIVER_SEARCH_DIR=\"\" \
          -D_EGL_OS_UNIX=1 \
          -DHAVE_NULL_PLATFORM

# -fvisibility=hidden  -g -O2 -Wall -std=c99 -Werror=implicit-function-declaration -Werror=missing-prototypes -fno-strict-aliasing -fno-builtin-memcmp

INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include

LIBS = libc

vpath %.c $(REP_DIR)/contrib/$(MESA_DIR)/src/egl/main

SHARED_LIB = yes

