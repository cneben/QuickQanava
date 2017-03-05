QuickQanava Quick Start 
============================

Dependencies
------------------


| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes** (GIT submodule)   |      MIT        |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

- **Protocol Buffer v3**: Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header or add a `QUICKQANAVA_HAS_PROTOBUF` defines. Protocol Buffer installation instructions can be found on:
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
#git submodule init
#git submodule update
```

QuickQanava use _qmake_ as its main build configuration system, dependencies are configured in the root directory _common.pri_ file:
- For Linux, just install protocol buffer developper package and let the `common.pri` unmodified: `sudo apt-get install libprotobuf-dev`
- On Windows, follow the installation instructions from source from the Protocol Buffer homepage, then modify `quickqanava-common.pri` to point to your protobuf installation directory:

```sh
# Uncomment to add Protocol Buffer v3 support
#DEFINES += QUICKQANAVA_HAS_PROTOBUF

# Win32 MSVC specific Protocol Buffer configuration
win32-msvc*:PROTOCOL_BUFFER3_DIR=C:/protobuf-3.0.2
```

The recommended way of using QuickQanava is to include the library directly as a GIT submodule in your project:

```sh
# Install QuickQanava as a GIT submodule
$ git submodule add https://github.com/cneben/QuickQanava
& git submodule update
```

Once GIT has finished downloading, QuickQanava and its dependencies ([GTpo](https://github.com/cneben/GTpo)) projects files could be included directly in your main qmake .pro file with the following two #include statements:

```sh
#in your project main .pro qmake configuration file
include($$PWD/QuickQanava/quickqanava-common.pri)	# Edit to add add Protocol Buffer support (opt-in)
include($$PWD/QuickQanava/src/quickqanava.pri)
```
