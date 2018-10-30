GCOV_PORT_DIR := $(call select_from_ports,gcov)

GCOV_DIR := $(GCOV_PORT_DIR)/src/gcov

SRC_CC = diagnostic.cc \
         diagnostic-color.cc \
         diagnostic-show-locus.cc \
         ggc-none.cc \
         hash-table.cc \
         input.cc \
         intl.cc \
         memory-block.cc \
         pretty-print.cc \
         vec.cc \
         version.cc

CC_OPT += -DIN_GCC

CC_OPT_version += -DBASEVER="\"6.3.0\"" -DDATESTAMP="\"\"" -DREVISION="\"\"" -DDEVPHASE="\"\"" -DPKGVERSION="\"(GCC) \"" -DBUGURL="\"<http://gcc.gnu.org/bugs.html>\"" -DIN_GCC

LIBS += libc gmp stdcxx

INC_DIR += $(GCOV_DIR)/include \
           $(GCOV_DIR)/libcpp/include \
           $(REP_DIR)/src/lib/gcov-libcommon/gcc

vpath %.cc $(GCOV_DIR)/gcc

CC_CXX_WARN_STRICT =
