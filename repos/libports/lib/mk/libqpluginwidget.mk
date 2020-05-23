include $(call select_from_repositories,lib/import/import-libqpluginwidget.mk)

TARGET = libqpluginwidget.lib.so

QMAKE_PROJECT_FILE = $(REP_DIR)/src/lib/qt5/qpluginwidget/qpluginwidget.pro

QT5_LIBS = libQt5Core libQt5Gui libQt5Network libQt5Widgets

LIBS = libc libqnitpickerviewwidget qoost stdcxx zlib $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

ifeq ($(called_from_lib_mk),yes)
all: $(TARGET)
endif
