TARGET = gcov

GCOV_PORT_DIR := $(call select_from_ports,gcov)

GCOV_DIR := $(GCOV_PORT_DIR)/src/lib/gcov

# XXX: alternative to symlink?
SRC_CC = gcov.cc

CC_OPT_gcov = -DIN_GCC -DHAVE_CONFIG_H

INC_DIR += $(GCOV_DIR)/gcc
INC_DIR += $(GCOV_DIR)/include
INC_DIR += $(GCOV_DIR)/libcpp/include
INC_DIR += $(PRG_DIR)

LIBS += base libc posix gmp stdcxx gcov-libcommon gcov-libcpp gcov-libiberty

vpath %.cc $(GCOV_DIR)/gcc

CC_CXX_WARN_STRICT =
