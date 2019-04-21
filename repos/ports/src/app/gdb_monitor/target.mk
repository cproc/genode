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

LIBS    =  stdcxx libc libc_terminal libc_pipe \
           gdbserver_platform gdbserver_libc_support

# libiberty
SRC_C   = argv.c \
          crc32.c

# gnulib
SRC_C  += rawmemchr.c \
          strchrnul.c

# common
SRC_CC += agent.cc \
          buffer.cc \
          cleanups.cc \
          common-debug.cc \
          common-exceptions.cc \
          common-utils.cc \
          errors.cc \
          filestuff.cc \
          format.cc \
          gdb_vecs.cc \
          posix-strerror.cc \
          print-utils.cc \
          ptid.cc \
          rsp-low.cc \
          signals.cc \
          vec.cc \
          xml-utils.cc

# gdbserver
SRC_CC += ax.cc \
          debug.cc \
          dll.cc \
          event-loop.cc \
          hostio.cc \
          i387-fp.cc \
          inferiors.cc \
          linux-low.cc \
          mem-break.cc \
          notif.cc \
          regcache.cc \
          remote-utils.cc \
          server.cc \
          symbol.cc \
          target.cc \
          tdesc.cc \
          tracepoint.cc \
          utils.cc \
          x86-low.cc \

# nat
SRC_CC += linux-ptrace.cc \
          x86-dregs.cc \

# target
SRC_CC += waitstatus.cc

# genode
SRC_CC += genode-low.cc \
          cpu_session_component.cc \
          cpu_thread_component.cc \
          region_map_component.cc \
          signal_handler_thread.cc \
          main.cc

CC_OPT += -DGDBSERVER -DPKGVERSION="\"8.0.1\"" -DREPORT_BUGS_TO="\"\""
CC_OPT += -DHAVE_SYS_WAIT_H -DHAVE_SYS_PTRACE_H -DHAVE_DECL_PTRACE -DHAVE_TERMIOS
CC_OPT_linux-low    += -fpermissive -Wno-unused-function
CC_OPT_linux-ptrace += -fpermissive
CC_OPT_genode-low   += -fpermissive

vpath %.c  $(GDB_CONTRIB_DIR)/gdb/gnulib/import
vpath %.c  $(GDB_CONTRIB_DIR)/libiberty
vpath %.cc $(GDB_CONTRIB_DIR)/gdb/common
vpath %.cc $(GDB_CONTRIB_DIR)/gdb/gdbserver
vpath %.cc $(GDB_CONTRIB_DIR)/gdb/nat
vpath %.cc $(GDB_CONTRIB_DIR)/gdb/target
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
