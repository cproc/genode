include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

SHARED = yes

LIBS = libc libm mesa stdcxx
LIBS += libQt5Core libQt5Gui libQt5Network libQt5Sql libQt5Test libQt5Widgets

built.tag: env.sh qmake_root

	@#
	@# add Qt dependencies to qmake_root
	@#

ifeq ($(CONTRIB_DIR),)
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Core/libQt5Core.abi.so       qmake_root/lib/libQt5Core.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Gui/libQt5Gui.abi.so         qmake_root/lib/libQt5Gui.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Network/libQt5Network.abi.so qmake_root/lib/libQt5Network.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Sql/libQt5Sql.abi.so         qmake_root/lib/libQt5Sql.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Test/libQt5Test.abi.so       qmake_root/lib/libQt5Test.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Widgets/libQt5Widgets.abi.so qmake_root/lib/libQt5Widgets.lib.so
else
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Core.lib.so    qmake_root/lib/libQt5Core.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Gui.lib.so     qmake_root/lib/libQt5Gui.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Network.lib.so qmake_root/lib/libQt5Network.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Sql.lib.so     qmake_root/lib/libQt5Sql.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Test.lib.so    qmake_root/lib/libQt5Test.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Widgets.lib.so qmake_root/lib/libQt5Widgets.lib.so
endif

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
