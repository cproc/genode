include $(call select_from_repositories,lib/import/import-qt5_qnitpickerviewwidget.mk)

SHARED_LIB = yes

SRC_CC = qnitpickerviewwidget.cpp \
         moc_qnitpickerviewwidget.cpp

vpath %.h $(call select_from_repositories,include/qt5/qnitpickerviewwidget)
vpath %.cpp $(REP_DIR)/src/lib/qt5/qnitpickerviewwidget

LIBS += libc qoost libQt5Core libQt5Gui libQt5Widgets

CC_CXX_WARN_STRICT =
