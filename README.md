[Overview](#QuickQanava) |
[Dependencies](#Dependencies) |
[Building](#building) |
[License](#license)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://www.destrat.io/quickqanava/doc)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/cneben/QuickQanava/blob/master/licence.txt)

2017 (January): Warning massive changes in master repository, report compilation problems to benoit@destrat.io (Removed dependencies to QuickProperties and PugiXML)

QuickQanava 
============================

`QuickQanava` is a C++11/Qt/QML library designed to display graphs in a Qt/QML application. QuickQanava provide components and classes to visualize medium-sized directed graph in a QML application. QuickQanava does not provide advanced layout algorithms, but rather focus on displaying relational content into an advanced dynamic user interface (with DnD support, resizable content, visual connection of content).

`QuickQanava` is released under LGPL v3, and is shipped with *[GTpo](https://github.com/cneben/GTpo)* library (MIT licence) and  *[QuickContainers](https://github.com/cneben/QuickQanava/tree/master/QuickContainers)* (LGPL) and *[QuickGeoGL](https://github.com/cneben/QuickQanava/tree/master/QuickGeoGL)*.


QuickQanava main repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava is primarily developed with Qt >= 5.7 with MSVC2015 and g++5.3 (minimal required Qt version is 5.7, QuickQanava is reported to work well with Qt 5.8 alpha)

+ Project homepage: http://www.destrat.io/quickqanava
+ Reference documentation: http://www.destrat.io/quickqanava/doc/index.html

For any questions, please contact: benoit@destrat.io

## Dependencies:

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes** (GIT submodule)   |      MIT        |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

- **Protocol Buffer v3**: Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header. Protocol Buffer installation instructions can be found on:
  - Protocol Buffer homepage: https://developers.google.com/protocol-buffers/
  - Protocol Buffer v3 GitHub: https://github.com/google/protobuf

- **Google Test** is a *GTpo* dependency, it optionnal for QuickQanava until you intent to use a graph with custom non-STL/non-Qt containers:
  - Google Test GitHub: https://github.com/google/googletest/

## Building:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
#git submodule init		# No longer necessary since 2017/01
#git submodule update
# Eventually, launch Protocol Buffer compiler with ./run_protoc.sh
```
2. Open quickqanava2.pro in QtCreator
  1. QuickQanava Protocol Buffer v3 support is opt-in: in `quickqanava-common.pri`, uncomment line `DEFINES += QUICKQANAVA_HAS_PROTOBUF` to add Protocol Buffer in the static library.
  4. Building 'tests': Tests will not compile without a working Protocol Buffer v3 installation: for Windows, modify `quickqanava-common.pri` `win32-msvc*:PROTOCOL_BUFFER3_DIR`, win32-msvc*:GMOCK_DIR and win32-msvc*:GTEST_DIR with path containing a valid build directory for Protocol Buffer, Google Test and Google Mock. On Linux, no configuration is required, just install the '-dev' package for theses libraries.

3. Select a kit, build and launch samples.

Compilation errors with messages refering to 'google::' types are usually related to the Protocol Buffer of your local platform being incompatible with the code generated for protocol buffer shipped with QuickQanava, run `run_protoc.sh` script to update `quickqanava.pb.h` and `quickqanava.pb.cc` files.


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

Copyright (c) 2017 Delia Stratégie

