include $(REP_DIR)/lib/import/import-qpa_nitpicker.mk

# get the correct harfbuzz header included
QT_DEFINES += -DQT_BUILD_GUI_LIB

SRC_CC = qgenericunixeventdispatcher.cpp \
         qunixeventdispatcher.cpp \
         qbasicfontdatabase.cpp \
         qfontengine_ft.cpp

SRC_CC += main.cpp \
          qnitpickerintegration.cpp \
          qnitpickerwindowsurface.cpp \
          moc_qnitpickerplatformwindow.cpp \
          moc_qnitpickerintegrationplugin.cpp \
          qevdevkeyboardhandler.cpp \
          moc_qunixeventdispatcher_qpa_p.cpp \
          moc_qevdevkeyboardhandler_p.cpp

INC_DIR += $(REP_DIR)/contrib/$(QT4)/qtbase/src/platformsupport/eventdispatchers \
           $(REP_DIR)/contrib/$(QT4)/qtbase/src/platformsupport/fontdatabases/basic \
           $(REP_DIR)/contrib/$(QT4)/qtbase/src/3rdparty/harfbuzz/src \
           $(REP_DIR)/src/lib/qt5/qtbase/src/plugins/platforms/nitpicker \
           $(REP_DIR)/src/lib/qt5/qtbase/src/platformsupport/input/evdevkeyboard \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2 \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtGui/5.0.2/QtGui \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2 \
           $(REP_DIR)/contrib/$(QT4)/qtbase/include/QtCore/5.0.2/QtCore
           
LIBS += qt_xml

vpath % $(call select_from_repositories,contrib/$(QT4)/qtbase/src/platformsupport/eventdispatchers)
vpath % $(call select_from_repositories,contrib/$(QT4)/qtbase/src/platformsupport/fontdatabases/basic)
vpath % $(call select_from_repositories,contrib/$(QT4)/qtbase/src/gui/text)
vpath % $(call select_from_repositories,src/lib/qt5/qtbase/src/plugins/platforms/nitpicker)
vpath % $(call select_from_repositories,src/lib/qt5/qtbase/src/platformsupport/input/evdevkeyboard)
