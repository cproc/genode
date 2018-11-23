SANITIZER_PORT_DIR := $(call select_from_ports,sanitizer)

SANITIZER_DIR := $(SANITIZER_PORT_DIR)/src/lib/sanitizer/libsanitizer

SRC_CC = \
         asan_allocator.cc \
         asan_fake_stack.cc \
         asan_flags.cc \
         asan_genode.cc \
         asan_globals.cc \
         asan_interceptors.cc \
         asan_poisoning.cc \
         asan_report.cc \
         asan_rtl.cc \
         asan_stack.cc \
         asan_stats.cc \
         asan_suppressions.cc \
         asan_thread.cc

LIBS += libc sanitizer-libsanitizer_common sanitizer-libubsan

INC_DIR += \
           $(SANITIZER_DIR)

vpath %.cc $(SANITIZER_DIR)/asan

CC_CXX_WARN_STRICT =
