SRC_DIR := src/app/menu_view
include $(GENODE_DIR)/repos/base/recipes/src/content.inc

content: include/polygon_gfx

include/polygon_gfx:
	mkdir -p $@
	cp $(GENODE_DIR)/repos/gems/include/polygon_gfx/* $@
