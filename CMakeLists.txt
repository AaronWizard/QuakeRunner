cmake_minimum_required(VERSION 3.19)
project(QuakeRunner LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(QuakeRunner
    WIN32 MACOSX_BUNDLE
    main.cpp

    view/quakerunner.h
    view/quakerunner.cpp
    view/quakerunner.ui
)

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
