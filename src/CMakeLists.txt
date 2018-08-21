
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

file (GLOB qan_qml_files ${CMAKE_CURRENT_SOURCE_DIR}/*.qml)
set (qan_plugin_files
    ${CMAKE_CURRENT_SOURCE_DIR}/qanPlugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qanPlugin.h
)

# Configure Qt
set(CMAKE_AUTOMOC ON)
qt5_wrap_cpp(qan_source_files, qan_header_files)
set(CMAKE_AUTORCC ON)
list(APPEND RESOURCES QuickQanava.qrc)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QT_QML_DEBUG>)

# Configure QuickQanava library ###############################################
add_library(QuickQanava ${qan_source_files} ${qan_header_files} ${RESOURCES})
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
    DESTINATION include/quickqanava
    )

install(TARGETS QuickQanava
    EXPORT Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/quickqanava
    )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
target_link_libraries(QuickQanava PUBLIC GTpo QuickContainers Qt5::Core Qt5::Gui Qt5::QuickControls2)


# Configure QuickQanava QML module plugin #####################################
set(PLUGIN_TARGET "quickqanavaplugin")
add_library(${PLUGIN_TARGET} SHARED ${qan_plugin_files})
target_link_libraries(${PLUGIN_TARGET} QuickQanava)

# QML module deployment
set(URI QuickQanava)
string(REPLACE "." "/" TARGETPATH ${URI})
if (NOT DEFINED QT_QMAKE_EXECUTABLE)
    set(QT_QMAKE_EXECUTABLE "qmake")
endif()
execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_INSTALL_QML OUTPUT_VARIABLE QT_INSTALL_QML_RAW)
string(STRIP ${QT_INSTALL_QML_RAW} QT_INSTALL_QML)
set(DESTDIR "${QT_INSTALL_QML}/${TARGETPATH}")
install(TARGETS ${PLUGIN_TARGET} DESTINATION ${DESTDIR})

message("QT_INSTALL_QML=" ${QT_INSTALL_QML})

# Install qmldir file
add_custom_command(
    TARGET ${PLUGIN_TARGET}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
        ${DESTDIR}/qmldir
)

# Install source QML files to ease debugging
add_custom_command(
    TARGET ${PLUGIN_TARGET}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${qan_qml_files}
        ${DESTDIR}
)

