content: src/noux-pkg/expect LICENSE

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/expect)

src/noux-pkg/expect:
	mkdir -p $@
	cp -a $(PORT_DIR)/src/noux-pkg/expect/* $@
	cp -a  $(REP_DIR)/src/noux-pkg/expect/* $@

LICENSE:
	cp $(PORT_DIR)/src/noux-pkg/expect/FAQ $@
