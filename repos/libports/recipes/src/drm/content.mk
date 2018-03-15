MIRROR_FROM_REP_DIR := lib/mk/drm.mk \
                       src/lib/drm

content: $(MIRROR_FROM_REP_DIR) src/lib/drm/target.mk

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

src/lib/drm/target.mk:
	mkdir -p $(dir $@)
	echo "LIBS = drm" > $@

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/drm)

MIRROR_FROM_PORT_DIR := src/lib/drm/src

content: $(MIRROR_FROM_PORT_DIR)

$(MIRROR_FROM_PORT_DIR):
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/$@ $(dir $@)

content: LICENSE

LICENSE:
	echo "MIT" > $@
