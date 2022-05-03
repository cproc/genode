include $(PRG_DIR)/../../target.inc

CC_OPT += -DJEMALLOC_INTEGRATION_TEST

vpath %.c $(JEMALLOC_DIR)/test/integration
