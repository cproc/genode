include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_LIBS = libQt5Core libQt5Gui libQt5Network libQt5Sql libQt5Test libQt5Widgets

LIBS = libc libm mesa stdcxx $(QT5_LIBS)

built.tag: qmake_prepared.tag

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/genode-x86-g++/qt.conf \
		$(QT_DIR)/qtdeclarative/qtdeclarative.pro \
		-- \
		-no-feature-qml-devtools

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
		$(STRIP) libQt5Qml.lib.so -o libQt5Qml.lib.so.stripped && \
		$(STRIP) libQt5Quick.lib.so -o libQt5Quick.lib.so.stripped && \
		$(STRIP) libQt5QuickWidgets.lib.so -o libQt5QuickWidgets.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Qml.lib.so.stripped $(PWD)/bin/libQt5Qml.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Quick.lib.so.stripped $(PWD)/bin/libQt5Quick.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5QuickWidgets.lib.so.stripped $(PWD)/bin/libQt5QuickWidgets.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Qml.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5Quick.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5QuickWidgets.lib.so $(PWD)/debug/

	@#
	@# create QML plugin archives and links
	@#

	ln -sf .$(CURDIR)/qmake_root install/qt

	tar chf $(PWD)/bin/qt5_qtquick2.tar -C install --exclude '*.lib.so' qt/qml/QtQuick.2
	tar chf $(PWD)/bin/qt5_qtquick2plugin.tar -C install qt/qml/QtQuick.2/libqtquick2plugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick.2/libqtquick2plugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_qtquick_window.tar -C install --exclude '*.lib.so' qt/qml/QtQuick/Window.2
	tar chf $(PWD)/bin/qt5_windowplugin.tar -C install qt/qml/QtQuick/Window.2/libwindowplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Window.2/libwindowplugin.lib.so $(PWD)/bin/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
