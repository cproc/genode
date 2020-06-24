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
		$(QT_DIR)/qtquickcontrols2/qtquickcontrols2.pro

	@#
	@# build
	@#

	source env.sh && $(MAKE) sub-src

	@#
	@# install into local 'install' directory
	@#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install sub-src-install_subtargets

	@#
	@# create stripped versions
	@#

	cd $(CURDIR)/install$(CURDIR)/qmake_root/lib && \
		$(STRIP) libQt5QuickControls2.lib.so -o libQt5QuickControls2.lib.so.stripped && \
		$(STRIP) libQt5QuickTemplates2.lib.so -o libQt5QuickTemplates2.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5QuickControls2.lib.so.stripped $(PWD)/bin/libQt5QuickControls2.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5QuickTemplates2.lib.so.stripped $(PWD)/bin/libQt5QuickTemplates2.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5QuickControls2.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5QuickTemplates2.lib.so $(PWD)/debug/

	@#
	@# create QML archive and links
	@#

	ln -sf .$(CURDIR)/qmake_root install/qt
	tar chf $(PWD)/bin/qt5_quickcontrols2_qml.tar -C install qt/qml
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Controls.2/libqtquickcontrols2plugin.lib.so $(PWD)/bin/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Templates.2/libqtquicktemplates2plugin.lib.so $(PWD)/bin/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
