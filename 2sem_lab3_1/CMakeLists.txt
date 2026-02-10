cmake_minimum_required(VERSION 4.0)
project(lab3_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(lab3_1 main.cpp
        logger/simple_logger.h
        view/MainWindow.cpp
        view/MainWindow.h)
target_link_libraries(lab3_1
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

