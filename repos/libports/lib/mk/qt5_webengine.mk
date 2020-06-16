include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

QT5_PORT_LIBS += libQt5Core libQt5Gui libQt5Network libQt5Widgets
QT5_PORT_LIBS += libQt5Qml libQt5Quick libQt5QuickWidgets

LIBS = egl expat libc libm mesa qt5_component stdcxx $(QT5_PORT_LIBS)

built.tag: qmake_prepared.tag

	@#
	@# run qmake
	@#

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/genode-x86-g++/qt.conf \
		$(QT_DIR)/qtwebengine/qtwebengine.pro \
		-- \
		-no-webengine-alsa \
		-no-webengine-embedded-build \
		-qt-webengine-icu \
		-qt-webengine-ffmpeg \
		-qt-webengine-opus \
		-qt-webengine-webp \
		-no-webengine-pepper-plugins \
		-no-webengine-printing-and-pdf \
		-no-webengine-proprietary-codecs \
		-no-webengine-pulseaudio \
		-no-webengine-spellchecker \
		-no-webengine-native-spellchecker \
		-no-webengine-extensions \
		-no-webengine-webrtc \
		-no-webengine-geolocation \
		-no-webengine-v8-snapshot \
		-no-webengine-webchannel \
		-no-webengine-kerberos \
		-webengine-widgets \
		-webengine-qml \
		-no-webengine-sndio

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
		$(STRIP) libQt5WebEngine.lib.so -o libQt5WebEngine.lib.so.stripped && \
		$(STRIP) libQt5WebEngineCore.lib.so -o libQt5WebEngineCore.lib.so.stripped

	@#
	@# create symlinks in 'bin' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5WebEngine.lib.so.stripped $(PWD)/bin/libQt5WebEngine.lib.so
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5WebEngineCore.lib.so.stripped $(PWD)/bin/libQt5WebEngineCore.lib.so

	@#
	@# create symlinks in 'debug' directory
	@#

	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5WebEngine.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install$(CURDIR)/qmake_root/lib/libQt5WebEngineCore.lib.so $(PWD)/debug/

	@#
	@# create resources archive, QML archive and links
	@#

	ln -sf .$(CURDIR)/qmake_root install/qt
	tar chf $(PWD)/bin/qt5_webengine_resources.tar -C install qt/resources
	tar chf $(PWD)/bin/qt5_webengine_qml.tar -C install qt/qml
	ln -sf $(CURDIR)/install/qt/qml/QtWebEngine/libqtwebengineplugin.lib.so $(PWD)/bin/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
