#
# Build shader compiler as host tool
#

QMAKE_DIR := $(BUILD_BASE_DIR)/tool/qt5/qmake
MOC_DIR   := $(BUILD_BASE_DIR)/tool/qt5/moc
RCC_DIR   := $(BUILD_BASE_DIR)/tool/qt5/rcc
UIC_DIR   := $(BUILD_BASE_DIR)/tool/qt5/uic

HOST_TOOLS += $(QMAKE_DIR)/qmake $(MOC_DIR)/moc $(RCC_DIR)/rcc $(UIC_DIR)/uic

include $(REP_DIR)/lib/mk/qt5_version.inc

QT5_PORT_DIR := $(call select_from_ports,qt5)
QT5_CONTRIB_DIR := $(QT5_PORT_DIR)/src/lib/qt5/$(QT5)

QMAKESPEC = $(QT5_CONTRIB_DIR)/qtbase/mkspecs/linux-g++

#
# Build qmake
#
$(QMAKE_DIR)/qmake:
	mkdir -p $(QMAKE_DIR)
	QMAKESPEC=$(QMAKESPEC) QT5_CONTRIB_DIR=$(QT5_CONTRIB_DIR) make -C $(QMAKE_DIR) -f $(REP_DIR)/tool/qt5/qmake/Makefile

#
# Build the other tools using qmake
#
vpath bootstrap.pro $(REP_DIR)/contrib/$(QT5)/qtbase/src/tools/bootstrap
vpath       moc.pro $(REP_DIR)/contrib/$(QT5)/qtbase/src/tools/moc
vpath       rcc.pro $(REP_DIR)/contrib/$(QT5)/qtbase/src/tools/rcc
vpath       uic.pro $(REP_DIR)/contrib/$(QT5)/qtbase/src/tools/uic

#
# The Makefile needs to rebuild itself to get the correct source paths.
# This gets done by the 'qmake' target.
#

bootstrap/libQtBootstrap.a: bootstrap/Makefile
	QMAKESPEC=$(QMAKESPEC) make -C bootstrap qmake
	QMAKESPEC=$(QMAKESPEC) make -C bootstrap

moc/moc: bootstrap/libQtBootstrap.a moc/Makefile
	QMAKESPEC=$(QMAKESPEC) make -C moc qmake
	QMAKESPEC=$(QMAKESPEC) make -C moc

rcc/rcc: rcc/Makefile bootstrap/libQtBootstrap.a
	QMAKESPEC=$(QMAKESPEC) make -C rcc qmake
	QMAKESPEC=$(QMAKESPEC) make -C rcc

uic/uic: uic/Makefile bootstrap/libQtBootstrap.a
	QMAKESPEC=$(QMAKESPEC) make -C uic qmake
	QMAKESPEC=$(QMAKESPEC) make -C uic

#
# Rule to generate tool Makefiles from the respective pro files via qmake
#
# The second include path is required to resolve the Genode-specific
# 'gconfig.cpp' file.  Even though this is a 'cpp' file, it is used via
# '#include'. So we have to make its location known to the 'INCLUDEPATH'.
#
%/Makefile: %.pro
	QMAKESPEC=$(QMAKESPEC) qmake/qmake -o $*/Makefile \
		QT_BUILD_TREE=$(REP_DIR)/contrib/$(QT5)/qtbase \
		QT_CONFIG+=zlib \
		INCLUDEPATH+=$(REP_DIR)/include/qt5 \
		INCLUDEPATH+=$(REP_DIR)/contrib/$(QT5)/qtbase/include \
		INCLUDEPATH+=$(REP_DIR)/include/qt5/qtbase \
		INCLUDEPATH+=$(REP_DIR)/contrib/$(QT5)/qtbase/include \
		INCLUDEPATH+=$(REP_DIR)/include/qt5/qtbase/QtCore \
		INCLUDEPATH+=$(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore \
		INCLUDEPATH+=$(REP_DIR)/src/lib/qt5/qtbase/src/corelib/global \
		INCLUDEPATH+=$(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/5.1.0 \
		INCLUDEPATH+=$(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/5.1.0/QtCore \
		INCLUDEPATH+=$(REP_DIR)/include/qt5/qtbase/QtXml \
		INCLUDEPATH+=$(REP_DIR)/contrib/$(QT5)/qtbase/include/QtXml \
		-after DESTDIR= \
		-after "LIBS+=-lQtBootstrap -L../bootstrap" \
		$^

#
# Clean rule
#
clean:
	make -C qmake clean
	rm -rf bootstrap moc rcc uic

distclean: clean
