cmake_minimum_required(VERSION 3.16)

project(TCPSERV VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TCPSERV
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        mainwindesigner.cpp

        structs.h
        helpfoo.h
        helpfoo.cpp
        constants.h


        structs_smallimpl.cpp
        structs_ch.h
        initializators_help.h
        initializators_help.cpp
        roomsform.cpp roomsform.ui
        server_setters.cpp
        server_create_and_getters.cpp

        constants.cpp
        answer_obj.h
        answer_obj.cpp
        tokenizer.h
        tokenizer.cpp
        alias.h
        server_sigslots.cpp
        server_session.cpp
        chatroom.cpp
        server_room.cpp
        sql.h
        constants_load.h
        constants_load.cpp
        sql_worker_create.cpp
        formmaster.h formmaster.cpp formmaster.ui
        sql_worker_modandget.cpp
        sql_worker_prepared_queries.cpp






    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TCPSERV APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TCPSERV SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TCPSERV
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TCPSERV
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TCPSERV)
endif()
set_target_properties(TCPSERV PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TCPSERV
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TCPSERV)
endif()
add_subdirectory(CLIENT)
