MIRROR_FROM_REP_DIR := lib/mk/libqgenodeviewwidget.mk \
                       src/lib/qt5/qgenodeviewwidget

content: $(MIRROR_FROM_REP_DIR) LICENSE src/lib/libqgenodeviewwidget/target.mk

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

src/lib/libqgenodeviewwidget/target.mk:
	mkdir -p $(dir $@)
	echo "LIBS = libqgenodeviewwidget" > $@

LICENSE:
	cp $(GENODE_DIR)/LICENSE $@
