include $(REP_DIR)/lib/import/import-qt5_v8.mk

SHARED_LIB = yes

include $(REP_DIR)/lib/mk/qt5_v8_generated.inc

QT_DEFINES += -UV8_TARGET_ARCH_X64

include $(REP_DIR)/lib/mk/qt5.inc

#INC_DIR += $(REP_DIR)/include/qt5/qtbase/QtXml/private \
#           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/$(QT_VERSION)/QtCore \

#LIBS += qt5_core libc
