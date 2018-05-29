Graph Topology and Visualization
============================

Data Model
------------------

![Graph Datamodel](graph/graph-datamodel.png)


QuickQanava data model enforce a clear separation between description of topology and it's visual representation:

- Topology is defined from C++ or QML (with imperative Js) using the `qan::Graph` interface. 
- Topology primitives returned by `createX()` or `insertX()` have a visual counterpart available trough their `getItem()` method or `item` property.

To keep data-model simple and efficient, topology is defined in a imperative way in C++ code or in a QML `Component.onCompleted()` handler. Visual representation of graph topology is then managed from a view, usually in pure QML with `Qan.GraphView` component. The unique proxy for editing topology is `qan::Graph` class and it's QML interface `Qan.Graph`.

Graph topology is internally modeled using [adjacency lists](https://en.wikipedia.org/wiki/Adjacency_list), these lists are exposed to QML and C++ with Qt abstract item models: a change in topology trigger a signal or a virtual method call in behavior observers. For example, any view connected to a node `outNodes` model is automatically updated when a directed edge is inserted from that node to another one.

QuickQanava Initialization 
------------------

QuickQanava should be initialized in your c++ main function:

``` cpp hl_lines="9"
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

And create a `Qan.Graph` component to build a simple directed graph:
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

Navigation
------------------

A graph is not "navigable" by default, to allow navigation using mouse panning and zooming, `Qan.Graph` component must be defined in the `graph` property of a `Qan.GraphView` item:

``` cpp hl_lines="5"
Qan.GraphView {
  id: graphView
  anchors.fill: parent
  navigable   : true
  graph: Qan.Graph {
      id: topology
    } // Qan.Graph: topology
} // Qan.GraphView
```

Navigation could be disabled by setting the `QanGraph.navigable` property to false (it default to true).


Grid
------------------
