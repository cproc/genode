content: qt5_qtquick_virtualkeyboard.tar

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

INSTALL_DIR   := qt/qml/QtQuick/VirtualKeyboard
QMLDIR        := $(INSTALL_DIR)/qmldir
STYLES_QMLDIR := $(INSTALL_DIR)/Styles/qmldir

$(INSTALL_DIR):
	$(VERBOSE)mkdir -p $@

$(INSTALL_DIR)/Styles:
	$(VERBOSE)mkdir -p $@

$(QMLDIR): $(INSTALL_DIR)
	$(VERBOSE)cp $(PORT_DIR)/src/lib/qt5/qt-everywhere-opensource-src-5.8.0/qtvirtualkeyboard/src/virtualkeyboard/import/qmldir $@

$(STYLES_QMLDIR): $(INSTALL_DIR)/Styles
	$(VERBOSE)cp $(PORT_DIR)/src/lib/qt5/qt-everywhere-opensource-src-5.8.0/qtvirtualkeyboard/src/virtualkeyboard/styles/qmldir $@

qt5_qtquick_virtualkeyboard.tar: $(QMLDIR) $(STYLES_QMLDIR)
	$(VERBOSE)tar cf $@ qt
	$(VERBOSE)rm -rf qt
