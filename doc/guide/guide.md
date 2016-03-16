QuickQanava Developper Guide  {#mainpage}
============================

QuickQanava2 is an open-source C++11/QML library designed to display graphs in a Qt QML application. QuickQanava provide classes to generate various layouts of directed graphs and visualize their nodes with custom styles on a graphic canvas. 

QuickQanava2 is released under LGPL v3, and is shipped with GTpo library (distributed under MIT licence). 

QuickQanava2 source repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava2 use code from the QuickProperties project on: https://github.com/cneben/quickproperties (code is actually shipped as a subtree in main QuickQanava source tree).

QuickQanava is primarily developed with Qt >= 5.6 and Qt Creator with MSVC 2015 but should compiles well with g++ for Linux and GCC-ARM for Android. 

For any questions, please contact: benoit@qanava.org

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes**                   |      MIT        |
| Pugy XML                  | No                |       **Yes**                   |      MIT        |
| QuickProperties2          | **Yes**           |       **Yes**                   |      LPGL       |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

PugiXML source beeing distributed under an MIT licence, it is compatible with GTpo an is thus included directly in the GTpo source tree. For more informations on PugyXML, see:
+ PugiXML homepage: http://pugixml.org/
+ PugiXML GitHub: https://github.com/zeux/pugixml

Google Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header. Protocol Buffer homepage and installation instructions could be found on:
+ Protocol Buffer homepage: https://developers.google.com/protocol-buffers/
+ Protocol Buffer v3 GitHub: https://github.com/google/protobuf

Google Test is a GTpo dependency, it is not mandatory for QuickQanava until you intent to use a graph with custom non-STL non-Qt containers:
+ Google Test GitHub: https://github.com/google/googletest/

[TOC]

Installation
------------------

QuickQanava use _qmake_ to manage build process, dependencies are configured in the root directory _common.pri_ file:

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
git subtree add --prefix=./\QuickProperties --squash \QuickProperties master (avec \QuickProperties installé en remote avant...)
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

Displaying Custom Nodes
------------------

Defining Styles
------------------


Serializing Topology
-------------

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

Exemple Protocol Buffer v3 configuration file for custom node serialization:

~~~~~~~~~~~~~{.cpp}
syntax = "proto3"; 
import "gtpo.proto";
package qan.pb;

message QanImgNode {
    .gtpo.pb.GTpoNode base=1;
    int32   img_data_size=4;
    bytes   img_data=5;
}
~~~~~~~~~~~~~


