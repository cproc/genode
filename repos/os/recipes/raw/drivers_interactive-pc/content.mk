content: drivers.config fb_drv.config input_filter.config en_us.chargen

drivers.config fb_drv.config input_filter.config:
	cp $(REP_DIR)/recipes/raw/drivers_interactive-pc/$@ $@

en_us.chargen:
	cp $(REP_DIR)/src/server/input_filter/$@ $@
