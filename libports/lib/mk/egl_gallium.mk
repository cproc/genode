include $(REP_DIR)/lib/mk/mesa10.inc

include $(REP_DIR)/lib/import/import-egl.mk

# sources and compiler options extracted from the Linux build log for egl_gallium.so

SRC_C = egl.c \
        egl_pipe.c \
        egl_st.c

CC_OPT += -D_EGL_MAIN=_eglMain \
          -DFEATURE_GL=1 \
          -DFEATURE_ES2=1 \
          -DGALLIUM_SOFTPIPE \

#          -DGALLIUM_RBUG \
#          -DGALLIUM_TRACE \

# -fvisibility=hidden -I../../../../include -I../../../../src/loader -I../../../../src/gallium/drivers -I../../../../src/gallium/winsys -I../../../../src/gallium/include -I../../../../src/gallium/auxiliary -I../../../../src/gallium/state_trackers/egl -I../../../../src/egl/main -D_EGL_MAIN=_eglMain -I../../../../src/mesa -DFEATURE_GL=1 -DFEATURE_ES2=1 -DGALLIUM_SOFTPIPE -DGALLIUM_RBUG -DGALLIUM_TRACE -pthread -g -O2 -Wall -std=c99 -Werror=implicit-function-declaration -Werror=missing-prototypes -fno-strict-aliasing -fno-builtin-memcmp


INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/egl/main \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/drivers \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/state_trackers/egl \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/winsys \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/loader \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/mesa \

LIBS = libc gallium-aux

vpath %.c $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/targets/egl-static

SHARED_LIB = yes
