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

	ln -sf .$(CURDIR)/qmake_root install/qt

	@#
	@# create stripped versions
	@#

	cd $(CURDIR)/install/qt/lib && \
		$(STRIP) libQt5VirtualKeyboard.lib.so -o libQt5VirtualKeyboard.lib.so.stripped

	cd $(CURDIR)/install/qt/plugins/platforminputcontexts && \
		$(STRIP) libqtvirtualkeyboardplugin.lib.so -o libqtvirtualkeyboardplugin.lib.so.stripped

	cd $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard && \
		$(STRIP) libqtquickvirtualkeyboardplugin.lib.so -o libqtquickvirtualkeyboardplugin.lib.so.stripped

	cd $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Settings && \
		$(STRIP) libqtquickvirtualkeyboardsettingsplugin.lib.so -o libqtquickvirtualkeyboardsettingsplugin.lib.so.stripped

	cd $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Styles && \
		$(STRIP) libqtquickvirtualkeyboardstylesplugin.lib.so -o libqtquickvirtualkeyboardstylesplugin.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install/qt/lib/libQt5VirtualKeyboard.lib.so.stripped $(PWD)/bin/libQt5VirtualKeyboard.lib.so

	ln -sf $(CURDIR)/install/qt/plugins/platforminputcontexts/libqtvirtualkeyboardplugin.lib.so.stripped $(PWD)/bin/libqtvirtualkeyboardplugin.lib.so

	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/libqtquickvirtualkeyboardplugin.lib.so.stripped $(PWD)/bin/libqtquickvirtualkeyboardplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Settings/libqtquickvirtualkeyboardsettingsplugin.lib.so.stripped $(PWD)/bin/libqtquickvirtualkeyboardsettingsplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Styles/libqtquickvirtualkeyboardstylesplugin.lib.so.stripped $(PWD)/bin/libqtquickvirtualkeyboardstylesplugin.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install/qt/lib/libQt5VirtualKeyboard.lib.so $(PWD)/debug/

	ln -sf $(CURDIR)/install/qt/plugins/platforminputcontexts/libqtvirtualkeyboardplugin.lib.so $(PWD)/debug/

	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/libqtquickvirtualkeyboardplugin.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Settings/libqtquickvirtualkeyboardsettingsplugin.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/VirtualKeyboard/Styles/libqtquickvirtualkeyboardstylesplugin.lib.so $(PWD)/debug/

	@#
	@# create tar archives
	@#

	tar chf $(PWD)/bin/qt5_qtvirtualkeyboardplugin.tar --transform='s/\.stripped//' -C install qt/plugins/platforminputcontexts/libqtvirtualkeyboardplugin.lib.so.stripped
	tar chf $(PWD)/bin/qt5_virtualkeyboard_qml.tar --exclude='*.lib.so' --transform='s/\.stripped//' -C install qt/qml

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
