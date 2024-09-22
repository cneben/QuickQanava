cmake_minimum_required(VERSION 3.16)

project(QuickQanava VERSION 2.5.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Quick QuickControls2)

message("Building QuickQanava for Qt${QT_VERSION_MAJOR}")

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

add_subdirectory(src)

option(QUICK_QANAVA_BUILD_SAMPLES "Build QuickQanava samples" OFF)
if (${QUICK_QANAVA_CI})
    add_subdirectory(samples/groups)    # Used to test CI
endif()

if (${QUICK_QANAVA_BUILD_SAMPLES})
    add_subdirectory(samples/advanced)
    add_subdirectory(samples/connector)
    add_subdirectory(samples/cpp)

    add_subdirectory(samples/dataflow)
    add_subdirectory(samples/edges)
    add_subdirectory(samples/navigable)

    add_subdirectory(samples/groups)
    add_subdirectory(samples/nodes)
    add_subdirectory(samples/layouts)
    add_subdirectory(samples/selection)

    add_subdirectory(samples/style)
    add_subdirectory(samples/topology)
endif()
