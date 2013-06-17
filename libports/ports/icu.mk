ICU_VERSION = 51.2
ICU         = icu
ICU_TGZ     = $(ICU)4c-51_2-src.tgz
ICU_URL     = http://download.icu-project.org/files/icu4c/$(ICU_VERSION)/$(ICU_TGZ)

#
# Interface to top-level prepare Makefile
#
PORTS += $(ICU)

prepare-icu: $(CONTRIB_DIR)/$(ICU) include/icu

$(CONTRIB_DIR)/$(ICU):clean-icu

#
# Port-specific local rules
#
$(DOWNLOAD_DIR)/$(ICU_TGZ):
	$(VERBOSE)wget -c -P $(DOWNLOAD_DIR) $(ICU_URL) && touch $@

$(CONTRIB_DIR)/$(ICU): $(DOWNLOAD_DIR)/$(ICU_TGZ)
	$(VERBOSE)tar xfz $< -C $(CONTRIB_DIR) && touch $@

include/icu:
	$(VERBOSE)mkdir -p $@
	ln -sf ../../$(CONTRIB_DIR)/$(ICU)/source/common $@/common
	ln -sf ../../$(CONTRIB_DIR)/$(ICU)/source/i18n $@/i18n

clean-icu:
	$(VERBOSE)rm -rf include/icu
	$(VERBOSE)rm -rf $(CONTRIB_DIR)/$(ICU)
