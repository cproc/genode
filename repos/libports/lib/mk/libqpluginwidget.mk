include $(call select_from_repositories,lib/import/import-libqpluginwidget.mk)

QMAKE_PROJECT_FILE = $(REP_DIR)/src/lib/qt5/qpluginwidget/qpluginwidget.pro

QMAKE_TARGET_BINARIES = libqpluginwidget.lib.so

QT5_PORT_LIBS = libQt5Core libQt5Gui libQt5Network libQt5Widgets libqnitpicker

LIBS = libc libm libqnitpickerviewwidget mesa qoost stdcxx zlib $(QT5_PORT_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_GENODE_LIBS_SHLIB += libqnitpickerviewwidget.lib.so libqnitpicker.lib.so

qmake_root/include/qpluginwidget: qmake_root/include
	ln -sf $(call select_from_repositories,include/qt5/qpluginwidget) $@

qmake_prepared.tag: qmake_root/include/qpluginwidget \
                    qmake_root/lib/libqnitpickerviewwidget.lib.so \
                    qmake_root/lib/libqnitpicker.lib.so

ifeq ($(called_from_lib_mk),yes)
all: build_with_qmake
endif
