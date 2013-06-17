include $(REP_DIR)/lib/import/import-qt_ui_tools.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

include $(REP_DIR)/lib/mk/qt_ui_tools_generated.inc

# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \

include $(REP_DIR)/lib/mk/qt.inc

#INC_DIR += $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtSql/5.0.2/QtSql \
#           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore

LIBS += qt_core
