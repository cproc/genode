include $(REP_DIR)/ports/mesa-demos.inc

TARGET = es2gears
SRC_C  = es2gears.c
LIBS   = glesv2 eglut_screen egl libc libm libc_log pthread

INC_DIR += $(REP_DIR)/contrib/$(MESA_DEMOS)/src/util

vpath %.c $(REP_DIR)/contrib/$(MESA_DEMOS)/src/egl/opengles2
