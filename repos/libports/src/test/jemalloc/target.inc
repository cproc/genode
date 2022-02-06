TEST = $(shell basename $(PRG_DIR))

TARGET = test-jemalloc_$(TEST)

SRC_C += $(TEST).

JEMALLOC_DIR = $(call select_from_ports,jemalloc)/src/lib/jemalloc

SRC_C += SFMT.c mtx.c mq.c test.c thd.c timer.c

LIBS = posix jemalloc_test jemalloc

INC_DIR += $(PRG_DIR)/../../include
INC_DIR += $(REP_DIR)/src/lib/jemalloc/include
INC_DIR += $(REP_DIR)/src/lib/jemalloc_test/include

INC_DIR += $(JEMALLOC_DIR)/include
INC_DIR += $(JEMALLOC_DIR)/test/include

vpath %.c $(JEMALLOC_DIR)/test/src

CC_CXX_WARN_STRICT =
