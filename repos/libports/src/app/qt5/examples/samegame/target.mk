TARGET = samegame

QMAKE_PROJECT_FILE = $(PRG_DIR)/samegame.pro

QT5_LIBS += libQt5Core libQt5Gui libQt5Network
QT5_LIBS += libQt5Qml libQt5Quick

LIBS = libc libm mesa qt5_component stdcxx $(QT5_LIBS)

include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)
