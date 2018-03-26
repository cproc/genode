include $(call select_from_repositories,src/app/qt5/tmpl/target_defaults.inc)

include $(call select_from_repositories,src/app/qt5/tmpl/target_final.inc)

LIBS += qt5_qpluginwidget qt5_network qoost

$(TARGET): test-plugin.tar

test-plugin.tar: config.plugin
	$(VERBOSE)tar cf $@ -C $(PRG_DIR) config.plugin

clean:
	$(VERBOSE)rm test-plugin.tar

CC_CXX_WARN_STRICT =
