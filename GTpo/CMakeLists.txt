cmake_minimum_required(VERSION 3.1.0)
project(GTpo)

set (gtpo_header_files
    src/gtpo/adjacent_behaviour.h
    src/gtpo/adjacent_behaviour.hpp
    src/gtpo/behaviourable.h
    src/gtpo/behaviourable.hpp
    src/gtpo/behaviour.h
    src/gtpo/config.h
    src/gtpo/container_adapter.h
    src/gtpo/edge.h
    src/gtpo/edge.hpp
    src/gtpo/graph_behaviour.h
    src/gtpo/graph_behaviour.hpp
    src/gtpo/graph.h
    src/gtpo/graph.hpp
    src/gtpo/graph_property.h
    src/gtpo/group_behaviour.h
    src/gtpo/group_behaviour.hpp
    src/gtpo/group.h
    src/gtpo/group.hpp
    src/gtpo/GTpo
    src/gtpo/GTpo.h
    src/gtpo/node_behaviour.h
    src/gtpo/node_behaviour.hpp
    src/gtpo/node.h
    src/gtpo/node.hpp
    src/gtpo/algorithm.h
    src/gtpo/algorithm.hpp
    src/gtpo/functional.h
    src/gtpo/qt_container_adapter.h
    src/gtpo/utils.h
)

add_library(GTpo INTERFACE)

target_include_directories(GTpo
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/gtpo>
    )

# Require C++14
target_compile_features(GTpo
    INTERFACE
        cxx_std_14
    )

install(FILES
    ${gtpo_header_files}
    DESTINATION include/gtpo
    )
    
install(TARGETS GTpo
    EXPORT Targets
    INCLUDES DESTINATION include/gtpo
    )
