
cmake_minimum_required(VERSION 3.5.0)

project(QuickQanava)

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
source_group("Headers" FILES ${qan_header_files})
source_group("Sources" FILES ${qan_source_files})

file (GLOB qan_qml_files ${CMAKE_CURRENT_SOURCE_DIR}/*.qml)
set (qan_plugin_files
    ${CMAKE_CURRENT_SOURCE_DIR}/qanPlugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qanPlugin.h
)

# Configure Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core 5.10 REQUIRED)
find_package(Qt5Widgets 5.10 REQUIRED)
find_package(Qt5Gui 5.10 REQUIRED)
find_package(Qt5Quick 5.10 REQUIRED)
find_package(Qt5Qml 5.10 REQUIRED)
find_package(Qt5QuickControls2 5.10 REQUIRED)

qt5_wrap_cpp(qan_source_files, qan_header_files)

set(CMAKE_AUTORCC ON)
list(APPEND RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/QuickQanava.qrc)
#qt5_add_resources(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/QuickQanava.qrc)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)

include_directories( ${Qt5Quick_INCLUDE_DIRS} )
include_directories( ${Qt5Qml_INCLUDE_DIRS} )
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_library(QuickQanava ${qan_source_files} ${RESOURCES})

# Require C++14
target_compile_features(QuickQanava PUBLIC cxx_std_14)

target_include_directories(QuickQanava INTERFACE ../GTpo/src)
target_include_directories(QuickQanava INTERFACE GTpo QuickContainers)
target_sources(QuickQanava INTERFACE ${qan_header_files})
target_link_libraries(QuickQanava PUBLIC GTpo QuickContainers Qt5::Core Qt5::Gui Qt5::QuickControls2)

# Build Qt QML plugin library
set(PLUGIN_TARGET "quickqanavaplugin")
add_library(${PLUGIN_TARGET} SHARED ${qan_plugin_files})
target_link_libraries(${PLUGIN_TARGET} QuickQanava)

# Deployment
set(URI QuickQanava)
string(REPLACE "." "/" TARGETPATH ${URI})
execute_process(COMMAND qmake -query QT_INSTALL_QML OUTPUT_VARIABLE QT_INSTALL_QML_RAW)
string(REPLACE "\n" "" QT_INSTALL_QML ${QT_INSTALL_QML_RAW})
set(DESTDIR "${QT_INSTALL_QML}/${TARGETPATH}")
install(TARGETS ${PLUGIN_TARGET} DESTINATION ${DESTDIR})

# During plugin searching Qt will be looking for 'qmldir' file
# So we should place it next to our plugin lib.
add_custom_command(
    TARGET ${PLUGIN_TARGET}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
        ${DESTDIR}/qmldir
)

# Deploying the QML files also on the physical drive, not only as a resources
# helps during debugging, as Qt Creator is able to step into the library code.
add_custom_command(
    TARGET ${PLUGIN_TARGET}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${qan_qml_files}
        ${DESTDIR}
)
