MIRROR_FROM_REP_DIR := lib/mk/qt5_quickcontrols2.mk

content: $(MIRROR_FROM_REP_DIR) src/lib/qt5_quickcontrols2/target.mk

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

src/lib/qt5_quickcontrols2/target.mk:
	mkdir -p $(dir $@)
	echo "LIBS = qt5_quickcontrols2" > $@

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

MIRROR_FROM_PORT_DIR := src/lib/qt5/qtquickcontrols2

content: $(MIRROR_FROM_PORT_DIR)

$(MIRROR_FROM_PORT_DIR):
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/$@ $(dir $@)

content: LICENSE

LICENSE:
	cp $(PORT_DIR)/src/lib/qt5/LICENSE.LGPLv3 $@
