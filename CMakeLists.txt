cmake_minimum_required(VERSION 3.22)
project(Whatsie VERSION 0.1.0 LANGUAGES CXX)

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
)

target_link_libraries(whatsie PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::WebEngineCore Qt6::WebEngineWidgets
)

if(UNIX AND NOT APPLE)
    target_compile_definitions(whatsie PRIVATE QT_NO_FOREACH QT_DEPRECATED_WARNINGS)
endif()
