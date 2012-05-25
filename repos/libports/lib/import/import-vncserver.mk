LIBVNCSERVER_PORT_DIR := $(call select_from_ports,vncserver)
LIBVNCSERVER_REP_INC_DIR := $(call select_from_repositories,include/vncserver)

INC_DIR += $(LIBVNCSERVER_PORT_DIR)/src/lib/vncserver \
           $(LIBVNCSERVER_REP_INC_DIR)
