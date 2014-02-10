include $(REP_DIR)/ports/mesa-demos.inc

TARGET = es1gears
SRC_C  = gears.c
LIBS   = libc libm libc_log gallium eglut_screen

CC_OPT_gears = -Wno-implicit-function-declaration

vpath %.c $(REP_DIR)/contrib/$(MESA_DEMOS)/src/egl/opengles1
