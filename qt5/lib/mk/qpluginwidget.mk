SHARED_LIB = yes

SRC_CC   = qpluginwidget.cpp

HEADERS += qpluginwidget.h

vpath %.h $(REP_DIR)/include/qt5/qpluginwidget
vpath %.cpp $(REP_DIR)/src/lib/qt5/qpluginwidget

LIBS += qt_gui qt_widgets qt_network qnitpickerviewwidget qt_core libc zlib
