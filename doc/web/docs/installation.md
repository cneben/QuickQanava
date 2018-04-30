QuickQanava Quick Start 
============================

Dependencies
------------------

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes** (GIT submodule)   |   ![](https://img.shields.io/badge/license-MIT-blue.svg) MIT|
| Google Test/Mock          | No                |       No                        |    Permissive   |

- **Google Test** is a *GTpo* dependency, it is optional for QuickQanava until your intention is to use a graph with custom non-STL/non-Qt containers: ![Google Test GitHub](https://github.com/google/googletest)

Building from sources
------------------

Get the latest QuickQanava sources:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
```

QuickQanava could be used with either _qmake_ or _CMake_ build configuration system.

1. Open _quickqanava.pro_ in QtCreator.

2. Select a kit, build and launch samples.

or (CMake > 3.5)

1. Open _CMakeLists.txt_ in QtCreator.

2. In 'Projects' panel, set DBUILD_SAMPLES option to true in CMake configuration panel.

3. Select a kit, build and launch samples.

Or manually using CMAke:

```sh
$ git submodule add https://github.com/cneben/QuickQanava
$ git submodule update
$ cd QuickQanava
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SAMPLES=TRUE ..
$ cmake --build .
# Then run the samples
```

Using from external projects
------------------

The recommended way of using QuickQanava is to include the library directly as a GIT submodule in your project:

```sh
# Install QuickQanava as a GIT submodule
$ git submodule add https://github.com/cneben/QuickQanava
* git submodule update
```

Once GIT has finished downloading, QuickQanava project file could be included directly in your main qmake .pro file with the following two #include statements:

```sh
#in your project main .pro qmake configuration file
include($$PWD/QuickQanava/src/quickqanava.pri)
```
