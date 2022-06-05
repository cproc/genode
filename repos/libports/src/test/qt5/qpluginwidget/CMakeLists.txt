cmake_minimum_required(VERSION 3.10.0)

project(test-qpluginwidget LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(test-qpluginwidget
    main.cpp
)

target_link_libraries(test-qpluginwidget Qt5::Core)
target_link_libraries(test-qpluginwidget Qt5::Gui)
target_link_libraries(test-qpluginwidget Qt5::Network)
target_link_libraries(test-qpluginwidget Qt5::Widgets)
