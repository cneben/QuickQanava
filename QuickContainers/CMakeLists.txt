cmake_minimum_required(VERSION 3.1.0)

project(QuickContainers)

# Configure Qt
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
set(CMAKE_AUTOMOC ON)

set (qcm_source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmAbstractContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmContainerModel.cpp
)

set (qcm_header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qcmAbstractContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qcmAdapter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qcmContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qcmContainerModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QuickContainers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QuickContainers
)

set (qcm_plugin_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmPlugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmPlugin.h
)

# Configure QuickContainers library ###########################################
include_directories( ${Qt5Core_INCLUDE_DIRS} )
include_directories( ${Qt5Qml_INCLUDE_DIRS} )
include_directories( include )

#target_include_directories(QuickContainers INTERFACE include)
#target_sources(QuickContainers INTERFACE ${qcm_header_files})

add_library(QuickContainers STATIC ${qcm_header_files} ${qcm_source_files})
set_target_properties(QuickContainers PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(QuickContainers
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/quickcontainers>
    )

target_compile_features(QuickContainers
    PUBLIC
        cxx_std_14
    )

install(FILES
    ${qcm_header_files}
    DESTINATION include/quickcontainers
    )

install(TARGETS QuickContainers
    EXPORT Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include/quickcontainers
    )

# Configure QML QuickContainers plugin ########################################
set(PLUGIN_TARGET "quickcontainersplugin")
add_library(${PLUGIN_TARGET} SHARED ${qcm_plugin_files})

target_link_libraries(QuickContainers Qt5::Core Qt5::Qml)
target_link_libraries(${PLUGIN_TARGET} QuickContainers)

# QML module deployment
set(URI QuickContainers)
string(REPLACE "." "/" TARGETPATH ${URI})
if (NOT DEFINED QT_QMAKE_EXECUTABLE)
    set(QT_QMAKE_EXECUTABLE "qmake")
endif()
execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_INSTALL_QML OUTPUT_VARIABLE QT_INSTALL_QML_RAW)
string(STRIP ${QT_INSTALL_QML_RAW} QT_INSTALL_QML)
set(DESTDIR "${QT_INSTALL_QML}/${TARGETPATH}")
install(TARGETS ${PLUGIN_TARGET} DESTINATION ${DESTDIR})

message("QT_QMAKE_EXECUTABLE=" ${QT_QMAKE_EXECUTABLE})
message("QT_INSTALL_QML=" ${QT_INSTALL_QML})
message("DESTDIR=" ${DESTDIR})

# Install QML module qmldir file
add_custom_command(
    TARGET ${PLUGIN_TARGET}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/src/qmldir
        ${DESTDIR}/qmldir
)

