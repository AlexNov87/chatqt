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

add_library(LIB_CONSTANTS
   alias.h
   constants.h
   constants_load.h
   constants.cpp
   constants_admin.cpp
   constants_load.cpp


)
target_link_libraries(LIB_CONSTANTS PUBLIC Qt${QT_VERSION_MAJOR}::Core)

add_library(LIB_HELPFOO
helpfoo.h
initializators_help.h
helpfoo.cpp
initializators_help.cpp
answer_obj.h
answer_obj.cpp
answer_checker.h
answer_checker.cpp
request_json.h
request_json.cpp
)
target_link_libraries(LIB_HELPFOO PUBLIC
    LIB_CONSTANTS
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

add_library(LIB_SQL_WORK_AND_INIT
formmaster.h
formmaster.cpp
formmaster.ui
sql.h
sql_worker_create.cpp
sql_worker_modandget.cpp
sql_worker_prepared_queries.cpp
)
target_link_libraries(LIB_SQL_WORK_AND_INIT PUBLIC
LIB_HELPFOO Qt${QT_VERSION_MAJOR}::Sql
)

add_library(LIB_RANDOMIZER
tokenizer.h
tokenizer.cpp
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TCPSERV
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        mainwindesigner.cpp
        structs.h
        structs_smallimpl.cpp
        structs_ch.h


        server_setters.cpp
        server_create_and_getters.cpp

        server_sigslots.cpp
        server_session.cpp
        chatroom.cpp
        server_room.cpp

        formlogin.h formlogin.cpp formlogin.ui
        socket_complect.cpp
        formadmin.h formadmin.cpp formadmin.ui
        server_setters_admin.cpp
        server_session_admin.cpp
        server_graphics.h
        server_base.cpp
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
    PUBLIC
    LIB_CONSTANTS
    LIB_HELPFOO
    LIB_SQL_WORK_AND_INIT
    LIB_RANDOMIZER
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
