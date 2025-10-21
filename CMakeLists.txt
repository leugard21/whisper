cmake_minimum_required(VERSION 3.22)
project(Whatsie VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core Gui Widgets WebEngineCore WebEngineWidgets)

add_executable(whatsie
    src/main.cpp
    src/WebProfileManager.cpp
    src/ClientPage.cpp
    src/PermissionHandler.cpp
    src/AppWindow.cpp
    src/Downloader.cpp
    src/Settings.cpp
    src/SingleInstance.cpp
    src/AutostartManager.cpp
    resources/resources.qrc
)

target_link_libraries(whatsie PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::WebEngineCore Qt6::WebEngineWidgets
)

if(UNIX AND NOT APPLE)
    target_compile_definitions(whatsie PRIVATE QT_NO_FOREACH QT_DEPRECATED_WARNINGS)
endif()

install(TARGETS whatsie
    RUNTIME DESTINATION bin
)

install(FILES resources/linux/whatsie.desktop
    DESTINATION share/applications
)

install(FILES
    resources/icons/whatsie.png
    DESTINATION share/icons/hicolor/256x256/apps
)
install(FILES
    resources/icons/whatsie.svg
    DESTINATION share/icons/hicolor/scalable/apps
)

set(CPACK_PACKAGE_NAME "whatsie")
set(CPACK_PACKAGE_VENDOR "Whatsie")
set(CPACK_PACKAGE_CONTACT "islahulkafaa@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Whatsie — Whatsapp Web Client on Linux")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_HOMEPAGE_URL "https://web.whatsapp.com/")
set(CPACK_STRIP_FILES ON)

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" CPACK_ARCH_LOWER)
set(CPACK_PACKAGE_FILE_NAME "whatsie_${CPACK_PACKAGE_VERSION}_${CPACK_ARCH_LOWER}")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name")
set(CPACK_DEBIAN_PACKAGE_SECTION "net")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
