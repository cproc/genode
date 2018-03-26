ifeq ($(CONTRIB_DIR),)
QT5_INC_DIR += $(call select_from_repositories,include/qt5/qnitpickerviewwidget)
else
QT5_INC_DIR += $(REP_DIR)/include/qt5/qnitpickerviewwidget
endif
