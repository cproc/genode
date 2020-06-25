include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_PORT_LIBS = libQt5Core libQt5Gui libQt5Network libQt5Sql libQt5Test libQt5Widgets

LIBS = libc libm mesa stdcxx $(QT5_PORT_LIBS)

built.tag: qmake_prepared.tag

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/$(QMAKE_PLATFORM)/qt.conf \
		$(QT_DIR)/qtdeclarative/qtdeclarative.pro \
		-- \
		-no-feature-qml-devtools

	@#
	@# build
	@#

	source env.sh && $(MAKE) sub-src

	@#
	@# install into local 'install' directory
	@#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install sub-src-install_subtargets

	ln -sf .$(CURDIR)/qmake_root install/qt

	@#
	@# create stripped versions
	@#

	cd $(CURDIR)/install/qt/lib && \
		$(STRIP) libQt5Qml.lib.so -o libQt5Qml.lib.so.stripped && \
		$(STRIP) libQt5Quick.lib.so -o libQt5Quick.lib.so.stripped && \
		$(STRIP) libQt5QuickWidgets.lib.so -o libQt5QuickWidgets.lib.so.stripped

	cd $(CURDIR)/install/qt/qml/Qt/labs/folderlistmodel && \
		$(STRIP) libqmlfolderlistmodelplugin.lib.so -o libqmlfolderlistmodelplugin.lib.so.stripped

	cd $(CURDIR)/install/qt/qml/QtQuick.2 && \
		$(STRIP) libqtquick2plugin.lib.so -o libqtquick2plugin.lib.so.stripped

	cd $(CURDIR)/install/qt/qml/QtQuick/Layouts && \
		$(STRIP) libqquicklayoutsplugin.lib.so -o libqquicklayoutsplugin.lib.so.stripped

	cd $(CURDIR)/install/qt/qml/QtQuick/Window.2 && \
		$(STRIP) libwindowplugin.lib.so -o libwindowplugin.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install/qt/lib/libQt5Qml.lib.so.stripped $(PWD)/bin/libQt5Qml.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5Quick.lib.so.stripped $(PWD)/bin/libQt5Quick.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5QuickWidgets.lib.so.stripped $(PWD)/bin/libQt5QuickWidgets.lib.so

	ln -sf $(CURDIR)/install/qt/qml/Qt/labs/folderlistmodel/libqmlfolderlistmodelplugin.lib.so.stripped $(PWD)/bin/libqmlfolderlistmodelplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick.2/libqtquick2plugin.lib.so.stripped $(PWD)/bin/libqtquick2plugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Layouts/libqquicklayoutsplugin.lib.so.stripped $(PWD)/bin/libqquicklayoutsplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Window.2/libwindowplugin.lib.so.stripped $(PWD)/bin/libwindowplugin.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install/qt/lib/libQt5Qml.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5Quick.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5QuickWidgets.lib.so $(PWD)/debug/

	ln -sf $(CURDIR)/install/qt/qml/Qt/labs/folderlistmodel/libqmlfolderlistmodelplugin.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick.2/libqtquick2plugin.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Layouts/libqquicklayoutsplugin.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Window.2/libwindowplugin.lib.so $(PWD)/debug/

	@#
	@# create tar archive
	@#

	tar chf $(PWD)/bin/qt5_declarative_qml.tar --exclude='*.lib.so' --transform='s/\.stripped//' -C install qt/qml

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
