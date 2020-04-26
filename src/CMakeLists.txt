
set( qan_source_files

	qanBehaviour.cpp
	qanBottomRightResizer.cpp
	qanConnector.cpp
	qanDraggable.cpp
	qanDraggableCtrl.cpp
	qanEdge.cpp
	qanEdgeItem.cpp
	qanGraph.cpp
	qanGraphView.cpp
	qanGrid.cpp
	qanLineGrid.cpp
	qanGroup.cpp
	qanGroupItem.cpp
	qanNavigable.cpp
	qanNavigablePreview.cpp
	qanNode.cpp
	qanNodeItem.cpp
	qanPortItem.cpp
	qanSelectable.cpp
	qanStyle.cpp
	qanStyleManager.cpp
	qanUtils.cpp
)

set (qan_header_files

	qanAbstractDraggableCtrl.h
	qanBehaviour.h
	qanBottomRightResizer.h
	qanConnector.h
	qanContainerAdapter.h
	qanDraggableCtrl.h
	qanDraggable.h
	qanEdge.h
	qanEdgeItem.h
	qanGraphConfig.h
	qanGraph.h
	qanGraphView.h
	qanGrid.h
	qanGrid.h
	qanGroup.h
	qanGroupItem.h
	qanNavigable.h
	qanNavigablePreview.h
	qanNode.h
	qanNodeItem.h
	qanPortItem.h
	qanSelectable.h
	qanStyle.h
	qanStyleManager.h
	qanUtils.h
	QuickQanava.h
)
source_group("Headers" FILES ${qan_header_files})
source_group("Sources" FILES ${qan_source_files})

set (qan_plugin_files
    ${CMAKE_CURRENT_SOURCE_DIR}/qanPlugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qanPlugin.h
)

# Configure Qt
set(CMAKE_AUTOMOC ON)
qt5_wrap_cpp(qan_source_files, qan_header_files)
set(CMAKE_AUTORCC ON)
list(APPEND RESOURCES QuickQanava_plugin.qrc)

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
    get_target_property (QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    if (NOT QT_QMAKE_EXECUTABLE)
        message(FATAL_ERROR "Cannot find qmake")
    endif()
endif()
execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_INSTALL_QML OUTPUT_VARIABLE QT_INSTALL_QML_RAW)
string(STRIP ${QT_INSTALL_QML_RAW} QT_INSTALL_QML)
set(DESTDIR "${QT_INSTALL_QML}/${TARGETPATH}")
install(TARGETS ${PLUGIN_TARGET} DESTINATION ${DESTDIR})

#message("QT_INSTALL_QML=" ${QT_INSTALL_QML})

# Install qmldir file
install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/qmldir_plugin
	DESTINATION ${DESTDIR}
	RENAME qmldir
)

# Install source QML files to ease debugging
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	DESTINATION ${DESTDIR}
	FILES_MATCHING PATTERN "*.qml"
)

