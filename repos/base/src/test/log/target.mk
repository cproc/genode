TARGET = test-log
SRC_CC = main.cc
LIBS   = base sanitizer-libubsan

CC_OPT += -fsanitize=undefined
