$(CONTRIB_DIR)/gdb-$(GDB_VERSION)/configure:: $(CONTRIB_DIR)/gdb-$(GDB_VERSION)
	$(ECHO) "$(BRIGHT_COL)patching gdb...$(DEFAULT_COL)"
	$(VERBOSE)patch -d $(CONTRIB_DIR)/gdb-$(GDB_VERSION) -p1 -i $(GENODE_DIR)/tool/patches/gdb-7.3.1.patch
