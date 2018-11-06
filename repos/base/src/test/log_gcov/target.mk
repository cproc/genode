TARGET = test-log_gcov
SRC_CC = main.cc func.cc
LIBS   = base libgcov

#CC_OPT += -fprofile-arcs -ftest-coverage -fprofile-dir='/test/log'
#CC_OPT += -fprofile-arcs -ftest-coverage

COVERAGE = yes
