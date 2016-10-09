## 2016/10/09:
Update documentation: http://www.destrat.io/quickqanava
Update API documentation: http://www.destrat.io/quickqanava/doc
Integrate QuickGeoGL library to draw antialiased lines/polylines and convex polygon with optimized vertex/geometry shader in Qt Quick.
Major changes: GTpo serialization, multiple bug fixes in connector drop node component, use quick controls 2 in most samples.
QuickQanava is running well with Qt5.8 alpha.

## 2016/08/30:
Fix a small bug in qan::Graph::addControlNode().
Fix method qan::Graph::graphChildAt().
Small JS fixes in Qan.Graph component.

## 2016/08/16:
Create a Qan.GraphView component to split grahv visualization from Qan.Graph: subclasses of qan::Graph could now be used
in any graph view.

## 2016/07/26:
Fix Qanava to support the latest changes in QuickProperties serialization code.
Use Quick Controls 2.0 for properties editions.

## 2016/07/13:
Add multiple selection dragging (with group support).
Add support for visual edition of nodes label in Qan.RectNodeTemplate.

## 2016/07/12:
Add node selection and multiple selection support.

## 2016/07/11:
Massive optimization of node dragging code: node dragging is now 100% c++, MouseArea and Drag QML components are
no longer necessary in Qan.RectNodeTemplate (faster node JS compilation and creation, and way faster runtime execution)
Remove Qt drag event handling from qan::Group (do not have a look to Qt internal drag and drop code if you don't want to
loose all faith in the framework). Various qan::Group optimization and cleaning.
Fix the c++ bottom right resizer to work perfeclty at non 1.0 zoom level.

## 2016/07/10:
WARNING: Main graph interface has changed: it is no longer necessary to enclose Qan.Graph in a Qan.Navigable.

Update GTpo and QAN serializer to support custom group serialization.
Remove the pure QML implementation of Qan.BottomRightResizer, replaced with an optimized pure C++ QQuickItem.

