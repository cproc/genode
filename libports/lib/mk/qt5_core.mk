include $(REP_DIR)/lib/import/import-qt5_core.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN = -Wno-unused-but-set-variable -Wno-deprecated-declarations

include $(REP_DIR)/lib/mk/qt5_core_generated.inc

# add Genode-specific sources
QT_SOURCES += qprocess_genode.cpp \
              qthread_genode.cpp \
              qwaitcondition_genode.cpp

# remove unsupported UNIX-specific files
QT_SOURCES_FILTER_OUT = \
  qmutex_unix.cpp \
  qprocess_unix.cpp \
  qthread_unix.cpp \
  qwaitcondition_unix.cpp \
  qfilesystemwatcher_inotify.cpp \
  moc_qfilesystemwatcher_inotify_p.cpp \

# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \
  moc_qsharedmemory.cpp \
  moc_qfilesystemwatcher_inotify_p.cpp \

include $(REP_DIR)/lib/mk/qt5.inc

INC_DIR += $(REP_DIR)/include/qt5/qtbase/QtCore/private \
           $(REP_DIR)/src/lib/qt5/qtbase/src/corelib/thread \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/$(QT_VERSION) \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/$(QT_VERSION)/QtCore \
           $(REP_DIR)/contrib/$(QT5)/qtbase/include/QtCore/$(QT_VERSION)/QtCore/private

LIBS += launchpad zlib icu libc libm alarm libc_lock_pipe
