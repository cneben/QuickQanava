cmake_minimum_required(VERSION 3.16)

project(QuickQanava_sample_dataflow VERSION 2.5.0 LANGUAGES CXX)

# Configure Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml Quick QuickControls2)
qt_standard_project_setup(REQUIRES 6.6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} "../../src")

set(source_files
    qanDataFlow.cpp
)
set (header_files qanDataFlow.h)
qt_add_qml_module(SampleDataFlow
    VERSION 1.0
    URI SampleDataFlow
    SOURCES
        qanDataFlow.cpp ${header_files}
    QML_FILES
        SampleDataFlow.qml
        ColorNode.qml
        ColorPopup.qml
        ImageNode.qml
        OperationNode.qml
        PercentageNode.qml
        TintNode.qml
    RESOURCES
        Lenna.jpeg
        JS.jpeg
    OUTPUT_DIRECTORY SampleDataFlow
)

target_link_libraries(SampleDataFlow PRIVATE
    QuickQanava
    QuickQanavaplugin
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)

add_executable(sample_dataflow_exe dataflow.cpp)
qt6_add_resources(sample_dataflow_exe "controls_conf"
    PREFIX
        "/"
    FILES
        "qtquickcontrols2.conf"
)
target_link_libraries(sample_dataflow_exe PRIVATE
    QuickQanava
    QuickQanavaplugin
    SampleDataFlow
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)

