MIRROR_FROM_REP_DIR := lib/import/import-stdcxx.mk \
                       lib/mk/stdcxx-c++98.mk \
                       lib/mk/stdcxx.mk

content: $(MIRROR_FROM_REP_DIR)

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/stdcxx)

content: include/stdcxx

include/stdcxx:
	mkdir -p $@
	cp -r $(PORT_DIR)/include/stdcxx/* $@/
	cp -r $(REP_DIR)/include/stdcxx/bits/* $@/bits/

MIRROR_FROM_PORT_DIR := src/lib/stdcxx

content: $(MIRROR_FROM_PORT_DIR)

$(MIRROR_FROM_PORT_DIR):
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/$@ $(dir $@)

content: LICENSE

LICENSE:
	( echo "GNU GPL version 3 with runtime library exception,"; \
	  echo "see src/lib/stdcxx/doc/html/manual/license.html" ) > $@

