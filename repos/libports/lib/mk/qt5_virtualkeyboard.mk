include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_PORT_LIBS  = libQt5Core libQt5Gui libQt5Network libQt5Widgets
QT5_PORT_LIBS += libQt5Qml libQt5Quick
QT5_PORT_LIBS += libQt5Svg

LIBS = libc libm mesa stdcxx $(QT5_PORT_LIBS)

built.tag: qmake_prepared.tag

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/$(QMAKE_PLATFORM)/qt.conf \
		$(QT_DIR)/qtvirtualkeyboard/qtvirtualkeyboard.pro

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
		$(STRIP) libQt5VirtualKeyboard.lib.so -o libQt5VirtualKeyboard.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5VirtualKeyboard.lib.so.stripped $(PWD)/bin/libQt5VirtualKeyboard.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5VirtualKeyboard.lib.so $(PWD)/debug/

	@#
	@# create plugin archive, QML archive and links
	@#

	ln -sf .$(CURDIR)/qmake_root install/qt

	tar chf $(PWD)/bin/qt5_qtvirtualkeyboardplugin.tar -C install qt/plugins/platforminputcontexts/libqtvirtualkeyboardplugin.lib.so
	ln -sf $(CURDIR)/install/qt/plugins/platforminputcontexts/libqtvirtualkeyboardplugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_virtualkeyboard_qml.tar -C install qt/qml
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/libqtquickvirtualkeyboardplugin.lib.so $(PWD)/bin/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Settings/libqtquickvirtualkeyboardsettingsplugin.lib.so $(PWD)/bin/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Styles/libqtquickvirtualkeyboardstylesplugin.lib.so $(PWD)/bin/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
