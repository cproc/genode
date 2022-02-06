include $(PRG_DIR)/../../target.inc

CC_OPT += -DJEMALLOC_STRESS_TEST

vpath %.c $(JEMALLOC_DIR)/test/stress
