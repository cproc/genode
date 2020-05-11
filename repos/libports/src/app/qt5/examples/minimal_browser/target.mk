TARGET = built.tag

#
# build dependency libraries in var/libcache
#

LIBS = libc libm stdcxx qt5_component mesa expat

#
# Using existing qt5 libraries at the moment (new ones need to be debugged),
# but they cannot be added to LIBS because mixed headers cause
# conflicts. So these libraries must be built externally for now.
#

#LIBS += qt5_core qt5_gui qt5_network qt5_qml qt5_quick qt5_widgets
#LIBS += qt5_qpa_nitpicker

#
# qmake, installed by updated tool/tool_chain_qt5 script
#

QMAKE = /usr/local/genode/qt5/20.08/bin/qmake

QT_PORT_DIR := $(call select_from_ports,qt5_new)
QT_DIR      := $(QT_PORT_DIR)/src/lib/qt5

#
# flags to be passed to mkspecs/genode-x86-g++/qmake.conf via env.sh
#
# information collected from global.mk, lib.mk and prg.mk, only variables
# from global.mk and prg.mk are available
#

GENODE_QMAKE_CFLAGS = \
-D__FreeBSD__=12 \
-D__GENODE__ \
-ffunction-sections \
-fno-strict-aliasing \
$(CC_OPT_NOSTDINC) \
$(CC_MARCH) \
$(CC_OPT_PIC) \
$(filter-out -I.,$(INCLUDES))

GENODE_QMAKE_LFLAGS_APP = \
\
$(addprefix $(LD_OPT_PREFIX),$(LD_MARCH)) \
$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_GC_SECTIONS)) \
$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_ALIGN_SANE)) \
\
$(addprefix $(LD_OPT_PREFIX),--dynamic-list=$(BASE_DIR)/src/ld/genode_dyn.dl) \
\
$(LD_OPT_NOSTDLIB) \
\
-Wl,-Ttext=$(LD_TEXT_ADDR) \
$(CC_MARCH) \
-Wl,--dynamic-linker=$(DYNAMIC_LINKER).lib.so \
-Wl,--eh-frame-hdr \
-Wl,-rpath-link=. \
$(addprefix $(LD_SCRIPT_PREFIX),$(LD_SCRIPT_DYN)) \
-L$(CURDIR)/qt/lib \
-l:ld.lib.so \
-l:libc.lib.so \
-l:libm.lib.so \
-l:qt5_component.lib.so \
-l:qt5_qpa_nitpicker.lib.so \
-l:stdcxx.lib.so \
$(LD_LIBGCC)

GENODE_QMAKE_LFLAGS_SHLIB = \
\
$(LD_OPT_NOSTDLIB) \
\
-Wl,--eh-frame-hdr \
\
$(addprefix $(LD_OPT_PREFIX),$(LD_MARCH)) \
$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_GC_SECTIONS)) \
$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_ALIGN_SANE)) \
\
$(addprefix $(LD_SCRIPT_PREFIX),$(LD_SCRIPT_SO)) \
$(addprefix $(LD_OPT_PREFIX),--entry=0x0) \
-L$(CURDIR)/qt/lib \
-Wl,--whole-archive \
-Wl,--start-group \
-l:ld.lib.so \
-l:libc.lib.so \
-l:libm.lib.so \
-l:stdcxx.lib.so \
-l:ldso-startup.lib.a \
$(LD_LIBGCC) \
-Wl,--end-group \
-Wl,--no-whole-archive

GENODE_QMAKE_INCDIR_OPENGL = $(call select_from_ports,mesa)/include
GENODE_QMAKE_LIBS_OPENGL = $(CURDIR)/qt/lib/mesa.lib.so

#
# export variables for mkspecs/genode-x86-g++/qmake.conf
#

env.sh:
	rm -f $@
	echo "export GENODE_QMAKE_CC='$(CC)'" >> $@
	echo "export GENODE_QMAKE_CXX='$(CXX)'" >> $@
	echo "export GENODE_QMAKE_LINK='$(CXX)'" >> $@
	echo "export GENODE_QMAKE_AR='$(AR)'" >> $@
	echo "export GENODE_QMAKE_OBJCOPY='$(OBJCOPY)'" >> $@
	echo "export GENODE_QMAKE_NM='$(NM)'" >> $@
	echo "export GENODE_QMAKE_STRIP='$(STRIP)'" >> $@
	echo "export GENODE_QMAKE_CFLAGS='$(GENODE_QMAKE_CFLAGS)'" >> $@
	echo "export GENODE_QMAKE_LFLAGS_APP='$(GENODE_QMAKE_LFLAGS_APP)'" >> $@
	echo "export GENODE_QMAKE_LFLAGS_SHLIB='$(GENODE_QMAKE_LFLAGS_SHLIB)'" >> $@
	echo "export GENODE_QMAKE_INCDIR_OPENGL='$(GENODE_QMAKE_INCDIR_OPENGL)'" >> $@
	echo "export GENODE_QMAKE_LIBS_OPENGL='$(GENODE_QMAKE_LIBS_OPENGL)'" >> $@


built.tag: env.sh

	#
	# prepare a directory named 'qt' where qmake can find needed files
	#

	mkdir -p qt/lib
	ln -sf $(BUILD_BASE_DIR)/var/libcache/ld/ld.abi.so qt/lib/ld.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libc/libc.abi.so qt/lib/libc.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libm/libm.abi.so qt/lib/libm.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/mesa/mesa.abi.so qt/lib/mesa.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/qt5_component/qt5_component.abi.so qt/lib/qt5_component.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/qt5_qpa_nitpicker/qt5_qpa_nitpicker.abi.so qt/lib/qt5_qpa_nitpicker.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/stdcxx/stdcxx.abi.so qt/lib/stdcxx.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/ldso-startup/ldso-startup.lib.a qt/lib/ldso-startup.lib.a

	#
	# run configure
	#

	source env.sh && $(QT_DIR)/configure \
	-prefix / \
	-xplatform genode-x86-g++ \
	-opensource \
	-confirm-license \
	-no-pch \
	-debug \
	-no-strip \
	-opengl desktop \
	-qpa minmal \
	-nomake tools \
	-no-accessibility \
	-no-feature-dbus \
	-no-feature-filesystemwatcher \
	-no-feature-networkinterface \
	-no-feature-process \
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

	#
	# build
	#

	source env.sh && $(MAKE)

	#
	# install into local 'install' directory
	#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install install

	#
	# create stripped versions
	#

	cd $(CURDIR)/install/lib && \
		$(STRIP) libQt5Core.lib.so -o libQt5Core.lib.so.stripped && \
		$(STRIP) libQt5Gui.lib.so -o libQt5Gui.lib.so.stripped && \
		$(STRIP) libQt5Network.lib.so -o libQt5Network.lib.so.stripped && \
		$(STRIP) libQt5Qml.lib.so -o libQt5Qml.lib.so.stripped && \
		$(STRIP) libQt5Quick.lib.so -o libQt5Quick.lib.so.stripped && \
		$(STRIP) libQt5Widgets.lib.so -o libQt5Widgets.lib.so.stripped && \
		$(STRIP) libQt5WebEngineCore.lib.so -o libQt5WebEngineCore.lib.so.stripped && \
		$(STRIP) libQt5WebEngine.lib.so -o libQt5WebEngine.lib.so.stripped

	cd $(CURDIR)/install/examples/webengine/minimal && \
		$(STRIP) minimal -o minimal.stripped

	#
	# create symlinks in 'bin' directory
	#

	ln -sf qt5_core.lib.so $(PWD)/bin/libQt5Core.lib.so
	ln -sf qt5_gui.lib.so $(PWD)/bin/libQt5Gui.lib.so
	ln -sf qt5_network.lib.so $(PWD)/bin/libQt5Network.lib.so
	ln -sf qt5_qml.lib.so $(PWD)/bin/libQt5Qml.lib.so
	ln -sf qt5_quick.lib.so $(PWD)/bin/libQt5Quick.lib.so
	ln -sf qt5_widgets.lib.so $(PWD)/bin/libQt5Widgets.lib.so

	#ln -sf $(CURDIR)/install/lib/libQt5Core.lib.so.stripped $(PWD)/bin/libQt5Core.lib.so
	#ln -sf $(CURDIR)/install/lib/libQt5Gui.lib.so.stripped $(PWD)/bin/libQt5Gui.lib.so
	#ln -sf $(CURDIR)/install/lib/libQt5Network.lib.so.stripped $(PWD)/bin/libQt5Network.lib.so
	#ln -sf $(CURDIR)/install/lib/libQt5Qml.lib.so.stripped $(PWD)/bin/libQt5Qml.lib.so
	#ln -sf $(CURDIR)/install/lib/libQt5Quick.lib.so.stripped $(PWD)/bin/libQt5Quick.lib.so
	#ln -sf $(CURDIR)/install/lib/libQt5Widgets.lib.so.stripped $(PWD)/bin/libQt5Widgets.lib.so

	ln -sf $(CURDIR)/install/lib/libQt5WebEngineCore.lib.so.stripped $(PWD)/bin/libQt5WebEngineCore.lib.so
	ln -sf $(CURDIR)/install/lib/libQt5WebEngine.lib.so.stripped $(PWD)/bin/libQt5WebEngine.lib.so
	ln -sf $(CURDIR)/install/examples/webengine/minimal/minimal.stripped $(PWD)/bin/minimal

	#
	# create symlinks in 'debug' directory
	#

	ln -sf qt5_core.lib.so $(PWD)/debug/libQt5Core.lib.so
	ln -sf qt5_gui.lib.so $(PWD)/debug/libQt5Gui.lib.so
	ln -sf qt5_network.lib.so $(PWD)/debug/libQt5Network.lib.so
	ln -sf qt5_qml.lib.so $(PWD)/debug/libQt5Qml.lib.so
	ln -sf qt5_quick.lib.so $(PWD)/debug/libQt5Quick.lib.so
	ln -sf qt5_widgets.lib.so $(PWD)/debug/libQt5Widgets.lib.so

	#ln -sf $(CURDIR)/install/lib/libQt5Core.lib.so $(PWD)/debug/
	#ln -sf $(CURDIR)/install/lib/libQt5Gui.lib.so $(PWD)/debug/
	#ln -sf $(CURDIR)/install/lib/libQt5Network.lib.so $(PWD)/debug/
	#ln -sf $(CURDIR)/install/lib/libQt5Qml.lib.so $(PWD)/debug/
	#ln -sf $(CURDIR)/install/lib/libQt5Quick.lib.so $(PWD)/debug/
	#ln -sf $(CURDIR)/install/lib/libQt5Widgets.lib.so $(PWD)/debug/

	ln -sf $(CURDIR)/install/lib/libQt5WebEngineCore.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/lib/libQt5WebEngine.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/examples/webengine/minimal/minimal $(PWD)/debug/

	#
	# create QML plugin archives and links
	#

	ln -snf . install/qt

	tar chf $(PWD)/bin/qt5_qtquick2.tar -C install --exclude '*.lib.so' qt/qml/QtQuick.2
	ln -sf libqtquick2plugin.lib.so install/qml/QtQuick.2/qt5_qtquick2plugin.lib.so

	tar chf $(PWD)/bin/qt5_qtquick2plugin.tar -C install qt/qml/QtQuick.2/qt5_qtquick2plugin.lib.so
	ln -sf $(CURDIR)/install/qml/QtQuick.2/qt5_qtquick2plugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_qtquick_window.tar -C install --exclude '*.lib.so' qt/qml/QtQuick/Window.2
	ln -sf libwindowplugin.lib.so install/qml/QtQuick/Window.2/qt5_windowplugin.lib.so
	tar chf $(PWD)/bin/qt5_windowplugin.tar -C install qt/qml/QtQuick/Window.2/qt5_windowplugin.lib.so
	ln -sf $(CURDIR)/install/qml/QtQuick/Window.2/qt5_windowplugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_qtquick_controls.tar -C install --exclude='*.lib.so' qt/qml/QtQuick/Controls
	ln -sf libqtquickcontrolsplugin.lib.so install/qml/QtQuick/Controls/qt5_qtquickcontrolsplugin.lib.so
	tar chf $(PWD)/bin/qt5_qtquickcontrolsplugin.tar -C install qt/qml/QtQuick/Controls/qt5_qtquickcontrolsplugin.lib.so
	ln -sf $(CURDIR)/install/qml/QtQuick/Controls/qt5_qtquickcontrolsplugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_qtwebengine.tar -C install --exclude='*.lib.so' qt/qml/QtWebEngine
	ln -sf libqtwebengineplugin.lib.so install/qml/QtWebEngine/qt5_qtwebengineplugin.lib.so
	tar chf $(PWD)/bin/qt5_qtwebengineplugin.tar -C install qt/qml/QtWebEngine/qt5_qtwebengineplugin.lib.so qt/resources
	ln -sf $(CURDIR)/install/qml/QtWebEngine/qt5_qtwebengineplugin.lib.so $(PWD)/bin/

	# mark as done
	#touch $@

clean:
	rm -f $(BUILD_BASE_DIR)/bin/libQt5WebEngineCore.lib.so
	rm -f $(BUILD_BASE_DIR)/bin/libQt5WebEngine.lib.so
	rm -f $(BUILD_BASE_DIR)/bin/minimal
