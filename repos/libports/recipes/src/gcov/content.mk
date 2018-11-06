content: lib/mk/gcov-libcommon.mk \
         lib/mk/gcov-libcpp.mk \
         lib/mk/gcov-libiberty.mk \
         src/lib/gcov-libcommon \
         src/lib/gcov-libcpp \
         src/lib/gcov-libiberty \
         src/app/gcov \
         src/lib/gcov \
         LICENSE

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/gcov)

lib/mk/%.mk:
	$(mirror_from_rep_dir)

src/lib/gcov:
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/src/lib/gcov $@

src/lib/%:
	$(mirror_from_rep_dir)

src/app/gcov:
	$(mirror_from_rep_dir)

LICENSE:
	cp $(PORT_DIR)/src/lib/gcov/COPYING $@
