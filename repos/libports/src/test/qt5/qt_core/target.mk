TARGET = test-qt_core

QMAKE_PROJECT_FILE = $(PRG_DIR)/qt_core.pro

QT5_LIBS = libQt5Core

LIBS = libc libm qt5_component stdcxx $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
