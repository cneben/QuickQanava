# QuickQanava 

![Win64 Qt6 msvc CMake](https://github.com/cneben/QuickQanava/actions/workflows/build-win64-qt6.yml/badge.svg) 

[![Documentation](https://img.shields.io/badge/docs-mkdocs-blue.svg)](http://cneben.github.io/QuickQanava/)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
![](https://img.shields.io/badge/version-2.5.0-blue.svg)
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)

![](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/home.png)

`QuickQanava` is a C++17 library developed for rendering graphs and relational content within a Qt/QML application. It offers QML components and C++ classes designed for visualizing medium-sized directed graphs in a C++/QML application. QuickQanava emphasizes the presentation of relational content in a dynamic user interface, featuring Drag-and-Drop support, resizable content, and visual creation of topology. `QuickQanava` focus authoring of topology delegates in pure QML.

QuickQanava main repository is hosted on GitHub: http://cneben.github.io/QuickQanava/index.html

QuickQanava is primarily developed with Qt 6.6.1 with MSVC2019 and g++11 (minimal required Qt version is **Qt 6.5.0**, recommended is **Qt 6.6.1**). 

If you are interested in **Qt5.15** support, last release supporting Qt5 is [2.4.0](https://github.com/cneben/QuickQanava/releases/tag/2.4.0)

- [CHANGELOG.md](CHANGELOG.md)
- Project homepage: http://cneben.github.io/QuickQanava/index.html
- [Building](./doc/BUILDING.md)
-  [Installation](http://cneben.github.io/QuickQanava/installation.html)
-  [Graph Data Model](http://cneben.github.io/QuickQanava/graph.html#data-model)
    - [Ports and Docks](http://cneben.github.io/QuickQanava/nodes.html#docks-and-ports)
    - [Visual node connection](http://cneben.github.io/QuickQanava/edges.html#visual-creation-of-edges)
    -  [Selection](http://cneben.github.io/QuickQanava/nodes.html#selection)
    - [Groups](http://cneben.github.io/QuickQanava/nodes.html#grouping-nodes)
- [Styling](http://cneben.github.io/QuickQanava/styles.html)
- [Advanced use, custom topology](http://cneben.github.io/QuickQanava/advanced.html)

:question: benoit_à_destrat.io

## QuickQanava Showcase

**Edges:**

![Curved-Straight Edges](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/edges-curved-straight.gif) 

**Nodes:** User Doc:  [QuickStart - Custom Nodes](http://cneben.github.io/QuickQanava/nodes.html#defining-custom-nodes)

![Custom content](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-nodes.gif)

**Visual Connector:** User Doc:  [QuickStart - Visual Connector](http://cneben.github.io/QuickQanava/edges.html#visual-connectors)

![Visual Connector](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/sample-dataflow-short.gif)

**Groups:** User Doc:  [QuickStart - Using Groups](http://cneben.github.io/QuickQanava/nodes.html#grouping-nodes)

![Groups](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/nodes/sample-groups.gif)

**Styles:** User Doc:  [QuickStart - Using Styles](http://cneben.github.io/QuickQanava/styles.html)

## Contributors

![GitHub Contributors Image](https://contrib.rocks/image?repo=cneben/QuickQanava)

 - @kwallner / Conan.io and CMake configuration
 - @Letrab / Edge configuration
 - @machinekoder / QML plugin and CMake configuration
 - @NielsBillen / Ports support and stability fixes
 - @walterbrebels
 - @chesnoksatan / Major connector feature, build configuration

QuickQanava include `bezier` library distributed with MIT licence on https://github.com/oysteinmyrmo/bezier

## License

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2017-2024 BA

