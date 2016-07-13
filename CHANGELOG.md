
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

