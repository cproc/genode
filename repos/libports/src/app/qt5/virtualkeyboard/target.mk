QMAKE_PROJECT_FILE = $(PRG_DIR)/virtualkeyboard.pro

QMAKE_TARGET_BINARIES = virtualkeyboard

QT5_PORT_LIBS += libQt5Core libQt5Gui libQt5Network libQt5Widgets
QT5_PORT_LIBS += libQt5Qml libQt5QmlModels libQt5Quick libQt5QuickWidgets

LIBS = libc libm mesa qt5_component stdcxx $(QT5_PORT_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
