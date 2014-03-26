include $(REP_DIR)/lib/mk/mesa10.inc

# sources and compiler options extracted from the Linux build log for libloader.a

SRC_C = loader.c

INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include \

#           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary \
#           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/include \
#           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/state_trackers/egl \
#           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/winsys/sw \
#           $(REP_DIR)/contrib/$(MESA_DIR)/src/egl/main \

LIBS = libc libdrm

vpath %.c $(REP_DIR)/contrib/$(MESA_DIR)/src/loader

