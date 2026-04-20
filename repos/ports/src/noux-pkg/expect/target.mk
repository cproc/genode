CONFIGURE_ARGS += --with-tcl=$(PWD)
CONFIGURE_ARGS += --with-tclinclude=$(TCL_PORT_DIR)/include
CONFIGURE_ARGS += --disable-rpath
CONFIGURE_ARGS += "tcl_cv_sys_version=FreeBSD-12.0-RELEASE"

CFLAGS += "-Wno-error=implicit-function-declaration"
CFLAGS += "-Wno-error=incompatible-pointer-types"

LIBS += libtcl8.6 ldso_so_support

INSTALL_TARGET := install

$(INSTALL_DIR)/libexpect5454.lib.so: built.tag
	$(VERBOSE)ln -sf $(PWD)/install/lib/expect5.45.4/libexpect5454.lib.so $@

installed.tag: $(INSTALL_DIR)/libexpect5454.lib.so

include $(call select_from_repositories,mk/noux.mk)

Makefile: dummy_libs tclConfig.sh

.SECONDARY: dummy_libs
dummy_libs: libdl.a

libdl.a:
	$(VERBOSE)$(AR) -rc $@

tclConfig.sh:
	$(VERBOSE)touch $@
