QuickQanava Quick Start 
============================

Simple Directed Graph
------------------

Both QuickQanava and QuickProperties should be initialized in your c++ main function:

``` cpp
#include <QuickQanava>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;
	// Or in a custom QQuickView constructor:
    QuickQanava::initialize();
	engine.load( ... );
    return app.exec();
}
```

Then, in QML import QuickQanava:
``` cpp
import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan
```

And create a `Qan.Graph` component:
``` cpp
Qan.Graph {
    id: graph
    anchors.fill: parent
    Component.onCompleted: {
        var n1 = graph.insertNode()
        n1.label = "Hello World"
    }
}
```

Topology
------------------

### Navigation

A graph is not "navigable" by default, to allow navigation using mouse panning and zooming, `Qan.Graph` component must be defined in the `graph` property of a `Qan.GraphView` component:
``` cpp
Qan.GraphView {
  id: graphView
  anchors.fill: parent
  navigable   : true
  graph: Qan.Graph {
      id: topology
      anchors.fill: parent
      clip: true
      onNodeRightClicked: {
        /*var globalPos = node.mapToItem( topology, pos.x, pos.y )	// Conversion from a node coordinate system to user view coordinate system
        menu.x = globalPos.x; menu.y = globalPos.y						// For example to open a popup menu
        menu.open()*/
      }
    } // Qan.Graph: topology
  onRightClicked: {
    /*var globalPos = graphView.mapToItem( topology, pos.x, pos.y ) // Conversion from graph view coordinate system to user view coordinate system
    menu.x = globalPos.x; menu.y = globalPos.y							// For example to open a popup menu
	menu.open()*/
  }
} // Qan.GraphView
```

Navigation could be disabled by setting the `navigable` property to false (it default to true).

### Using visual node connector

QuickQanava allow visual connection of node with the `Qan.ConnectorDropNode` component. Visual connection of nodes is configured in your main `Qan.Graph` component with the following properties:

- `resizeHandlerColor` (color): Color of the visual drop node component (could be set to Material.accent for example)
- `enableConnectorDropNode` (bool): Set to true to enable visual connection of nodes (default to false).
- `connectorDropNodeEdgeClassName` (string): String that could be modified by the user to specify a custom class name for the edges created by visual drop node connector (default to "qan::Edge", default edge delegate component).

![Groups](images/visual-node-connector.gif)

``` cpp
Qan.GraphView {
  id: graphView
  anchors.fill: parent
  graph : Qan.Graph {
    anchors.fill: parent
    enableConnectorDropNode: true
  } // Qan.Graph
} // Qan.GraphView
```

Selection
------------------
   
Selection can be modified at graph level just by changing the graph selection policy property `Qan.Graph.selectionPolicy`:

- `Qan.AbstractGraph.NoSelection`: Selection will be disabled in the whole graph.
- `Qan.AbstractGraph.SelectOnClick`: Node are selected when clicked, multiple selection is enabled when CTRL is pressed.
- `Qan.AbstractGraph.SelectOnCtrlClick`: Node are selected only if CTRL is pressed when node is clicked (multiple selection is still available).

Selection can also be configured with the following Qan.Graph properties:

- `Qan.Graph.selectionColor` / `qan::Graph::setSelectionColor()`: Color for the node selection rectangle.
- `Qan.Graph.selectionWeight` / `qan::Graph::setSelectionWeight()`: Border width of the node selection rectangle.
- `Qan.Graph.selectionMargin` / `qan::Graph::setSelectionMargin()`: Margin between the node selection rectangle and the node content (selection weight is taken into account).

All theses properties could be changed dynamically.

Selection could also be disabled at node level by setting `Qan.Node.selectable` to false, node become unselectable even if global graph selection policy is not set to `NoSelection`.

Current multiple selection (or single selection) is available trought Qan.Graph `selectedNodes` property. `selectedNodes` is a list model, it can be used in any QML view, or iterated from C++ to read the current selection:
``` cpp
// Viewing the actual selected nodes with a QML ListView:
ListView {
	id: selectionListView
	Layout.fillWidth: true; Layout.fillHeight: true
	clip: true
	model: graph.selectedNodes		// <---------
	spacing: 4; focus: true; flickableDirection : Flickable.VerticalFlick
	highlightFollowsCurrentItem: false
	highlight: Rectangle {
		x: 0; y: ( selectionListView.currentItem !== null ? selectionListView.currentItem.y : 0 );
		width: selectionListView.width; height: selectionListView.currentItem.height
		color: "lightsteelblue"; opacity: 0.7; radius: 5
	}
	delegate: Item {
		id: selectedNodeDelegate
		width: ListView.view.width; height: 30;
		Text { text: "Label: " + itemData.label }		// <----- itemData is a Qan.Node, node 
														// label could be accessed directly
		MouseArea {
			anchors.fill: selectedNodeDelegate
			onClicked: { selectedNodeDelegate.ListView.view.currentIndex = index }
		}
	}
}
```

In C++, `selectedNodes` could be iterated directly, the current node should be tested to ensure it is non nullptr, since the underlining model is thread-safe and could have
been modified from another thread:
``` cpp
	auto graph = std::make_unique<qan::Graph>();
    for ( auto& node : graph->getSelectNodes() ) {
        if ( node != nullptr )
            node->doWhateverYouWant();
    }
```

Example of `Qan.AbstractGraph.SelectOnClick` selection policy with multiple selection dragging inside a group:
![selection](images/Selection.gif)


Using Groups
------------------

![Groups](images/groups-overview.gif)

``` cpp
Qan.Graph {
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
```


Displaying Custom Nodes
------------------

See the `custom` sample for more informations on specifying custom delegates for nodes and edges.

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
``` cpp
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
```

Compile this Protobuf IDL file with the following command:
``` bash
#!/bin/bash
$ protoc custom.proto --cpp_out=.
```

> Note: It might be necessary to manually copy "gtpo.proto" and "quickqanava.proto" in your .proto directory before calling `protoc`, theses files could be removed once custom.pb.cc and custom.pb.h have been generated.

You can then add `custom.pb.cc` and `custom.pb.h` to your qmake project configuration file, and register custom in/out functors in a qan::ProtoSerializer. Mapping between functors and the target node is done via the first string parameter of registerNodeOutFunctor(), the value must correspond to qan::Node::getDynamicClassName().

``` cpp
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
```

See the topology sample for a compilable example.



