include $(REP_DIR)/ports/fuse-ext2.inc
FUSE_EXT2_DIR = $(REP_DIR)/contrib/$(FUSE_EXT2)
E2FSPROGS_DIR = $(FUSE_EXT2_DIR)/e2fsprogs-1.41.12.newgit
EXT2FS_DIR    = $(E2FSPROGS_DIR)/ext2fs
ET_DIR        = $(E2FSPROGS_DIR)/et

FILTER_OUT = bmove.c dosio.c irel_ma.c nt_io.c tdbtool.c
SRC_C = $(filter-out $(FILTER_OUT), $(notdir $(wildcard $(EXT2FS_DIR)/*.c)))
SRC_C += $(notdir $(wildcard $(ET_DIR)/*.c))

INC_DIR += $(REP_DIR)/include/fuse-ext2 \
           $(REP_DIR)/src/lib/fuse-ext2 \
           $(ET_DIR)

CC_OPT += -DHAVE_CONFIG_H -D__BSD_VISIBLE

LIBS += libc

vpath %.c $(EXT2FS_DIR)
vpath %.c $(ET_DIR)
