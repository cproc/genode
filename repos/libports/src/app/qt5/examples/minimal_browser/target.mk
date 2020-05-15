TARGET = minimal

QMAKE_PROJECT_FILE = $(QT_DIR)/qtwebengine/examples/webengine/minimal/minimal.pro

QT5_LIBS += libQt5Core libQt5Gui libQt5Network
QT5_LIBS += libQt5Qml libQt5Quick
QT5_LIBS += libQt5WebEngineCore libQt5WebEngine

LIBS = libc libm mesa qt5_component stdcxx qt5_quickcontrols $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
