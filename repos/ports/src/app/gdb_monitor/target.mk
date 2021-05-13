TARGET  =  gdb_monitor

GDB_CONTRIB_DIR = $(call select_from_ports,gdb)/src/noux-pkg/gdb

INC_DIR += $(GDB_CONTRIB_DIR)/include \
           $(GDB_CONTRIB_DIR) \
           $(GDB_CONTRIB_DIR)/gdb \
           $(GDB_CONTRIB_DIR)/gdb/gdbserver \
           $(GDB_CONTRIB_DIR)/gdb/regformats \
           $(GDB_CONTRIB_DIR)/gnulib/import \
           $(REP_DIR)/src/lib/gdbserver_libc_support \
           $(PRG_DIR)/gdbserver \
           $(PRG_DIR)

LIBS    =  stdcxx libc \
           gdbserver_platform gdbserver_libc_support

# libiberty
SRC_C   = argv.c \
          concat.c \
          crc32.c \
          xstrdup.c

# gnulib
SRC_C  += rawmemchr.c \
          strchrnul.c

# gdbsupport
SRC_CC += agent.cc \
          buffer.cc \
          cleanups.cc \
          common-debug.cc \
          common-exceptions.cc \
          common-inferior.cc \
          common-utils.cc \
          environ.cc \
          errors.cc \
          filestuff.cc \
          format.cc \
          gdb_tilde_expand.cc \
          gdb_vecs.cc \
          job-control.cc \
          netstuff.cc \
          pathstuff.cc \
          print-utils.cc \
          ptid.cc \
          rsp-low.cc \
          safe-strerror.cc \
          signals.cc \
          tdesc.cc \
          xml-utils.cc

# gdb
SRC_CC += gdbserver/ax.cc \
          gdbserver/debug.cc \
          gdbserver/dll.cc \
          gdbserver/event-loop.cc \
          gdbserver/fork-child.cc \
          gdbserver/hostio.cc \
          gdbserver/i387-fp.cc \
          gdbserver/inferiors.cc \
          gdbserver/linux-low.cc \
          gdbserver/mem-break.cc \
          gdbserver/notif.cc \
          gdbserver/regcache.cc \
          gdbserver/remote-utils.cc \
          gdbserver/server.cc \
          gdbserver/symbol.cc \
          gdbserver/target.cc \
          gdbserver/tdesc.cc \
          gdbserver/tracepoint.cc \
          gdbserver/utils.cc \
          gdbserver/x86-low.cc \
          nat/fork-inferior.cc \
          nat/linux-ptrace.cc \
          nat/x86-dregs.cc \
          target/waitstatus.cc \
          alloc.cc

# genode
SRC_CC += genode-low.cc \
          cpu_session_component.cc \
          cpu_thread_component.cc \
          region_map_component.cc \
          signal_handler_thread.cc \
          main.cc

CC_OPT += -DGDBSERVER -DPKGVERSION="\"8.3.0\"" -DREPORT_BUGS_TO="\"\""
CC_OPT += -DHAVE_SYS_WAIT_H -DHAVE_SYS_PTRACE_H -DHAVE_DECL_PTRACE -DHAVE_TERMIOS
CC_OPT += -fpermissive -Wno-unused-function

vpath %.c  $(GDB_CONTRIB_DIR)/gnulib/import
vpath %.c  $(GDB_CONTRIB_DIR)/libiberty
vpath %.cc $(GDB_CONTRIB_DIR)/gdb
vpath %.cc $(GDB_CONTRIB_DIR)/gdbsupport
vpath %.cc $(PRG_DIR)/gdbserver

#
# Files from sandbox library
#
# Because the 'server.h' file exists both in gdb and in init and both gdb's
# 'server.c' and init's 'server.cc' are compiled to a 'server.o' file, the
# parent directory of the init source is used as reference.
#

SANDBOX_SRC_DIR    = $(call select_from_repositories,src/lib/sandbox)
SANDBOX_PARENT_DIR = $(abspath $(addsuffix /..,$(SANDBOX_SRC_DIR)))

SRC_CC += sandbox/server.cc

# needed to compile gdbserver/genode-low.cc
INC_DIR += $(SANDBOX_PARENT_DIR)

vpath sandbox/%.cc $(SANDBOX_PARENT_DIR)

#
# Import headers needed from sandbox library, but exclude server.h because it
# collides with the GDB server's server.h
#
SANDBOX_HEADERS := $(notdir $(wildcard $(addsuffix /*.h,$(SANDBOX_SRC_DIR))))
SANDBOX_HEADERS := $(filter-out server.h,$(SANDBOX_HEADERS))

genode-low.o sandbox/server.o: $(SANDBOX_HEADERS)

%.h: $(SANDBOX_SRC_DIR)/%.h
	ln -sf $< $@

CC_CXX_WARN_STRICT =
