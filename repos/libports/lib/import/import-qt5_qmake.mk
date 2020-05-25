QT_TOOLS_DIR = /usr/local/genode/qt5/20.08
QMAKE        = $(QT_TOOLS_DIR)/bin/qmake

ifeq ($(CONTRIB_DIR),)
QT_DIR     := $(call select_from_repositories,src/lib/qt5)
QT_API_DIR := $(call select_from_repositories,mkspecs)/..
else
QT_PORT_DIR := $(call select_from_ports,qt5_new)
QT_DIR      := $(QT_PORT_DIR)/src/lib/qt5
QT_API_DIR  := $(QT_DIR)/api
endif

#
# Genode libraries to be linked to Qt applications and libraries
#

QT5_GENODE_LIBS_APP   := libc.lib.so libm.lib.so stdcxx.lib.so qt5_component.lib.so
QT5_GENODE_LIBS_SHLIB := libc.lib.so libm.lib.so stdcxx.lib.so

#
# flags to be passed to qmake via env.sh and mkspecs/common/genode.conf
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
$(addprefix -l:,$(QT5_GENODE_LIBS_APP)) \
$(shell $(CC) $(CC_MARCH) -print-libgcc-file-name)

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
-Wl,-T -Wl,$(LD_SCRIPT_SO) \
$(addprefix $(LD_OPT_PREFIX),--entry=0x0) \
-L$(CURDIR)/qmake_root/lib \
-Wl,--whole-archive \
-Wl,--start-group \
$(addprefix -l:,$(QT5_GENODE_LIBS_SHLIB)) \
-l:ldso-startup.lib.a \
$(shell $(CC) $(CC_MARCH) -print-libgcc-file-name) \
-Wl,--end-group \
-Wl,--no-whole-archive

ifeq ($(CONTRIB_DIR),)
GENODE_QMAKE_INCDIR_OPENGL = $(call select_from_repositories,include/GL)/..
GENODE_QMAKE_INCDIR_EGL = $(call select_from_repositories,include/EGL)/..
else
GENODE_QMAKE_INCDIR_OPENGL = $(call select_from_ports,mesa)/include
GENODE_QMAKE_INCDIR_EGL = $(call select_from_ports,mesa)/include
endif

GENODE_QMAKE_LIBS_OPENGL = $(CURDIR)/qmake_root/lib/mesa.lib.so
GENODE_QMAKE_LIBS_EGL = $(CURDIR)/qmake_root/lib/egl.lib.so

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


qmake_root:

	#
	# prepare a directory named 'qmake_root' where qmake can find needed files
	#

	rm -rf qmake_root

	mkdir -p qmake_root/bin
	ln -s $(QT_TOOLS_DIR)/bin/* qmake_root/bin/

	mkdir -p qmake_root/include
	ln -s $(QT_API_DIR)/include/* qmake_root/include/

	mkdir -p qmake_root/lib
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libc/libc.abi.so qmake_root/lib/libc.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/libm/libm.abi.so qmake_root/lib/libm.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/egl/egl.abi.so qmake_root/lib/egl.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/mesa/mesa.abi.so qmake_root/lib/mesa.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/qt5_component/qt5_component.abi.so qmake_root/lib/qt5_component.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/stdcxx/stdcxx.abi.so qmake_root/lib/stdcxx.lib.so
	ln -sf $(BUILD_BASE_DIR)/var/libcache/ldso-startup/ldso-startup.lib.a qmake_root/lib/ldso-startup.lib.a

	mkdir -p qmake_root/mkspecs
	ln -s $(QT_API_DIR)/mkspecs/* qmake_root/mkspecs/
	ln -s genode-x86-g++/qconfig.pri qmake_root/mkspecs/
	ln -s genode-x86-g++/qmodule.pri qmake_root/mkspecs/
	rm qmake_root/mkspecs/modules
	mkdir qmake_root/mkspecs/modules
	ln -s $(QT_API_DIR)/mkspecs/modules/*.pri qmake_root/mkspecs/modules/
