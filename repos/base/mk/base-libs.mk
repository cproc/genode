#
# Genode base libaries
#
# These static libraries are filtered out when linking dynamically linked
# binaries.
#
BASE_LIBS = base-common base cxx timed_semaphore alarm config

#
# Name of Genode's dynamic linker
#
DYNAMIC_LINKER = ld
