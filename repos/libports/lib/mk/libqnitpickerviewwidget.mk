include $(call select_from_repositories,lib/import/import-libqnitpickerviewwidget.mk)

TARGET = libqnitpickerviewwidget.lib.so

QMAKE_PROJECT_FILE = $(REP_DIR)/src/lib/qt5/qnitpickerviewwidget/qnitpickerviewwidget.pro

QT5_LIBS = libQt5Core libQt5Gui libQt5Widgets

LIBS = qoost stdcxx $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

ifeq ($(called_from_lib_mk),yes)
all: $(TARGET)
endif
