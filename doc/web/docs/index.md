---
title: QuickQanava
description: "C+11/Qt/QML graph visualization library"
type: index
weight: 0
---

![home](images/home.png)

!!! warning "QuickQanava is alpha software" 

`QuickQanava` is a C++11/Qt/QML library designed to display graphs in a Qt/QML application. QuickQanava provide components and classes to visualize medium-sized directed graph in a QML application. QuickQanava does not provide advanced layout algorithms, but rather focus on displaying relationnal content into an advanced dynamic user interface (with DnD support, resizable content, visual connection of content).

`QuickQanava` is released under LGPL v3, and is shipped with *[GTpo](https://github.com/cneben/GTpo)* library (MIT licence) and  *[QuickProperties](https://github.com/cneben/quickproperties)* (LGPL).


QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt >= 5.7 with MSVC2015 and g++5.3 (Minimal version number for Qt is 5.7, QuickQanava is reported to works well on 5.8 alpha)

+ Project homepage: [http://www.destrat.io/quickqanava](http://www.destrat.io/quickqanava)
+ Reference documentation: [http://www.destrat.io/qanava/doc/index.html](http://www.destrat.io/qanava/doc/index.html)

## Installation

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
git submodule init
git submodule update
```

[Installing QuickQanava](installation.md)

## Quick Start

[Getting started guide](gettingstarted.md)

## Gallery

[QuickQanava samples](samples.md)

| Node Grouping                | Visual Connection of Nodes         | 
| :---:                       | :---:             | 
| ![groups](images/groups-overview.gif) | ![connector](images/visual-node-connector.gif) | 

![styles](images/style.png)



