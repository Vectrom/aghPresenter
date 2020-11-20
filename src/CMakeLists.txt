cmake_minimum_required(VERSION 3.15.5)

set(MODULE_NAME ${CMAKE_PROJECT_NAME})

set(MODULE_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    Presentation.cpp
    Presentation.h
    PresentationWindow.cpp
    PresentationWindow.h
    Tab.cpp
    Tab.h
    TabWidget.cpp
    TabWidget.h
)

add_executable(${MODULE_NAME} WIN32
    ${MODULE_SOURCES}
)

target_include_directories(${MODULE_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${MODULE_NAME}
    poppler::poppler
    qt::qt
)

# Set path to Qt tools (windeployqt, macdeployqt etc.)
set(QT_TOOLS_DIR "$<$<CONFIG:Release>:${qt_LIB_DIRS_RELEASE}>$<$<CONFIG:Debug>:${qt_LIB_DIRS_DEBUG}>/../bin")

# Use windeployqt to bring dlls which are used by Qt
if(WIN32)
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
        COMMAND "${QT_TOOLS_DIR}/windeployqt.exe" "--no-compiler-runtime" "--no-translations" "$<TARGET_FILE:${MODULE_NAME}>"
        )
endif()

