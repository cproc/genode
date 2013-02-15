include $(BASE_DIR)/lib/mk/env.mk

SRC_CC += utcb.cc

LIBS += debug

vpath utcb.cc $(REP_DIR)/src/base/env
vpath env.cc $(BASE_DIR)/src/base/env
