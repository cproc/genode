LIBC_RESOLV_DIR = $(LIBC_DIR)/libc/resolv

SRC_C = $(notdir $(wildcard $(LIBC_RESOLV_DIR)/*.c))

SRC_C += nsap_addr.c

include $(REP_DIR)/lib/mk/libc-common.inc

vpath %.c $(LIBC_RESOLV_DIR)
vpath nsap_addr.c $(LIBC_DIR)/libc/inet
