# QuickQanava Quick Start 

!!! note "Starting from version 2.4.0, QuickQanava is now a fully static library. It is recommended to install it from a Git submodule using CMake."

  - `CMake`: For Qt6, can be used trough `add_subdirectory()` or [ExternalProject_Add()](https://cmake.org/cmake/help/latest/module/ExternalProject.html)


## Using from external projects with CMake

The recommended way of using QuickQanava is to include the library directly as a GIT submodule in your project:

```sh
# Install QuickQanava as a GIT submodule
$ git submodule add https://github.com/cneben/QuickQanava
* git submodule update
```

From your `CMakeLists.txt`:
```sh
add_subdirectory(PATH_TO_QUICKQANAVA_SUBMODULE)

target_include_directories(my_project PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::QuickControls2)

target_link_libraries(sample_advanced
    QuickQanava
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickControls2
)
```

QuickQanava might also be included in CMake trough [ExternalProject_Add()](https://cmake.org/cmake/help/latest/module/ExternalProject.html) directly from GitHub.



## Building samples qmake / CMake (development)

**Using CMake (CMake > 3.16):**

1. Open _CMakeLists.txt_ in QtCreator.

2. for testing purposes, in 'Projects' panel, set `QUICK_QANAVA_BUILD_SAMPLES` option to `ON` in CMake configuration panel.

3. Select a kit, build and launch samples.


Or build manually using CMake (CI):

```sh
$ cd QuickQanava
$ mkdir build
$ cd build

# IF QT_DIR IS CONFIGURED AND QMAKE IN PATH
$ cmake -DCMAKE_BUILD_TYPE=Release -DQUICK_QANAVA_BUILD_SAMPLES=OFF ..

# IF QT DIR IS NOT CONFIGURED, CONFIGURE KIT MANUALLY
$ cmake -DCMAKE_PREFIX_PATH="/path/to/Qt/5.15.14/gcc_64" -DQT_QMAKE_EXECUTABLE="/path/to/Qt/6.6.1/gcc_64/bin/qmake"  -DQUICK_QANAVA_BUILD_SAMPLES=OFF ../QuickQanava/

$ cmake --build .
# Then run the samples in ./samples

# There is no make install
```

## Dependencies

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| Google Test/Mock          | No                |       No                        |    Permissive   |

- **Google Test** is optional only to build and run tests ![Google Test GitHub](https://github.com/google/googletest).

