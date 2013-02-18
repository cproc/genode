#
# \brief  Portions of base library shared by core and non-core processes
# \author Norman Feske
# \date   2013-02-14
#

LIBS += cxx l4 startup

SRC_CC += cap_copy.cc
SRC_CC += ipc/ipc.cc ipc/pager.cc ipc/ipc_marshal_cap.cc
SRC_CC += pager/pager.cc
SRC_CC += avl_tree/avl_tree.cc
SRC_CC += allocator/slab.cc
SRC_CC += allocator/allocator_avl.cc
SRC_CC += heap/heap.cc heap/sliced_heap.cc
SRC_CC += console/console.cc
SRC_CC += child/child.cc
SRC_CC += process/process.cc
SRC_CC += elf/elf_binary.cc
SRC_CC += lock/lock.cc
SRC_CC += signal/signal.cc
SRC_CC += server/server.cc server/common.cc
SRC_CC += thread/thread.cc thread/thread_bootstrap.cc
SRC_CC += env/utcb.cc
SRC_CC += lock/cmpxchg.cc

INC_DIR += $(REP_DIR)/src/base/lock
INC_DIR += $(REP_DIR)/include/codezero/dummies

vpath cap_copy.cc $(BASE_DIR)/src/platform
vpath %.cc        $(REP_DIR)/src/base
vpath %.cc        $(BASE_DIR)/src/base
