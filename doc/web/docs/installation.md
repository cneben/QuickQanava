QuickQanava Quick Start 
============================

Dependencies
------------------

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes** (GIT submodule)   |   ![](https://img.shields.io/badge/license-MIT-blue.svg) MIT|
| Google Test/Mock          | No                |       No                        |    Permissive   |

- **Google Test** is a *GTpo* dependency, it is optional for QuickQanava until your intention is to use a graph with custom non-STL/non-Qt containers: ![Google Test GitHub](https://github.com/google/googletest)

Building from sources
------------------

Get the latest QuickQanava sources:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
```

QuickQanava could be used with either _qmake_ or _CMake_ build configuration system.

QuickQanava could be used with either _qmake_ or _CMake_ build configuration system.

| qmake                | cmake             | 
| :---:                | :---:             | 
| Static build, no QML module, all resources are linked statically trough QRC | Installable or embedable, QuickQanava is loaded using a QML module that need to be installed | 

Using qmake:

1. Open _quickqanava.pro_ in QtCreator.

2. Select a kit, build and launch samples.

or (CMake > 3.5)

1. Open _CMakeLists.txt_ in QtCreator.

2. In 'Projects' panel, set DBUILD_SAMPLES option to true in CMake configuration panel.

3. Select a kit, build and launch samples.

Or manually using CMake:

```sh
$ cd QuickQanava
$ mkdir build
$ cd build

# IF QT_DIR IS CONFIGURED AND QMAKE IN PATH
$ cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SAMPLES=TRUE ..

# IF QT DIR IS NOT CONFIGURED, CONFIGURE KIT MANUALLY
$ cmake -DCMAKE_PREFIX_PATH="/home/b/Qt/5.11.0/gcc_64" -DQT_QMAKE_EXECUTABLE="/home/b/Qt/5.11.0/gcc_64/bin/qmake"  -DBUILD_SAMPLES=TRUE ../QuickQanava/

$ cmake --build .
# Then run the samples in ./samples

# Eventually make install
```


Note that a previously installed "QML plugin" version of QuickQanava might interfere with a fully static build using direct .pri inclusion. Typical error message looks like:

```
QQmlApplicationEngine failed to load component
qrc:/nodes.qml:33 module "QuickQanava" plugin "quickqanavaplugin" not found
```

QuickQanava and QuickContainers plugins directories could be removed manually from `$QTDIR\..\qml` to fix the problem (ex: rm -rf '~/Qt/5.11.1/gcc_64/qml/QuickQanava').


Using from external projects
------------------

The recommended way of using QuickQanava is to include the library directly as a GIT submodule in your project:

```sh
# Install QuickQanava as a GIT submodule
$ git submodule add https://github.com/cneben/QuickQanava
* git submodule update
```

Once GIT has finished downloading, QuickQanava project file could be included directly in your main qmake .pro file with the following two #include statements:

```sh
#in your project main .pro qmake configuration file
include($$PWD/QuickQanava/src/quickqanava.pri)
```
