SRC_CC = vfs.cc

INC_DIR += $(REP_DIR)/src/lib/vfs/usb

LIBS  += libc jitterentropy

vpath %.cc $(REP_DIR)/src/lib/vfs/usb

SHARED_LIB = yes
