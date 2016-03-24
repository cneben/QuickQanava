# GTpo

GTpo (Generic Topology) is the C++11 graph topology library used in QuickQanava2 graph visualization library. GTpo is available under the 
MIT licence.

GTpo is not a generic library in the Boot Graph Library sense, but rather an highly configurable graph modelling library designed for
ease of use an maximum performances over "genericity". GTpo heavily rely on C++11/C++17 functionnalities to hide template complexity from the user. GTpo
root namespace 'gtpo' contains generic code, user not interested in using specific containers could use one of the following namespaces for accessing a 
"concrete GTpo library" directly:
    
* 'stpo': Direct use of GTpo with C++ standard library.
* 'qtpo': Direct use of GTpo for Qt (used as a demonstration for using custom containers with GTpo).
* QuickQanava2: Concrete implementation of all GTpo features for QT/QML with complete graph visualization, see: ![QuickQanava2](https://github.com/cneben/QuickQanava)

![GTpo data model schema](https://github.com/cneben/GTpo/blob/master/doc/graph-datamodel.png)

GTpo is **highly** alpha.

+ Project homepage: http://www.qanava.org/
+ Reference documentation: http://www.delia-strategie.fr/doc/gtpo/index.html
+ Developper manual: https://github.com/cneben/GTpo/blob/master/doc/manual/manual.md

## Dependencies:
* Protocol Buffer v3: https://developers.google.com/protocol-buffers/ and https://github.com/google/protobuf
* Googletest: https://github.com/google/googletest/ 
* PugiXML: https://github.com/zeux/pugixml
	GTpo include code from PugiXML distributed under the MIT licence, copyright (c) 2006-2015 Arseny Kapoulkine.

## Roadmap:

- [x] Basic directed graph topology support with custom containers and property support.
- [x] Topology serialization using Protocol Buffer v3.
- [x] Basic GraphML support (OUT).
- [X] Virtual behaviours/layouts/observers support.
- [ ] Gephi GEXF file format IN/OUT support.
- [ ] Complete asynchronous graph access with a read/write graph view MUTEX protection.
- [ ] Advanced asynchronous graph access with Intel TBD thread safe containers.
- [ ] Behaviour/layourts/observer static support.
- [ ] Graph search algorithms (Djikstra/A-Star).

## Installation:
GTpo use _qmake_ as its main build configuration system, dependencies are configured in the root directory _common.pri_ file:

~~~~~~~~~~~~~{.cpp}
# Comment if you do not intent to serialize graph to Protocol Buffer
DEFINES     += "GTPO_HAVE_PROTOCOL_BUFFER"

INCLUDEPATH += c:/path/to/protobuf3/src
PROTOCOL_BUFFER_LIBDIR_RELEASE = C:/path/to/protobuf3/cmake/build/Release
PROTOCOL_BUFFER_LIBDIR_DEBUG = C:/path/to/protobuf3/cmake/build/Debug
~~~~~~~~~~~~~

The simplest way of using GTpo is to statically integrate the library as a GIT subtree in your own project:

~~~~~~~~~~~~~{.cpp}
// Install GTpo as a remote repository:
git remote add GTpo https://github.com/cneben/GTpo

// Then in your project root, use the following command:
git subtree add --prefix=./GTpo --squash GTpo master 
~~~~~~~~~~~~~

Once GIT has download the whole subtree, GTpo could be included directly in your main
qmake .pro file:

~~~~~~~~~~~~~{.cpp}
# in your project main .pro qmake configuration file
include(./GTpo/common.pri)
include(./GTpo/src/gtpo.pri)
~~~~~~~~~~~~~

## Screenshots:
![GTpo in QuickQanava2](https://github.com/cneben/QuickQanava/blob/master/doc/samples/topology.png)


