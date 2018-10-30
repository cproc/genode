GCOV_PORT_DIR := $(call select_from_ports,gcov)

GCOV_DIR := $(GCOV_PORT_DIR)/src/lib/gcov

SRC_C = \
        argv.c \
        concat.c \
        cp-demangle.c \
        cplus-dem.c \
        d-demangle.c \
        filename_cmp.c \
        fopen_unlocked.c \
        hashtab.c \
        lbasename.c \
        obstack.c \
        safe-ctype.c \
        vprintf-support.c \
        xexit.c \
        xmalloc.c \
        xstrdup.c \
        xstrerror.c \
        xvasprintf.c

CC_OPT += -DHAVE_CONFIG_H

LIBS += libc stdcxx

INC_DIR += \
           $(REP_DIR)/src/lib/gcov-libiberty \
           $(GCOV_DIR)/include
#           $(GCOV_DIR)/gcc \

vpath %.c $(GCOV_DIR)/libiberty
#vpath %.cc $(REP_DIR)/src/lib/gcov

CC_CXX_WARN_STRICT =
