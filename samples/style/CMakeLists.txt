cmake_minimum_required(VERSION 3.1.0)

project(QuickQanava_sample_style VERSION 2.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(source_files
    custom.cpp
    style.cpp
)

set (header_files custom.h)

# Configure Qt
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui QuickControls2 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT TARGET QuickQanava)
    add_subdirectory(../../ quickqanava_build)  # Use ../../src as quickqanava_build subdirectory
    # see https://stackoverflow.com/questions/50408169/cmake-error-add-subdirectory-not-given-a-binary-directory/50408555
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sample_style ${source_files} style.qrc)
target_include_directories(sample_style PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries(sample_style QuickQanava Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::QuickControls2)

