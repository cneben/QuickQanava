
cmake_minimum_required(VERSION 3.1.0)

# Configure Qt
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml)
find_package(Qt5QuickControls2 REQUIRED)
set(CMAKE_AUTOMOC ON)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( source_files
    groups.cpp
)

set (header_files )

set(CMAKE_AUTORCC ON)
qt5_wrap_cpp(source_files, header_files)
qt5_add_resources(RESOURCES groups.qrc)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories( ${Qt5Quick_INCLUDE_DIRS} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} "../../src" )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sample_groups ${source_files} ${RESOURCES})
target_include_directories(sample_groups PUBLIC QuickQanava Qt5::QuickControls2)
target_link_libraries(sample_groups QuickQanava QuickContainers Qt5::Core Qt5::Gui Qt5::QuickControls2)


