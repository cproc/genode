#
# \brief  Build configs that are specific to base-hw/x86_64 on the Muen SK
# \author Martin Stein
# \date   2011-12-20
#

# denote wich specs are also fullfilled by this spec
SPECS += hw x86_64_muen
SPECS += pci ps2 vesa framebuffer

# configure multiprocessor mode
NR_OF_CPUS = 1

# set address where to link text segment at
LD_TEXT_ADDR ?= 0x200000

# include implied specs
include $(call select_from_repositories,mk/spec-hw.mk)
include $(call select_from_repositories,mk/spec-x86_64.mk)
