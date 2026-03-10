cmake_minimum_required(VERSION 4.0)
project(lab6_login_registration)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Sql
        Widgets
        REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(lab6_login_registration main.cpp
        logger/logger.h
        view/MainWindow.cpp
        view/MainWindow.h
        authorization/ui/view/login/loginScreen.cpp
        authorization/ui/view/login/loginScreen.h
        authorization/ui/view/registration/registrationScreen.cpp
        authorization/ui/view/registration/registrationScreen.h
        authorization/data/model/User.h
        authorization/data/repository/UserRepository.h
        authorization/data/local_db/db.cpp
        authorization/data/local_db/db.h
        authorization/data/local_db/UserDao.cpp
        authorization/data/local_db/UserDao.h
        authorization/data/local_db/SimpleAES.h
)
target_link_libraries(lab6_login_registration
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
        crypto
)

