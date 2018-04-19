include $(REP_DIR)/lib/import/import-qt5_svg.mk

SHARED_LIB = yes

LIBS	+= qt5_widgets mesa zlib

include $(REP_DIR)/lib/mk/qt5_svg_generated.inc

include $(REP_DIR)/lib/mk/qt5.inc

INC_DIR += $(QT5_CONTRIB_DIR)/qtsvg/include/QtSvg/$(QT_VERSION)/QtSvg \

CC_CXX_WARN_STRICT =
