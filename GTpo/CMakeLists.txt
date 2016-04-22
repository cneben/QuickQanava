
cmake_minimum_required(VERSION 2.8.11)	# Necessary for Qt
set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.6.0\\5.6\\msvc2015_64")

project (QanTopo CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (QanTopo_VERSION_MAJOR 0)
set (QanTopo_VERSION_MINOR 1)

#configure_file (
#  "${PROJECT_SOURCE_DIR}/QanTopo.Config.h.in"
#  "${PROJECT_BINARY_DIR}/QanTopo.Config.h"
#  )

set(QanTopo_Library_SRCS
  src/qtpoEdge.h
  src/qtpoEdge.hpp
  src/qtpoGraph.h
  src/qtpoGraph.hpp
  src/qtpoIDInterface.h
  src/qtpoIDInterface.cpp
  src/qtpoNode.h
  src/qtpoNode.hpp  
)

set( MY_SRCS
src/qtpoEdge.hpp
src/qtpoEdge.h
)

add_library(QanTopo STATIC ${QanTopo_Library_SRCS})
include_directories(
  ${QanTopo_SOURCE_DIR}/src
  ${QanTopo_SOURCE_DIR})
  
add_executable(sample-basic ${PROJECT_SOURCE_DIR}/samples/basic/basic.cpp)
target_link_libraries (sample-basic QanTopo)

FILE(GLOB_RECURSE INC_ALL "src/*.h")



