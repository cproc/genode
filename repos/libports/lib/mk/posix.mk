SRC_CC = construct.cc

LIBS += libc libm

SHARED_LIB = yes

vpath construct.cc $(REP_DIR)/src/lib/posix

CC_CXX_WARN_STRICT =
