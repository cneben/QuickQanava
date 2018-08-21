
cmake_minimum_required(VERSION 3.1.0)

project(GTpo)

# Configure Qt
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
set(CMAKE_AUTOMOC ON)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

include_directories( ${Qt5Core_INCLUDE_DIRS} )
include_directories( ${Qt5Qml_INCLUDE_DIRS} )
include_directories( include )
add_library(QuickContainers STATIC ${qcm_source_files})
target_include_directories(QuickContainers INTERFACE include)
target_sources(QuickContainers INTERFACE ${qcm_header_files})

# Build Qt QML plugin library
set(PLUGIN_TARGET "quickcontainersplugin")
add_library(${PLUGIN_TARGET} SHARED ${qcm_plugin_files})
target_include_directories(${PLUGIN_TARGET} INTERFACE include)
target_link_libraries(${PLUGIN_TARGET} QuickContainers)

# Deployment
set(URI QuickContainers)
string(REPLACE "." "/" TARGETPATH ${URI})
message("QT_QMAKE_EXECUTABLE=" ${QT_QMAKE_EXECUTABLE})
execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_INSTALL_QML OUTPUT_VARIABLE QT_INSTALL_QML_RAW)
message("QT_INSTALL_QML_RAW=" ${QT_INSTALL_QML_RAW})
#string(REPLACE "\n" "" ${QT_INSTALL_QML} ${QT_INSTALL_QML_RAW})
string(STRIP ${QT_INSTALL_QML_RAW} QT_INSTALL_QML)
message("QT_INSTALL_QML=" ${QT_INSTALL_QML})
set(DESTDIR "${QT_INSTALL_QML}/${TARGETPATH}")
message("DESTDIR=" ${DESTDIR})
install(TARGETS ${PLUGIN_TARGET} DESTINATION ${DESTDIR})

# During plugin searching Qt will be looking for 'qmldir' file
# So we should place it next to our plugin lib.
add_custom_command(
    TARGET ${PLUGIN_TARGET}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/src/qmldir
        ${DESTDIR}/qmldir
)
