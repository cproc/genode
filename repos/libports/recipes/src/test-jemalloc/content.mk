content: src/lib/jemalloc src/lib/jemalloc_test src/test/jemalloc LICENSE

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/jemalloc)

src/lib/jemalloc:
	mkdir -p $@
	cp -r $(PORT_DIR)/src/lib/jemalloc/include $@/
	cp -r $(PORT_DIR)/src/lib/jemalloc/test $@/
	cp -r $(REP_DIR)/src/lib/jemalloc/* $@/

src/lib/jemalloc_test:
	mkdir -p $(dir $@)
	cp -r $(REP_DIR)/src/lib/jemalloc_test $@

src/test/jemalloc:
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/src/lib/jemalloc/test $@
	cp -r $(REP_DIR)/src/test/jemalloc/* $@

LICENSE:
	cp $(PORT_DIR)/src/lib/jemalloc/COPYING $@
