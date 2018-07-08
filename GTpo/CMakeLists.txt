set (gtpo_header_files
    src/adjacent_behaviour.h                                                              
    src/adjacent_behaviour.hpp                                                                            
    src/behaviourable.h                                                                                   
    src/behaviourable.hpp                                                                                 
    src/behaviour.h                                                                                       
    src/config.h                                                                                          
    src/container_adapter.h                                                                               
    src/edge.h                                                                                            
    src/edge.hpp                                                                                          
    src/graph_behaviour.h
    src/graph_behaviour.hpp
    src/graph.h
    src/graph.hpp
    src/graph_property.h
    src/group_behaviour.h
    src/group_behaviour.hpp
    src/group.h
    src/group.hpp
    src/GTpo.h
    src/node_behaviour.h
    src/node_behaviour.hpp
    src/node.h
    src/node.hpp
    src/qt_container_adapter.h
    src/utils.h
)

add_library(GTpo INTERFACE)

target_include_directories(GTpo
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
    )
    
target_compile_features(GTpo
    INTERFACE
        cxx_std_14
    )

install(FILES
    ${gtpo_header_files}
    DESTINATION include
    )
    
install(TARGETS GTpo
    EXPORT Targets
    INCLUDES DESTINATION include
    )
