include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_PORT_LIBS = libQt5Core libQt5Gui libQt5Widgets

LIBS = libc libm mesa stdcxx $(QT5_PORT_LIBS)

built.tag: qmake_prepared.tag

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/$(QMAKE_PLATFORM)/qt.conf \
		$(QT_DIR)/qtsvg/qtsvg.pro

	@#
	@# build
	@#

	source env.sh && $(MAKE) sub-src

	@#
	@# install into local 'install' directory
	@#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install sub-src-install_subtargets

	@#
	@# create stripped versions
	@#

	cd $(CURDIR)/install$(CURDIR)/qmake_root/lib && \
		$(STRIP) libQt5Svg.lib.so -o libQt5Svg.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Svg.lib.so.stripped $(PWD)/bin/libQt5Svg.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Svg.lib.so $(PWD)/debug/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
