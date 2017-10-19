#
# \brief  Build config for Genodes core process
# \author Stefan Kalkowski
# \author Martin Stein
# \date   2012-10-04
#

REQUIRES = muen

LIBS += sinfo-muen

# add include paths
INC_DIR += $(REP_DIR)/src/core/spec/x86_64/muen

# add C++ sources
SRC_CC += spec/x86_64/muen/kernel/cpu_exception.cc
SRC_CC += spec/x86_64/muen/kernel/thread_exception.cc
SRC_CC += spec/x86_64/muen/platform_support.cc
SRC_CC += spec/x86_64/muen/kernel/vm.cc
SRC_CC += spec/x86_64/muen/platform_services.cc
SRC_CC += spec/x86_64/muen/sinfo_instance.cc
SRC_CC += spec/x86_64/muen/timer.cc
SRC_CC += kernel/vm_thread_on.cc

# include less specific configuration
include $(BASE_DIR)/../base-hw/lib/mk/spec/x86_64/core-hw.inc
