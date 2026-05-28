cmake_minimum_required(VERSION 3.16)
project(lab6_login_registration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Sql
        Widgets
        Concurrent
        Network
)

find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Collect source files
set(SOURCES
        main.cpp
        logger/logger.h
        view/MainWindow.cpp
        view/MainWindow.h
        features/auth/login/loginScreen.cpp
        features/auth/login/loginScreen.h
        features/auth/registration/registrationScreen.cpp
        features/auth/registration/registrationScreen.h
        core/data/local_db/repository/UserRepository.h
        core/data/local_db/db.cpp
        core/data/local_db/db.h
        core/data/local_db/dao/UserDao.cpp
        core/data/local_db/dao/UserDao.h
        core/data/local_db/SimpleAES.h
        core/model/User.cpp
        core/model/User.h
        core/model/Photo.cpp
        core/model/Photo.h
        core/model/Session.cpp
        core/model/Session.h
        core/model/exceptions/DomainException.cpp
        core/model/exceptions/DomainException.h
        features/profile/ProfileScreen.cpp
        features/profile/ProfileScreen.h
        features/profile/photoPicker/PhotoPickerScreen.cpp
        features/profile/photoPicker/PhotoPickerScreen.h
        features/profile/photoPicker/thumbnail_widget/PhotoPickerThumbnailWidget.cpp
        features/profile/photoPicker/thumbnail_widget/PhotoPickerThumbnailWidget.h
        features/profile/photoPicker/data/PhotoStorageHandler.cpp
        features/profile/photoPicker/data/PhotoStorageHandler.h
        features/profile/editProfile/EditProfileScreen.cpp
        features/profile/editProfile/EditProfileScreen.h
        features/profile/lab6_json_raii_smartpointer/JsonFileReader.h
        features/profile/lab6_json_raii_smartpointer/JsonFileWriter.h
        features/profile/lab6_json_raii_smartpointer/ProductInterface.h
        features/profile/lab6_json_raii_smartpointer/ProductImpl.cpp
        features/profile/lab6_json_raii_smartpointer/ProductImpl.h
        features/profile/lab6_json_raii_smartpointer/ProductFactory.h
        features/profile/lab6_json_raii_smartpointer/facade/JsonProcessorFacade.cpp
        features/profile/lab6_json_raii_smartpointer/facade/JsonProcessorFacade.h
        features/profile/lab6_json_raii_smartpointer/JsonResultDialog.cpp
        features/profile/lab6_json_raii_smartpointer/JsonResultDialog.h
        features/profile/lab7_REST_client/RestClientWidget.cpp
        features/profile/lab7_REST_client/RestClientWidget.h


)

# Create executable
add_executable(lab6_login_registration ${SOURCES})

# Link libraries - IMPORTANT: Order matters
#target_link_libraries(lab6_login_registration
#        Qt5::Core
#        Qt5::Gui
#        Qt5::Widgets
#        Qt5::Sql      # Make sure this is linked
#        ${OPENSSL_LIBRARIES}
#)

target_link_libraries(lab6_login_registration
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
        Qt6::Concurrent
        Qt6::Network
        ${OPENSSL_LIBRARIES}
)

# For Linux, explicitly link SQLite
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SQLITE3 REQUIRED sqlite3)
    target_link_libraries(lab6_login_registration ${SQLITE3_LIBRARIES})
endif()

# For Windows, copy Qt SQL plugin
if(WIN32)
    add_custom_command(TARGET lab6_login_registration POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${Qt5_DIR}/../../../plugins/sqldrivers/qsqlite.dll
            ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/
    )
endif()