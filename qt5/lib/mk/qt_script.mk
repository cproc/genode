include $(REP_DIR)/lib/import/import-qt_script.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

include $(REP_DIR)/lib/mk/qt_script_generated.inc

QT_INCPATH += qtscript/src/script/api \

# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \

include $(REP_DIR)/lib/mk/qt.inc

INC_DIR += $(REP_DIR)/contrib/$(QT4)/qtscript/include/QtScript/5.0.2/QtScript \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore \

#           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore

LIBS += qt_core
