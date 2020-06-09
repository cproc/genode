TARGET = openglwindow

QMAKE_PROJECT_FILE = $(QT_DIR)/qtbase/examples/gui/openglwindow/openglwindow.pro

QT5_LIBS = libQt5Core libQt5Gui

LIBS = libc libm mesa qt5_component stdcxx $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
