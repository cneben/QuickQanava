Graph Topology and Visualization
============================

Data Model
------------------

![Graph Datamodel](graph/graph-datamodel.png)


QuickQanava data model enforce a clear separation between description of topology and it's visual representation. Topology is usually defined from C++ or QML using the `qan::Graph` interface. To keep datamodel simple and efficient, topology is defined in a imperative way in C++ code or in a QML `Component.onCompleted()` handler. Visual representation of graph topology is then managed with a view, usually in pure QML with `Qan.GraphView` component. The unique proxy for editing topology is `qan::Graph` class and it's QML interface `Qan.Graph`.


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
