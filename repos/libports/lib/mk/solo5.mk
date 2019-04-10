REQUIRES += 64bit

SHARED_LIB = yes

CC_OPT += -D__SOLO5_BINDINGS__ -Drestrict=__restrict__

# GCC 8 complains about a mismatch of the 'log()' function declaration
CC_OPT += -Wno-error=builtin-declaration-mismatch

SRC_CC = bindings.cc

SOLO5_PORT_DIR := $(call select_from_ports,solo5)

INC_DIR += $(SOLO5_PORT_DIR)/src/lib/solo5/bindings
INC_DIR += $(SOLO5_PORT_DIR)/include/solo5
INC_DIR += $(REP_DIR)/include/solo5

vpath %.cc $(SOLO5_PORT_DIR)/src/lib/solo5/bindings/genode
vpath %.cc $(REP_DIR)/src/lib/solo5/bindings/genode

REQUIRES = adaptation_to_block_session_api_change
