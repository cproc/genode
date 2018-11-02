SANITIZER_PORT_DIR := $(call select_from_ports,sanitizer)

SANITIZER_DIR := $(SANITIZER_PORT_DIR)/src/lib/sanitizer/libsanitizer

SRC_CC = \
         sanitizer_allocator.cc \
         sanitizer_common.cc \
         sanitizer_common_libcdep.cc \
         sanitizer_coverage_libcdep.cc \
         sanitizer_flag_parser.cc \
         sanitizer_flags.cc \
         sanitizer_libc.cc \
         sanitizer_posix.cc \
         sanitizer_posix_libcdep.cc \
         sanitizer_printf.cc \
         sanitizer_stacktrace_libcdep.cc \
         sanitizer_stacktrace_printer.cc \
         sanitizer_suppressions.cc \
         sanitizer_symbolizer.cc \
         sanitizer_symbolizer_libbacktrace.cc \
         sanitizer_symbolizer_libcdep.cc \
         sanitizer_symbolizer_posix_libcdep.cc

LIBS += libc

INC_DIR += \
           $(SANITIZER_DIR)

#           $(REP_DIR)/src/lib/gcov-libbacktrace \
#           $(GCOV_DIR)/include \
#           $(GCOV_DIR)/libcpp/include \


vpath %.cc $(SANITIZER_DIR)/sanitizer_common

CC_CXX_WARN_STRICT =
