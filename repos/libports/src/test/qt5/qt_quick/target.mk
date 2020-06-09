TARGET = test-qt_quick

QMAKE_PROJECT_FILE = $(PRG_DIR)/qt_quick.pro

QT5_LIBS += libQt5Core libQt5Gui libQt5Network
QT5_LIBS += libQt5Qml libQt5Quick

LIBS = libc libm mesa qt5_component stdcxx $(QT5_LIBS)
