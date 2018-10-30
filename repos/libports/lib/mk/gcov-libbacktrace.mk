GCOV_PORT_DIR := $(call select_from_ports,gcov)

GCOV_DIR := $(GCOV_PORT_DIR)/src/lib/gcov

SRC_C = \
        alloc.c \
        backtrace.c \
        fileline.c \
        state.c

LIBS += libc

INC_DIR += \
           $(REP_DIR)/src/lib/gcov-libbacktrace \
#           $(GCOV_DIR)/libbacktrace
#           $(GCOV_DIR)/include \
#           $(GCOV_DIR)/libcpp/include \


vpath %.c $(GCOV_DIR)/libbacktrace

CC_CXX_WARN_STRICT =
