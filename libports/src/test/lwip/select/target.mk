TARGET   = test-select
LIBS     = cxx env libc libc_log libc_lwip lwip pthread
SRC_CC   = main.cc

CC_OPT_main += -fpermissive

INC_DIR += $(REP_DIR)/src/lib/lwip/include
