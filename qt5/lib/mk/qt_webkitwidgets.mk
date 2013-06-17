include $(REP_DIR)/lib/import/import-qt_webkitwidgets.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

include $(REP_DIR)/lib/mk/qt_webkitwidgets_generated.inc

QT_INCPATH += qtwebkit/Source/WebCore/generated

QT_VPATH += qtwebkit/Source/WebKit/qt/Api \
  
# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \

include $(REP_DIR)/lib/mk/qt.inc

#INC_DIR += $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtSql/5.0.2/QtSql \
#           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore

LIBS += qt_webkit qt_printsupport icu
