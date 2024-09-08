cmake_minimum_required(VERSION 3.16)

project(QuickQanava_sample_groups VERSION 2.5.0 LANGUAGES CXX)

# Configure Qt
find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Quick QuickControls2)
qt_standard_project_setup(REQUIRES 6.5)
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
qt_add_qml_module(sample_groups
    VERSION 1.0
    URI sample_groups
    QML_FILES
        groups.qml
    OUTPUT_DIRECTORY sample_groups
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} "../../src")
if(NOT TARGET QuickQanava)
    add_subdirectory(../../)  # Use ../../src as quickqanava_build subdirectory
    # see https://stackoverflow.com/questions/50408169/cmake-error-add-subdirectory-not-given-a-binary-directory/50408555
endif()

# add_executable(sample_groups ${source_files} groups.qrc)
# target_include_directories(sample_groups PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries(sample_groups_exe PUBLIC
    QuickQanava
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)


