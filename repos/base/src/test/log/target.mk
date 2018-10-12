TARGET = test-log
SRC_CC = main.cc func.cc
LIBS   = base gcov

CC_OPT += -fprofile-arcs -ftest-coverage -fprofile-dir='/gcov'

