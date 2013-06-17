include $(REP_DIR)/lib/import/import-qt_widgets.mk

SHARED_LIB = yes

include $(REP_DIR)/lib/mk/qt_widgets_generated.inc

# UI headers
qfiledialog.o: ui_qfiledialog.h

include $(REP_DIR)/lib/mk/qt.inc

INC_DIR += $(REP_DIR)/include/qt5/qtbase/QtWidgets/private \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtWidgets/5.0.2/QtWidgets \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2 \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2/QtGui \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2 \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore

LIBS += qt_core libc

#vpath % $(REP_DIR)/include/qt5/qtbase/QtWidgets
#vpath % $(REP_DIR)/include/qt5/qtbase/QtWidgets/private
