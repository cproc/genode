include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

TARGET = minimal

LIBS = libc libm mesa qt5_component stdcxx
LIBS += libQt5Core libQt5Gui libQt5Network libQt5Widgets
LIBS += libQt5Qml libQt5Quick libQt5QuickWidgets
LIBS += qt5_quickcontrols
LIBS += libQt5WebEngineCore libQt5WebEngine

$(TARGET): env.sh qmake_root

	@#
	@# add Qt dependencies to qmake_root
	@#

ifeq ($(CONTRIB_DIR),)
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Core/libQt5Core.abi.so                   qmake_root/lib/libQt5Core.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Gui/libQt5Gui.abi.so                     qmake_root/lib/libQt5Gui.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Network/libQt5Network.abi.so             qmake_root/lib/libQt5Network.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Qml/libQt5Qml.abi.so                     qmake_root/lib/libQt5Qml.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Quick/libQt5Quick.abi.so                 qmake_root/lib/libQt5Quick.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Widgets/libQt5Widgets.abi.so             qmake_root/lib/libQt5Widgets.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5WebEngineCore/libQt5WebEngineCore.abi.so qmake_root/lib/libQt5WebEngineCore.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5WebEngine/libQt5WebEngine.abi.so         qmake_root/lib/libQt5WebEngine.lib.so
else
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Core.lib.so          qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Gui.lib.so           qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Network.lib.so       qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Qml.lib.so           qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Quick.lib.so         qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Widgets.lib.so       qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5WebEngineCore.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5WebEngine.lib.so     qmake_root/lib/
endif

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/genode-x86-g++/qt.conf \
		$(QT_DIR)/qtwebengine/examples/webengine/minimal/minimal.pro

	@#
	@# build
	@#

	source env.sh && $(MAKE)

	@#
	@# create stripped version
	@#

	$(STRIP) $(TARGET) -o $(TARGET).stripped

	@#
	@# create symlink in 'bin' directory
	@#

	ln -sf $(CURDIR)/$(TARGET).stripped $(PWD)/bin/$(TARGET)

	@#
	@# create symlink in 'debug' directory
	@#

	ln -sf $(CURDIR)/$(TARGET) $(PWD)/debug/
