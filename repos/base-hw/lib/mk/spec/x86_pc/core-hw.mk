#
# \brief  Build config for Genodes core process
# \author Stefan Kalkowski
# \author Martin Stein
# \date   2012-10-04
#

REQUIRES = x86_pc

# add C++ sources
SRC_CC += kernel/vm_thread_off.cc
SRC_CC += spec/x86_64/pic.cc
SRC_CC += spec/x86_64/timer.cc
SRC_CC += spec/x86_64/kernel/cpu_exception.cc
SRC_CC += spec/x86_64/kernel/thread_exception.cc
SRC_CC += spec/x86_64/platform_support.cc
SRC_CC += spec/x86/platform_services.cc

# include less specific configuration
include $(BASE_DIR)/../base-hw/lib/mk/spec/x86_64/core-hw.inc
