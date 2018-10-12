GCOV_PORT_DIR := $(call select_from_ports,gcov)

GCOV_DIR := $(GCOV_PORT_DIR)/src/lib/gcov

SRC_C = libgcov-merge.c \
        libgcov-profiler.c \
        libgcov-interface.c \
        libgcov-driver.c

LIBGCOV_MERGE = _gcov_merge_add \
                _gcov_merge_single \
                _gcov_merge_delta \
                _gcov_merge_ior \
                _gcov_merge_time_profile \
                _gcov_merge_icall_topn

LIBGCOV_PROFILER = _gcov_interval_profiler \
                   _gcov_pow2_profiler \
                   _gcov_one_value_profiler \
                   _gcov_indirect_call_profiler	\
                   _gcov_average_profiler \
                   _gcov_ior_profiler \
                   _gcov_indirect_call_profiler_v2 \
                   _gcov_time_profiler \
                   _gcov_indirect_call_topn_profiler

LIBGCOV_INTERFACE = _gcov_dump \
                    _gcov_flush \
                    _gcov_fork \
                    _gcov_execl \
                    _gcov_execlp \
                    _gcov_execle \
                    _gcov_execv \
                    _gcov_execvp \
                    _gcov_execve \
                    _gcov_reset

LIBGCOV_DRIVER = _gcov

CC_OPT += -fbuilding-libgcc -DIN_GCC -DIN_LIBGCC2

CC_OPT += $(addprefix -DL,$(LIBGCOV_MERGE))
CC_OPT += $(addprefix -DL,$(LIBGCOV_PROFILER))
CC_OPT += $(addprefix -DL,$(LIBGCOV_INTERFACE))
CC_OPT += $(addprefix -DL,$(LIBGCOV_DRIVER))

LIBS += libc

INC_DIR += $(GCOV_DIR)/include \
           $(GCOV_DIR)/gcc

vpath %.c $(GCOV_DIR)/libgcc

# XXX: most symbols were missing in the shared library
#SHARED_LIB = yes

CC_CXX_WARN_STRICT =
