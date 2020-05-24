MIRROR_FROM_REP_DIR := lib/mk/libqnitpickerviewwidget.mk \
                       src/lib/qt5/qnitpickerviewwidget

content: $(MIRROR_FROM_REP_DIR) LICENSE src/lib/libqnitpickerviewwidget/target.mk

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

src/lib/libqnitpickerviewwidget/target.mk:
	mkdir -p $(dir $@)
	echo "LIBS = libqnitpickerviewwidget" > $@

LICENSE:
	cp $(GENODE_DIR)/LICENSE $@
