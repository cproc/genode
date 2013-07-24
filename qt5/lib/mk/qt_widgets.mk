include $(REP_DIR)/lib/import/import-qt_widgets.mk

SHARED_LIB = yes

include $(REP_DIR)/lib/mk/qt_widgets_generated.inc

# UI headers
qfiledialog.o: ui_qfiledialog.h

include $(REP_DIR)/lib/mk/qt.inc

INC_DIR += $(REP_DIR)/include/qt5/qtbase/QtWidgets/private \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtWidgets/$(QT_VERSION)/QtWidgets \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtGui/$(QT_VERSION) \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtGui/$(QT_VERSION)/QtGui \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/$(QT_VERSION) \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/$(QT_VERSION)/QtCore

LIBS += qt_core libc
