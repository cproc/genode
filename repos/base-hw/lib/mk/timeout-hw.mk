SRC_CC += timeout.cc
SRC_CC += timer.cc
SRC_CC += hw/timer_timestamp.cc
SRC_CC += duration.cc

LIBS += alarm

INC_DIR += $(BASE_DIR)/src/include

vpath % $(BASE_DIR)/../os/src/lib/timeout
