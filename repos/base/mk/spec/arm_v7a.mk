SPECS    += arm_v7
CC_MARCH ?= -march=armv7-a -mfpu=vfp -mfloat-abi=softfp

include $(BASE_DIR)/mk/spec/arm_v7.mk

