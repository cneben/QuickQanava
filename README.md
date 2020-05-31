QuickQanava 
============================

![](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/home.png)

[Overview](#QuickQanava) |
[Dependencies](#Dependencies) |
[Building](#building) |
[Roadmap](#Roadmap) |
[License](#license)

[![Build Status](https://travis-ci.org/cneben/QuickQanava.svg?branch=master)](https://travis-ci.org/cneben/QuickQanava)  (Linux / g++6 / Qt5.12.1 - OSX / Clang 8.1 / Qt 5.12.1 CMake)

[![Build status](https://ci.appveyor.com/api/projects/status/ghpiaqqew63er8ea?svg=true)](https://ci.appveyor.com/project/cneben/quickqanava) (Windows / MSVC 2015 x64 / Qt5.10.1 / qmake)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://cneben.github.io/QuickQanava/) |
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) |
![](https://img.shields.io/badge/version-0.15.0-blue.svg) |
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)

**There is quite a lot of traffic and clones actually: Please remember to star the project and reports bugs....**

+ **CHANGELOG 20191110:** Complete rewrite of LineGrid, various cosmetic fixes and bugfixes (update to v0.15.0).
+ **CHANGELOG 20190825:** Maintenance release, add more flexible content creation methods in qan::Graph.
+ [CHANGELOG.md](CHANGELOG.md)


`QuickQanava` is a C++14 library designed to display graphs and relational content in a Qt/QML application. QuickQanava provide QML components and C++ classes to visualize medium-sized directed graphs in a C++/QML application. QuickQanava focus on displaying relational content into a dynamic user interface with DnD support, resizable content and visual creation of topology. More advanced layouts algorithms might be integrated in future versions.

QuickQanava main repository is hosted on GitHub: http://cneben.github.io/QuickQanava/index.html

QuickQanava is primarily developed for Linux with Qt 5.15 with MSVC2017 and g++7.5 (minimal required Qt version is **Qt 5.10**, recommended is **Qt 5.14.2**)

+ Project homepage: http://cneben.github.io/QuickQanava/index.html

+ [Installation](http://cneben.github.io/QuickQanava/installation.html)
+ [Graph Data Model](http://cneben.github.io/QuickQanava/graph.html#data-model)
  + [Ports and Docks](http://cneben.github.io/QuickQanava/nodes.html#docks-and-ports)
  + [Visual node connection](http://cneben.github.io/QuickQanava/edges.html#visual-creation-of-edges)
  + [Selection](http://cneben.github.io/QuickQanava/nodes.html#selection)
  + [Groups](http://cneben.github.io/QuickQanava/nodes.html#grouping-nodes)
+ [Styling](http://cneben.github.io/QuickQanava/styles.html)
+ [Advanced use, custom topology](http://cneben.github.io/QuickQanava/advanced.html)


For any questions, please contact: benoit@destrat.io

QuickQanava focus on writing content delegates in QML (even if they could be 100% defined in C++, a QML engine is still necessary in the background).

## QuickQanava Showcase

[![Toplogy sample video](https://img.youtube.com/vi/bUTO_PeegP4/0.jpg)](https://www.youtube.com/watch?v=bUTO_PeegP4)

| Edges       | 
| :---:       |
| ![Curved-Straight Edges](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/edges-curved-straight.gif) |

| Nodes       | 
| :---:       |
| ![Custom content](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-nodes.gif) |

  - User Doc:  [QuickStart - Custom Nodes](http://cneben.github.io/QuickQanava/topology/index.html#displaying-custom-nodes)

| Visual Connector       |   
| :---:                  | 
![Visual Connector](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-dataflow-short.gif) |

  - User Doc:  [QuickStart - Visual Connector](http://cneben.github.io/QuickQanava/topology/index.html#visual-connection-of-nodes)
  - Reference documentation: qan::Connector interface and Qan.VisualConnector component

| Groups       | 
| :---:        | 
| ![Groups](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/nodes/sample-groups.gif) |

  - User Doc:  [QuickStart - Using Groups](http://cneben.github.io/QuickQanava/topology/index.html#using-groups)

| Styles       |
| :---:        | 
| ![Custom styles](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-styles.gif) |

  - User Doc:  [QuickStart - Using Styles](http://cneben.github.io/QuickQanava/styles/index.html)

## Dependencies

- **Qt 5.10** _is mandatory_ for Qt Quick Shapes support.
- **Google Test** is a *GTpo* dependency, it is optional for QuickQanava until you intent to use a graph with custom non-STL/non-Qt containers: ![Google Test GitHub](https://github.com/google/googletest)

## Building

Get the latest QuickQanava sources:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
```

Or install as a Git submodule:

```sh
$ git submodule add https://github.com/cneben/QuickQanava
$ git submodule update
```

QuickQanava could be used with either _qmake_ or _CMake_ build configuration system.

| qmake                | cmake             | 
| :---:                | :---:             | 
| Static build, no QML module, all resources are linked statically trough QRC | Installable or embedable, QuickQanava is loaded using a QML module that need to be installed, resources can be linked statically trough QRC | 


Using qmake (**preferred and supported way of integrating QuickQanava**):

1. Open _quickqanava.pro_ in QtCreator.

2. Select a kit, build and launch samples.


:warning: CMake support is "community maintained"

or with (CMake >= 3.5) and Qt Creator:

1. Open _CMakeLists.txt_ in QtCreator.

2. In 'Projects' panel, set DBUILD_SAMPLES option to true in CMake configuration panel.

3. Select a kit, build and launch samples.

Or manually in command line using CMake:

```sh
$ cd QuickQanava
$ mkdir build
$ cd build

# IF QT_DIR IS CONFIGURED AND QMAKE IN PATH
$ cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SAMPLES=TRUE -DBUILD_STATIC_QRC=TRUE ..

# IF QT DIR IS NOT CONFIGURED, CONFIGURE KIT MANUALLY
$ cmake -DCMAKE_PREFIX_PATH="/home/b/Qt/5.11.0/gcc_64" -DQT_QMAKE_EXECUTABLE="/home/b/Qt/5.11.0/gcc_64/bin/qmake"  -DBUILD_SAMPLES=TRUE -DBUILD_STATIC_QRC=TRUE ../QuickQanava/

$ cmake --build .
# Then run the samples in ./samples

# Eventually make install
```

Detailed instructions:  [Installation](http://cneben.github.io/QuickQanava/installation/index.html)

Note that a previously installed "QML plugin" version of QuickQanava might interfere with a fully static build using direct .pri inclusion. Typical error message looks like:

```
QQmlApplicationEngine failed to load component
qrc:/nodes.qml:33 module "QuickQanava" plugin "quickqanavaplugin" not found
```

QuickQanava and QuickContainers plugins directories could be removed manually from `$QTDIR\..\qml` to fix the problem (ex: rm -rf '~/Qt/5.11.1/gcc_64/qml/QuickQanava').

## Roadmap / Changelog

  - **v0.15.x:**
    - [X] Add full support for groups inside group (ie subgraphs).
    - [ ] Update geometry creation interface and delegate management.
  - **v0.16.x:**
    - [ ] Add support for direct visual dragging of port items.
    - [X] Fix current qan::LineGrid bugs
    - [ ] Add "snap to grid" support.
  - **v0.17.x:**
    - [ ] Rewrite CMake configuration, add install step, use QML plugins.
  - **v1.: Advanced edge visualization**
    - [ ] Add better support for graph fine grained locking strategies.
    - [ ] Add simple layout algorithms (force directed, tree).
    - [ ] Publish the 4k sample (40k is probably too much for QML without dedicated culling and LOD code).

Contributors
=======

 - @kwallner / Conan.io and CMake configuration
 - @Letrab / Edge configuration
 - @machinekoder / QML plugin and CMake configuration
 - @NielsBillen / Ports support and stability fixes

License
=======

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2017-2019 BA

