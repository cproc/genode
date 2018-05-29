content: qt5_qtquick_window.tar

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

INSTALL_DIR := qt/qml/QtQuick/Window.2
QMLDIR      := $(INSTALL_DIR)/qmldir

$(INSTALL_DIR):
	mkdir -p $@

$(QMLDIR): $(INSTALL_DIR)
	cp $(PORT_DIR)/src/lib/qt5/qt5/qtdeclarative/src/imports/window/qmldir $@

qt5_qtquick_window.tar: $(QMLDIR)
	tar cf $@ --mtime='1970-01-01' qt
	rm -rf qt
