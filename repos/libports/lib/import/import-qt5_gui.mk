IMPORT_QT5_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT5_INC)

QT5_INC_DIR += $(QT5_CONTRIB_DIR)/qtbase/include/QtGui \

#
# install fonts
#

$(TARGET): $(BUILD_BASE_DIR)/bin/qt5_fs/$(TARGET)/qt/lib/fonts

$(BUILD_BASE_DIR)/bin/qt5_fs/$(TARGET)/qt/lib/fonts:
	mkdir -p $@
	ln -sf $(QT5_CONTRIB_DIR)/qtbase/lib/fonts/DejaVuSans.ttf $@/
