##
## The following externally defined variables are evaluated:
##
## QMAKE_PROJECT_FILE:   path to the qmake project file (for applications with target.mk)
## QMAKE_TARGET_BINARIES binaries to be stripped and linked into 'bin' and 'debug' directories
## QT5_PORT_LIBS:        Qt5 libraries used from port (for example libQt5Core)
##
#
QT_TOOLS_DIR = /usr/local/genode/qt5/20.08
#QMAKE        = $(QT_TOOLS_DIR)/bin/qmake
#
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

ifeq ($(CONTRIB_DIR),)
QT_DIR     = $(call select_from_repositories,src/lib/qt5)
QT_API_DIR = $(call select_from_repositories,mkspecs)/..
else
QT_PORT_DIR = $(call select_from_ports,qt5)
QT_DIR      = $(QT_PORT_DIR)/src/lib/qt5
QT_API_DIR  = $(QT_DIR)/genode/api
endif

ifneq ($(VERBOSE),)
QT5_OUTPUT_FILTER = > /dev/null
endif

#
# Genode libraries to be linked to Qt applications and libraries
#

QT5_GENODE_LIBS_APP   = libc.lib.so libm.lib.so stdcxx.lib.so qt5_component.lib.so
QT5_GENODE_LIBS_SHLIB = libc.lib.so libm.lib.so stdcxx.lib.so

#
# flags to be passed to qmake via env.sh and mkspecs/common/genode.conf
#

GENODE_CMAKE_CFLAGS = \
	-D__FreeBSD__=12 \
	-D__GENODE__ \
	-ffunction-sections \
	-fno-strict-aliasing \
	$(CC_OPT_NOSTDINC) \
	$(CC_MARCH) \
	$(CC_OPT_PIC) \
	$(filter-out -I.,$(INCLUDES)) \
	-I$(CURDIR)/cmake_root/include/QtCore/spec/$(QMAKE_PLATFORM)

GENODE_CMAKE_LFLAGS_APP = \
	$(addprefix $(LD_OPT_PREFIX),$(LD_MARCH)) \
	$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_GC_SECTIONS)) \
	$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_ALIGN_SANE)) \
	$(addprefix $(LD_OPT_PREFIX),--dynamic-list=$(BASE_DIR)/src/ld/genode_dyn.dl) \
	$(LD_OPT_NOSTDLIB) \
	-Wl,-Ttext=0x01000000 \
	$(CC_MARCH) \
	-Wl,--dynamic-linker=$(DYNAMIC_LINKER).lib.so \
	-Wl,--eh-frame-hdr \
	-Wl,-rpath-link=. \
	-Wl,-T -Wl,$(LD_SCRIPT_DYN) \
	-L$(CURDIR)/cmake_root/lib \
	-Wl,--whole-archive \
	-Wl,--start-group \
	$(addprefix -l:,$(QT5_GENODE_LIBS_APP)) \
	-Wl,--end-group \
	-Wl,--no-whole-archive

GENODE_CMAKE_LFLAGS_SHLIB = \
	$(LD_OPT_NOSTDLIB) \
	-Wl,-shared \
	-Wl,--eh-frame-hdr \
	$(addprefix $(LD_OPT_PREFIX),$(LD_MARCH)) \
	$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_GC_SECTIONS)) \
	$(addprefix $(LD_OPT_PREFIX),$(LD_OPT_ALIGN_SANE)) \
	-Wl,-T -Wl,$(LD_SCRIPT_SO) \
	$(addprefix $(LD_OPT_PREFIX),--entry=0x0) \
	-L$(CURDIR)/cmake_root/lib \
	-Wl,--whole-archive \
	-Wl,--start-group \
	$(addprefix -l:,$(QT5_GENODE_LIBS_SHLIB)) \
	-l:ldso_so_support.lib.a \
	-Wl,--end-group \
	-Wl,--no-whole-archive

#
# libgcc must appear on the command line after all other libs
# (including those added by qmake) and using the QMAKE_LIBS
# variable achieves this, fortunately
#
#GENODE_QMAKE_LIBS = \
#	$(shell $(CC) $(CC_MARCH) -print-libgcc-file-name)

ifeq ($(CONTRIB_DIR),)
GENODE_CMAKE_GL_INCDIRS = $(call select_from_repositories,include/GL)/..
#GENODE_QMAKE_INCDIR_OPENGL = $(call select_from_repositories,include/GL)/..
#GENODE_QMAKE_INCDIR_EGL = $(call select_from_repositories,include/EGL)/..
else
GENODE_CMAKE_GL_INCDIRS = $(call select_from_ports,mesa)/include
#GENODE_QMAKE_INCDIR_OPENGL = $(call select_from_ports,mesa)/include
#GENODE_QMAKE_INCDIR_EGL = $(call select_from_ports,mesa)/include
endif
#
GENODE_CMAKE_OPENGL_LIBS = $(CURDIR)/cmake_root/lib/mesa.lib.so
#GENODE_QMAKE_LIBS_EGL = $(CURDIR)/qmake_root/lib/egl.lib.so
#
##
## export variables for qmake.conf
##
#
#env.sh:
#	$(VERBOSE)rm -f $@
#	$(VERBOSE)echo "export GENODE_QMAKE_CC='$(CC)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_CXX='$(CXX)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_LINK='$(CXX)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_AR='$(AR)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_OBJCOPY='$(OBJCOPY)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_NM='$(NM)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_STRIP='$(STRIP)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_CFLAGS='$(GENODE_QMAKE_CFLAGS)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_LFLAGS_APP='$(GENODE_QMAKE_LFLAGS_APP)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_LFLAGS_SHLIB='$(GENODE_QMAKE_LFLAGS_SHLIB)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_LIBS='$(GENODE_QMAKE_LIBS)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_INCDIR_OPENGL='$(GENODE_QMAKE_INCDIR_OPENGL)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_LIBS_OPENGL='$(GENODE_QMAKE_LIBS_OPENGL)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_INCDIR_EGL='$(GENODE_QMAKE_INCDIR_EGL)'" >> $@
#	$(VERBOSE)echo "export GENODE_QMAKE_LIBS_EGL='$(GENODE_QMAKE_LIBS_EGL)'" >> $@
#

#
# prepare a directory named 'qmake_root' where qmake can find needed files
#

cmake_root:
	$(VERBOSE)mkdir -p $@

cmake_root/bin: cmake_root
	$(VERBOSE)mkdir -p $@
	$(VERBOSE)ln -sf $(QT_TOOLS_DIR)/bin/* $@/

cmake_root/include: cmake_root
	$(VERBOSE)mkdir -p $@
	$(VERBOSE)ln -snf $(QT_API_DIR)/include/* $@/

cmake_root/lib: cmake_root
	$(VERBOSE)mkdir -p $@

cmake_root/lib/cmake: cmake_root/lib
	$(VERBOSE)ln -snf $(QT_API_DIR)/lib/cmake $@

cmake_root/lib/%.lib.so: cmake_root/lib
	$(VERBOSE)ln -sf $(BUILD_BASE_DIR)/var/libcache/$*/$*.abi.so $@

cmake_root/lib/%.lib.a: cmake_root/lib
	$(VERBOSE)ln -sf $(BUILD_BASE_DIR)/var/libcache/$*/$*.lib.a $@


cmake_root/mkspecs: cmake_root
	$(VERBOSE)ln -sf $(QT_API_DIR)/mkspecs $@

#qmake_root/mkspecs: qmake_root
#	$(VERBOSE)mkdir -p $@
#	$(VERBOSE)ln -sf $(QT_API_DIR)/mkspecs/* $@/
#	$(VERBOSE)rm -f $@/modules
#	$(VERBOSE)mkdir $@/modules
#	$(VERBOSE)ln -snf $(QT_API_DIR)/mkspecs/modules/* $@/modules/
#	$(VERBOSE)ln -sf $(QMAKE_PLATFORM)/qconfig.pri $@/
#	$(VERBOSE)ln -sf $(QMAKE_PLATFORM)/qmodule.pri $@/
#
#qmake_prepared.tag: env.sh \
#                    qmake_root/bin \
#                    qmake_root/include \
#                    qmake_root/lib/libc.lib.so \
#                    qmake_root/lib/libm.lib.so \
#                    qmake_root/lib/egl.lib.so \
#                    qmake_root/lib/mesa.lib.so \
#                    qmake_root/lib/qt5_component.lib.so \
#                    qmake_root/lib/stdcxx.lib.so \
#                    qmake_root/lib/ldso_so_support.lib.a \
#                    qmake_root/mkspecs

cmake_prepared.tag: \
                    cmake_root/bin \
                    cmake_root/include \
                    cmake_root/lib/cmake \
                    cmake_root/lib/libc.lib.so \
                    cmake_root/lib/libm.lib.so \
                    cmake_root/lib/egl.lib.so \
                    cmake_root/lib/mesa.lib.so \
                    cmake_root/lib/qt5_component.lib.so \
                    cmake_root/lib/stdcxx.lib.so \
                    cmake_root/lib/ldso_so_support.lib.a \
                    cmake_root/mkspecs

# add symlinks for Qt5 libraries listed in the 'QT5_PORT_LIBS' variable
ifeq ($(CONTRIB_DIR),)
	$(VERBOSE)for qt5_lib in $(QT5_PORT_LIBS); do \
		ln -sf $(BUILD_BASE_DIR)/var/libcache/$${qt5_lib}/$${qt5_lib}.abi.so cmake_root/lib/$${qt5_lib}.lib.so; \
	done
else
	$(VERBOSE)for qt5_lib in $(QT5_PORT_LIBS); do \
		ln -sf $(BUILD_BASE_DIR)/bin/$${qt5_lib}.lib.so cmake_root/lib/; \
	done
endif
	$(VERBOSE)touch $@

.PHONY: build_with_cmake

#build_with_qmake: qmake_prepared.tag
#
#	$(VERBOSE)source env.sh && $(QMAKE) \
#		-qtconf qmake_root/mkspecs/$(QMAKE_PLATFORM)/qt.conf \
#		$(QMAKE_PROJECT_FILE) \
#		$(QT5_OUTPUT_FILTER)
#
#	$(VERBOSE)source env.sh && $(MAKE) $(QT5_OUTPUT_FILTER)
#
#	$(VERBOSE)for qmake_target_binary in $(QMAKE_TARGET_BINARIES); do \
#		$(STRIP) $${qmake_target_binary} -o $${qmake_target_binary}.stripped; \
#		ln -sf $(CURDIR)/$${qmake_target_binary}.stripped $(PWD)/bin/$${qmake_target_binary}; \
#		ln -sf $(CURDIR)/$${qmake_target_binary} $(PWD)/debug/; \
#	done
#

# 'make' called by cmake uses '/bin/sh', which does not understand '-o pipefail'
unexport .SHELLFLAGS

ifeq ($(VERBOSE),)
CMAKE_MAKE_VERBOSE="1"
endif

build_with_cmake: cmake_prepared.tag
	$(VERBOSE)CMAKE_PREFIX_PATH="$(CURDIR)/cmake_root" \
	cmake \
	--no-warn-unused-cli \
	-DCMAKE_MODULE_PATH="$(CURDIR)/cmake_root/lib/cmake/Modules" \
	-DCMAKE_SYSTEM_NAME="Genode" \
	-DCMAKE_C_COMPILER="$(CC)" \
	-DCMAKE_C_FLAGS="$(GENODE_CMAKE_CFLAGS)" \
	-DCMAKE_CXX_COMPILER="$(CXX)" \
	-DCMAKE_CXX_STANDARD="17" \
	-DCMAKE_CXX_FLAGS="$(GENODE_CMAKE_CFLAGS)" \
	-DCMAKE_EXE_LINKER_FLAGS="$(GENODE_CMAKE_LFLAGS_APP)" \
	-DCMAKE_SHARED_LINKER_FLAGS="$(GENODE_CMAKE_LFLAGS_SHLIB)" \
	-DCMAKE_MODULE_LINKER_FLAGS="$(GENODE_CMAKE_LFLAGS_SHLIB)" \
	-DCMAKE_GL_INCDIRS="$(GENODE_CMAKE_GL_INCDIRS)" \
	-DCMAKE_OPENGL_LIBS="$(GENODE_CMAKE_OPENGL_LIBS)" \
	$(CMAKE_LISTS_DIR) \
	$(QT5_OUTPUT_FILTER)

	$(VERBOSE)$(MAKE) VERBOSE=$(CMAKE_MAKE_VERBOSE) $(QT5_OUTPUT_FILTER)

#
# Not every cmake project has an 'install' target, so execute
# this target only if a binary to be installed has "install/" in
# its path.
#
ifneq ($(findstring install/,$(CMAKE_TARGET_BINARIES)),)
	$(VERBOSE)$(MAKE) VERBOSE=$(CMAKE_MAKE_VERBOSE) DESTDIR=install install $(QT5_OUTPUT_FILTER)
endif

	$(VERBOSE)for cmake_target_binary in $(CMAKE_TARGET_BINARIES); do \
		$(STRIP) $${cmake_target_binary} -o $${cmake_target_binary}.stripped; \
		ln -sf $(CURDIR)/$${cmake_target_binary}.stripped $(PWD)/bin/`basename $${cmake_target_binary}`; \
		ln -sf $(CURDIR)/$${cmake_target_binary} $(PWD)/debug/; \
	done

#
# build applications with cmake
#
TARGET ?= $(CMAKE_LISTS_DIR).cmake_target
.PHONY: $(TARGET)
$(TARGET): build_with_cmake
