QuickQanava Developper Guide  {#mainpage}
============================

QuickQanava2 is an open-source C++11/QML library designed to display graphs in a Qt QML application. QuickQanava provide classes to generate various layouts of directed graphs and visualize their nodes with custom styles on a graphic canvas. 

QuickQanava2 is released under LGPL v3, and is shipped with *GTpo* library (distributed under MIT licence). 

QuickQanava2 source repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava2 use code from the QuickProperties project on: https://github.com/cneben/quickproperties (code is actually shipped as a subtree in main QuickQanava source tree).

QuickQanava is primarily developed with Qt >= 5.6 and Qt Creator with MSVC 2015 but should compiles well with g++ for Linux and GCC-ARM for Android. 

+ Project homepage: http://www.qanava.org/
+ Reference documentation: http://www.delia-strategie.fr/doc/qan/index.html

For any questions, please contact: benoit@qanava.org

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes**                   |      MIT        |
| Pugi XML                  | No                |       **Yes**                   |      MIT        |
| QuickProperties2          | **Yes**           |       **Yes**                   |      LPGL       |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

*PugiXML* source beeing distributed under an MIT licence, it is compatible with *GTpo* an is included directly in the *GTpo* source tree. For more informations on *PugiXML*, see:
+ PugiXML homepage: http://pugixml.org/
+ PugiXML GitHub: https://github.com/zeux/pugixml

Google Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header. Protocol Buffer homepage and installation instructions could be found on:
+ Protocol Buffer homepage: https://developers.google.com/protocol-buffers/
+ Protocol Buffer v3 GitHub: https://github.com/google/protobuf

Google Test is a *GTpo* dependency, it is not mandatory for QuickQanava until you intent to use a graph with custom non-STL non-Qt containers:
+ Google Test GitHub: https://github.com/google/googletest/

[TOC]

Installation
------------------

QuickQanava use _qmake_ as its main build configuration system, dependencies are configured in the root directory _common.pri_ file:

~~~~~~~~~~~~~{.cpp}
PROTOCOL_BUFFER3_DIR= **path to protocol buffer**

INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR\src
PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/cmake/build/Release
PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR\cmake/build/Debug
~~~~~~~~~~~~~

The simplest way of using QuickQanava is to statically integrate the library as a GIT subtree in your own project:

~~~~~~~~~~~~~{.cpp}
// Install QuickQanava as a remote repository:
git remote add QuickQanava https://github.com/cneben/QuickQanava

// Then in your project root, use the following command:
git subtree add --prefix=./QuickQanava --squash QuickQanava master 
~~~~~~~~~~~~~

Once GIT has download the whole subtree, QuickQanava and its dependencies (GTpo) projects files could be included directly in your main
qmake .pro file:

~~~~~~~~~~~~~{.cpp}
# in your project main .pro qmake configuration file
include(./QuickQanava/GTpo/src/gtpo.pri)
include(./QuickQanava/src/quickqanava.pri)
~~~~~~~~~~~~~

Samples
------------------

[Samples overview](@ref samples)

Displaying a Simple Directed Graph
------------------

Both QuickQanava and QuickProperties should be initialized in your c++ main function:

~~~~~~~~~~~~~{.cpp}
#include <QuickQanava>
 // ...
 // In your custom QQuickView constructor:
    QScopedPointer< qps::AbstractTranslator > translator{ new qps::AbstractTranslator() };
    QuickProperties::initialize( engine(), translator.data() );
    QuickQanava::initialize();
~~~~~~~~~~~~~

Basic Graph Topology
------------------


### Group topology

~~~~~~~~~~~~~{.cpp}
    Qan.Navigable {
        id: navigable
        anchors.fill: parent
        Qan.Graph {
            parent: navigable.containerItem
            id: graph
            objectName: "graph"
            anchors.fill: parent

            Component.onCompleted: {
                var n1 = graph.insertNode( )
                n1.label = "N1"
                var n2 = graph.insertNode( )
                n2.label = "N2"
                var n3 = graph.insertNode( )
                n3.label = "N3"
                graph.insertEdge( n1, n2 )
                graph.insertEdge( n2, n3 )

				var gg = graph.insertGroup();
                gg.label = "Group"
            }
        } // Qan.Graph: graph
    } // Qan.Navigable: navigable
~~~~~~~~~~~~~

![Qan qan::Group overview](https://github.com/cneben/QuickQanava/blob/master/doc/guide/groups-overview.gif)

Displaying Custom Nodes
------------------

Defining Styles
------------------


Serializing Topology
-------------

Preferred way for dealing with persistance in QuickQanava is writing to Protocol Buffer repository with the qan::ProtoSerializer class:

1. Define a protobuf v3 message for your custom topology (usually a class inheriting from qan::Node)
2. Compile your message.
3. Register in/out functors in qan::ProtoSerializer to write your custom messages.
4. Call qan::ProtoSerializer saveGraphTo() or loadGraphFrom().

For example, if you have defined a custom "image node" called qan::ImageNode, write a protobuf3 message to store persistent data:
~~~~~~~~~~~~~{.cpp}
// File custom.proto
syntax = "proto3"; 
import "gtpo.proto";
import "quickqanava.proto";
package qan.pb;

message QanImgNode {
    .gtpo.pb.GTpoNode base=1;
    int32   img_data_size=4;
    bytes   img_data=5;
}
~~~~~~~~~~~~~

Compile this Protobuf IDL file with the following command:
~~~~~~~~~~~~~{.cpp}
$ protoc custom.proto --cpp_out=.
~~~~~~~~~~~~~

> Note: It might be necessary to manually copy "gtpo.proto" and "quickqanava.proto" in your .proto directory before calling `protoc`, theses files could be removed once custom.pb.cc and custom.pb.h have been generated.

You can then add `custom.pb.cc` and `custom.pb.h` to your qmake project configuration file, and register custom in/out functors in a qan::ProtoSerializer. Mapping between functors and the target node is done via the first string parameter of registerNodeOutFunctor(), the value must correspond to qan::Node::getDynamicClassName().

~~~~~~~~~~~~~{.cpp}
#include <QuickQanava>
 // ...
	QScopedPointer< qan::ProtoSerializer > _serializer{ new qan::ProtoSerializer() };
    engine()->rootContext( )->setContextProperty( "qanSerializer", _serializer.data() );
	
    _serializer->registerNodeOutFunctor( "qan::ImgNode",
                                         []( google::protobuf::Any* anyNodes,
                                         const qan::ProtoSerializer::WeakNode& weakNode,
                                         const qan::ProtoSerializer::NodeIdMap& nodeIdMap  ) {
            std::shared_ptr< qan::ImgNode > imageNode = std::static_pointer_cast< qan::ImgNode >( weakNode.lock() );
            if ( !imageNode )
                return;
	}
    _serializer->registerNodeInFunctor( []( const google::protobuf::Any& anyNode,
                                            qan::ProtoSerializer::Graph& graph,
                                            qan::ProtoSerializer::IdNodeMap& idNodeMap  ) -> qan::ProtoSerializer::WeakNode {
            if ( anyNode.Is< qan::pb::QanImgNode >() ) {
                qan::pb::QanImgNode pbImgNode;
                if ( anyNode.UnpackTo( &pbImgNode ) ) {
				}
			}
	}
~~~~~~~~~~~~~

See the topology sample for a compilable example.



