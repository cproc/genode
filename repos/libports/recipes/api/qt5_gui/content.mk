# XXX: remove qt5_version.inc?

MIRROR_FROM_REP_DIR := lib/import/import-qt5_gui.mk \
                       lib/import/import-qt5.inc \
                       lib/symbols/qt5_gui \
                       lib/mk/qt5_version.inc

content: $(MIRROR_FROM_REP_DIR)

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

content: include/QtGui

include/QtGui:
	mkdir -p $@
	cp -r $(PORT_DIR)/include/QtGui/* $@

content: LICENSE

LICENSE:
	cp $(PORT_DIR)/src/lib/qt5/qt-everywhere-opensource-src-5.8.0/LICENSE.LGPLv3 $@
