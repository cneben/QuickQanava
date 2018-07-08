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
        QuickQanava
        QuickQanava.h
        qanUtils.h
        qanGraphConfig.h
        qanGraphView.h
        qanEdge.h
        qanEdgeItem.h
        qanNode.h
        qanNodeItem.h
        qanPortItem.h
        qanSelectable.h
        qanDraggable.h
        qanAbstractDraggableCtrl.h
        qanDraggableCtrl.h
        qanDraggableCtrl.hpp
        qanConnector.h
        qanBehaviour.h
        qanGroup.h
        qanGroupItem.h
        qanGraph.h
        qanGraph.hpp
        qanStyle.h
        qanStyleManager.h
        qanNavigable.h
        qanNavigablePreview.h
        qanGrid.h
        qanContainerAdapter.h
        qanBottomRightResizer.h
)
source_group("Headers" FILES ${qan_header_files})
source_group("Sources" FILES ${qan_source_files})

# Configure Qt
set(CMAKE_AUTOMOC ON)

qt5_wrap_cpp(qan_source_files, qan_header_files)

set(CMAKE_AUTORCC ON)
list(APPEND RESOURCES QuickQanava.qrc)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_library(QuickQanava ${qan_source_files} ${qan_header_files} ${RESOURCES})

target_link_libraries(QuickQanava PUBLIC GTpo QuickContainers Qt5::Core Qt5::Gui Qt5::QuickControls2)

target_include_directories(QuickQanava
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    )

target_compile_features(QuickQanava
    PUBLIC
        cxx_std_14
    )

install(FILES
    ${qan_header_files}
    DESTINATION include
    )
    
install(TARGETS QuickQanava
    EXPORT Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )
