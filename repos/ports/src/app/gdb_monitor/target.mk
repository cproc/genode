TARGET  =  gdb_monitor

GDB_CONTRIB_DIR = $(call select_from_ports,gdb)/src/noux-pkg/gdb

INC_DIR += $(GDB_CONTRIB_DIR)/include \
           $(GDB_CONTRIB_DIR)/gdb \
           $(GDB_CONTRIB_DIR)/gdb/common \
           $(GDB_CONTRIB_DIR)/gdb/gdbserver \
           $(GDB_CONTRIB_DIR)/gdb/gnulib/import \
           $(GDB_CONTRIB_DIR)/gdb/regformats \
           $(REP_DIR)/src/lib/gdbserver_libc_support \
           $(PRG_DIR)/gdbserver \
           $(PRG_DIR)

LIBS    =  libc libc_terminal libc_pipe \
           gdbserver_platform gdbserver_libc_support

# libiberty
SRC_C   = argv.c \
          crc32.c

# gnulib
SRC_C  +=  rawmemchr.c \
           strchrnul.c

SRC_C  +=  agent.c \
           ax.c \
           buffer.c \
           cleanups.c \
           common-debug.c \
           common-exceptions.c \
           common-utils.c \
           debug.c \
           dll.c \
           errors.c \
           event-loop.c \
           filestuff.c \
           format.c \
           gdb_vecs.c \
           hostio.c \
           i387-fp.c \
           inferiors.c \
           mem-break.c \
           notif.c \
           posix-strerror.c \
           print-utils.c \
           ptid.c \
           remote-utils.c \
           regcache.c \
           rsp-low.c \
           server.c \
           signals.c \
           symbol.c \
           target.c \
           tdesc.c \
           tracepoint.c \
           utils.c \
           vec.c \
           waitstatus.c \
           x86-dregs.c \
           x86-low.c \
           xml-utils.c

SRC_C  +=  linux-low.c \
           linux-ptrace.c

CC_OPT += -DGDBSERVER -DPKGVERSION="\"7.11.1\"" -DREPORT_BUGS_TO="\"\""

CC_OPT += -DHAVE_SYS_WAIT_H -DHAVE_SYS_PTRACE_H -DHAVE_DECL_PTRACE

CC_OPT_linux-low += -Wno-unused-function

SRC_CC  =  genode-low.cc \
           cpu_session_component.cc \
           cpu_thread_component.cc \
           region_map_component.cc \
           signal_handler_thread.cc \
           main.cc

CC_OPT_genode-low += -fpermissive

vpath %.c  $(GDB_CONTRIB_DIR)/gdb/common
vpath %.c  $(GDB_CONTRIB_DIR)/gdb/gdbserver
vpath %.c  $(GDB_CONTRIB_DIR)/gdb/gnulib/import
vpath %.c  $(GDB_CONTRIB_DIR)/gdb/nat
vpath %.c  $(GDB_CONTRIB_DIR)/gdb/target
vpath %.c  $(GDB_CONTRIB_DIR)/libiberty
vpath %.cc $(PRG_DIR)/gdbserver

#
# Files from init
#
# Because the 'server.h' file exists both in gdb and in init and both gdb's
# 'server.c' and init's 'server.cc' are compiled to a 'server.o' file, the
# parent directory of the init source is used as reference.
#

INIT_PARENT_DIR = $(abspath $(dir $(call select_from_repositories,src/init/server.cc))/..)

INC_DIR += $(INIT_PARENT_DIR)

SRC_CC += init/server.cc

vpath init/%.cc $(INIT_PARENT_DIR)

CC_CXX_WARN_STRICT =
