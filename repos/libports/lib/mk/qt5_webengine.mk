include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

SHARED = yes

LIBS = egl expat libc libm mesa qt5_component stdcxx
LIBS += libQt5Core libQt5Gui libQt5Network libQt5Qml libQt5Quick libQt5QuickWidgets libQt5Widgets

built.tag: env.sh qmake_root

	@#
	@# add Qt dependencies to qmake_root
	@#

ifeq ($(CONTRIB_DIR),)
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Core/libQt5Core.abi.so                 qmake_root/lib/libQt5Core.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Gui/libQt5Gui.abi.so                   qmake_root/lib/libQt5Gui.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Network/libQt5Network.abi.so           qmake_root/lib/libQt5Network.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Qml/libQt5Qml.abi.so                   qmake_root/lib/libQt5Qml.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Quick/libQt5Quick.abi.so               qmake_root/lib/libQt5Quick.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5QuickWidgets/libQt5QuickWidgets.abi.so qmake_root/lib/libQt5QuickWidgets.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libQt5Widgets/libQt5Widgets.abi.so           qmake_root/lib/libQt5Widgets.lib.so
else
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Core.lib.so         qmake_root/lib/libQt5Core.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Gui.lib.so          qmake_root/lib/libQt5Gui.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Network.lib.so      qmake_root/lib/libQt5Network.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Qml.lib.so          qmake_root/lib/libQt5Qml.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Quick.lib.so        qmake_root/lib/libQt5Quick.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5QuickWidgets.lib.so qmake_root/lib/libQt5QuickWidgets.lib.so
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Widgets.lib.so      qmake_root/lib/libQt5Widgets.lib.so
endif

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
	@# create QML plugin archives and links
	@#

	ln -sf .$(CURDIR)/qmake_root install/qt

	tar chf $(PWD)/bin/qt5_qtwebengine.tar -C install --exclude='*.lib.so' qt/qml/QtWebEngine
	tar chf $(PWD)/bin/qt5_qtwebengineplugin.tar -C install qt/qml/QtWebEngine/libqtwebengineplugin.lib.so qt/resources
	ln -sf $(CURDIR)/install/qt/qml/QtWebEngine/libqtwebengineplugin.lib.so $(PWD)/bin/

	@#
	@# mark as done
	@#

	touch $@

clean:

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
