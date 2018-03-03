
cmake_minimum_required(VERSION 3.1.0)

project(GTpo)

# Configure Qt
find_package(Qt5Core)
find_package(Qt5Qml)
set(CMAKE_AUTOMOC ON)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set (qcm_source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmAbstractContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmContainerModel.h
)

set (qcm_header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmAbstractContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmAdapter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcmContainerModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuickContainers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuickContainers
)

include_directories( ${Qt5Core_INCLUDE_DIRS} )
include_directories( ${Qt5Qml_INCLUDE_DIRS} )
add_library(QuickContainers STATIC ${qcm_source_files})
target_include_directories(QuickContainers INTERFACE src)
target_sources(QuickContainers INTERFACE ${qcm_header_files})
