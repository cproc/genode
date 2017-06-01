TARGET   = test-ping_client_libc_lxip
LIBS     = posix libc_lxip
SRC_CC   = main.cc pingpong.cc

vpath main.cc     $(PRG_DIR)/..
vpath pingpong.cc $(PRG_DIR)/../..
