
cmake_minimum_required(VERSION 3.1.0)

project(GTpo)

# Configure Qt
find_package(Qt5Core)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (gtpo_header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoUtils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGraphConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenEdge.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenEdge.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenNode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenGroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenGroup.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenGraph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGenGraph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoBehaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoBehaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGraphBehaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGraphBehaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoNodeBehaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoNodeBehaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGroupBehaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoGroupBehaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoAdjacentBehaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoAdjacentBehaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gtpoContainerAdapter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GTpo.h
)

add_library(GTpo INTERFACE)
target_include_directories(GTpo INTERFACE src)
target_sources(GTpo INTERFACE ${gtpo_header_files})
