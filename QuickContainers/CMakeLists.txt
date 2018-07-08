set(CMAKE_AUTOMOC ON)

set (qcm_source_files
    src/qcmAbstractContainer.h
    src/qcmContainerModel.h
)

set (qcm_header_files
    src/qcmAbstractContainer.h
    src/qcmAdapter.h
    src/qcmContainer.h
    src/qcmContainerModel.h
    src/QuickContainers.h
    src/QuickContainers
)

add_library(QuickContainers STATIC ${qcm_header_files} ${qcm_source_files})
set_target_properties(QuickContainers PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(QuickContainers Qt5::Core Qt5::Qml)

target_include_directories(QuickContainers
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
    )

target_compile_features(QuickContainers
    PUBLIC
        cxx_std_14
    )

install(FILES
    ${qcm_header_files}
    DESTINATION include
    )
    
install(TARGETS QuickContainers
    EXPORT Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )
