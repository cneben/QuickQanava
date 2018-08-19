QuickQanava 
============================

![](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/home.png)

[Overview](#QuickQanava) |
[Dependencies](#Dependencies) |
[Building](#building) |
[Roadmap](#Roadmap) |
[License](#license)

[![Build Status](https://travis-ci.org/cneben/QuickQanava.svg?branch=master)](https://travis-ci.org/cneben/QuickQanava)  (Linux / g++6 / Qt5.10.1 - OSX / Clang 8.1 / Qt 5.11.1 CMake)

[![Build status](https://ci.appveyor.com/api/projects/status/ghpiaqqew63er8ea?svg=true)](https://ci.appveyor.com/project/cneben/quickqanava) (Windows / MSVC 2015 x64 / Qt5.10.1 / qmake)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://cneben.github.io/QuickQanava/) |
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) |
![](https://img.shields.io/badge/version-0.10.0-blue.svg) |
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)

**There is quite a lot of traffic and clones actually: Please remember to star the project and reports bugs....**

+ **CHANGELOG 20180704:** Complete refactoring of QuickQanava internal topology "backend" [GTpo](https://github.com/cneben/QuickQanava/tree/master/GTpo).
+ **CHANGELOG 20180602:** Major documentation update (see [Custom Groups](http://cneben.github.io/QuickQanava/nodes/index.html#custom-groups) and [Graph View](http://cneben.github.io/QuickQanava/graph/index.html#graph-view) and [Using from C++](http://cneben.github.io/QuickQanava/advanced/index.html#using-from-c).

`QuickQanava` is a C++14 library designed to display graphs and relational content in a Qt application. QuickQanava provide QML components and C++ classes to visualize medium-sized directed graphs in a C++/QML application. QuickQanava focus on displaying relational content into a dynamic user interface with DnD support, resizable content and visual creation of topology. More advanced layouts algorithms might be integrated in future versions.

QuickQanava main repository is hosted on GitHub: http://cneben.github.io/QuickQanava/index.html

QuickQanava is primarily developed with Qt 5.10 with MSVC2015U3 and g++7 (minimal required Qt version is **Qt 5.10**)

+ Project homepage: http://cneben.github.io/QuickQanava/index.html

+ [Installation](http://cneben.github.io/QuickQanava/installation/index.html)
+ [Graph Data Model](http://cneben.github.io/QuickQanava/graph/index.html)
  + [Ports and Docks](http://cneben.github.io/QuickQanava/nodes/index.html#docks-and-ports)
  + [Visual node connection](http://cneben.github.io/QuickQanava/edges/index.html#visual-creation-of-edges)
  + [Selection](http://cneben.github.io/QuickQanava/nodes/index.html#selection)
  + [Groups](http://cneben.github.io/QuickQanava/nodes/index.html#grouping-nodes)
+ [Styling](http://cneben.github.io/QuickQanava/styles/index.html#node-style)
+ [Advanced use, custom topology](http://cneben.github.io/QuickQanava/advanced/index.html)


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

QuickQanava could be used with either _qmake_ or _CMake_ build configuration system.

1. Open _quickqanava.pro_ in QtCreator.

2. Select a kit, build and launch samples.

or (CMake > 3.5)

1. Open _CMakeLists.txt_ in QtCreator.

2. In 'Projects' panel, set DBUILD_SAMPLES option to true in CMake configuration panel.

3. Select a kit, build and launch samples.

Or manually using CMake:

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

## Roadmap / Changelog

  - **v0.10.0:**	 
    - [X] Redesign GTpo (add complete support for static/dynamic behaviours).
    - [X] Push GTpo test coverage to 100% (ie increase coverage for subgroups).
  - **v0.11.x:**	 
    - [ ] Add support for direct visual dragging of port items.
    - [ ] Add full support for groups inside group (ie subgraphs).
    - [ ] Fix current qan::PointGrid bugs and add "snap to grid" support.
    - [ ] Add more configuration options to qan::Edge (source and destination arrow configuration).
  - **v1.: Advanced edge visualization**
    - [ ] Add better support for graph fine grained locking strategies.
    - [ ] Add simple layout algorithms (force directed, tree).
    - [ ] Publish the 4k sample (40k is probably too much for QML without dedicated culling and LOD code).

License
=======

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2017 BA

