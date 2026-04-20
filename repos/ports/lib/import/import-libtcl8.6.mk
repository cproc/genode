ifeq ($(CONTRIB_DIR),)
TCL_PORT_DIR := $(abspath $(call select_from_repositories,lib/import/import-libtcl8.6.mk)/../../..)
else
TCL_PORT_DIR := $(call select_from_ports,tcl)
endif

INC_DIR += $(TCL_PORT_DIR)/include
