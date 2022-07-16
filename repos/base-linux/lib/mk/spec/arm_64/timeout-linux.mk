SRC_CC += timeout.cc
SRC_CC += timer_connection.cc
SRC_CC += timer_connection_time.cc
SRC_CC += linux/timer_connection_timestamp.cc
SRC_CC += duration.cc

INC_DIR += $(BASE_DIR)/src/include

LIBS += syscall-linux

vpath % $(BASE_DIR)/src/lib/timeout
