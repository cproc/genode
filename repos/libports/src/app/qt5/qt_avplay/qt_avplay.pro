TEMPLATE   = app
TARGET     = qt_avplay
QT         = core gui xml
HEADERS    = avplay_slave.h \
             control_bar.h \
             main_window.h
SOURCES    = control_bar.cpp \
             framebuffer_session_component.cc \
             main.cpp \
             main_window.cpp
RESOURCES  = style.qrc
