---
title: QuickQanava
description: "C+11/Qt/QML graph visualization library"
type: index
weight: 0
---
![home](images/home.png)

[![Build Status](https://travis-ci.org/cneben/QuickQanava.svg?branch=master)](https://travis-ci.org/cneben/QuickQanava)  (Linux/g++6/Qt5.12.1 - OSX/Clang/Qt5.12.1)

[![Build status](https://ci.appveyor.com/api/projects/status/ghpiaqqew63er8ea?svg=true)](https://ci.appveyor.com/project/cneben/quickqanava) (Windows MSVC 2015 x64/Qt5.10.1)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://www.destrat.io/quickqanava/doc)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)


!!! warning "QuickQanava is alpha, interface may change before 1.0.0 release, but QuickQanava is already used extensively in production code."

!!! note "QuickQanava is licensed under BSD-3, specific features or support is available on demand: benoit@destrat.io"

## Introduction

`QuickQanava` is a C++14 library designed to display graphs and relational content in a QtQuick application. QuickQanava provide QML components and C++ classes to visualize medium-sized directed graphs in a C++/QML application. QuickQanava does not provide advanced layout algorithms, but rather focus on displaying relational content in advanced dynamic user interfaces (with DnD support, resizable content, visual connection of nodes).

QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt >= 5.13 with MSVC2015 and g++7. minimal required Qt version is **Qt 5.10**.

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


