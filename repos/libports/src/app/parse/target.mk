# we need pcsc-lite headers, but do not want link with it nor with its libccid dependency
include $(call select_from_repositories,lib/import/import-pcsc-lite.mk)

LIBCCID_DIR=$(call select_from_ports,libccid)

TARGET = parse
LIBS = libusb libc
SRC_C = parse.c debug.c ccid_usb.c tokenparser.c

INC_DIR += $(LIBCCID_DIR)/src/lib/libccid/src
INC_DIR += $(REP_DIR)/src/lib/libccid

vpath %.c $(LIBCCID_DIR)/src/lib/libccid/src
