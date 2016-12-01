
TEMPLATE	= lib
DEFINES		+= QANAVA
TARGET		= quickqanava
DESTDIR		= ../build
CONFIG		+= warn_on qt thread staticlib c++14
QT		+= core widgets gui xml qml quick

include(../quickqanava-common.pri)
contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
    # Specific QuickProperties configuration
    DEFINES += QUICKPROPERTIES_HAS_PROTOBUF
    include(../QuickProperties/src/quickproperties-pb-conf.pri)

    # Specific GTpo configuration
    DEFINES += GTPO_HAS_PROTOBUF

    win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR/include
    win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/lib
    win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR/lib
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34100  # Remove C4100 unreferenced formal parameter
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267  # Remove C4267 conversion from size_t to int (protobuf)
}
include(../GTpo/src/gtpo.pri)
include(../QuickProperties/src/quickproperties.pri)
include(../QuickGeoGL/src/quickgeogl.pri)

HEADERS +=  ./QuickQanava.h             \
            ./qanConfig.h               \
            ./qanGraphView.h            \
            ./qanEdge.h                 \
            ./qanNode.h                 \
            ./qanBehaviour.h            \
            ./qanGroup.h                \
            ./qanGraph.h                \
            ./qanLayout.h               \
            ./qanLinear.h               \
            ./qanProgressNotifier.h     \
            ./qanStyle.h                \
            ./qanStyleManager.h         \
            ./qanNavigable.h            \
            ./fqlBottomRightResizer.h

SOURCES +=  ./qanEdge.cpp               \
            ./qanGraphView.cpp          \
            ./qanNode.cpp               \
            ./qanBehaviour.cpp          \
            ./qanGroup.cpp              \
            ./qanGraph.cpp              \
            ./qanLayout.cpp             \
            ./qanLinear.cpp             \
            ./qanProgressNotifier.cpp   \
            ./qanStyle.cpp              \
            ./qanStyleManager.cpp       \
            ./qanNavigable.cpp          \
            ./fqlBottomRightResizer.cpp

OTHER_FILES +=  ./quickqanava.proto         \
                ./QuickQanava               \
                ./GraphView.qml             \
                ./Graph.qml                 \
                ./RectNodeTemplate.qml      \
                ./CanvasNodeTemplate.qml    \
                ./Group.qml                 \
                ./RectGroupTemplate.qml     \
                ./BottomRightResizer.qml    \
                ./Node.qml                  \
                ./Edge.qml                  \
                ./StyleListView.qml         \
                ./ConnectorDropNode.qml     \
                ./NodeLabelEditor.qml

contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
OTHER_FILES +=  $$PWD/quickqanava.proto         \

HEADERS +=      $$PWD/qanProtoSerializer.h      \       # Protocol Buffer serialization
                $$PWD/quickqanava.pb.h

SOURCES     +=  $$PWD/qanProtoSerializer.cpp    \   # Protocol Buffer serialization
                $$PWD/quickqanava.pb.cc
}

TRANSLATIONS = ./quickqanava-fr_FR.ts
lupdate_only {
    SOURCES = *.qml
}

CONFIG(release, debug|release) {
    linux-g++*:     TARGET    = quickqanava
    android:        TARGET    = quickqanava
    win32-msvc*:    TARGET    = quickqanava
    win32-g++*:     TARGET    = quickqanava
}

CONFIG(debug, debug|release) {
    linux-g++*:     TARGET    = quickqanavad
    android:        TARGET    = quickqanavad
    win32-msvc*:    TARGET    = quickqanavad
    win32-g++*:     TARGET    = quickqanavad
}


