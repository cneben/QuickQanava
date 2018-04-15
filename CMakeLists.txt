
cmake_minimum_required(VERSION 3.5.0)

project(QuickQanava VERSION 0.9.4 LANGUAGES CXX)

add_subdirectory(GTpo)
add_subdirectory(QuickContainers)
add_subdirectory(src)

option(BUILD_SAMPLES "Build the samples" TRUE)
option(DEPLOY "Use windeployqt on Windows" FALSE)

if (${BUILD_SAMPLES})
    #add_subdirectory(samples/resizer)
    add_subdirectory(samples/navigable)
    add_subdirectory(samples/nodes)
    add_subdirectory(samples/edges)
    add_subdirectory(samples/connector)
    add_subdirectory(samples/groups)
    add_subdirectory(samples/selection)
    add_subdirectory(samples/style)
    add_subdirectory(samples/dataflow)
    add_subdirectory(samples/topology)
endif()
