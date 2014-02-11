include $(REP_DIR)/lib/import/import-libdrm.mk

SRC_C = intel_bufmgr_gem.c \
        intel_bufmgr.c \
        xf86drm.c \
        xf86drmHash.c \
        xf86drmRandom.c \
        ioctl.cc

LIBDRM_DIR := $(REP_DIR)/contrib/libdrm-2.4.52
INC_DIR    += $(LIBDRM_DIR) $(LIBDRM_DIR)/include/drm $(LIBDRM_DIR)/intel

LIBS   += libc cxx
CC_OPT += -U__linux__
CC_OPT += -DHAVE_LIBDRM_ATOMIC_PRIMITIVES=1

vpath %.c      $(LIBDRM_DIR)/intel
vpath %.c      $(LIBDRM_DIR)
vpath ioctl.cc $(REP_DIR)/src/lib/libdrm
