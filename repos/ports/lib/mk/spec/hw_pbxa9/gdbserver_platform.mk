SRC_CC = spec/hw_pbxa9/low.cc \
         spec/hw/native_cpu.cc

SRC_C  = reg-arm.c \
         linux-arm-low.c

CC_OPT_linux-arm-low += -Wno-unused-function

include $(REP_DIR)/lib/mk/gdbserver_platform.inc
