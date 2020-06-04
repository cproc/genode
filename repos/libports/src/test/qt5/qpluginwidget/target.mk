TARGET = test-qpluginwidget

QMAKE_PROJECT_FILE = $(PRG_DIR)/qpluginwidget.pro

QMAKE_TARGET_BINARIES = $(TARGET)

QT5_LIBS = libQt5Core libQt5Gui libQt5Network libQt5Widgets

LIBS = libc libm mesa qt5_component stdcxx libqnitpickerviewwidget libqpluginwidget qoost $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

#
# add extra libraries
#

QT5_GENODE_LIBS_APP += libqnitpickerviewwidget.lib.so libqpluginwidget.lib.so

#
# create tar archive for test plugin
#

TEST_PLUGIN_TAR = $(BUILD_BASE_DIR)/bin/test-plugin.tar

qmake_prepared.tag: qmake_root/lib/libqnitpickerviewwidget.lib.so qmake_root/lib/libqpluginwidget.lib.so

$(TARGET): $(TEST_PLUGIN_TAR)

$(TEST_PLUGIN_TAR): config.plugin
	$(VERBOSE)tar cf $@ -C $(PRG_DIR) config.plugin

clean:
	$(VERBOSE)rm $(TEST_PLUGIN_TAR)
