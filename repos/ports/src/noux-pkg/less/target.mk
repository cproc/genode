TARGET = less

LIBS   = ncurses

#
# Make the ncurses linking test succeed and copy the actual Makefile
#
Makefile: dummy_libs

LDFLAGS += -L$(PWD)

dummy_libs: libncursesw.a

libncursesw.a:
	$(VERBOSE)$(AR) -rc $@

INSTALL_TARGET = install

include $(call select_from_repositories,mk/noux.mk)
