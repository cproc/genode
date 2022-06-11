include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_PORT_LIBS += libQt5Core libQt5Gui libQt5Network
QT5_PORT_LIBS += libQt5Qml libQt5Quick

LIBS = libc libm mesa stdcxx $(QT5_PORT_LIBS)

built.tag: qmake_prepared.tag

	@#
	@# run qmake
	@#

	$(VERBOSE)source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/$(QMAKE_PLATFORM)/qt.conf \
		$(QT_DIR)/qtgraphicaleffects/qtgraphicaleffects.pro \
		$(QT5_OUTPUT_FILTER)

	@#
	@# build
	@#

	$(VERBOSE)source env.sh && $(MAKE) sub-src $(QT5_OUTPUT_FILTER)

	@#
	@# install into local 'install' directory
	@#

	$(VERBOSE)$(MAKE) INSTALL_ROOT=$(CURDIR)/install sub-src-install_subtargets $(QT5_OUTPUT_FILTER)

	$(VERBOSE)ln -sf .$(CURDIR)/qmake_root install/qt

	@#
	@# create stripped version
	@#

	$(VERBOSE)cd $(CURDIR)/install/qt/qml/QtGraphicalEffects && $(STRIP) libqtgraphicaleffectsplugin.lib.so -o libqtgraphicaleffectsplugin.lib.so.stripped
	$(VERBOSE)cd $(CURDIR)/install/qt/qml/QtGraphicalEffects/private && $(STRIP) libqtgraphicaleffectsprivate.lib.so -o libqtgraphicaleffectsprivate.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	$(VERBOSE)ln -sf $(CURDIR)/install/qt/qml/QtGraphicalEffects/libqtgraphicaleffectsplugin.lib.so.stripped $(PWD)/bin/libqtgraphicaleffectsplugin.lib.so
	$(VERBOSE)ln -sf $(CURDIR)/install/qt/qml/QtGraphicalEffects/private/libqtgraphicaleffectsprivate.lib.so.stripped $(PWD)/bin/libqtgraphicaleffectsprivate.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	$(VERBOSE)ln -sf $(CURDIR)/install/qt/qml/QtGraphicalEffects/libqtgraphicaleffectsplugin.lib.so $(PWD)/debug/
	$(VERBOSE)ln -sf $(CURDIR)/install/qt/qml/QtGraphicalEffects/private/libqtgraphicaleffectsprivate.lib.so $(PWD)/debug/

	@#
	@# create tar archives
	@#

	$(VERBOSE)tar chf $(PWD)/bin/qt5_graphicaleffects_qml.tar --exclude='*.lib.so' --transform='s/\.stripped//' -C install qt/qml

	@#
	@# mark as done
	@#

	$(VERBOSE)touch $@


ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
