VNCSERVER_PORT_DIR := $(call select_from_ports,vncserver)

TARGET = vncserver

LIBS = vncserver libc zlib

#LIBS = vncclient sdl libc zlib libc_lwip_nic_dhcp config_args

SRC_C = example.c

vpath %c $(VNCSERVER_PORT_DIR)/src/lib/vncserver/examples
