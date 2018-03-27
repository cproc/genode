ifeq ($(CONTRIB_DIR),)
QT5_INC_DIR += $(call select_from_repositories,include/qt5/qpluginwidget)
else
QT5_INC_DIR += $(REP_DIR)/include/qt5/qpluginwidget
endif

# 'qpluginwidget.h' includes 'qnitpickerviewwidget.h'
include $(call select_from_repositories,lib/import/import-qt5_qnitpickerviewwidget.mk)
