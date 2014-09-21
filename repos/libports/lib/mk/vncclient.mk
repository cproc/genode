include $(REP_DIR)/lib/import/import-vncclient.mk

#LIBVNCCLIENT_PORT_DIR := $(call select_from_ports,vncserver)

LIBS += libc zlib jpeg libc_lwip libc_lwip_nic_dhcp

INC_DIR += $(LIBVNCCLIENT_PORT_DIR)/src/lib/vncserver/common

SRC_C := cursor.c \
         listen.c \
         rfbproto.c \
         sockets.c \
         vncviewer.c \
         minilzo.c \
         tls_none.c

SHARED_LIB = yes

vpath minilzo.c $(LIBVNCCLIENT_PORT_DIR)/src/lib/vncserver/common
vpath %.c $(LIBVNCCLIENT_PORT_DIR)/src/lib/vncserver/libvncclient
