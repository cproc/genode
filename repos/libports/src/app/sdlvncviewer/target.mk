SDLVNCVIEWER_PORT_DIR := $(call select_from_ports,vncserver)

TARGET = sdlvncviewer

SRC_C = SDLvncviewer.c scrap.c

LIBS = vncclient sdl libc zlib config_args

vpath %.c $(SDLVNCVIEWER_PORT_DIR)/src/lib/vncserver/client_examples
