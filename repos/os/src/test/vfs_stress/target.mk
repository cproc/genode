TARGET = vfs_stress
SRC_CC = main.cc
LIBS   = base vfs

CC_WARN += -Wno-error=array-bounds
