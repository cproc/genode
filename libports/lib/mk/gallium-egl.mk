include $(REP_DIR)/lib/mk/mesa10.inc

include $(REP_DIR)/lib/import/import-egl.mk

# sources and compiler options extracted from the Linux build log for libegl.a

SRC_C = common/egl_g3d_api.c \
        common/egl_g3d.c \
        common/egl_g3d_image.c \
        common/egl_g3d_st.c \
        common/egl_g3d_sync.c \
        common/native_helper.c \
        common/native_wayland_drm_bufmgr.c \

SRC_CC = genode/native_genode.cc

# TODO: winsys/sw is null-specific, probably not needed for Genode winsys
INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/state_trackers/egl \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/winsys/sw \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/egl/main \

CC_OPT += -DHAVE_GENODE_BACKEND

LIBS = libc

vpath %.c  $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/state_trackers/egl
vpath %.cc $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/state_trackers/egl

