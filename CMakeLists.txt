cmake_minimum_required(VERSION 3.22)
project(Whisper VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_definitions(WHISPER_VERSION="${PROJECT_VERSION}")

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    WebEngineCore
    WebEngineWidgets
)

add_executable(whisper
    src/app/main.cpp
    src/app/AppWindow.cpp
    src/app/SettingsDialog.cpp

    src/web/ClientPage.cpp
    src/web/WebProfileManager.cpp
    src/web/PermissionHandler.cpp

    src/system/AutostartManager.cpp
    src/system/SingleInstance.cpp
    src/system/Settings.cpp

    src/io/Downloader.cpp

    resources/resources.qrc
)

target_link_libraries(whisper PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineCore
    Qt6::WebEngineWidgets
)

if(UNIX AND NOT APPLE)
    target_compile_definitions(whisper PRIVATE QT_NO_FOREACH QT_DEPRECATED_WARNINGS)
endif()

install(TARGETS whisper
    RUNTIME DESTINATION bin
)

install(FILES resources/linux/whisper.desktop
    DESTINATION share/applications
)

install(FILES
    resources/icons/whisper.png
    DESTINATION share/icons/hicolor/256x256/apps
)

install(FILES
    resources/icons/whisper.svg
    DESTINATION share/icons/hicolor/scalable/apps
)

set(CPACK_PACKAGE_NAME "whisper")
set(CPACK_PACKAGE_VENDOR "Whisper")
set(CPACK_PACKAGE_CONTACT "islahulkafaa@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Whisper — Whatsapp Web Client on Linux")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_HOMEPAGE_URL "https://web.whatsapp.com/")
set(CPACK_STRIP_FILES ON)

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" CPACK_ARCH_LOWER)
set(CPACK_PACKAGE_FILE_NAME "whisper_${CPACK_PACKAGE_VERSION}_${CPACK_ARCH_LOWER}")

set(CPACK_GENERATOR "DEB;RPM")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Luna")
set(CPACK_DEBIAN_PACKAGE_SECTION "net")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_RPM_PACKAGE_LICENSE "Custom")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Internet")
set(CPACK_RPM_PACKAGE_AUTOREQPROV "yes")

include(CPack)
