
CONFIG      += warn_on qt thread c++14
QT          += core widgets gui qml quick

include(../QuickGeoGL/src/quickgeogl.pri)
include(../../GTpo/src/gtpo.pri)
include(../../QuickProperties/src/quickproperties.pri)

win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR/include
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/lib
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR/lib

# Remove C4100 unreferenced formal parameter
win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34100

# Remove C4267 conversion from size_t to int (protobuf)
win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267

# Used internally, override if you want to use a different working copy of GTpo
INCLUDEPATH	+= $$PWD/GTpo/src
GTPO_DIR        =  $$PWD/GTpo/src
INCLUDEPATH	+= $$GTPO_DIR
DEFINES         += "GTPO_HAVE_PROTOCOL_BUFFER"
DEPENDPATH      += $$PWD/src/

# Add the QuickQanava2 source directory to include path
INCLUDEPATH     += $$PWD/src
INCLUDEPATH     += $$PWD/QuickProperties/src

RESOURCES   +=  $$PWD/QuickGeoGL/src/QuickGeoGL.qrc             \
                $$PWD/QuickProperties/src/QuickProperties.qrc   \
                $$PWD/src/QuickQanava2.qrc

contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
CONFIG(release, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_RELEASE/libprotobuf.lib
}
CONFIG(debug, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
}
}

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/QuickQanava.h             \
            $$PWD/qanConfig.h               \
            $$PWD/qanGraphView.h            \
            $$PWD/qanEdge.h                 \
            $$PWD/qanNode.h                 \
            $$PWD/qanBehaviour.h            \
            $$PWD/qanGroup.h                \
            $$PWD/qanGraph.h                \
            $$PWD/qanLayout.h               \
            $$PWD/qanLinear.h               \
            $$PWD/qanProgressNotifier.h     \
            $$PWD/qanStyle.h                \
            $$PWD/qanStyleManager.h         \
            $$PWD/qanNavigable.h            \
            $$PWD/qanProtoSerializer.h      \       # Protocol Buffer serialization
            $$PWD/quickqanava.pb.h          \
            $$PWD/fqlBottomRightResizer.h

SOURCES +=  $$PWD/qanGraphView.cpp          \
            $$PWD/qanEdge.cpp               \
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
            $$PWD/quickqanava.pb.cc         \
            $$PWD/fqlBottomRightResizer.cpp
			
OTHER_FILES +=  $$PWD/quickqanava.proto         \
                $$PWD/QuickQanava               \
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
                $$PWD/ConnectorDropNode.qml     \
                $$PWD/NodeLabelEditor.qml

RESOURCES   +=  $$PWD/QuickQanava2.qrc
