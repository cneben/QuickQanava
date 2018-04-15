
cmake_minimum_required(VERSION 3.1.0)

# Configure Qt
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml)
find_package(Qt5QuickControls2 REQUIRED)
set(CMAKE_AUTOMOC ON)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( source_files
    navigable.cpp
    ../../src/qanNavigable.cpp
    ../../src/qanNavigablePreview.cpp
    ../../src/qanGrid.cpp
)

set (header_files )

set(CMAKE_AUTORCC ON)
#qt5_wrap_cpp(source_files, header_files)
qt5_add_resources(RESOURCES navigable.qrc)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories( ${Qt5Quick_INCLUDE_DIRS} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} "../../src" )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sample_navigable ${source_files} ${RESOURCES})
target_include_directories(sample_navigable PUBLIC Qt5::QuickControls2)
target_link_libraries(sample_navigable Qt5::Core Qt5::Gui Qt5::QuickControls2)

if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
    add_custom_command(TARGET sample_navigable POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_CURRENT_SOURCE_DIR} $<TARGET_FILE:sample_navigable> $<$<CONFIG:Debug>:--pdb>)
    add_custom_command(TARGET sample_navigable POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:QuickQanava> $<TARGET_FILE_DIR:sample_navigable>)
endif()



