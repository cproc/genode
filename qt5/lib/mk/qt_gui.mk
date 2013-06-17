include $(REP_DIR)/lib/import/import-qt_gui.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN = -Wno-unused-but-set-variable -Wno-deprecated-declarations

include $(REP_DIR)/lib/mk/qt_gui_generated.inc

# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \
  moc_qsessionmanager.cpp \
  moc_qsound.cpp \
  moc_qsound_p.cpp \
  moc_qmenudata.cpp \
  moc_qprintpreviewwidget.cpp \
  moc_qabstractprintdialog.cpp \
  moc_qabstractpagesetupdialog.cpp \
  moc_qpagesetupdialog.cpp \
  moc_qprintdialog.cpp \
  moc_qprintpreviewdialog.cpp \
  moc_qpagesetupdialog_unix_p.cpp

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \
  qprintpreviewwidget.moc \
  qprintdialog_unix.moc \
  qprintpreviewdialog.moc

# UI headers
qfiledialog.o: ui_qfiledialog.h

include $(REP_DIR)/lib/mk/qt.inc

INC_DIR += $(REP_DIR)/include/qt5/qtbase/QtGui/private \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2 \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2/QtGui \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2/QtGui/private \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2 \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore

LIBS += qt_core qpa_nitpicker libpng zlib libc libm freetype jpeg

vpath % $(REP_DIR)/include/qt5/qtbase/QtGui
vpath % $(REP_DIR)/include/qt5/qtbase/QtGui/private
