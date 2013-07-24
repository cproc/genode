# identify the qt4 repository by searching for a file that is unique for qt4
QT5_REP_DIR := $(call select_from_repositories,lib/import/import-QT5.inc)
QT5_REP_DIR := $(realpath $(dir $(QT5_REP_DIR))../..)

include $(QT5_REP_DIR)/src/app/tmpl/target_defaults.inc

include $(QT5_REP_DIR)/src/app/tmpl/target_final.inc

LIBS += qnitpickerviewwidget
