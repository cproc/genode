include $(REP_DIR)/ports/mesa10.inc

# common compiler options

CC_OPT += -DSTDC_HEADERS=1 \
          -DHAVE_SYS_TYPES_H=1 \
          -DHAVE_SYS_STAT_H=1 \
          -DHAVE_STDLIB_H=1 \
          -DHAVE_STRING_H=1 \
          -DHAVE_MEMORY_H=1 \
          -DHAVE_STRINGS_H=1 \
          -DHAVE_INTTYPES_H=1 \
          -DHAVE_STDINT_H=1 \
          -DHAVE_UNISTD_H=1 \
          -DHAVE_DLFCN_H=1 \
          -DLT_OBJDIR=\".libs/\" \
          -DYYTEXT_POINTER=1 \
          -DHAVE___BUILTIN_BSWAP32=1 \
          -DHAVE___BUILTIN_BSWAP64=1 \
          -DHAVE_CLOCK_GETTIME=1 \
          -DHAVE_PTHREAD=1 \
          -D_GNU_SOURCE \
          -DHAVE_PTHREAD \
          -DHAVE_DLOPEN \
          -DHAVE_POSIX_MEMALIGN \
          -DMESA_EGL_NO_X11_HEADERS \

CC_C_OPT += -std=c99
