#
# The following externally defined variables are evaluated:
#
# QT5_PORT_LIBS:        Qt5 libraries used from port (for example libQt5Core)
# QMAKE_PROJECT_FILE:   path to the qmake project file (for applications with target.mk)
# QMAKE_TARGET_BINARIES binaries to be stripped and linked in 'bin' and 'debug' directories
# TARGET:               name of the target binary (for applications with target.mk)
#

QT_TOOLS_DIR = /usr/local/genode/qt5/20.08
QMAKE        = $(QT_TOOLS_DIR)/bin/qmake

ifeq ($(CONTRIB_DIR),)
QT_DIR     = $(call select_from_repositories,src/lib/qt5)
QT_API_DIR = $(call select_from_repositories,mkspecs)/..
else
QT_PORT_DIR = $(call select_from_ports,qt5_new)
QT_DIR      = $(QT_PORT_DIR)/src/lib/qt5
QT_API_DIR  = $(QT_DIR)/api
endif

#
# Genode libraries to be linked to Qt applications and libraries
#

QT5_GENODE_LIBS_APP   = libc.lib.so libm.lib.so stdcxx.lib.so qt5_component.lib.so
QT5_GENODE_LIBS_SHLIB = libc.lib.so libm.lib.so stdcxx.lib.so

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
-Wl,--whole-archive \
-Wl,--start-group \
$(addprefix -l:,$(QT5_GENODE_LIBS_APP)) \
$(shell $(CC) $(CC_MARCH) -print-libgcc-file-name) \
-Wl,--end-group \
-Wl,--no-whole-archive

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
-l:ldso_so_support.lib.a \
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


#
# prepare a directory named 'qmake_root' where qmake can find needed files
#

qmake_root:
	mkdir -p $@

qmake_root/bin: qmake_root
	mkdir -p $@
	ln -sf $(QT_TOOLS_DIR)/bin/* $@/

qmake_root/include: qmake_root
	mkdir -p $@
	ln -snf $(QT_API_DIR)/include/* $@/

qmake_root/lib: qmake_root
	mkdir -p $@

qmake_root/lib/%.lib.so: qmake_root/lib
	ln -sf $(BUILD_BASE_DIR)/var/libcache/$*/$*.abi.so $@

qmake_root/lib/%.lib.a: qmake_root/lib
	ln -sf $(BUILD_BASE_DIR)/var/libcache/$*/$*.lib.a $@

qmake_root/mkspecs: qmake_root
	mkdir -p $@
	ln -sf $(QT_API_DIR)/mkspecs/* $@/
	ln -sf genode-x86-g++/qconfig.pri $@/
	ln -sf genode-x86-g++/qmodule.pri $@/

qmake_prepared.tag: env.sh \
                    qmake_root/bin \
                    qmake_root/include \
                    qmake_root/lib/libc.lib.so \
                    qmake_root/lib/libm.lib.so \
                    qmake_root/lib/egl.lib.so \
                    qmake_root/lib/mesa.lib.so \
                    qmake_root/lib/qt5_component.lib.so \
                    qmake_root/lib/stdcxx.lib.so \
                    qmake_root/lib/ldso_so_support.lib.a \
                    qmake_root/mkspecs
# add symlinks for Qt5 libraries listed in the 'QT5_PORT_LIBS' variable
ifeq ($(CONTRIB_DIR),)
	for qt5_lib in $(QT5_PORT_LIBS); do \
		ln -sf $(BUILD_BASE_DIR)/var/libcache/$${qt5_lib}/$${qt5_lib}.abi.so qmake_root/lib/$${qt5_lib}.lib.so; \
	done
else
	for qt5_lib in $(QT5_PORT_LIBS); do \
		ln -sf $(BUILD_BASE_DIR)/bin/$${qt5_lib}.lib.so qmake_root/lib/; \
	done
endif


.PHONY: build_with_qmake

build_with_qmake: qmake_prepared.tag

	source env.sh && $(QMAKE) \
		-qtconf qmake_root/mkspecs/genode-x86-g++/qt.conf \
		$(QMAKE_PROJECT_FILE)

	source env.sh && $(MAKE)

	for qmake_target_binary in $(QMAKE_TARGET_BINARIES); do \
		$(STRIP) $${qmake_target_binary} -o $${qmake_target_binary}.stripped; \
		ln -sf $(CURDIR)/$${qmake_target_binary}.stripped $(PWD)/bin/$${qmake_target_binary}; \
		ln -sf $(CURDIR)/$${qmake_target_binary} $(PWD)/debug/; \
	done

#
# build Qt5 applications with qmake
#

ifneq ($(TARGET),)
$(TARGET): build_with_qmake
QMAKE_TARGET_BINARIES ?= $(TARGET)
endif
