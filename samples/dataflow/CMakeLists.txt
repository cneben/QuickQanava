cmake_minimum_required(VERSION 3.16)

project(QuickQanava_sample_dataflow VERSION 2.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Configure Qt
find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Quick QuickControls2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT TARGET QuickQanava)
    add_subdirectory(../../ quickqanava_build)  # Use ../../src as quickqanava_build subdirectory
    # see https://stackoverflow.com/questions/50408169/cmake-error-add-subdirectory-not-given-a-binary-directory/50408555
endif()

set(source_files
    dataflow.cpp
    qanDataFlow.cpp
)

set (header_files qanDataFlow.h)

add_executable(sample_dataflow ${source_files} dataflow.qrc)
qt6_add_resources(sample_dataflow "controls_conf"
    PREFIX
        "/"
    FILES
        "qtquickcontrols2.conf"
)
target_include_directories(sample_dataflow PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries(sample_dataflow
    QuickQanava
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)
