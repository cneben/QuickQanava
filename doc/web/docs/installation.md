QuickQanava Quick Start 
============================

Dependencies
------------------


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


Building from sources
------------------

Get the latest QuickQanava sources:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
git submodule init
git submodule update
```

QuickQanava use _qmake_ as its main build configuration system, dependencies are configured in the root directory _common.pri_ file:
- For Linux, just install protocol buffer developper package and let the `common.pri` unmodified: `sudo apt-get install libprotobuf-dev`
- On Windows, follow the installation instructions from source from the Protocol Buffer homepage, then modify `common.pri` to point to your protobuf installation directory:

```sh
win32-msvc*:PROTOCOL_BUFFER3_DIR=C:/path/to/protobuf3
win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR\src
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/cmake/build/Release
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR\cmake/build/Debug
```

The recommended way of using QuickQanava is to include the library directly as a GIT submodule in your project:

```sh
# Install QuickQanava as a GIT submodule
$ git submodule add https://github.com/cneben/QuickQanava
& git submodule update
```

Once GIT has finished downloading, QuickQanava and its dependencies ([GTpo](https://github.com/cneben/GTpo) and [QuickProperties](https://github.com/cneben/QuickProperties)) projects files could be included directly in your main qmake .pro file:

```sh
# in your project main .pro qmake configuration file
include($$PWD/QuickQanava/GTpo/src/gtpo.pri)
include($$PWD/QuickQanava/common.pri)
include($$PWD/QuickQanava/src/quickqanava.pri)
```
