TARGET = calculatorform

#
# build dependency libraries in var/libcache
#
# TODO: build qt5 libraries automatically
#

LIBS = libc libm stdcxx qt5_component egl mesa expat

#
# qmake, installed by updated tool/tool_chain_qt5 script
#

QT_TOOLS_DIR = /usr/local/genode/qt5/20.08
QMAKE        = $(QT_TOOLS_DIR)/bin/qmake

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
-L$(CURDIR)/dependencies/lib \
-l:ld.lib.so \
-l:libc.lib.so \
-l:libm.lib.so \
-l:qt5_component.lib.so \
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
-L$(CURDIR)/dependencies/lib \
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
GENODE_QMAKE_LIBS_OPENGL = $(CURDIR)/dependencies/lib/mesa.lib.so

GENODE_QMAKE_INCDIR_EGL = $(call select_from_ports,egl_api)/include
GENODE_QMAKE_LIBS_EGL = $(CURDIR)/dependencies/lib/egl.lib.so

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
	echo "export GENODE_QMAKE_INCDIR_EGL='$(GENODE_QMAKE_INCDIR_EGL)'" >> $@
	echo "export GENODE_QMAKE_LIBS_EGL='$(GENODE_QMAKE_LIBS_EGL)'" >> $@


$(TARGET): env.sh

	#
	# prepare a directory named 'dependencies' where qmake can find needed files
	#

	mkdir -p dependencies/bin
	ln -s $(QT_TOOLS_DIR)/bin/* dependencies/bin/

	mkdir -p dependencies/include
	ln -s $(QT_DIR)/api/include/* dependencies/include/

	mkdir -p dependencies/lib
	ln -sf $(BUILD_BASE_DIR)/var/libcache/ld/ld.abi.so dependencies/lib/ld.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libc/libc.abi.so dependencies/lib/libc.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libm/libm.abi.so dependencies/lib/libm.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/egl/egl.abi.so dependencies/lib/egl.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/mesa/mesa.abi.so dependencies/lib/mesa.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/qt5_component/qt5_component.abi.so dependencies/lib/qt5_component.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/stdcxx/stdcxx.abi.so dependencies/lib/stdcxx.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/ldso-startup/ldso-startup.lib.a dependencies/lib/ldso-startup.lib.a

	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Core.lib.so dependencies/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Gui.lib.so dependencies/lib/
	ln -sf $(BUILD_BASE_DIR)/bin/libQt5Widgets.lib.so dependencies/lib/

	mkdir -p dependencies/mkspecs
	ln -s $(QT_DIR)/api/mkspecs/* dependencies/mkspecs/
	ln -s genode-x86-g++/qconfig.pri dependencies/mkspecs/
	ln -s genode-x86-g++/qmodule.pri dependencies/mkspecs/
	rm dependencies/mkspecs/modules
	mkdir dependencies/mkspecs/modules
	ln -s $(QT_DIR)/api/mkspecs/modules/*.pri dependencies/mkspecs/modules/

	#
	# run qmake
	#

	source env.sh && $(QMAKE) \
		-qtconf dependencies/mkspecs/genode-x86-g++/qt.conf \
		$(QT_DIR)/qttools/examples/designer/calculatorform/calculatorform.pro

	#
	# build
	#

	source env.sh && $(MAKE)

	#
	# create stripped version
	#

	$(STRIP) $(TARGET) -o $(TARGET).stripped

	#
	# create symlink in 'bin' directory
	#

	ln -sf $(CURDIR)/$(TARGET).stripped $(PWD)/bin/$(TARGET)

	#
	# create symlink in 'debug' directory
	#

	ln -sf $(CURDIR)/$(TARGET) $(PWD)/debug/
