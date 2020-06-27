include $(call select_from_repositories,lib/import/import-qt5_qmake.mk)

LIBS  = libc libm stdcxx qt5_component egl mesa expat
LIBS += base qoost

built.tag: qmake_prepared.tag qmake_root/lib/ld.lib.so

	@#
	@# run configure
	@#

	source env.sh && $(QT_DIR)/configure \
	-prefix /qt \
	-xplatform $(QMAKE_PLATFORM) \
	-qpa genode \
	-opensource \
	-confirm-license \
	-no-pch \
	-release \
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
	-no-feature-dbus \
	-no-feature-networkinterface \
	-no-feature-process

	@#
	@# build
	@#

	source env.sh && $(MAKE)

	#
	# install into local 'install' directory
	#

	$(MAKE) INSTALL_ROOT=$(CURDIR)/install install

ifeq ($(called_from_lib_mk),yes)
all: built.tag
endif
