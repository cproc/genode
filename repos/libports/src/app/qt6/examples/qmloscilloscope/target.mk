CMAKE_LISTS_DIR = $(PRG_DIR)

CMAKE_TARGET_BINARIES = qmloscilloscope

QT6_PORT_LIBS = libQt6Charts \
                libQt6Core \
                libQt6Gui \
                libQt6Network \
                libQt6OpenGL \
                libQt6OpenGLWidgets \
                libQt6Qml \
                libQt6QmlModels \
                libQt6Quick \
                libQt6Widgets

LIBS = qt6_cmake libc libm qt6_component stdcxx mesa egl
