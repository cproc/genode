#
# lwIP TCP/IP library
#
# The library implements TCP and UDP as well as DNS and DHCP.
#

LWIP_PORT_DIR := $(call select_from_ports,lwip)
LWIPDIR := $(LWIP_PORT_DIR)/src/lib/lwip/src

-include $(LWIPDIR)/Filelists.mk

# Genode platform files
SRC_CC = printf.cc sys_arch.cc rand.cc

# Core files
SRC_C += $(notdir $(COREFILES))

# IPv4 files
SRC_C += $(notdir $(CORE4FILES))

# IPv6 files
SRC_C += $(notdir $(CORE6FILES))

# Network interface files
SRC_C += $(notdir $(NETIFFILES))

LIBS     = alarm libc timed_semaphore

LD_OPT  += --version-script=$(REP_DIR)/src/lib/lwip/symbol.map

INC_DIR += $(REP_DIR)/include/lwip \
           $(LWIP_PORT_DIR)/include/lwip \
           $(LWIPDIR)/include \
           $(LWIPDIR)/include/ipv4 \
           $(LWIPDIR)/include/api \
           $(LWIPDIR)/include/netif \
           $(REP_DIR)/src/lib/lwip/include

vpath %.cc $(REP_DIR)/src/lib/lwip/platform
vpath %.c  $(LWIPDIR)/core
vpath %.c  $(LWIPDIR)/core/ipv4
vpath %.c  $(LWIPDIR)/core/ipv6
vpath %.c  $(LWIPDIR)/netif
