include $(REP_DIR)/lib/import/import-qt5_opengl.mk

SHARED_LIB = yes

include $(REP_DIR)/lib/mk/qt5_opengl_generated.inc

include $(REP_DIR)/lib/mk/qt5.inc

LIBS += qt5_gui

CC_CXX_WARN_STRICT =
