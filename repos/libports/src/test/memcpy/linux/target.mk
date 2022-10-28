TARGET = test-libc_memcpy
SRC_CC = main.cc
LIBS  += libc posix

INC_DIR += $(PRG_DIR)/..

CC_CXX_WARN_STRICT_CONVERSION =
