include $(REP_DIR)/lib/import/import-vncserver.mk

#LIBVNCSERVER_PORT_DIR := $(call select_from_ports,vncserver)

LIBS += libc zlib jpeg libpng pthread libc_lwip libc_lwip_nic_dhcp

INC_DIR += $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/libvncserver \
           $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/common

SRC_C = main.c \
        rfbserver.c \
        rfbregion.c \
        auth.c \
        sockets.c \
        stats.c \
        corre.c \
        hextile.c \
        rre.c \
        translate.c \
        cutpaste.c \
        httpd.c \
        cursor.c \
        font.c \
        draw.c \
        selbox.c \
        d3des.c \
        vncauth.c \
        cargs.c \
        minilzo.c \
        ultra.c \
        scale.c \
        zlib.c \
        zrle.c \
        zrleoutstream.c \
        zrlepalettehelper.c \
        zywrletemplate.c \
        tight.c \
        turbojpeg.c

SHARED_LIB = yes

vpath d3des.c          $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/common
vpath vncauth.c        $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/common
vpath minilzo.c        $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/common
vpath zywrletemplate.c $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/common
vpath turbojpeg.c      $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/common
vpath %.c              $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver/libvncserver
