#
# Make Linux headers of the host platform available to the program
#
include $(call select_from_repositories,lib/import/import-syscall.mk)

#
# Manually supply all library search paths of the host compiler to our tool
# chain.
#
HOST_LIB_SEARCH_DIRS := $(shell cc -print-search-dirs | grep libraries |\
                                sed "s/.*=//"   | sed "s/:/ /g" |\
                                sed "s/\/ / /g" | sed "s/\/\$$//")
#
# Add search path for 'limits.h'
#
INC_DIR += $(shell echo "int main() {return 0;}" |\
                   LANG=C $(CXX) -x c++ -v -E - 2>&1 |\
                   sed '/^\#include <\.\.\.> search starts here:/,/^End of search list/!d' |\
                   grep "include-fixed")

#
# Add search paths for normal libraries
#
CXX_LINK_OPT += $(addprefix -L,$(HOST_LIB_SEARCH_DIRS))

#
# Add search paths for shared-library lookup
#
# We add all locations of shared libraries present in the ld.cache to our
# library search path.
#
HOST_SO_SEARCH_DIRS := $(sort $(dir $(shell ldconfig -p | sed "s/^.* \//\//" | grep "^\/")))
LINK_ARG_PREFIX := -Wl,
CXX_LINK_OPT += $(addprefix $(LINK_ARG_PREFIX)-rpath-link $(LINK_ARG_PREFIX),$(HOST_SO_SEARCH_DIRS))

#
# The '__libc_csu_init' function is normally provided by the C library. We
# override the libc's version in our 'lx_hybrid' library to have a hook for
# Genode-specific initializations. Unfortunately, this way, we get two symbols
# with the same name. So we have to tell the linker to be forgiving. The order
# of the libraries at the linker command line determines which symbol is used.
# Therefore it is important to have 'lx_hybrid.lib.so' listed before '-lc',
# which is always the case when supplying '-lc' via 'EXT_OBJECTS' (not
# 'CXX_LINK_OPT').
#
CXX_LINK_OPT += -Wl,--allow-multiple-definition

#
# Make exceptions work
#
CXX_LINK_OPT += -Wl,--eh-frame-hdr

#
# Add all libraries and their dependencies specified at the 'LX_LIBS'
# variable to the linker command line
#
ifneq ($(LX_LIBS),)
EXT_OBJECTS = $(shell pkg-config --static --libs $(LX_LIBS))
endif

#
# Use the host's startup codes, linker script, and dynamic linker
#
EXT_OBJECTS += $(shell cc -print-file-name=crt1.o)
EXT_OBJECTS += $(shell cc -print-file-name=crti.o)
EXT_OBJECTS += $(shell cc -print-file-name=crtbegin.o)
EXT_OBJECTS += $(shell cc -print-file-name=crtend.o)
EXT_OBJECTS += $(shell cc -print-file-name=crtn.o)
EXT_OBJECTS += -lgcc -lgcc_s -lsupc++ -lc
EXT_OBJECTS += -lpthread

#
# Some header files installed on GNU/Linux test for the GNU compiler. For
# example, 'stdio.h' might complain with the following error otherwise:
#
#  /usr/include/stdio.h:432:27: error: expected initializer before ‘throw’
#  /usr/include/stdio.h:488:6: error: expected initializer before ‘throw’
#
# By manually defining '_GNU_SOURCE', the header files are processed as
# expected.
#
CC_OPT += -D_GNU_SOURCE

USE_HOST_LD_SCRIPT = yes

ifeq (x86_64,$(findstring x86_64,$(SPECS)))
CXX_LINK_OPT += -Wl,--dynamic-linker=/lib64/ld-linux-x86-64.so.2
else
CXX_LINK_OPT += -Wl,--dynamic-linker=/lib/ld-linux.so.2
endif

# because we use the host compiler's libgcc, omit the Genode toolchain's version
LD_LIBGCC =
