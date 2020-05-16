TARGET = tetrix

QMAKE_PROJECT_FILE = $(QT_DIR)/qtscript/examples/script/qstetrix/qstetrix.pro

#QT5_LIBS = libQt5Core libQt5Gui libQt5Widgets

LIBS = libc libm mesa qt5_component stdcxx $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
