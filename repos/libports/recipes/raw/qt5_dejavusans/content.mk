content: DejaVuSans.ttf

PORT_DIR := $(call port_dir,$(REP_DIR)/ports/qt5)

DejaVuSans.ttf:
	cp $(PORT_DIR)/src/lib/qt5/qt5/qtquickcontrols/examples/quickcontrols/extras/dashboard/fonts/$@ $@

