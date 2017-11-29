CONFIGURE_ARGS = --with-screen=ncurses --disable-nls

LIBS = glib ncurses pthread

Makefile: dummy_libs

LDFLAGS += -L$(PWD)

dummy_libs: libcurses.a libncurses.a libncursesw.a

libcurses.a:
	$(VERBOSE)$(AR) -rc $@

libncurses.a:
	$(VERBOSE)$(AR) -rc $@

libncursesw.a:
	$(VERBOSE)$(AR) -rc $@

include $(call select_from_repositories,mk/noux.mk)
