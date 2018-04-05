content: qt5_qtquick2.tar

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

INSTALL_DIR := qt/qml/QtQuick.2
QMLDIR      := $(INSTALL_DIR)/qmldir

$(INSTALL_DIR):
	$(VERBOSE)mkdir -p $@

$(QMLDIR): $(INSTALL_DIR)
	$(VERBOSE)cp $(PORT_DIR)/src/lib/qt5/qt-everywhere-opensource-src-5.8.0/qtdeclarative/src/imports/qtquick2/qmldir $@

qt5_qtquick2.tar: $(QMLDIR)
	$(VERBOSE)tar cf $@ qt
	$(VERBOSE)rm -rf qt
