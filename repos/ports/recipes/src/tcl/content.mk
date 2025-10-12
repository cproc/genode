content: src/noux-pkg/tcl LICENSE

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/tcl)

src/noux-pkg/tcl:
	mkdir -p $@
	cp -a $(PORT_DIR)/src/noux-pkg/tcl/* $@
	cp -a  $(REP_DIR)/src/noux-pkg/tcl/* $@

LICENSE:
	cp $(PORT_DIR)/src/noux-pkg/tcl/license.terms $@
