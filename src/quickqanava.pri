
CONFIG      += warn_on qt thread c++14
QT          += core widgets gui qml quick

include(../GTpo/src/gtpo.pri)
include(../QuickContainers/src/quickcontainers.pri)
include(../QuickGeoGL/src/quickgeogl.pri)

DEPENDPATH      += $$PWD
INCLUDEPATH     += $$PWD
RESOURCES       += $$PWD/QuickQanava.qrc

HEADERS +=  $$PWD/QuickQanava.h             \
            $$PWD/qanUtils.h                \
            $$PWD/qanGraphConfig.h          \
            $$PWD/qanGraphView.h            \
            $$PWD/qanEdge.h                 \
            $$PWD/qanEdgeItem.h             \
            $$PWD/qanNode.h                 \
            $$PWD/qanNodeItem.h             \
            $$PWD/qanPortItem.h             \
            $$PWD/qanSelectable.h           \
            $$PWD/qanDraggable.h            \
            $$PWD/qanAbstractDraggableCtrl.h\
            $$PWD/qanDraggableCtrl.h        \
            $$PWD/qanDraggableCtrl.hpp      \
            $$PWD/qanConnector.h            \
            $$PWD/qanBehaviour.h            \
            $$PWD/qanGroup.h                \
            $$PWD/qanGroupItem.h            \
            $$PWD/qanGraph.h                \
            $$PWD/qanGraph.hpp              \
            $$PWD/qanStyle.h                \
            $$PWD/qanStyleManager.h         \
            $$PWD/qanNavigable.h            \
            $$PWD/qanNavigablePreview.h     \
            $$PWD/qanPointGrid.h            \
            $$PWD/qanContainerAdapter.h     \
            $$PWD/qanBottomRightResizer.h

SOURCES +=  $$PWD/qanGraphView.cpp          \
            $$PWD/qanUtils.cpp              \
            $$PWD/qanEdge.cpp               \
            $$PWD/qanEdgeItem.cpp           \
            $$PWD/qanNode.cpp               \
            $$PWD/qanNodeItem.cpp           \
            $$PWD/qanPortItem.cpp           \
            $$PWD/qanSelectable.cpp         \
            $$PWD/qanDraggable.cpp          \
            $$PWD/qanConnector.cpp          \
            $$PWD/qanBehaviour.cpp          \
            $$PWD/qanGraph.cpp              \
            $$PWD/qanGroup.cpp              \
            $$PWD/qanGroupItem.cpp          \
            $$PWD/qanStyle.cpp              \
            $$PWD/qanStyleManager.cpp       \
            $$PWD/qanNavigable.cpp          \
            $$PWD/qanNavigablePreview.cpp   \
            $$PWD/qanPointGrid.cpp          \
            $$PWD/qanBottomRightResizer.cpp

OTHER_FILES +=  $$PWD/QuickQanava               \
                $$PWD/GraphView.qml             \
                $$PWD/Graph.qml                 \
                $$PWD/RectNodeTemplate.qml      \
                $$PWD/CanvasNodeTemplate.qml    \
                $$PWD/Group.qml                 \
                $$PWD/RectGroupTemplate.qml     \
                $$PWD/BottomRightResizer.qml    \
                $$PWD/Node.qml                  \
                $$PWD/Port.qml                  \
                $$PWD/Edge.qml                  \
                $$PWD/StyleListView.qml         \
                $$PWD/StyleEditor.qml           \
                $$PWD/VisualConnector.qml       \
                $$PWD/LabelEditor.qml
