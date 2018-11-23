SRC_CC   = main.cc
TARGET   = test-ldso
LIBS     = test-ldso_lib_1 test-ldso_lib_2 libc libm sanitizer-libasan
INC_DIR += $(REP_DIR)/src/test/ldso/include

#CC_OPT += -fsanitize=undefined
CC_OPT += -fsanitize=address

CC_CXX_WARN_STRICT =
