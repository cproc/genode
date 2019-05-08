CONFIGURE_ARGS = --with-screen=ncurses --disable-nls

LIBS = glib ncurses

Makefile: dummy_libs

LDFLAGS += -L$(PWD)

dummy_libs: libcurses.a libncurses.a libncursesw.a libglib-2.0.a

libcurses.a:
	$(VERBOSE)$(AR) -rc $@

libncurses.a:
	$(VERBOSE)$(AR) -rc $@

libncursesw.a:
	$(VERBOSE)$(AR) -rc $@

libglib-2.0.a:
	$(VERBOSE)$(AR) -rc $@

include $(call select_from_repositories,mk/noux.mk)
