TARGET   = test-libc
SRC_CC   = main.cc
LIBS     = posix gcov

CC_OPT += -fprofile-arcs -ftest-coverage -fprofile-dir='/gcov'

CC_CXX_WARN_STRICT =
