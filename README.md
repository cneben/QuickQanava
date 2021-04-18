# QuickQanava 

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
![](https://img.shields.io/badge/version-0.20.0-blue.svg) |
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)

**There is quite a lot of traffic and clones actually: Please remember to star the project and reports bugs....**

+ [CHANGELOG.md](CHANGELOG.md)

`QuickQanava` is a C++14 library designed to display graphs and relational content in a Qt/QML application. QuickQanava provide QML components and C++ classes to visualize medium-sized directed graphs in a C++/QML application. QuickQanava focus on displaying relational content into a dynamic user interface with DnD support, resizable content and visual creation of topology. More advanced layouts algorithms might be integrated in future versions.

QuickQanava main repository is hosted on GitHub: http://cneben.github.io/QuickQanava/index.html

QuickQanava is primarily developed for Linux with Qt 5.15 with MSVC2017 and g++7.5 (minimal required Qt version is **Qt 5.10**, recommended is **Qt 5.15.2**)

+ Project homepage: http://cneben.github.io/QuickQanava/index.html

+ [Building](./doc/BUILDING.md)

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


## Contributors

![GitHub Contributors Image](https://contrib.rocks/image?repo=cneben/QuickQanava)

 - @kwallner / Conan.io and CMake configuration
 - @Letrab / Edge configuration
 - @machinekoder / QML plugin and CMake configuration
 - @NielsBillen / Ports support and stability fixes
 - @walterbrebels
 - @chesnoksatan / Major connector feature, build configuration

## License

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2017-2020 BA

