include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

SHARED = yes

LIBS = libc libm mesa stdcxx
LIBS += libQt5Core libQt5Gui libQt5Network libQt5Qml libQt5Quick libQt5Widgets

built.tag: env.sh qmake_root

	@#
	@# add Qt dependencies to qmake_root
	@#

ifeq ($(CONTRIB_DIR),)
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Core/libQt5Core.abi.so       qmake_root/lib/libQt5Core.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Gui/libQt5Gui.abi.so         qmake_root/lib/libQt5Gui.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Network/libQt5Network.abi.so qmake_root/lib/libQt5Network.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Qml/libQt5Qml.abi.so         qmake_root/lib/libQt5Qml.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Quick/libQt5Quick.abi.so     qmake_root/lib/libQt5Quick.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Widgets/libQt5Widgets.abi.so qmake_root/lib/libQt5Widgets.lib.so
else
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Core.lib.so    qmake_root/lib/libQt5Core.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Gui.lib.so     qmake_root/lib/libQt5Gui.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Network.lib.so qmake_root/lib/libQt5Network.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Qml.lib.so     qmake_root/lib/libQt5Qml.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Quick.lib.so   qmake_root/lib/libQt5Quick.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Widgets.lib.so qmake_root/lib/libQt5Widgets.lib.so
endif

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/genode-x86-g++/qt.conf \
		$(QT_DIR)/qtquickcontrols/qtquickcontrols.pro

	@#
	@# build
	@#

	source env.sh && $(MAKE) sub-src

	@#
	@# install into local 'install' directory
	@#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install sub-src-install_subtargets

	@#
	@# create QML plugin archives and links
	@#

	ln -sf .$(CURDIR)/qmake_root install/qt

	tar chf $(PWD)/bin/qt5_qtquick_controls.tar -C install --exclude='*.lib.so' qt/qml/QtQuick/Controls
	tar chf $(PWD)/bin/qt5_qtquickcontrolsplugin.tar -C install qt/qml/QtQuick/Controls/libqtquickcontrolsplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Controls/libqtquickcontrolsplugin.lib.so $(PWD)/bin/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
