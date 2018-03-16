IMPORT_QT5_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT5_INC)

#
# Public QtGui headers include OpenGL headers
#
# We cannot just extend the 'LIBS' variable here because 'import-*.mk' are
# included (in 'base/mk/lib.mk') by iterating through the elements of the
# 'LIBS' variable. Hence, we also need to manually import the mesa snippet.
#
LIBS += mesa
include $(call select_from_repositories,lib/import/import-mesa.mk)

ifeq ($(CONTRIB_DIR),)
QT5_GUI_INC_DIR := $(abspath $(call select_from_repositories,include/QtGui)/..)
else
QT5_GUI_INC_DIR := $(QT5_PORT_DIR)/include
endif

QT5_INC_DIR += $(QT5_GUI_INC_DIR)
QT5_INC_DIR += $(QT5_GUI_INC_DIR)/QtGui
QT5_INC_DIR += $(QT5_GUI_INC_DIR)/QtGui/$(QT_VERSION)
QT5_INC_DIR += $(QT5_GUI_INC_DIR)/QtGui/$(QT_VERSION)/QtGui
QT5_INC_DIR += $(QT5_GUI_INC_DIR)/QtGui/$(QT_VERSION)/QtGui/private
