[Overview](#QuickQanava) |
[Dependencies](#Dependencies) |
[Building](#building) |
[License](#license)

20160713: Warning, master branch is actually under heavy development, compilation on g++ might be difficult for a few days. 

QuickQanava 
============================

`QuickQanava` is a C++11/Qt/QML library designed to display graphs in a Qt/QML application. QuickQanava provide components and classes to visualize medium-sized directed graph in a QML application. QuickQanava does not provide advanced layout algorithms, but rather focus on displaying relationnal content into an advanced dynamic user interface (with DnD support, resizable content, visual connection of content).

`QuickQanava` is released under LGPL v3, and is shipped with *[GTpo](https://github.com/cneben/GTpo)* library (MIT licence) and  *[QuickProperties](https://github.com/cneben/quickproperties)* (LGPL).


QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt >= 5.6 with MSVC2015 and g++5.3

+ Project homepage: http://www.destrat.io/quickqanava
+ Reference documentation: http://www.destrat.io/quickqanava/doc/index.html

For any questions, please contact: benoit@destrat.io

## Dependencies:

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes** (GIT submodule)   |      MIT        |
| Pugi XML                  | No                |       **Yes**                   |      MIT        |
| QuickProperties2          | **Yes**           |       **Yes** (GIT submodule)   |      LPGL       |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

- **PugiXML** is released under an MIT licence, it is compatible with *GTpo* an is included directly in *GTpo* source tree. For more informations on PugiXML, see:
  - PugiXML homepage: http://pugixml.org/
  - PugiXML GitHub: https://github.com/zeux/pugixml

- **Protocol Buffer v3**: Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header. Protocol Buffer installation instructions can be found on:
  - Protocol Buffer homepage: https://developers.google.com/protocol-buffers/
  - Protocol Buffer v3 GitHub: https://github.com/google/protobuf

- **Google Test** is a *GTpo* dependency, it optionnal for QuickQanava until you intent to use a graph with custom non-STL/non-Qt containers:
  - Google Test GitHub: https://github.com/google/googletest/

## Building:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
git submodule init
git submodule update
```
Build options are configured in `common.pri` (local Protocol Buffer source directory for win32). Main QtCreator project file is quickqanava2.pro

## QuickQanava Screenshots:

![selection](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/Selection.gif)

![resizing and groups sample](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/groups-overview.gif)

![visual node connection](https://github.com/cneben/QuickQanava/blob/master/doc/web/docs/images/visual-node-connector.gif)

![custom node sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/custom.png)

![navigable sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/navigable.png)

![style sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/style.png)

![topology sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/topology.png)

License
=======

[LGPLv3](https://github.com/cneben/QuickQanava/blob/master/licence.txt)

Copyright (c) 2016 Delia Stratégie

