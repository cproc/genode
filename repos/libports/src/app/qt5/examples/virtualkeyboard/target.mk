TARGET = basic

QMAKE_PROJECT_FILE = $(QT_DIR)/qtvirtualkeyboard/examples/virtualkeyboard/basic/basic.pro

QT5_PORT_LIBS += libQt5Core libQt5Gui libQt5Network
QT5_PORT_LIBS += libQt5Qml libQt5Quick

LIBS = libc libm mesa qt5_component stdcxx $(QT5_PORT_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
