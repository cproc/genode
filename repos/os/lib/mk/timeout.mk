SRC_CC += timeout.cc
SRC_CC += timer.cc
SRC_CC += timer_timestamp.cc
SRC_CC += duration.cc

LIBS += alarm

INC_DIR += $(BASE_DIR)/src/include

vpath % $(REP_DIR)/src/lib/timeout
