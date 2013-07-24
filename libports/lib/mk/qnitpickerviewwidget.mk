SHARED_LIB = yes

SRC_CC   = qnitpickerviewwidget.cpp

HEADERS += qnitpickerviewwidget.h

vpath %.h $(REP_DIR)/include/qt5/qnitpickerviewwidget
vpath %.cpp $(REP_DIR)/src/lib/qt5/qnitpickerviewwidget

LIBS += qt_gui qt_widgets qt_core libc
