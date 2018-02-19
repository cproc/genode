IMPORT_QT5_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT5_INC)

ifeq ($(CONTRIB_DIR),)
QT5_INC_DIR += $(call select_from_repositories,include/QtCore)
else
QT5_INC_DIR += $(QT5_PORT_DIR)/include/QtCore
endif
