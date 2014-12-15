#
# \brief  Rules for building software that uses the GNU build tools
# \author Norman Feske
# \date   2011-02-02
#
# This file is meant to be included by 'target.mk' files. Instead of building
# a normal Genode target, the 'target.mk' file will then trigger the build
# of the contrib package specified with the 'NOUX_PKG' variable. The build
# consists to the following steps
#
# 1. Configuring the package by invoking the package's 'configure' script
#    with the arguments required for cross compiling the package for
#    Genode.
# 2. Building the package by invoking the 'Makefile' generated by the
#    'configure' script.
#
# Limitations
# -----------
#
# In contrast to the Genode build system, library dependencies are not covered.
# So if a library used by a noux package is changed, the noux package gets not
# relinked automatically. In this case, 'make clean' must be invoked manually
# within the build subdirectory of the noux package.
#

#
# Use name of the directory hosting the 'target.mk' file as 'TARGET'
# and 'NOUX_PKG' by default.
#
TARGET ?= $(lastword $(subst /, ,$(PRG_DIR)))

NOUX_PKG ?= $(TARGET)

LIBS  += libc libm libc_noux

NOUX_PKG_DIR ?= $(call select_from_ports,$(NOUX_PKG))/src/noux-pkg/$(NOUX_PKG)

PWD = $(shell pwd)

#
# Detect missing preparation of noux package
#
ifeq ($(NOUX_PKG_DIR),)
REQUIRES += prepare_$(NOUX_PKG)
endif

#
# Verbose build
#
ifeq ($(VERBOSE),)
NOUX_MAKE_VERBOSE = V=1
NOUX_CONFIGURE_VERBOSE =

#
# Non-verbose build
#
else
NOUX_MAKE_VERBOSE = -s
NOUX_CONFIGURE_VERBOSE = --quiet

# filter for configure output of noux package
NOUX_CONFIGURE_OUTPUT_FILTER = > stdout.log 2> stderr.log ||\
	  (echo "Error: configure script of $(NOUX_PKG) failed" && \
	   cat stderr.log && false)

# filter for make output of noux package
NOUX_BUILD_OUTPUT_FILTER = 2>&1 | sed "s/^/      [$(NOUX_PKG)]  /"

endif

ifeq ($(findstring arm, $(SPECS)), arm)
NOUX_CONFIGURE_ARGS += --host arm-none-eabi
else
ifeq ($(findstring x86, $(SPECS)), x86)
NOUX_CONFIGURE_ARGS += --host x86_64-pc-elf
endif
endif

NOUX_CONFIGURE_ARGS += --srcdir=$(NOUX_PKG_DIR)
NOUX_CONFIGURE_ARGS += --prefix /

CONFIG_GUESS_SCRIPT = $(NOUX_PKG_DIR)/config.guess)
ifneq ($(wildcard $(CONFIG_GUESS_SCRIPT)),)
NOUX_CONFIGURE_ARGS += --build $(shell $(CONFIG_GUESS_SCRIPT))
else
NOUX_CONFIGURE_ARGS += --build x86-linux
endif

NOUX_CONFIGURE_ARGS += $(NOUX_CONFIGURE_VERBOSE)

NOUX_LDFLAGS += -nostdlib $(CXX_LINK_OPT) $(CC_MARCH) -Wl,-T$(LD_SCRIPT_DYN) \
                -Wl,--dynamic-linker=$(DYNAMIC_LINKER).lib.so \
                -Wl,--eh-frame-hdr
NOUX_LIBTOOLFLAGS = --preserve-dup-deps

LIBGCC = $(shell $(CC) $(CC_MARCH) -print-libgcc-file-name)

NOUX_CPPFLAGS += -nostdinc $(INCLUDES)
NOUX_CPPFLAGS += -D_GNU_SOURCE=1

# flags to be used in both CFLAGS and CXXFLAGS
NOUX_COMMON_CFLAGS_CXXFLAGS += -ffunction-sections $(CC_OLEVEL) $(CC_MARCH)
NOUX_COMMON_CFLAGS_CXXFLAGS += -g

NOUX_CFLAGS += $(NOUX_COMMON_CFLAGS_CXXFLAGS)
NOUX_CXXFLAGS += $(NOUX_COMMON_CFLAGS_CXX_FLAGS)

#
# We have to specify 'LINK_ITEMS' twice to resolve inter-library dependencies.
# Unfortunately, the use of '--start-group' and '--end-group' does not suffice
# in all cases because 'libtool' strips those arguments from the 'LIBS' variable.
#

NOUX_LIBS_A  = $(filter %.a, $(sort $(LINK_ITEMS)) $(EXT_OBJECTS) $(LIBGCC))
NOUX_LIBS_SO = $(filter %.so,$(sort $(LINK_ITEMS)) $(EXT_OBJECTS) $(LIBGCC))
NOUX_LIBS += $(NOUX_LIBS_A) $(NOUX_LIBS_SO) $(NOUX_LIBS_A)

#
# Re-configure the Makefile if the Genode build environment changes
#
Makefile reconfigure: $(MAKEFILE_LIST)

#
# Invoke configure script with the Genode environment
#
Makefile reconfigure: noux_env.sh
	@$(MSG_CONFIG)$(TARGET)
	$(VERBOSE)source noux_env.sh && $(NOUX_PKG_DIR)/configure $(NOUX_ENV) $(NOUX_CONFIGURE_ARGS) $(NOUX_CONFIGURE_OUTPUT_FILTER)

noux_env.sh:
	$(VERBOSE)rm -f $@
	$(VERBOSE)echo "export CC='$(CC)'" >> $@
	$(VERBOSE)echo "export CXX='$(CXX)'" >> $@
	$(VERBOSE)echo "export LD='$(LD)'" >> $@
	$(VERBOSE)echo "export AR='$(AR)'" >> $@
	$(VERBOSE)echo "export NM='$(NM)'" >> $@
	$(VERBOSE)echo "export RANLIB='$(RANLIB)'" >> $@
	$(VERBOSE)echo "export STRIP='$(STRIP)'" >> $@
	$(VERBOSE)echo "export CPPFLAGS='$(NOUX_CPPFLAGS)'" >> $@
	$(VERBOSE)echo "export CFLAGS='$(NOUX_CFLAGS)'" >> $@
	$(VERBOSE)echo "export CXXFLAGS='$(NOUX_CXXFLAGS)'" >> $@
	$(VERBOSE)echo "export LDFLAGS='$(NOUX_LDFLAGS)'" >> $@
	$(VERBOSE)echo "export LIBS='$(NOUX_LIBS)'" >> $@
	$(VERBOSE)echo "export LIBTOOLFLAGS='$(NOUX_LIBTOOLFLAGS)'" >> $@
	$(VERBOSE)echo "export PS1='<noux>'" >> $@

#
# Invoke the 'Makefile' generated by the configure script
#
# The 'MAN=' argument prevent manual pages from being created. This would
# produce an error when the package uses the 'help2man' tool. This tool
# tries to extract the man page of a program by executing it with the
# '--help' argument on the host. However, we cannot run Genode binaries
# this way.
#
noux_built.tag: noux_env.sh Makefile
	@$(MSG_BUILD)$(TARGET)
	$(VERBOSE)source noux_env.sh && $(MAKE) $(NOUX_MAKE_ENV) $(NOUX_MAKE_VERBOSE) MAN= $(NOUX_BUILD_OUTPUT_FILTER)
	@touch $@

NOUX_INSTALL_TARGET ?= install-strip

noux_installed.tag: noux_built.tag
	@$(MSG_INST)$(TARGET)
	$(VERBOSE)source noux_env.sh && $(MAKE) $(NOUX_MAKE_ENV) $(NOUX_MAKE_VERBOSE) $(NOUX_INSTALL_TARGET) DESTDIR=$(PWD)/install MAN= >> stdout.log 2>> stderr.log
	$(VERBOSE)rm -f $(INSTALL_DIR)/$(TARGET)
	$(VERBOSE)ln -sf $(PWD)/install $(INSTALL_DIR)/$(TARGET)
	@touch $@

$(TARGET): noux_installed.tag
	@touch $@

#
# The clean rule is expected to be executed within the 3rd-party build
# directory. The check should prevent serious damage if this condition
# is violated (e.g., while working on the build system).
#
ifeq ($(notdir $(PWD)),$(notdir $(PRG_DIR)))
clean_noux_build_dir:
	$(VERBOSE)rm -rf $(PWD)

clean_prg_objects: clean_noux_build_dir
endif


