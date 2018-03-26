MIRROR_FROM_REP_DIR := include/qt5/qpluginwidget \
                       lib/symbols/qt5_qpluginwidget

content: $(MIRROR_FROM_REP_DIR) LICENSE

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

LICENSE:
	cp $(GENODE_DIR)/LICENSE $@
