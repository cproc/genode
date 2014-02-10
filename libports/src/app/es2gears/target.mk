include $(REP_DIR)/ports/mesa-demos.inc

TARGET = es2gears
SRC_C  = es2gears.c
LIBS   = libc libm libc_log pthread gallium eglut_screen

INC_DIR += $(REP_DIR)/contrib/$(MESA_DEMOS)/src/util

vpath %.c $(REP_DIR)/contrib/$(MESA_DEMOS)/src/egl/opengles2
