cmake_minimum_required(VERSION 3.16)

project(QuickQanava_sample_groups VERSION 2.5.0 LANGUAGES CXX)

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

set(source_files
    groups.cpp
)
qt_add_executable(sample_groups_exe groups.cpp)
qt_add_qml_module(SampleGroups
    VERSION 1.0
    URI SampleGroups
    QML_FILES
    SampleGroups.qml
    OUTPUT_DIRECTORY SampleGroups
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} "../../src")

set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/qml ${CMAKE_BINARY_DIR}/imports CACHE STRING "" FORCE)

target_link_libraries(sample_groups_exe PRIVATE
    QuickQanava
    QuickQanavaplugin
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)


