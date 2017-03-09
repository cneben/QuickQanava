
CONFIG      += warn_on qt thread c++14
QT          += core widgets gui qml quick

# If QuickQanava is configured with Protocol Buffer, add it to QuickProperties
# and GTpo too
contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
    # Specific QuickProperties configuration
    DEFINES += QUICKPROPERTIES_HAS_PROTOBUF
    include(./quickqanava-pb-conf.pri)

    # Specific GTpo configuration
    DEFINES += GTPO_HAS_PROTOBUF

    win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR/include
    win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/lib
    win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR/lib
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34100  # Remove C4100 unreferenced formal parameter
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267  # Remove C4267 conversion from size_t to int (protobuf)
}
include(../GTpo/src/gtpo.pri)
include(../QuickContainers/src/quickcontainers.pri)
include(../QuickGeoGL/src/quickgeogl.pri)

DEPENDPATH      += $$PWD
INCLUDEPATH     += $$PWD
RESOURCES       +=  $$PWD/QuickQanava.qrc

contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
CONFIG(release, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_RELEASE/libprotobuf.lib
    linux-g++*:     LIBS	+= -lprotobuf
    android:        LIBS	+= -lprotobuf
    win32-g++*:     LIBS	+= -lprotobuf
}
CONFIG(debug, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
    linux-g++*:     LIBS	+= -lprotobuf
    android:        LIBS	+= -lprotobuf
    win32-g++*:     LIBS	+= -lprotobuf
}
}

HEADERS +=  $$PWD/QuickQanava.h             \
            $$PWD/qanGraphConfig.h          \
            $$PWD/qanGraphView.h            \
            $$PWD/qanEdge.h                 \
            $$PWD/qanEdgeItem.h             \
            $$PWD/qanNode.h                 \
            $$PWD/qanNodeItem.h             \
            $$PWD/qanBehaviour.h            \
            $$PWD/qanGroup.h                \
            $$PWD/qanGroupItem.h            \
            $$PWD/qanGraph.h                \
            $$PWD/qanProgressNotifier.h     \
            $$PWD/qanStyle.h                \
            $$PWD/qanStyleManager.h         \
            $$PWD/qanNavigable.h            \
            $$PWD/qanPointGrid.h            \
            $$PWD/qanContainerAdapter.h     \
            $$PWD/fqlBottomRightResizer.h

SOURCES +=  $$PWD/qanGraphView.cpp          \
            $$PWD/qanEdge.cpp               \
            $$PWD/qanEdgeItem.cpp           \
            $$PWD/qanNode.cpp               \
            $$PWD/qanNodeItem.cpp           \
            $$PWD/qanBehaviour.cpp          \
            $$PWD/qanGraph.cpp              \
            $$PWD/qanGroup.cpp              \
            $$PWD/qanGroupItem.cpp          \
            $$PWD/qanProgressNotifier.cpp   \
            $$PWD/qanStyle.cpp              \
            $$PWD/qanStyleManager.cpp       \
            $$PWD/qanNavigable.cpp          \
            $$PWD/qanPointGrid.cpp          \
            $$PWD/fqlBottomRightResizer.cpp
			
OTHER_FILES +=  $$PWD/QuickQanava               \
                $$PWD/GraphView.qml             \
                $$PWD/Graph.qml                 \
                $$PWD/RectNodeTemplate.qml      \
                $$PWD/CanvasNodeTemplate.qml    \
                $$PWD/Group.qml                 \
                $$PWD/RectGroupTemplate.qml     \
                $$PWD/BottomRightResizer.qml    \
                $$PWD/Node.qml                  \
                $$PWD/Edge.qml                  \
                $$PWD/StyleListView.qml         \
                $$PWD/StyleEditor.qml           \
                $$PWD/VisualConnector.qml       \
                $$PWD/NodeLabelEditor.qml
