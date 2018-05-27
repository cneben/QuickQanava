QuickQanava 
============================

![](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/home.png)

[Overview](#QuickQanava) |
[Dependencies](#Dependencies) |
[Building](#building) |
[Roadmap](#Roadmap) |
[License](#license)

[![Build Status](https://travis-ci.org/cneben/QuickQanava.svg?branch=master)](https://travis-ci.org/cneben/QuickQanava)  (Linux/g++6/Qt5.10 - OSX/Clang/Qt5.9)

[![Build status](https://ci.appveyor.com/api/projects/status/ghpiaqqew63er8ea?svg=true)](https://ci.appveyor.com/project/cneben/quickqanava) (Windows/MSVC 2015 x64/Qt5.10/qmake)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://cneben.github.io/QuickQanava/) |
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) |
![](https://img.shields.io/badge/version-0.9.4-blue.svg) |
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)

**There is quite a lot of traffic and clones actually: Please remember to star the project and reports bugs....**

+ **CHANGELOG 20180527:** Fix major issues with grouping, see [CHANGELOG](https://github.com/cneben/QuickQanava/blob/master/CHANGELOG.md).
+ **CHANGELOG 20180429:** Major documentation update (see [Graph Data Model](http://cneben.github.io/QuickQanava/graph/index.html) and [Node Style](http://cneben.github.io/QuickQanava/styles/index.html#node-style).

`QuickQanava` is a C++14 library designed to display graphs and relational content in a Qt application. QuickQanava provide QML components and C++ classes to visualize medium-sized directed graphs in a C++/QML application. QuickQanava focus on displaying relational content into a dynamic user interface with DnD support, resizable content and visual creation of topology. More advanced layouts algorithms might be integrated in future versions.

QuickQanava main repository is hosted on GitHub: http://cneben.github.io/QuickQanava/index.html

QuickQanava is primarily developed with Qt 5.10 with MSVC2015U3 and g++5.4 (minimal required Qt version is **Qt 5.10**)

+ Project homepage: http://cneben.github.io/QuickQanava/topology/index.html

+ [Installation](http://cneben.github.io/QuickQanava/installation/index.html)
+ [Graph Data Model](http://cneben.github.io/QuickQanava/graph/index.html)
  + [Ports and Docks](http://cneben.github.io/QuickQanava/nodes/index.html#docks-and-ports)
  + [Visual node connection](http://cneben.github.io/QuickQanava/edges/index.html#visual-creation-of-edges)
  + [Selection](http://cneben.github.io/QuickQanava/nodes/index.html#selection)
  + [Groups](http://cneben.github.io/QuickQanava/nodes/index.html#grouping-nodes)
+ [Styling](http://cneben.github.io/QuickQanava/styles/index.html#node-style)
+ [Advanced use, custom topology](http://cneben.github.io/QuickQanava/advanced/index.html)


For any questions, please contact: benoit@destrat.io

QuickQanava focus on writing content delegates in QML (even if they could be 100% defined in C++, a QML engine is still necessary in the background), if you are looking for a pure C++/QGraphicsView solution, have a look to: [NodeEditor](https://github.com/paceholder/nodeeditor)

Why visual programming does matter: [Medium @ni55an](https://medium.com/@ni55an/visual-programming-and-d3ne-efcd49690703)

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

  - **v0.9.4:**	 
	- QuickContainers (Qt/QML observable adapter for STL or Qt containers):
		- [X] Redesign QuickContainers: qcm::ContainerModel<> memory footprint is too high (inheritance from QAbstractItemModel comes with a strong virtual and signals/slots overhead)
		- [X] Add support for standard library containers.
		- [X] Increase test coverage.
    - [X] Fix port remove issues.
    - [X] Add full CMake support..    
  - **v0.9.5:**	 
    - [ ] Add support for direct visual dragging of port items.
    - [ ] Add full support for groups inside group (ie subgraphs).
    - [ ] Fix current qan::PointGrid bugs and add "snap to grid" support.
  - **v0.9.6:**
    - [ ] Add more configuration options to qan::Edge (source and destination arrow configuration).
  - **v1.0.0: Advanced edge visualization**	 
    - GTpo (Configurable topology library):
      - [ ] Redesign using adjacency matrix, immutable and full move semantic support.
	  - [ ] Push test coverage to 100% (ie increase coverage for subgroups).- [80%] Redesign qan::Graph interface for creating content.
    - [ ] Publish the 4k sample (40k is probably too much for QML without dedicated culling and LOD code).

License
=======

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2017 BA

