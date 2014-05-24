#
# Build the tools using qmake
#

QMAKESPEC = $(QT5_CONTRIB_DIR)/qtbase/mkspecs/linux-g++

#
# The Makefile needs to rebuild itself to get the correct source paths.
# This gets done by the 'qmake' target.
#

bootstrap/libQtBootstrap.a: bootstrap/Makefile
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C bootstrap qmake
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C bootstrap

moc/moc: bootstrap/libQtBootstrap.a moc/Makefile
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C moc qmake
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C moc

rcc/rcc: rcc/Makefile bootstrap/libQtBootstrap.a
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C rcc qmake
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C rcc

uic/uic: uic/Makefile bootstrap/libQtBootstrap.a
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C uic qmake
	QMAKESPEC=$(QMAKESPEC) $(MAKE) -C uic

#
# Rule to generate tool Makefiles from the respective pro files via qmake
#
# The second include path is required to resolve the Genode-specific
# 'gconfig.cpp' file.  Even though this is a 'cpp' file, it is used via
# '#include'. So we have to make its location known to the 'INCLUDEPATH'.
#
%/Makefile: %.pro
	QMAKESPEC=$(QMAKESPEC) qmake/bin/qmake -o $*/Makefile \
		QT_BUILD_TREE=$(QT5_CONTRIB_DIR)/qtbase \
		QT_CONFIG+=zlib \
		INCLUDEPATH+=$(REP_DIR)/include/qt5 \
		INCLUDEPATH+=$(QT5_CONTRIB_DIR)/qtbase/include \
		INCLUDEPATH+=$(REP_DIR)/include/qt5/qtbase \
		INCLUDEPATH+=$(QT5_CONTRIB_DIR)/qtbase/include \
		INCLUDEPATH+=$(REP_DIR)/include/qt5/qtbase/QtCore \
		INCLUDEPATH+=$(QT5_CONTRIB_DIR)/qtbase/include/QtCore \
		INCLUDEPATH+=$(REP_DIR)/src/lib/qt5/qtbase/src/corelib/global \
		INCLUDEPATH+=$(QT5_CONTRIB_DIR)/qtbase/include/QtCore/5.1.0 \
		INCLUDEPATH+=$(QT5_CONTRIB_DIR)/qtbase/include/QtCore/5.1.0/QtCore \
		INCLUDEPATH+=$(REP_DIR)/include/qt5/qtbase/QtXml \
		INCLUDEPATH+=$(QT5_CONTRIB_DIR)/qtbase/include/QtXml \
		-after DESTDIR= \
		-after "LIBS+=-lQtBootstrap -L../bootstrap" \
		$^
$(info QT5_CONTRIB_DIR=$(QT5_CONTRIB_DIR))
vpath bootstrap.pro $(QT5_CONTRIB_DIR)/qtbase/src/tools/bootstrap
vpath       moc.pro $(QT5_CONTRIB_DIR)/qtbase/src/tools/moc
vpath       rcc.pro $(QT5_CONTRIB_DIR)/qtbase/src/tools/rcc
vpath       uic.pro $(QT5_CONTRIB_DIR)/qtbase/src/tools/uic
