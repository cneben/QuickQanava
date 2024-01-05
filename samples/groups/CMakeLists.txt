
cmake_minimum_required(VERSION 3.1.0)

project(QuickQanava_sample_groups VERSION 2.4.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(source_files
    groups.cpp
)

# Configure Qt
find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Quick QuickControls2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${Qt5Quick_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} "../../src")

if(NOT TARGET QuickQanava)
    add_subdirectory(../../ quickqanava_build)  # Use ../../src as quickqanava_build subdirectory
    # see https://stackoverflow.com/questions/50408169/cmake-error-add-subdirectory-not-given-a-binary-directory/50408555
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sample_groups ${source_files} groups.qrc)
target_include_directories(sample_groups PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries(sample_groups
    QuickQanava
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)


