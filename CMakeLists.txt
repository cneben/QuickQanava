cmake_minimum_required(VERSION 3.5.0)

project(QuickQanava VERSION 2.4.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(QUICKQANAVA_QT_MAJOR_VERSION "Force Qt Versions to 5 or 6 (default to 6)" 6)
option(QUICKQANAVA_BUILD_STATIC_QRC "Build *.qrc resources statically" TRUE)
option(QUICKQANAVA_WINDOWS_DEPLOY "Use windeployqt on Windows" FALSE)

if (${QUICKQANAVA_QT_MAJOR_VERSION} EQUAL 5)
    include(CMakeLists.qt5.txt)
else()
    include(CMakeLists.qt6.txt)
endif()

add_subdirectory(src)
#add_subdirectory(exports)
