$(CONTRIB_DIR)/gcc-$(GCC_VERSION)/configure:: $(CONTRIB_DIR)/gcc-$(GCC_VERSION)
	$(ECHO) "$(BRIGHT_COL)patching gcc build system...$(DEFAULT_COL)"
	$(VERBOSE)patch -d $(CONTRIB_DIR)/gcc-$(GCC_VERSION) -p1 -i $(GENODE_DIR)/tool/patches/gcc-4.7.2.patch
	@#
	@# Re-generate configure scripts
	@#
	$(VERBOSE)cd $(CONTRIB_DIR)/gcc-$(GCC_VERSION); autogen Makefile.def
	$(VERBOSE)cd $(CONTRIB_DIR)/gcc-$(GCC_VERSION)/libgcc; $(AUTOCONF)
	$(VERBOSE)cd $(CONTRIB_DIR)/gcc-$(GCC_VERSION)/libstdc++-v3; $(AUTOCONF)
	$(VERBOSE)cd $(CONTRIB_DIR)/gcc-$(GCC_VERSION)/gcc; $(AUTOCONF)
	$(VERBOSE)cd $(CONTRIB_DIR)/gcc-$(GCC_VERSION); $(AUTOCONF)
	@#
	@# Fix a bug in gcc 4.6.1 that causes compile errors when building Qt4 for ARM
	@# More detailed description at and solution from http://gcc.gnu.org/ml/gcc-patches/2010-11/msg02245.html
	@#
ifeq ($(GCC_VERSION),4.6.1)
	$(ECHO) "$(BRIGHT_COL)patching gcc...$(DEFAULT_COL)"
	$(VERBOSE)sed -i "/|| (volatilep && flag_strict_volatile_bitfields > 0/s/)/ \&\& (bitpos % GET_MODE_ALIGNMENT (mode) != 0))/" $(CONTRIB_DIR)/gcc-$(GCC_VERSION)/gcc/expr.c
endif
