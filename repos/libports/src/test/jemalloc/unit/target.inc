include $(PRG_DIR)/../../target.inc

CC_OPT += -DJEMALLOC_UNIT_TEST

vpath %.c $(JEMALLOC_DIR)/test/unit
