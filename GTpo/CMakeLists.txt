
cmake_minimum_required(VERSION 3.1.0)

project(GTpo)

# Configure Qt
find_package(Qt5Core)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (gtpo_header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/adjacent_behaviour.h                                                              
    ${CMAKE_CURRENT_SOURCE_DIR}/src/adjacent_behaviour.hpp                                                                            
    ${CMAKE_CURRENT_SOURCE_DIR}/src/behaviourable.h                                                                                   
    ${CMAKE_CURRENT_SOURCE_DIR}/src/behaviourable.hpp                                                                                 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/behaviour.h                                                                                       
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h                                                                                          
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container_adapter.h                                                                               
    ${CMAKE_CURRENT_SOURCE_DIR}/src/edge.h                                                                                            
    ${CMAKE_CURRENT_SOURCE_DIR}/src/edge.hpp                                                                                          
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph_behaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph_behaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph_property.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/group_behaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/group_behaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/group.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GTpo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/node_behaviour.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/node_behaviour.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/node.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/node.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt_container_adapter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.h
)

add_library(GTpo INTERFACE)
target_include_directories(GTpo INTERFACE src)
target_sources(GTpo INTERFACE ${gtpo_header_files})
