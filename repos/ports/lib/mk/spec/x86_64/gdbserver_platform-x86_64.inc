INC_DIR += $(REP_DIR)/src/lib/gdbserver_platform/spec/x86_64

SRC_C   = i386.cc \
          i386-avx.cc \
          amd64.cc \
          amd64-avx.cc \
          linux-x86-low.cc \
          x86-linux.cc \
          x86-linux-dregs.cc

CC_OPT_linux-x86-low   += -fpermissive -Wno-unused-function
CC_OPT_x86-linux-dregs += -fpermissive

include $(REP_DIR)/lib/mk/gdbserver_platform.inc
