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
    PresentationBaseWindow.cpp
    PresentationBaseWindow.h
    PresentationWidget.cpp
    PresentationWidget.h
    PresenterEnums.h
    PresenterWindow.cpp
    PresenterWindow.h
    ResizablePixmapLabel.cpp
    ResizablePixmapLabel.h
    SettingsDialog.cpp
    SettingsDialog.h
    Tab.cpp
    Tab.h
    TabWidget.cpp
    TabWidget.h
    ThumbnailsListWidget.cpp
    ThumbnailsListWidget.h
    Timer.cpp
    Timer.h
)

add_executable(${MODULE_NAME} WIN32 MACOSX_BUNDLE
    ${MODULE_SOURCES}
)

if(UNIX AND NOT APPLE)
    target_compile_options(${MODULE_NAME} PRIVATE -fPIC)
endif()

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

if(APPLE)
    set_target_properties(${MODULE_NAME}
    PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/BundleInfo.plist.in
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
    )

    set(MACOSX_BUNDLE_GUI_IDENTIFIER       "com.aghpresenter.aghpresenter")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING  "AGHPresenter version: ${AGHPresenter_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_NAME          "AGHPresenter")
    set(MACOSX_BUNDLE_BUNDLE_VERSION       "${AGHPresenter_VERSION}")
endif()

# Set path to Qt tools (windeployqt, macdeployqt etc.)
set(QT_TOOLS_DIR "$<$<CONFIG:Release>:${qt_LIB_DIRS_RELEASE}>$<$<CONFIG:Debug>:${qt_LIB_DIRS_DEBUG}>/../bin")

# Use windeployqt to bring dlls which are used by Qt
if(WIN32)
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
        COMMAND "${QT_TOOLS_DIR}/windeployqt.exe" "--no-compiler-runtime" "--no-translations" "$<TARGET_FILE:${MODULE_NAME}>"
        )
elseif(APPLE)
add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
    COMMAND "${QT_TOOLS_DIR}/macdeployqt" "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${MODULE_NAME}.app"
    )
endif()
