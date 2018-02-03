
cmake_minimum_required(VERSION 3.1.0)

project(QuickQanava)

add_subdirectory(GTpo)
add_subdirectory(src)

option(ENABLE_SAMPLES "Build the samples" FALSE)
if (${ENABLE_SAMPLES})
    #add_subdirectory(samples/resizer)
    #add_subdirectory(samples/navigable)
    add_subdirectory(samples/nodes)
    #add_subdirectory(samples/edges)
    #add_subdirectory(samples/connector)
    #add_subdirectory(samples/groups)
    #add_subdirectory(samples/selection)
    #add_subdirectory(samples/style)
    #add_subdirectory(samples/dataflow)
    #add_subdirectory(samples/topology)
endif()
