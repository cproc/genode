SHARED_LIB = yes

SRC_CC   = qpluginwidget.cpp

HEADERS += qpluginwidget.h

vpath %.h $(REP_DIR)/include/qpluginwidget
vpath %.cpp $(REP_DIR)/src/lib/qpluginwidget

LIBS += qt_gui qt_widgets qt_network qnitpickerviewwidget qt_core libc zlib
