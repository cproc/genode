MIRROR_FROM_REP_DIR := lib/mk/qt5_scriptclassic.mk \
                       lib/mk/qt5_scriptclassic_generated.inc \
                       lib/mk/qt5.inc \

content: $(MIRROR_FROM_REP_DIR) src/lib/qt5_scriptclassic/target.mk

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

src/lib/qt5_scriptclassic/target.mk:
	mkdir -p $(dir $@)
	echo "LIBS = qt5_scriptclassic" > $@

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

MIRROR_FROM_PORT_DIR := src/lib/qt5/qtscriptclassic-1.0_1-opensource/include/QtScript \
                        src/lib/qt5/qtscriptclassic-1.0_1-opensource/src

content: $(MIRROR_FROM_PORT_DIR)

$(MIRROR_FROM_PORT_DIR):
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/$@ $(dir $@)

content: LICENSE

LICENSE:
	cp $(PORT_DIR)/src/lib/qt5/qt-everywhere-opensource-src-5.8.0/LICENSE.LGPLv3 $@
