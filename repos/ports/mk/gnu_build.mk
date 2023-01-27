#
# \brief  Rules for building software that uses the GNU build tools
# \author Norman Feske
# \date   2011-02-02
#
# This file is meant to be included by 'target.mk' files. Instead of building
# a normal Genode target, the 'target.mk' file will then trigger the build
# of the contrib package specified with the 'PKG' variable. The build
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
# So if a library used by a package is changed, the package gets not relinked
# automatically. In this case, 'make clean' must be invoked manually within the
# build subdirectory of the package.
#

#
# Use name of the directory hosting the 'target.mk' file as 'TARGET'
# and 'PKG' by default.
#
TARGET ?= $(lastword $(subst /, ,$(PRG_DIR)))
PKG    ?= $(TARGET)

#
# Select how to make the build result available at Genode's 'INSTALL_DIR'.
# By default, a single tar archive containing the results of the package's
# 'make install' rule is created. By setting the 'INSTALL_TREE' variable
# to a non-empty value, a symlink to the actual file tree is created.
#
# By default, the generated tar archive contains the entire content of the
# package's install directory. The 'INSTALL_TAR_CONTENT' variable may be
# overriden to manually specify only the parts of the content.
#
INSTALL_TREE        ?=
INSTALL_TAR_ARCHIVE ?= yes
INSTALL_TAR_CONTENT ?= .

LIBS += posix

PWD = $(shell pwd)

#
# Detect missing preparation ofpackage
#
ifeq ($(PKG_DIR),)
REQUIRES += prepare_$(PKG)
endif

#
# Verbose build
#
ifeq ($(VERBOSE),)
MAKE_VERBOSE = V=1
CONFIGURE_VERBOSE =

#
# Non-verbose build
#
else
MAKE_VERBOSE = -s
CONFIGURE_VERBOSE = --quiet

# filter for configure output of package
CONFIGURE_OUTPUT_FILTER = > stdout.log 2> stderr.log ||\
	  (echo "Error: configure script of $(PKG) failed" && \
	   cat stderr.log && false)

# filter for make output of package
BUILD_OUTPUT_FILTER = 2>&1 | sed "s/^/      [$(PKG)]  /"

endif

ifeq ($(findstring arm, $(SPECS)), arm)
CONFIGURE_ARGS += --host arm-none-eabi
endif
ifeq ($(findstring arm_v8, $(SPECS)), arm_v8)
CONFIGURE_ARGS += --host aarch64-none-elf
endif
ifeq ($(findstring riscv, $(SPECS)), riscv)
CONFIGURE_ARGS += --host riscv64-unknown-elf
endif
ifeq ($(findstring x86, $(SPECS)), x86)
CONFIGURE_ARGS += --host x86_64-pc-elf
endif

CONFIGURE_ARGS += --srcdir=$(PKG_DIR)
CONFIGURE_ARGS += --prefix /

CONFIG_GUESS_SCRIPT = $(PKG_DIR)/config.guess
ifneq ($(wildcard $(CONFIG_GUESS_SCRIPT)),)
CONFIGURE_ARGS += --build $(shell $(CONFIG_GUESS_SCRIPT))
else
CONFIGURE_ARGS += --build x86-linux
endif

CONFIGURE_ARGS += $(CONFIGURE_VERBOSE)

LDFLAGS += -nostdlib $(CXX_LINK_OPT) $(CC_MARCH) -Wl,-T$(LD_SCRIPT_DYN) \
           -Wl,-rpath-link=$(PWD) \
           -Wl,--dynamic-linker=$(DYNAMIC_LINKER).lib.so \
           -Wl,--eh-frame-hdr

LIBTOOLFLAGS = --preserve-dup-deps

LIBGCC = $(shell $(CC) $(CC_MARCH) -print-libgcc-file-name)

CPPFLAGS += -nostdinc $(INCLUDES)
CPPFLAGS += -D_GNU_SOURCE=1 -fPIC

# flags to be used in both CFLAGS and CXXFLAGS
COMMON_CFLAGS_CXXFLAGS += -ffunction-sections $(CC_OLEVEL) $(CC_MARCH)
COMMON_CFLAGS_CXXFLAGS += -g

CFLAGS   += $(COMMON_CFLAGS_CXXFLAGS)
CXXFLAGS += $(COMMON_CFLAGS_CXXFLAGS)

#
# We have to specify 'LINK_ITEMS' twice to resolve inter-library dependencies.
# Unfortunately, the use of '--start-group' and '--end-group' does not suffice
# in all cases because 'libtool' strips those arguments from the 'LIBS' variable.
#
# 'STATIC_LIBS', 'EXT_OBJECTS' and 'LIBGCC' contain absolute paths.
# We convert these to '-l:*.a' so libtool does not add these existing static
# libraries (found in 'LIBS') to any new static libraries it creates.
#
LDLIBS_A_ABSOLUTE = $(filter %.a, $(sort $(STATIC_LIBS)) $(EXT_OBJECTS) $(LIBGCC))
LDLIBS_A  = $(addprefix -l:,$(notdir $(LDLIBS_A_ABSOLUTE)))
LDLIBS_SO = $(addprefix -l:,$(sort $(SHARED_LIBS)))
LDLIBS   += -L$(PWD) $(LDLIBS_A) $(LDLIBS_SO) $(LDLIBS_A)

static_libs_symlinks.tag:
	$(VERBOSE)for lib in $(LDLIBS_A_ABSOLUTE); do ln -s $$lib; done
	$(VERBOSE)touch $@

#
# By default, assume that there exists a 'configure' script in the top-level
# of the package.
#
CONFIGURE_SCRIPT ?= $(PKG_DIR)/configure

#
# Re-configure the Makefile if the Genode build environment changes
#
Makefile reconfigure: $(MAKEFILE_LIST)

#
# Invoke configure script with the Genode environment
#
Makefile reconfigure: env.sh static_libs_symlinks.tag $(SHARED_LIBS)
	@$(MSG_CONFIG)$(TARGET)
	$(VERBOSE)source env.sh && $(CONFIGURE_SCRIPT) $(MKENV) $(CONFIGURE_ARGS) $(CONFIGURE_OUTPUT_FILTER)

env.sh:
	$(VERBOSE)rm -f $@
	$(VERBOSE)echo "export CC='$(CC)'" >> $@
	$(VERBOSE)echo "export CXX='$(CXX)'" >> $@
	$(VERBOSE)echo "export LD='$(LD)'" >> $@
	$(VERBOSE)echo "export AR='$(AR)'" >> $@
	$(VERBOSE)echo "export NM='$(NM)'" >> $@
	$(VERBOSE)echo "export RANLIB='$(RANLIB)'" >> $@
	$(VERBOSE)echo "export STRIP='$(STRIP)'" >> $@
	$(VERBOSE)echo "export CPPFLAGS='$(CPPFLAGS)'" >> $@
	$(VERBOSE)echo "export CFLAGS='$(CFLAGS)'" >> $@
	$(VERBOSE)echo "export CXXFLAGS='$(CXXFLAGS)'" >> $@
	$(VERBOSE)echo "export LDFLAGS='$(LDFLAGS)'" >> $@
	$(VERBOSE)echo "export LIBS='$(LDLIBS)'" >> $@
	$(VERBOSE)echo "export LIBTOOLFLAGS='$(LIBTOOLFLAGS)'" >> $@
	$(VERBOSE)echo "export PS1='<gnu_build>'" >> $@

#
# Invoke the 'Makefile' generated by the configure script
#
# The 'MAN=' argument prevent manual pages from being created. This would
# produce an error when the package uses the 'help2man' tool. This tool
# tries to extract the man page of a program by executing it with the
# '--help' argument on the host. However, we cannot run Genode binaries
# this way.
#
built.tag: env.sh Makefile
	@$(MSG_BUILD)$(TARGET)
	$(VERBOSE)source env.sh &&\
	          $(MAKE) $(MAKE_ENV) $(MAKE_VERBOSE) $(MAKE_TARGET) MAN= \
	          $(BUILD_OUTPUT_FILTER)
	@touch $@

INSTALL_TARGET ?= install-strip

#
# Install result of the build in an 'install/' directory local to the target's
# build directory. The default install step 'make install-strip' can be
# customized by setting 'INSTALL_TARGET' to be empty. This is useful to
# explicitly filter the installed content in the 'target.mk' file.
#
installed.tag: built.tag

ifneq ($(INSTALL_TARGET),)
installed.tag:
	@$(MSG_INST)$(TARGET)
	$(VERBOSE)source env.sh && $(MAKE) $(MAKE_ENV) $(MAKE_VERBOSE) $(INSTALL_TARGET) DESTDIR=$(PWD)/install MAN= >> stdout.log 2>> stderr.log
	@touch $@
endif

$(TARGET): installed.tag
	@touch $@

#
# Trigger creation of symlinks to the build results at '<build-dir>/bin/'
#
ifneq ($(INSTALL_TAR_ARCHIVE),)
$(TARGET): installed_tar.tag
endif

ifneq ($(INSTALL_TREE),)
$(TARGET): installed_tree.tag
endif

#
# Install symlink to the install directory
#
installed_tree.tag: installed.tag
	$(VERBOSE)rm -f $(INSTALL_DIR)/$(TARGET)
	$(VERBOSE)ln -sf $(PWD)/install $(INSTALL_DIR)/$(TARGET)

#
# Install symlink to the archived install directory, ready to by mounted
# via the VFS tar file system
#
installed_tar.tag: installed.tag
	$(VERBOSE)tar cf $(TARGET).tar -h -C $(PWD)/install $(INSTALL_TAR_CONTENT)
	$(VERBOSE)rm -f $(INSTALL_DIR)/$(TARGET)
	$(VERBOSE)ln -sf $(PWD)/$(TARGET).tar $(INSTALL_DIR)/$(TARGET).tar

#
# The clean rule is expected to be executed within the 3rd-party build
# directory. The check should prevent serious damage if this condition
# is violated (e.g., while working on the build system).
#
ifeq ($(notdir $(PWD)),$(notdir $(PRG_DIR)))
clean_dir:
	$(VERBOSE)find $(PWD) -mindepth 1 -delete

clean_prg_objects: clean_dir
endif
