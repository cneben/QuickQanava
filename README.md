[Overview](#QuickQanava) |
[Dependencies](#Dependencies) |
[Building](#building) |
[Roadmap](#Roadmap) |
[License](#license)

[![Build Status](https://travis-ci.org/cneben/QuickQanava.svg?branch=master)](https://travis-ci.org/cneben/QuickQanava)  (Linux/g++5/Qt5.8 - OSX/Clang/Qt5.9) |
[![Build status](https://ci.appveyor.com/api/projects/status/ghpiaqqew63er8ea?svg=true)](https://ci.appveyor.com/project/cneben/quickqanava) (Windows MSVC 2015 x64/Qt5.9) |
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://www.destrat.io/quickqanava/doc) |
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

QuickQanava 
============================

![](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/home.png)

`QuickQanava` is a C++14 library designed to display graphs an relational content in a QtQuick application. QuickQanava provide components and classes to visualize medium-sized directed graphs in a QML application. QuickQanava does not provide advanced layout algorithms, but rather focus on displaying relational content into a dynamic user interface (with DnD support, resizable content, visual creation of topology).

`QuickQanava` is released under BSD-3, and is shipped with *[GTpo](https://github.com/cneben/GTpo)* library (BSD-3 licence), *[QuickContainers](https://github.com/cneben/QuickQanava/tree/master/QuickContainers)* (BSD-3) and *[QuickGeoGL](https://github.com/cneben/QuickQanava/tree/master/QuickGeoGL)*.

QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt 5.10 with MSVC2015U3 and g++5.4 (minimal required Qt version is 5.7, some samples requires Qt5.8)

+ Project homepage: http://www.destrat.io/quickqanava (**updated 20170323**)
+ Reference documentation: http://www.destrat.io/quickqanava/doc/index.html (**updated 20170323**)

For any questions, please contact: benoit@destrat.io

QuickQanava focus on writting content delegates in QML (even if they could be 100% defined in C++, a QML engine is still necessary in the background), if you are looking for a pure C++/QGraphicsView solution, have a look to: [NodeEditor](https://github.com/paceholder/nodeeditor)

## Dependencies:

- Qt5.10 for full curved edge support (Qt 5.8 will works, but rendering will be done using QuickGeoGL).
- **Google Test** is a *GTpo* dependency, it is optional for QuickQanava until you intent to use a graph with custom non-STL/non-Qt containers: ![Google Test GitHub](https://github.com/google/googletest)

## Building:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
```
2. Open quickqanava.pro in QtCreator
  4. Building 'tests' (_optional_): Tests will not compile without a working Google Test installation: for Windows, modify win32-msvc*:GMOCK_DIR and win32-msvc*:GTEST_DIR in `tests.pro` with path containing a valid build directory. On Linux, no configuration is required, just install the '-dev' package for theses libraries.

3. Select a kit, build and launch samples.

## QuickQanava Samples:

| Nodes       | 
| :---:       |
| ![Custom content](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-nodes.gif) |

  - User Doc:  [QuickStart - Custom Nodes](http://www.destrat.io/quickqanava/gettingstarted/index.html#displaying-custom-nodes)

| Visual Connector       |   
| :---:                  | 
![Visual Connector](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-connector.gif) |

  - User Doc:  [QuickStart - Visual Connector](http://www.destrat.io/quickqanava/gettingstarted/index.html#topology)
  - Reference documentation: [qan::Connector interface](http://www.destrat.io/quickqanava/doc/classqan_1_1_connector.html) and [Qan.VisualConnector component](http://www.destrat.io/quickqanava/doc/class_visual_connector.html)

| Groups       | 
| :---:        | 
| ![Groups](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-groups.gif) |

  - User Doc:  [QuickStart - Using Groups](http://www.destrat.io/quickqanava/gettingstarted/index.html#using-groups)

| Styles       |
| :---:        | 
| ![Custom styles](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-styles.gif) |

  - User Doc:  [QuickStart - Using Styles](http://www.destrat.io/quickqanava/gettingstarted/index.html#defining-styles)

  
## Roadmap:

  - **v0.9.1:**:	 
    - [ ] Add better heuristic for Bezier curve edge control points generation.
    - [ ] Add full support for groups inside group (ie subgraphs).
    - [ ] Remove QuickGeoGL support, QuickQanava will become Qt 5.10 only.
	- QuickContainers (Qt/QML observable adapter for STL or Qt containers):
		- [ ] Redesign QuickContainers: qcm::ContainerModel<> memory footprint is too high (inheritance from QAbstractItemModel comes with a strong virtual and signals/slots overhead)
		- [ ] Add support for standard library containers.
		- [ ] Increase test coverage.
  - **v1.0.0: Advanced edge visualization**	 
    - GTpo (Configurable topology library):
	  - [ ] Push test coverage to 100% (ie increase coverage for subgroups).- [80%] Redesign qan::Graph interface for creating content.
    - [ ] Publish the 4k sample (40k is probably too much for QML without dedicated culling and LOD code).
    - [ ] Fix current qan::PointGrid bugs and add "snap to grid" support.

License
=======

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2017 Delia Stratégie

