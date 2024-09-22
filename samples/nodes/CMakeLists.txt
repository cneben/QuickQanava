cmake_minimum_required(VERSION 3.16)

project(QuickQanava_sample_nodes VERSION 2.5.0 LANGUAGES CXX)

# Configure Qt
find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Quick QuickControls2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} "../../src")

set(source_files
    nodes.cpp
)
set (header_files)
add_executable(sample_nodes ${source_files} nodes.qrc)
target_include_directories(sample_nodes PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries(sample_nodes
    QuickQanava
    QuickQanavaplugin
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)
