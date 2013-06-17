include $(REP_DIR)/lib/import/import-qt_printsupport.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

include $(REP_DIR)/lib/mk/qt_printsupport_generated.inc

# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \

include $(REP_DIR)/lib/mk/qt.inc

INC_DIR += $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtPrintSupport/5.0.2/QtPrintSupport \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtWidgets/5.0.2/QtWidgets \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2/QtGui \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore \

LIBS += qt_core
