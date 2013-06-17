include $(REP_DIR)/lib/import/import-qt_sql.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

include $(REP_DIR)/lib/mk/qt_sql_generated.inc

# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \

include $(REP_DIR)/lib/mk/qt.inc

INC_DIR += $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtSql/$(QT_VERSION)/QtSql \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/$(QT_VERSION)/QtCore

LIBS += qt_core
