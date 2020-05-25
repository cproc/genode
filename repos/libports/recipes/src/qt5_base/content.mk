MIRROR_FROM_REP_DIR := lib/mk/qt5_base.mk

content: $(MIRROR_FROM_REP_DIR) src/lib/qt5_base/target.mk

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

src/lib/qt5_base/target.mk:
	mkdir -p $(dir $@)
	echo "LIBS = qt5_base" > $@

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5_new)

MIRROR_FROM_PORT_DIR := src/lib/qt5/qtbase

content: $(MIRROR_FROM_PORT_DIR)

$(MIRROR_FROM_PORT_DIR):
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/$@ $(dir $@)

MIRROR_FROM_OS := include/pointer/shape_report.h

content: $(MIRROR_FROM_OS)

$(MIRROR_FROM_OS):
	mkdir -p $(dir $@)
	cp -r $(GENODE_DIR)/repos/os/$@ $(dir $@)

content: LICENSE

LICENSE:
	cp $(PORT_DIR)/src/lib/qt5/LICENSE.LGPLv3 $@