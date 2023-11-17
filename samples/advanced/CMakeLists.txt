cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(source_files main.cpp)

# Configure Qt
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui QuickControls2 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(./QuickQanava)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sample_advanced ${source_files} advanced.qrc)
target_include_directories(sample_advanced PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries(sample_advanced
    QuickQanava
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)
