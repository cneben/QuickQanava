cmake_minimum_required(VERSION 3.5.0)

project(QuickQanava VERSION 0.11.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Conan.io support
if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)

add_subdirectory(GTpo)
add_subdirectory(QuickContainers)
add_subdirectory(src)

option(QUICK_QANAVA_BUILD_SAMPLES "Build the samples" FALSE)
option(QUICK_QANAVA_BUILD_STATIC_QRC "Build *.qrc resources statically" TRUE)
option(QUICK_QANAVA_WINDOWS_DEPLOY "Use windeployqt on Windows" FALSE)

if (${QUICK_QANAVA_BUILD_SAMPLES})
    #add_subdirectory(samples/resizer)
    add_subdirectory(samples/navigable)
    add_subdirectory(samples/nodes)
    add_subdirectory(samples/cpp)
    add_subdirectory(samples/edges)
    add_subdirectory(samples/connector)
    add_subdirectory(samples/groups)
    add_subdirectory(samples/selection)
    add_subdirectory(samples/style)
    add_subdirectory(samples/dataflow)
    add_subdirectory(samples/topology)
endif()

add_subdirectory(exports)
