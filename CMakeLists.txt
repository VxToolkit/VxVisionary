cmake_minimum_required(VERSION 4.0)
project(VxtVisionary)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Quick REQUIRED)

qt_standard_project_setup()

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(VxtVisionary
    src/Main.cpp
)

target_include_directories(VxtVisionary PRIVATE src)

qt_add_qml_module(VxtVisionary
    URI "Main"
    VERSION 1.0

    SOURCES
    src/AppController.cpp
    src/AppController.h

    QML_FILES
    content/Main.qml

    RESOURCES
    assets/VXVisionarySplash.png
)

target_link_libraries(VxtVisionary
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
)