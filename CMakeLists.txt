cmake_minimum_required(VERSION 3.19)
project(QuakeRunner LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

if(WIN32)
    set(APP_ICON_RESOURCE "icons/icon.ico")
elseif(APPLE)
    set(APP_ICON_RESOURCE "icons/icon.icns")
endif()

qt_add_executable(QuakeRunner
    WIN32 MACOSX_BUNDLE

    main.cpp

    view/quakerunner.h
    view/quakerunner.cpp
    view/quakerunner.ui

    ${APP_ICON_RESOURCE}
)

qt_add_resources(QuakeRunner "app_resources"
    PREFIX "/"
    FILES
        "icons/icon.png"
)

if(APPLE)
    set_target_properties(QuakeRunner PROPERTIES
        MACOSX_BUNDLE_ICON_FILE "icon.icns"
    )
    set_source_files_properties(${APP_ICON_RESOURCE} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
endif()

target_link_libraries(QuakeRunner
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS QuakeRunner
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QuakeRunner
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
