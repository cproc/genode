REQUIRES = arm_v8a

TARGET   = imx8_fb_drv
LIBS     = base imx8_fb_include lx_kit_setjmp imx8_fb_drv blit
SRC_CC   = main.cc platform.cc lx_emul.cc
#SRC_C    = dummies.c i915_params.c lx_emul_c.c

# lx_kit
#SRC_CC  += \
#           mapped_io_mem_range.cc \
#           pci.cc \
#           printf.cc \

SRC_CC += \
          env.cc \
          irq.cc \
          malloc.cc \
          scheduler.cc \
          timer.cc \
          work.cc

INC_DIR += $(REP_DIR)/src/include

vpath %.cc $(PRG_DIR)
vpath %.cc $(REP_DIR)/src/lx_kit

CC_CXX_WARN_STRICT =
CC_OPT += -Wno-narrowing
