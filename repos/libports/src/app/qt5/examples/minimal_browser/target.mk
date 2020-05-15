include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

TARGET = minimal

#
# build dependency libraries in var/libcache
#

LIBS = qt5_base qt5_declarative qt5_quickcontrols qt5_webengine stdcxx

$(TARGET): env.sh qmake_root

	@#
	@# add Qt dependencies to qmake_root
	@#

	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Core.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Gui.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Network.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Qml.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Quick.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Widgets.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5WebEngineCore.lib.so qmake_root/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5WebEngine.lib.so qmake_root/lib/

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
