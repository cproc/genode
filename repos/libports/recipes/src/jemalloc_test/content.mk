content: src/lib/jemalloc src/lib/jemalloc_test lib/mk/jemalloc_test.mk LICENSE

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/jemalloc)

src/lib/jemalloc:
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/src/lib/jemalloc $@
	cp -r $(REP_DIR)/src/lib/jemalloc/* $@

src/lib/jemalloc_test:
	mkdir -p $(dir $@)
	cp -r $(REP_DIR)/src/lib/jemalloc_test $@
	echo "LIBS = jemalloc_test" > $@/target.mk

lib/mk/%.mk:
	$(mirror_from_rep_dir)

LICENSE:
	cp $(PORT_DIR)/src/lib/jemalloc/COPYING $@
