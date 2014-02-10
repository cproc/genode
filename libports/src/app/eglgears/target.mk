include $(REP_DIR)/ports/mesa-demos.inc

TARGET = eglgears
SRC_C  = eglgears.c
LIBS   = libc libm libc_log gallium eglut_screen

INC_DIR += $(REP_DIR)/contrib/$(MESA_DEMOS)/src/util

vpath %.c $(REP_DIR)/contrib/$(MESA_DEMOS)/src/egl/opengl
