LIBVNCSERVER_PORT_DIR := $(call select_from_ports,vncserver)

INC_DIR += $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver \
           $(REP_DIR)/include/vncserver
