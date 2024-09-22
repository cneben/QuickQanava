cmake_minimum_required(VERSION 3.16)

project(QuickQanava_sample_topology VERSION 2.5.0 LANGUAGES CXX)

# Configure Qt
find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Quick QuickControls2)
qt_standard_project_setup(REQUIRES 6.6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} "../../src")

if(NOT TARGET QuickQanava)
    add_subdirectory(../../ quickqanava_build)  # Use ../../src as quickqanava_build subdirectory
    # see https://stackoverflow.com/questions/50408169/cmake-error-add-subdirectory-not-given-a-binary-directory/50408555
endif()

set(source_files
    topology.cpp
    qanFaceNode.cpp
)

set (header_files qanFaceNode.h)

add_executable(sample_topology ${source_files} topology.qrc)
target_include_directories(sample_topology PUBLIC QuickQanava)
target_link_libraries(sample_topology
    QuickQanava
    QuickQanavaplugin
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)
