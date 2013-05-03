include $(REP_DIR)/lib/import/import-qpa_nitpicker.mk

SRC_CC       = main.cpp qnitpickerintegration.cpp qnitpickerwindowsurface.cpp

INC_DIR     += $(REP_DIR)/src/lib/qt4/src/plugins/platforms/nitpicker

vpath % $(call select_from_repositories,src/lib/qt4/src/plugins/platforms/nitpicker)
