IMPORT_QT5_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT5_INC)

# include dirs needed for 'qnitpickerplatformwindow.h', which is used by
# the qt5_qnitpickerviewwidget library

#include $(call select_from_repositories,lib/import/import-qt5_gui.mk)

#INC_DIR += $(QT5_REP_DIR)/src/lib/qt5/qtbase/src/plugins/platforms/nitpicker \
#           $(QT5_CONTRIB_DIR)/qtbase/src/platformsupport/input/evdevkeyboard \
#           $(QT5_CONTRIB_DIR)/qtbase/include/QtGui/$(QT_VERSION)/QtGui \

