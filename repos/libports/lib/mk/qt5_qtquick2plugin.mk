IMPORT_QT5_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT5_INC)

SHARED_LIB = yes

include $(REP_DIR)/lib/mk/qt5_qtquick2plugin_generated.inc

QT_DEFINES += -UQT_STATICPLUGIN

include $(REP_DIR)/lib/mk/qt5.inc

LIBS += qt5_core qt5_gui qt5_qml qt5_quick

# install the QtQuick QML plugin

QTQUICK_PLUGIN_NAME := qt5_qtquick2plugin
QTQUICK_INSTALL_DIR := qt/qml/QtQuick.2
QTQUICK_QMLDIR      := $(QTQUICK_INSTALL_DIR)/qmldir
QTQUICK_PLUGIN      := $(QTQUICK_INSTALL_DIR)/$(QTQUICK_PLUGIN_NAME).lib.so
QTQUICK_TAR_ARCHIVE := $(BUILD_BASE_DIR)/bin/qt5_qtquick2plugin.tar

$(QTQUICK_INSTALL_DIR):
	$(VERBOSE)mkdir -p $@

$(QTQUICK_QMLDIR): $(QTQUICK_INSTALL_DIR)
	$(VERBOSE)cp $(QT5_CONTRIB_DIR)/qtdeclarative/src/imports/qtquick2/qmldir $@

$(QTQUICK_PLUGIN): $(QTQUICK_INSTALL_DIR) $(LIB).lib.so.stripped
	$(VERBOSE)cp $(QTQUICK_PLUGIN_NAME).lib.so.stripped $@

$(QTQUICK_TAR_ARCHIVE): $(QTQUICK_QMLDIR) $(QTQUICK_PLUGIN)
	$(VERBOSE)tar chf $@ qt

ifneq ($(call select_from_ports,qt5),)
$(QTQUICK_PLUGIN_NAME).lib.tag: $(QTQUICK_TAR_ARCHIVE)
endif

#
# unfortunately, these clean rules don't trigger
#

clean-qtquick_tar_archive:
	rm -rf $(QTQUICK_TAR_ARCHIVE)

clean: clean-qtquick_tar_archive

CC_CXX_WARN_STRICT =
