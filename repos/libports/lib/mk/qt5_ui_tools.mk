include $(call select_from_repositories,lib/import/import-qt5_ui_tools.mk)

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

include $(REP_DIR)/lib/mk/qt5_ui_tools_generated.inc

# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \

#QT_VPATH += qttools/src/designer/src/lib

include $(REP_DIR)/lib/mk/qt5.inc

LIBS += qt5_core qt5_gui qt5_widgets

CC_CXX_WARN_STRICT =
