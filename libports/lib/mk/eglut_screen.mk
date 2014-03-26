include $(REP_DIR)/ports/mesa-demos.inc

include $(REP_DIR)/lib/import/import-eglut_screen.mk

EGLUT_DIR = $(REP_DIR)/contrib/$(MESA_DEMOS)/src/egl/eglut

SRC_C = eglut.c eglut_screen.c

LIBS += egl libc

vpath %.c $(EGLUT_DIR)

SHARED_LIB = yes
