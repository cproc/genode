content: include lib/import/import-libtcl8.6.mk lib/symbols/libtcl8.6 LICENSE

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/tcl)

include:
	cp -r $(PORT_DIR)/include $@

lib/import/import-libtcl8.6.mk:
	$(mirror_from_rep_dir)

lib/symbols/libtcl8.6:
	$(mirror_from_rep_dir)

LICENSE:
	cp $(PORT_DIR)/src/noux-pkg/tcl/license.terms $@
