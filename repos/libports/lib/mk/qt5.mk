ifeq ($(filter-out $(SPECS),arm),)
QMAKE_PLATFORM = genode-arm-g++
else ifeq ($(filter-out $(SPECS),arm_64),)
QMAKE_PLATFORM = genode-aarch64-g++
else ifeq ($(filter-out $(SPECS),x86_32),)
QMAKE_PLATFORM = genode-x86_32-g++
else ifeq ($(filter-out $(SPECS),x86_64),)
QMAKE_PLATFORM = genode-x86_64-g++
else
$(error Error: unsupported platform)
endif

#
# build dependency libraries in var/libcache
#

LIBS = libc libm stdcxx qt5_component egl mesa expat

#
# additional dependencies for QPA plugin
#

LIBS += base qoost

#
# qmake, installed by updated tool/tool_chain_qt5 script
#

QMAKE = /usr/local/genode/qt5/20.08/bin/qmake

QT_PORT_DIR := $(call select_from_ports,qt5_new)
QT_DIR      := $(QT_PORT_DIR)/src/lib/qt5

#
# flags to be passed to qmake.conf via env.sh
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
-Wl,-Ttext=0x01000000 \
$(CC_MARCH) \
-Wl,--dynamic-linker=$(DYNAMIC_LINKER).lib.so \
-Wl,--eh-frame-hdr \
-Wl,-rpath-link=. \
-Wl,-T -Wl,$(LD_SCRIPT_DYN) \
-L$(CURDIR)/qmake_root/lib \
-l:ld.lib.so \
-l:libc.lib.so \
-l:libm.lib.so \
-l:qt5_component.lib.so \
-l:stdcxx.lib.so \
$(shell $(CC) $(CC_MARCH) -print-libgcc-file-name)

GENODE_QMAKE_LFLAGS_SHLIB = \
\
$(LD_OPT_NOSTDLIB) \
\
-Wl,-shared \
-Wl,--eh-frame-hdr \
\
$(addprefix $(LD_OPT_PREFIX),$(LD_MARCH)) \
$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_GC_SECTIONS)) \
$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_ALIGN_SANE)) \
\
-Wl,-T -Wl,$(LD_SCRIPT_SO) \
$(addprefix $(LD_OPT_PREFIX),--entry=0x0) \
-L$(CURDIR)/qmake_root/lib \
-Wl,--whole-archive \
-Wl,--start-group \
-l:ld.lib.so \
-l:libc.lib.so \
-l:libm.lib.so \
-l:stdcxx.lib.so \
-l:ldso_so_support.lib.a \
$(shell $(CC) $(CC_MARCH) -print-libgcc-file-name) \
-Wl,--end-group \
-Wl,--no-whole-archive

GENODE_QMAKE_INCDIR_OPENGL = $(call select_from_ports,mesa)/include
GENODE_QMAKE_LIBS_OPENGL = $(CURDIR)/qmake_root/lib/mesa.lib.so

GENODE_QMAKE_INCDIR_EGL = $(call select_from_ports,mesa)/include
GENODE_QMAKE_LIBS_EGL = $(CURDIR)/qmake_root/lib/egl.lib.so

#
# export variables for qmake.conf
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
	echo "export GENODE_QMAKE_INCDIR_EGL='$(GENODE_QMAKE_INCDIR_EGL)'" >> $@
	echo "export GENODE_QMAKE_LIBS_EGL='$(GENODE_QMAKE_LIBS_EGL)'" >> $@


built.tag: env.sh

	#
	# prepare a directory named 'qt' where qmake can find needed files
	#

	mkdir -p qmake_root/lib
	ln -sf $(BUILD_BASE_DIR)/var/libcache/ld/ld.abi.so qmake_root/lib/ld.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libc/libc.abi.so qmake_root/lib/libc.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libm/libm.abi.so qmake_root/lib/libm.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/egl/egl.abi.so qmake_root/lib/egl.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/mesa/mesa.abi.so qmake_root/lib/mesa.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/qt5_component/qt5_component.abi.so qmake_root/lib/qt5_component.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/stdcxx/stdcxx.abi.so qmake_root/lib/stdcxx.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/ldso_so_support/ldso_so_support.lib.a qmake_root/lib/ldso_so_support.lib.a

	#
	# run configure
	#

	source env.sh && $(QT_DIR)/configure \
	-prefix /qt \
	-xplatform $(QMAKE_PLATFORM) \
	-qpa nitpicker \
	-opensource \
	-confirm-license \
	-no-pch \
	-debug \
	-no-strip \
	-opengl desktop \
	-nomake tools \
	-skip qt3d \
	-skip qtlocation \
	-skip qtmultimedia \
	-skip qtsensors \
	-skip qtserialport \
	-skip qttools \
	-skip qtwebglplugin \
	-skip qtwebengine \
	-no-sse2 \
	-no-accessibility \
	-no-feature-dbus \
	-no-feature-filesystemwatcher \
	-no-feature-networkinterface \
	-no-feature-process

	#
	# build
	#

	source env.sh && $(MAKE)

	#
	# install into local 'install' directory
	#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install install

	#
	# stop here when only the generated API files are needed
	#

	exit 1

	#
	# create stripped versions
	#

	cd $(CURDIR)/install/qt/lib && \
		$(STRIP) libQt5Core.lib.so -o libQt5Core.lib.so.stripped && \
		$(STRIP) libQt5Gui.lib.so -o libQt5Gui.lib.so.stripped && \
		$(STRIP) libQt5Network.lib.so -o libQt5Network.lib.so.stripped && \
		$(STRIP) libQt5Qml.lib.so -o libQt5Qml.lib.so.stripped && \
		$(STRIP) libQt5Quick.lib.so -o libQt5Quick.lib.so.stripped && \
		$(STRIP) libQt5Widgets.lib.so -o libQt5Widgets.lib.so.stripped && \
		$(STRIP) libQt5WebEngineCore.lib.so -o libQt5WebEngineCore.lib.so.stripped && \
		$(STRIP) libQt5WebEngine.lib.so -o libQt5WebEngine.lib.so.stripped

	cd $(CURDIR)/install/qt/plugins/platforms && \
		$(STRIP) libqnitpicker.lib.so -o libqnitpicker.lib.so.stripped

	#
	# create symlinks in 'bin' directory
	#

	ln -sf $(CURDIR)/install/qt/lib/libQt5Core.lib.so.stripped $(PWD)/bin/libQt5Core.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5Gui.lib.so.stripped $(PWD)/bin/libQt5Gui.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5Network.lib.so.stripped $(PWD)/bin/libQt5Network.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5Qml.lib.so.stripped $(PWD)/bin/libQt5Qml.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5Quick.lib.so.stripped $(PWD)/bin/libQt5Quick.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5Widgets.lib.so.stripped $(PWD)/bin/libQt5Widgets.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5WebEngineCore.lib.so.stripped $(PWD)/bin/libQt5WebEngineCore.lib.so
	ln -sf $(CURDIR)/install/qt/lib/libQt5WebEngine.lib.so.stripped $(PWD)/bin/libQt5WebEngine.lib.so
	ln -sf $(CURDIR)/install/qt/plugins/platforms/libqnitpicker.lib.so.stripped $(PWD)/bin/libqnitpicker.lib.so

	#
	# create symlinks in 'debug' directory
	#

	ln -sf $(CURDIR)/install/qt/lib/libQt5Core.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5Gui.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5Network.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5Qml.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5Quick.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5Widgets.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5WebEngineCore.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/lib/libQt5WebEngine.lib.so $(PWD)/debug/
	ln -sf $(CURDIR)/install/qt/plugins/platforms/libqnitpicker.lib.so $(PWD)/debug/libqnitpicker.lib.so

	#
	# create QML plugin archives and links
	#

	tar chf $(PWD)/bin/qt5_qtquick2.tar -C install --exclude '*.lib.so' qt/qml/QtQuick.2
	tar chf $(PWD)/bin/qt5_qtquick2plugin.tar -C install qt/qml/QtQuick.2/libqtquick2plugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick.2/libqtquick2plugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_qtquick_window.tar -C install --exclude '*.lib.so' qt/qml/QtQuick/Window.2
	tar chf $(PWD)/bin/qt5_windowplugin.tar -C install qt/qml/QtQuick/Window.2/libwindowplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Window.2/libwindowplugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_qtquick_controls.tar -C install --exclude='*.lib.so' qt/qml/QtQuick/Controls
	tar chf $(PWD)/bin/qt5_qtquickcontrolsplugin.tar -C install qt/qml/QtQuick/Controls/libqtquickcontrolsplugin.lib.so
	ln -sf $(CURDIR)/install/qt/qml/QtQuick/Controls/libqtquickcontrolsplugin.lib.so $(PWD)/bin/

	tar chf $(PWD)/bin/qt5_qtwebengine.tar -C install --exclude='*.lib.so' qt/qml/QtWebEngine
	tar chf $(PWD)/bin/qt5_qtwebengineplugin.tar -C install qt/qml/QtWebEngine/libqtwebengineplugin.lib.so qt/resources
	ln -sf $(CURDIR)/install/qt/qml/QtWebEngine/libqtwebengineplugin.lib.so $(PWD)/bin/

	# mark as done
	#touch $@

clean:
	#rm -f $(BUILD_BASE_DIR)/bin/libQt5WebEngineCore.lib.so
	#rm -f $(BUILD_BASE_DIR)/bin/libQt5WebEngine.lib.so

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
