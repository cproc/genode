include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

LIBS = base libc libm stdcxx qt5_component egl mesa expat qoost

# the Nitpicker QPA plugin uses the base library
QT5_GENODE_LIBS_SHLIB += ld.lib.so

built.tag: qmake_prepared.tag qmake_root/lib/ld.lib.so

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/$(QMAKE_PLATFORM)/qt.conf \
		$(QT_DIR)/qtbase/qtbase.pro \
		-- \
		-prefix /qt \
		-xplatform $(QMAKE_PLATFORM) \
		-qpa nitpicker \
		-opensource \
		-confirm-license \
		-no-pch \
		-debug \
		-no-strip \
		-opengl desktop \
		-no-sse2 \
		-no-accessibility \
		-no-feature-dbus \
		-no-feature-filesystemwatcher \
		-no-feature-networkinterface \
		-no-feature-process

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
		$(STRIP) libQt5Core.lib.so -o libQt5Core.lib.so.stripped && \
		$(STRIP) libQt5Gui.lib.so -o libQt5Gui.lib.so.stripped && \
		$(STRIP) libQt5Network.lib.so -o libQt5Network.lib.so.stripped && \
		$(STRIP) libQt5PrintSupport.lib.so -o libQt5PrintSupport.lib.so.stripped && \
		$(STRIP) libQt5Sql.lib.so -o libQt5Sql.lib.so.stripped && \
		$(STRIP) libQt5Test.lib.so -o libQt5Test.lib.so.stripped && \
		$(STRIP) libQt5Widgets.lib.so -o libQt5Widgets.lib.so.stripped

	cd $(CURDIR)/install$(CURDIR)/qmake_root/plugins/platforms && \
		$(STRIP) libqnitpicker.lib.so -o libqnitpicker.lib.so.stripped

	cd $(CURDIR)/install$(CURDIR)/qmake_root/plugins/imageformats && \
		$(STRIP) libqjpeg.lib.so -o libqjpeg.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Core.lib.so.stripped $(PWD)/bin/libQt5Core.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Gui.lib.so.stripped $(PWD)/bin/libQt5Gui.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Network.lib.so.stripped $(PWD)/bin/libQt5Network.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5PrintSupport.lib.so.stripped $(PWD)/bin/libQt5PrintSupport.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Sql.lib.so.stripped $(PWD)/bin/libQt5Sql.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Test.lib.so.stripped $(PWD)/bin/libQt5Test.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Widgets.lib.so.stripped $(PWD)/bin/libQt5Widgets.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/plugins/platforms/libqnitpicker.lib.so.stripped $(PWD)/bin/libqnitpicker.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/plugins/imageformats/libqjpeg.lib.so.stripped $(PWD)/bin/libqjpeg.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Core.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Gui.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Network.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5PrintSupport.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Sql.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Test.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Widgets.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/plugins/platforms/libqnitpicker.lib.so $(PWD)/debug/libqnitpicker.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/plugins/imageformats/libqjpeg.lib.so $(PWD)/debug/libqjpeg.lib.so

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
