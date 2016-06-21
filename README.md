[Overview](#QuickQanava) |
[Building](#building) |
[License](#license)

QuickQanava 
============================

`QuickQanava` is a C++11/QML library designed to display graphs in a Qt/QML application. QuickQanava provide classes to generate various layouts of directed graphs and visualize their nodes with custom styles on a graphic canvas. 

`QuickQanava` is released under LGPL v3, and is shipped with *[GTpo](https://github.com/cneben/GTpo)* library (MIT licence) and  [QuickProperties](https://github.com/cneben/quickproperties) (LGPL).


QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt >= 5.6 with MSVC2015 and g++5.3

+ Project homepage: http://www.qanava.org/
+ Reference documentation: http://www.delia-strategie.fr/doc/qan/index.html
+ QuickQanava developper guide: https://github.com/cneben/QuickQanava/blob/master/doc/guide/guide.md

For any questions, please contact: benoit@destrat.io

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes** (GIT submodule)   |      MIT        |
| Pugi XML                  | No                |       **Yes**                   |      MIT        |
| QuickProperties2          | **Yes**           |       **Yes** (GIT submodule)   |      LPGL       |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

*PugiXML* source beeing distributed under an MIT licence, it is compatible with *GTpo* an is included directly in the *GTpo* source tree. For more informations on PugiXML, see:
+ PugiXML homepage: http://pugixml.org/
+ PugiXML GitHub: https://github.com/zeux/pugixml

- **Protocol Buffer v3**: Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header. Protocol Buffer installation instructions can be found on:
  - Protocol Buffer homepage: https://developers.google.com/protocol-buffers/
  - Protocol Buffer v3 GitHub: https://github.com/google/protobuf

**Google Test** is a *GTpo* dependency, it is not mandatory for QuickQanava until you intent to use a graph with custom non-STL non-Qt containers:
+ Google Test GitHub: https://github.com/google/googletest/

## Building:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
git submodule init
git submodule update
```
Then configure common.pri with your local Protocol Buffer source directory (win32 only), and open quickqanava2.pro

## QuickQanava Screenshots:

![custom node sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/custom.png)

![navigable sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/navigable.png)

![style sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/style.png)

![topology sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/topology.png)

License
=======

[LGPLv3](https://github.com/cneben/QuickQanava/blob/master/licence.txt)

Copyright (c) 2016 Delia Stratégie

