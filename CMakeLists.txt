cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(MusicTL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Widgets Multimedia)

add_executable(MusicTL
    src/main.cpp
    src/MainWindow.cpp
    src/ui/Player.cpp
    src/ui/Settings.cpp
    src/ui/TierList.cpp
    src/ui/Content.cpp
    src/ui/FlowLayout.cpp

    
)

target_link_libraries(MusicTL PRIVATE 
  Qt6::Core 
  Qt6::Gui 
  Qt6::Widgets 
  Qt6::Multimedia
)
