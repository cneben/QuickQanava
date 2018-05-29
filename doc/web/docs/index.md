---
title: QuickQanava
description: "C+11/Qt/QML graph visualization library"
type: index
weight: 0
---
![home](images/home.png)

[![Build Status](https://travis-ci.org/cneben/QuickQanava.svg?branch=master)](https://travis-ci.org/cneben/QuickQanava)  (Linux/g++5/Qt5.8 - OSX/Clang/Qt5.9)

[![Build status](https://ci.appveyor.com/api/projects/status/ghpiaqqew63er8ea?svg=true)](https://ci.appveyor.com/project/cneben/quickqanava) (Windows MSVC 2015 x64/Qt5.9)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://www.destrat.io/quickqanava/doc)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)


!!! warning "QuickQanava is alpha, interface may change before 1.0.0 release, but QuickQanava is already used extensively in production code."

!!! note "QuickQanava is licensed under BSD-3, specific features or support is available on demand: benoit@destrat.io"

## Introduction

`QuickQanava` is a C++14 library designed to display graphs and relational content in a QtQuick application. QuickQanava provide QML components and C++ classes to visualize medium-sized directed graphs in a C++/QML application. QuickQanava does not provide advanced layout algorithms, but rather focus on displaying relational content in advanced dynamic user interfaces (with DnD support, resizable content, visual connection of nodes).

QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt >= 5.10 with MSVC2015 and g++7. minimal required Qt version is **Qt 5.10**.

+ Project homepage: [http://cneben.github.io/QuickQanava/index.html](http://cneben.github.io/QuickQanava/index.html)

Please refer to [Installation](installation.md) manual and [Graph](graph.md), [Nodes](nodes.md) and [Graph](edges.md) sections for more information about installing and using QuickQanava.

## Gallery

[QuickQanava samples](samples.md)

| Node Grouping                | Visual Connection of Nodes         | 
| :---:                       | :---:             | 
| ![groups](nodes/sample-groups.gif) | ![connector](images/sample-nodes.gif) | 
| ![connector](images/sample-dataflow-short.gif) | ![styles](images/sample-styles.gif) | 

![styles](samples/topology.png)

## Roadmap

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
        - [ ] Push test coverage to 100% (ie increase coverage for subgroups).- [80%] Redesign qan::Graph interface for creating content.
    - [ ] Publish the 4k sample (40k is probably too much for QML without dedicated culling and LOD code).


