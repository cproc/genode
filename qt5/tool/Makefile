#
# \brief  Makefile for building the Qt4 tools
# \author Christian Prochaska
# \author Norman Feske
# \date   2009-05-15
#

REP_DIR := $(realpath ..)

include $(REP_DIR)/lib/mk/qt_version.inc

#
# Compound rule for building the tools in the right order
#
all: qmake/qmake moc/moc rcc/rcc uic/uic

#
# Determine qmakespec to be passed to the sub makefiles
#
ifeq ($(shell uname -m),x86_64)
HOST_ARCH := 64
else
HOST_ARCH := 32
endif

QMAKESPEC = $(REP_DIR)/contrib/$(QT4)/qtbase/mkspecs/linux-g++-$(HOST_ARCH)

#
# Build qmake
#
qmake/qmake:
	QMAKESPEC=$(QMAKESPEC) make -C qmake

#
# Build the other tools using qmake
#
vpath bootstrap.pro $(REP_DIR)/contrib/$(QT4)/qtbase/src/tools/bootstrap
vpath       moc.pro $(REP_DIR)/contrib/$(QT4)/qtbase/src/tools/moc
vpath       rcc.pro $(REP_DIR)/contrib/$(QT4)/qtbase/src/tools/rcc
vpath       uic.pro $(REP_DIR)/contrib/$(QT4)/qtbase/src/tools/uic

bootstrap/libbootstrap.a: bootstrap/Makefile
	make -C bootstrap

moc/moc: bootstrap/libbootstrap.a moc/Makefile
	make -C moc

rcc/rcc: rcc/Makefile bootstrap/libbootstrap.a
	make -C rcc

uic/uic: uic/Makefile bootstrap/libbootstrap.a
	make -C uic

#
# Rule to generate tool Makefiles from the respective pro files via qmake
#
# The second include path is required to resolve the Genode-specific
# 'gconfig.cpp' file.  Even though this is a 'cpp' file, it is used via
# '#include'. So we have to make its location known to the 'INCLUDEPATH'.
#
%/Makefile: %.pro
	mkdir -p $*
	QMAKESPEC=$(QMAKESPEC) qmake/qmake -o $*/Makefile \
		QT_BUILD_TREE=$(REP_DIR)/contrib/$(QT4)/qtbase \
		INCLUDEPATH+=$(REP_DIR)/include/qt4 \
		INCLUDEPATH+=$(REP_DIR)/include/qt4/QtCore \
		INCLUDEPATH+=$(REP_DIR)/src/lib/qt4/src/corelib/global \
		-after DESTDIR= \
		-after LIBS+=-L../bootstrap\
		$^

#
# Clean rule
#
clean:
	make -C qmake clean
	rm -rf bootstrap moc rcc uic

distclean: clean
