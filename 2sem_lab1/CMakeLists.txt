cmake_minimum_required(VERSION 4.0)
project(lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(lab1 main.cpp
        Counter.cpp
        Counter.h)
# Option 1: Simple - just point to .qrc file
qt_add_resources(lab1 "app_resources"
#        PREFIX "/res"
        FILES "resources.qrc"
)


target_link_libraries(lab1
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

