include $(REP_DIR)/lib/import/import-qt5_qpa_nitpicker.mk

SHARED_LIB = yes

SRC_CC = qgenericunixeventdispatcher.cpp \
         qunixeventdispatcher.cpp \
         qbasicfontdatabase.cpp \
         qfontengine_ft.cpp \
         qeglconvenience.cpp \
         qevdevkeyboardhandler.cpp

SRC_CC += main.cpp \
          qgenodeclipboard.cpp \
          qnitpickercursor.cpp \
          qnitpickerglcontext.cpp \
          qnitpickerintegration.cpp \
          qnitpickerplatformwindow.cpp \
          qnitpickerwindowsurface.cpp \
          qsignalhandlerthread.cpp \
          moc_qnitpickerplatformwindow.cpp \
          moc_qnitpickerwindowsurface.cpp \
          moc_qnitpickerintegrationplugin.cpp \
          moc_qunixeventdispatcher_qpa_p.cpp \
          moc_qevdevkeyboardhandler_p.cpp \
          moc_qsignalhandlerthread.cpp

INC_DIR += $(QT5_PORT_DIR)/include/QtEglSupport/$(QT_VERSION) \
           $(QT5_PORT_DIR)/include/QtEventDispatcherSupport/$(QT_VERSION) \
           $(QT5_PORT_DIR)/include/QtFontDatabaseSupport/$(QT_VERSION) \
           $(QT5_PORT_DIR)/include/QtInputSupport/$(QT_VERSION)

LIBS += qt5_gui qt5_core libm freetype mesa egl qoost

vpath % $(QT5_CONTRIB_DIR)/qtbase/src/platformsupport/eglconvenience
vpath % $(QT5_CONTRIB_DIR)/qtbase/src/platformsupport/eventdispatchers
vpath % $(QT5_CONTRIB_DIR)/qtbase/src/platformsupport/fontdatabases/basic
vpath % $(QT5_CONTRIB_DIR)/qtbase/src/platformsupport/input/evdevkeyboard
vpath % $(QT5_CONTRIB_DIR)/qtbase/src/gui/text
vpath % $(REP_DIR)/src/lib/qt5/qtbase/src/plugins/platforms/nitpicker

CC_CXX_WARN_STRICT =
