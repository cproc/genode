# identify the qt4 repository by searching for a file that is unique for qt4
QT5_REP_DIR := $(call select_from_repositories,lib/import/import-qt5.inc)
QT5_REP_DIR := $(realpath $(dir $(QT5_REP_DIR))../..)

include $(QT5_REP_DIR)/lib/mk/qt_version.inc

QMAKE_PROJECT_PATH = $(realpath $(QT5_REP_DIR)/contrib/$(QT5)/qttools/examples/designer/calculatorform)
QMAKE_PROJECT_FILE = $(QMAKE_PROJECT_PATH)/calculatorform.pro

vpath % $(QMAKE_PROJECT_PATH)

include $(QT5_REP_DIR)/src/app/tmpl/target_defaults.inc

include $(QT5_REP_DIR)/src/app/tmpl/target_final.inc
