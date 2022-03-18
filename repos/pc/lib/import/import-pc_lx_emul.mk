LIBS += pc_linux_generated

LX_SRC_DIR := $(call select_from_ports,linux)/src/linux
ifeq ($(wildcard $(LX_SRC_DIR)),)
LX_SRC_DIR := $(call select_from_repositories,src/linux)
endif

LX_GEN_DIR := $(LIB_CACHE_DIR)/pc_linux_generated

include $(call select_from_repositories,lib/import/import-lx_emul_common.inc)

# Handle specific source requirements
CC_OPT_drivers/usb/host/xhci-trace += -I$(LX_SRC_DIR)/drivers/usb/host

SRC_CC  += lx_emul/clock.cc
SRC_CC  += lx_emul/io_mem.cc
SRC_CC  += lx_emul/io_port.cc
SRC_CC  += lx_emul/irq.cc
SRC_C   += lx_emul/shadow/kernel/dma/mapping.c
SRC_C   += lx_emul/shadow/kernel/irq/spurious.c
SRC_C   += lx_emul/shadow/kernel/rcu/tree.c
SRC_C   += lx_emul/shadow/kernel/sched/sched.c
SRC_C   += lx_emul/shadow/lib/devres.c
SRC_C   += lx_emul/shadow/lib/smp_processor_id.c
SRC_C   += lx_emul/shadow/mm/memblock.c
SRC_C   += lx_emul/shadow/mm/page_alloc.c
SRC_CC  += lx_emul/pci_config_space.cc
SRC_CC  += lx_emul/pci_init.cc
SRC_CC  += lx_kit/device.cc
SRC_CC  += lx_kit/memory_dma.cc
SRC_CC  += lx_kit/spec/x86/platform.cc
