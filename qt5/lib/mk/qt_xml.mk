include $(REP_DIR)/lib/import/import-qt_xml.mk

SHARED_LIB = yes

include $(REP_DIR)/lib/mk/qt_xml_generated.inc

include $(REP_DIR)/lib/mk/qt.inc

INC_DIR += $(REP_DIR)/include/qt5/qtbase/QtXml/private \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/$(QT_VERSION)/QtCore \

LIBS += qt_core libc

vpath % $(REP_DIR)/include/qt5/qtbase/QtXml
vpath % $(REP_DIR)/include/qt5/qtbase/QtXml/private
