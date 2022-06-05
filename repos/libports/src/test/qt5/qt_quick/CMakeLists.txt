cmake_minimum_required(VERSION 3.10.0)

project(test-qt_quick LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Quick REQUIRED)

add_executable(test-qt_quick
    main.cpp
    qt_quick.qml
    qt_quick.qrc
)

target_link_libraries(test-qt_quick Qt5::Quick)
