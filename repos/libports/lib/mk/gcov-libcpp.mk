GCOV_PORT_DIR := $(call select_from_ports,gcov)

GCOV_DIR := $(GCOV_PORT_DIR)/src/gcov

SRC_CC = line-map.cc

LIBS += libc

INC_DIR += $(REP_DIR)/src/lib/gcov-libcpp \
           $(GCOV_DIR)/include \
           $(GCOV_DIR)/libcpp/include

vpath %.cc $(GCOV_DIR)/libcpp

CC_CXX_WARN_STRICT =
