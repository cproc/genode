include $(call select_from_repositories,lib/import/import-libqnitpickerviewwidget.mk)

QMAKE_PROJECT_FILE = $(REP_DIR)/src/lib/qt5/qnitpickerviewwidget/qnitpickerviewwidget.pro

QMAKE_TARGET_BINARIES = libqnitpickerviewwidget.lib.so

QT5_PORT_LIBS = libQt5Core libQt5Gui libQt5Widgets

LIBS = libc libm mesa qoost stdcxx $(QT5_PORT_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

qmake_root/include/qnitpickerviewwidget: qmake_root/include
	ln -sf $(call select_from_repositories,include/qt5/qnitpickerviewwidget) $@

qmake_prepared.tag: qmake_root/include/qnitpickerviewwidget

ifeq ($(called_from_lib_mk),yes)
all: build_with_qmake
endif
