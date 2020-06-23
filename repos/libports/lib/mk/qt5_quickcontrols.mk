include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_PORT_LIBS += libQt5Core libQt5Gui libQt5Network libQt5Widgets
QT5_PORT_LIBS += libQt5Qml libQt5Quick

LIBS = libc libm mesa stdcxx $(QT5_PORT_LIBS)

built.tag: qmake_prepared.tag

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/$(QMAKE_PLATFORM)/qt.conf \
		$(QT_DIR)/qtquickcontrols/qtquickcontrols.pro

	@#
	@# build
	@#

	source env.sh && $(MAKE) sub-src

	@#
	@# install into local 'install' directory
	@#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install sub-src-install_subtargets

	@#
	@# create QML archive and links
	@#

	ln -sf .$(CURDIR)/qmake_root install/qt
	tar chf $(PWD)/bin/qt5_quickcontrols_qml.tar -C install qt/qml
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Controls/libqtquickcontrolsplugin.lib.so $(PWD)/bin/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
