
cmake_minimum_required(VERSION 3.1.0)

project(QuickQanava)

# Configure Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( qan_source_files
    qanGraphView.cpp
    qanUtils.cpp
    qanEdge.cpp
    qanEdgeItem.cpp
    qanNode.cpp
    qanNodeItem.cpp
    qanPortItem.cpp
    qanSelectable.cpp
    qanDraggable.cpp
    qanConnector.cpp
    qanBehaviour.cpp
    qanGraph.cpp
    qanGroup.cpp
    qanGroupItem.cpp
    qanStyle.cpp
    qanStyleManager.cpp
    qanNavigable.cpp
    qanNavigablePreview.cpp
    qanGrid.cpp
    qanBottomRightResizer.cpp
)

set (qan_header_files 
        ${CMAKE_CURRENT_SOURCE_DIR}/QuickQanava
        ${CMAKE_CURRENT_SOURCE_DIR}/QuickQanava.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanUtils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanGraphConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanGraphView.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanEdge.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanEdgeItem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanNode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanNodeItem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanPortItem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanSelectable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanDraggable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanAbstractDraggableCtrl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanDraggableCtrl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanDraggableCtrl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qanConnector.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanBehaviour.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanGroup.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanGroupItem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanGraph.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanGraph.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qanStyle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanStyleManager.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanNavigable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanNavigablePreview.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanGrid.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanContainerAdapter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qanBottomRightResizer.h
)
qt5_wrap_cpp(qan_source_files, qan_header_files)
qt5_add_resources(RESOURCES quickqanava.qrc)
include_directories( ${Qt5Quick_INCLUDE_DIRS} )
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_library(QuickQanava SHARED ${qan_source_files} )

target_include_directories(QuickQanava INTERFACE ../GTpo/src)
target_include_directories(QuickQanava INTERFACE GTpo QuickContainers)
#target_include_directories(QuickQanava INTERFACE ${qan_header_files})
target_sources(QuickQanava INTERFACE ${qan_header_files})
target_link_libraries(QuickQanava GTpo)


