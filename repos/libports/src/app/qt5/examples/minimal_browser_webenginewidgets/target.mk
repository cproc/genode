QMAKE_PROJECT_FILE = $(QT_DIR)/qtwebengine/examples/webenginewidgets/minimal/minimal.pro

QMAKE_TARGET_BINARIES = minimal

QT5_PORT_LIBS += libQt5Core libQt5Gui libQt5Network libQt5Widgets
QT5_PORT_LIBS += libQt5Qml libQt5Quick libQt5QuickWidgets
QT5_PORT_LIBS += libQt5WebEngine libQt5WebEngineCore libQt5WebEngineWidgets

LIBS = libc libm mesa qt5_component stdcxx qt5_quickcontrols $(QT5_PORT_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
