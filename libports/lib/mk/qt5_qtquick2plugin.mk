include $(REP_DIR)/lib/import/import-qt5_qtquick2plugin.mk

SHARED_LIB = yes

include $(REP_DIR)/lib/mk/qt5_qtquick2plugin_generated.inc

QT_DEFINES += -UQT_STATICPLUGIN

#QT_INCPATH += qtdeclarative/src/quick/items

include $(REP_DIR)/lib/mk/qt5.inc

LIBS += qt5_qml
