# XXX: remove qt5_version.inc?

MIRROR_FROM_REP_DIR := lib/import/import-qt5_ui_tools.mk \
                       lib/import/import-qt5.inc \
                       lib/symbols/qt5_ui_tools \
                       lib/mk/qt5_version.inc

content: $(MIRROR_FROM_REP_DIR)

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

MIRROR_FROM_PORT_DIR := include/QtDesigner \
                        include/QtUiPlugin \
                        include/QtUiTools

content: $(MIRROR_FROM_PORT_DIR)

$(MIRROR_FROM_PORT_DIR):
	mkdir -p $@
	cp -r $(PORT_DIR)/$@ $(dir $@)

content: LICENSE

LICENSE:
	cp $(PORT_DIR)/src/lib/qt5/qt-everywhere-opensource-src-5.8.0/LICENSE.LGPLv3 $@
