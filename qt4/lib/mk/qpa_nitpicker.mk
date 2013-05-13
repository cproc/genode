include $(REP_DIR)/lib/import/import-qpa_nitpicker.mk

SRC_CC = qbasicunixfontdatabase.cpp \
         qfontengine_ft.cpp

SRC_CC += main.cpp \
          qnitpickerintegration.cpp \
          qnitpickerwindowsurface.cpp \
          moc_qnitpickerplatformwindow.cpp

INC_DIR += $(REP_DIR)/contrib/$(QT4)/src/plugins/platforms/fontdatabases/basicunix \
           $(REP_DIR)/contrib/$(QT4)/src/3rdparty/harfbuzz/src \
           $(REP_DIR)/src/lib/qt4/src/plugins/platforms/nitpicker

LIBS += qt_xml

vpath % $(call select_from_repositories,contrib/$(QT4)/src/plugins/platforms/fontdatabases/basicunix)
vpath % $(call select_from_repositories,contrib/$(QT4)/src/gui/text)
vpath % $(call select_from_repositories,src/lib/qt4/src/plugins/platforms/nitpicker)
