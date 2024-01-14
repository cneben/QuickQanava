---
title: QuickQanava
description: "C+11/Qt/QML graph visualization library"
type: index
weight: 0
---
![home](images/home.png)


![Linux Qt5 g++ qmake](https://github.com/cneben/QuickQanava/actions/workflows/build-linux-qt5.yml/badge.svg)  (Linux, g++, Qt5.15, qmake)

![Win64 Qt5 msvc qmake](https://github.com/cneben/QuickQanava/actions/workflows/build-win64-qt5.yml/badge.svg) (Windows, msvc2019, Qt5.15, qmake)

![Win64 Qt6 msvc CMake](https://github.com/cneben/QuickQanava/actions/workflows/build-win64-qt6.yml/badge.svg) (Windows, msvc2019, Qt6.4, CMake)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://www.destrat.io/quickqanava/doc)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)

!!! note "QuickQanava is licensed under BSD-3, specific features or support is available on demand: benoit@destrat.io"
!!! note "Open an issue with `question` tag for support request / questions"

## Introduction

`QuickQanava` is a C++17 library developed for rendering graphs and relational content within a Qt/QML application. It offers QML components and C++ classes designed for visualizing medium-sized directed graphs in a C++/QML application. QuickQanava emphasizes the presentation of relational content in a dynamic user interface, featuring Drag-and-Drop support, resizable content, and visual creation of topology. `QuickQanava` focus authoring of topology delegates in pure QML.

QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt 6.6.1 with MSVC2019 and g++11 (minimal required Qt version is **Qt 6.5.0**, recommended is **Qt 6.6.1**). 

If you are interested in **Qt5.15** support, last release supporting Qt5 is [2.4.0](https://github.com/cneben/QuickQanava/releases/tag/2.4.0)

+ Project homepage: [http://cneben.github.io/QuickQanava/index.html](http://cneben.github.io/QuickQanava/index.html)

Please refer to [Installation](installation.md) manual and [Graph](graph.md), [Nodes](nodes.md) and [Graph](edges.md) sections for more information about installing and using QuickQanava.

## Gallery

[QuickQanava samples](samples.md)

| Node Grouping                | Visual Connection of Nodes         | 
| :---:                       | :---:             | 
| ![groups](nodes/sample-groups.gif) | ![connector](images/sample-nodes.gif) | 
| ![connector](images/sample-dataflow-short.gif) | ![styles](images/sample-styles.gif) | 

![styles](samples/topology.png)

