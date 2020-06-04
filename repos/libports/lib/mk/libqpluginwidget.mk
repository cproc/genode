include $(call select_from_repositories,lib/import/import-libqpluginwidget.mk)

QMAKE_TARGET_BINARIES = libqpluginwidget.lib.so

QMAKE_PROJECT_FILE = $(REP_DIR)/src/lib/qt5/qpluginwidget/qpluginwidget.pro

QT5_LIBS = libQt5Core libQt5Gui libQt5Network libQt5Widgets

LIBS = libc libm libqnitpickerviewwidget mesa qoost stdcxx zlib $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

qmake_root/include/qpluginwidget: qmake_root/include
	ln -sf $(call select_from_repositories,include/qt5/qpluginwidget) $@

qmake_prepared.tag: qmake_root/include/qpluginwidget

ifeq ($(called_from_lib_mk),yes)
all: build_with_qmake
endif
