content: include/file_system \
         lib/mk/libgcov.mk \
         src/gcov \
         src/lib/gcov \
         LICENSE

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/gcov)

include/file_system:
	mkdir -p $@
	cp $(GENODE_DIR)/repos/os/include/file_system/* $@

lib/mk/libgcov.mk:
	$(mirror_from_rep_dir)

src/gcov:
	mkdir -p $(dir $@)
	cp -r $(PORT_DIR)/src/gcov $@

src/lib/gcov:
	$(mirror_from_rep_dir)
	echo "LIBS = libgcov" > $@/target.mk

LICENSE:
	cp $(PORT_DIR)/src/gcov/COPYING $@
