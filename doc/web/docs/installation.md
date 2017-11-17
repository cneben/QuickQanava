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

QuickQanava use _qmake_ as its main build configuration system.

1. Open _quickqanava.pro_ in QtCreator

2. Select a kit, build and launch samples.

Using from external projects
------------------

The recommended way of using QuickQanava is to include the library directly as a GIT submodule in your project:

```sh
# Install QuickQanava as a GIT submodule
$ git submodule add https://github.com/cneben/QuickQanava
& git submodule update
```

Once GIT has finished downloading, QuickQanava and its dependencies ([GTpo](https://github.com/cneben/GTpo)) projects files could be included directly in your main qmake .pro file with the following two #include statements:

```sh
#in your project main .pro qmake configuration file
include($$PWD/QuickQanava/src/quickqanava.pri)
```
