MIRROR_FROM_REP_DIR := lib/mk/libqpluginwidget.mk \
                       src/lib/qt5/qpluginwidget

content: $(MIRROR_FROM_REP_DIR) LICENSE src/lib/libqpluginwidget/target.mk

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

src/lib/libqpluginwidget/target.mk:
	mkdir -p $(dir $@)
	echo "LIBS = libqpluginwidget" > $@

LICENSE:
	cp $(GENODE_DIR)/LICENSE $@
