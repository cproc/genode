GDB_CONTRIB_DIR = $(call select_from_ports,gdb)/src/noux-pkg/gdb

INC_DIR += $(REP_DIR)/src/lib/gdbserver_libc_support \
           $(REP_DIR)/src/lib/gdbserver_platform \
           $(REP_DIR)/src/app/gdb_monitor \
           $(GDB_CONTRIB_DIR)/include \
           $(GDB_CONTRIB_DIR)/gdb \
           $(GDB_CONTRIB_DIR)/gdb/common \
           $(GDB_CONTRIB_DIR)/gdb/gdbserver \
           $(GDB_CONTRIB_DIR)/gdb/gnulib/import \
           $(GDB_CONTRIB_DIR)/gdb/regformats \
           $(REP_DIR)/src/app/gdb_monitor/gdbserver

SRC_CC += gdbserver_platform_helper.cc

CC_OPT_gdbserver_platform_helper += -fpermissive

CC_OPT += -DGDBSERVER

LIBS += libc

# generated files

REGFORMATS_DIR := $(abspath ${GDB_CONTRIB_DIR}/gdb/regformats)

reg-arm.c:
	$(VERBOSE) cd $(dir $@) && $(SHELL) $(REGFORMATS_DIR)/regdat.sh $(REGFORMATS_DIR)/reg-arm.dat $(notdir $@)

i386.c:
	$(VERBOSE) cd $(dir $@) && $(SHELL) $(REGFORMATS_DIR)/regdat.sh $(REGFORMATS_DIR)/i386/i386.dat $(notdir $@)

i386-avx.c:
	$(VERBOSE) cd $(dir $@) && $(SHELL) $(REGFORMATS_DIR)/regdat.sh $(REGFORMATS_DIR)/i386/i386-avx.dat $(notdir $@)

amd64.c:
	$(VERBOSE) cd $(dir $@) && $(SHELL) $(REGFORMATS_DIR)/regdat.sh $(REGFORMATS_DIR)/i386/amd64.dat $(notdir $@)

amd64-avx.c:
	$(VERBOSE) cd $(dir $@) && $(SHELL) $(REGFORMATS_DIR)/regdat.sh $(REGFORMATS_DIR)/i386/amd64-avx.dat $(notdir $@)

vpath %.cc $(REP_DIR)/src/lib/gdbserver_platform
vpath %.c  $(GDB_CONTRIB_DIR)/gdb/gdbserver
vpath %.c  $(GDB_CONTRIB_DIR)/gdb/nat
