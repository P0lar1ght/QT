cmake_minimum_required(VERSION 3.20)
project(QT)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "E:/Qt/Qt5.12.12/5.12.12/mingw73_64")

find_package(Qt5 COMPONENTS
        Widgets
        Core
        Gui
        )
add_executable(
        QT main.cpp UI/ui_widget.h UI/widget.h UI/widget.cpp
        sqlite3/sqlite3.h sqlite3/sqlite3.c
        SQLPackage/SQLiteConnection.h SQLPackage/SQLiteConnection.cpp
        Query/CQueryIdCardFromFile.h Query/CQueryIdCardFromFile.cpp
        Query/CQueryPhoneFromFile.h Query/CQueryPhoneFromFile.cpp
        Query/CQueryIPFromFile.h Query/CQueryIPFromFile.cpp
        UI/GetDialog.cpp UI/GetDialog.h
        )
target_link_libraries(QT
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )
if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
