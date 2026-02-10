cmake_minimum_required(VERSION 4.0)
project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        REQUIRED)

add_executable(lab2 main.cpp
        MyString.cpp
        MyString.h)
target_link_libraries(lab2
        Qt5::Core
)

