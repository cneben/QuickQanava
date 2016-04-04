
CONFIG      += warn_on qt thread c++11
QT          += core widgets gui qml quick

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/QuickQanava.h         \
            $$PWD/qanConfig.h           \
            $$PWD/qanEdge.h             \
            $$PWD/qanNode.h             \
            $$PWD/qanBehaviour.h        \
            $$PWD/qanGroup.h            \
            $$PWD/qanGraph.h            \
            $$PWD/qanLayout.h           \
            $$PWD/qanLinear.h           \
            $$PWD/qanProgressNotifier.h \
            $$PWD/qanStyle.h            \
            $$PWD/qanStyleManager.h     \
            $$PWD/qanNavigable.h        \
            $$PWD/qanProtoSerializer.h  \       # Protocol Buffer serialization
            $$PWD/quickqanava.pb.h

SOURCES +=  $$PWD/qanEdge.cpp               \
            $$PWD/qanNode.cpp               \
            $$PWD/qanBehaviour.cpp          \
            $$PWD/qanGraph.cpp              \
            $$PWD/qanGroup.cpp              \
            $$PWD/qanLayout.cpp             \
            $$PWD/qanLinear.cpp             \
            $$PWD/qanProgressNotifier.cpp   \
            $$PWD/qanStyle.cpp              \
            $$PWD/qanStyleManager.cpp       \
            $$PWD/qanNavigable.cpp          \
            $$PWD/qanProtoSerializer.cpp    \   # Protocol Buffer serialization
            $$PWD/quickqanava.pb.cc

OTHER_FILES += $$PWD/quickqanava.proto        \
                $$PWD/QuickQanava             \
                $$PWD/Graph.qml               \
                $$PWD/RectNodeTemplate.qml    \
                $$PWD/CanvasNodeTemplate.qml  \
                $$PWD/Group.qml               \
                $$PWD/Node.qml                \
                $$PWD/Edge.qml                \
                $$PWD/StyleListView.qml       \
                $$PWD/ConnectorDropNode.qml

RESOURCES   +=  $$PWD/QuickQanava2.qrc
