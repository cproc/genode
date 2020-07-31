QMAKE_PROJECT_FILE = $(call select_from_ports,falkon)/src/app/falkon/falkon.pro

QMAKE_TARGET_BINARIES = falkon libFalkon.lib.so

QT5_PORT_LIBS += libQt5Core libQt5Gui libQt5Network libQt5PrintSupport libQt5Sql libQt5Widgets
QT5_PORT_LIBS += libQt5Qml libQt5Quick libQt5QuickWidgets
QT5_PORT_LIBS += libQt5WebChannel
QT5_PORT_LIBS += libQt5WebEngineCore libQt5WebEngineWidgets

LIBS = libc libm mesa qt5_component stdcxx libcrypto $(QT5_PORT_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_GENODE_LIBS_APP += libcrypto.lib.so

qmake_prepared.tag: qmake_root/lib/libcrypto.lib.so
