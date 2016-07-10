
TEMPLATE	= lib
LANGUAGE	= C++
DEFINES		+= QANAVA
TARGET		= quickqanava2
DESTDIR		= ../build
CONFIG		+= warn_on qt thread staticlib c++11
QT		+= core widgets gui xml qml quick quickwidgets

include(../common.pri)
include(../GTpo/src/gtpo.pri)
include(../QuickProperties/src/quickproperties2.pri)
INCLUDEPATH += ../QuickProperties/src

HEADERS +=  ./QuickQanava.h             \
            ./qanConfig.h               \
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
            ./fqlBottomRightResizer.h   \
            ./qanProtoSerializer.h      \       # Protocol Buffer serialization
            ./quickqanava.pb.h

SOURCES +=  ./qanEdge.cpp               \
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
            ./fqlBottomRightResizer.cpp \
            ./qanProtoSerializer.cpp    \   # Protocol Buffer serialization
            ./quickqanava.pb.cc

OTHER_FILES +=  quickqanava.proto       \
                QuickQanava             \
                Graph.qml               \
                RectNodeTemplate.qml    \
                CanvasNodeTemplate.qml  \
                Group.qml               \
                RectGroupTemplate.qml   \
                Node.qml                \
                Edge.qml                \
                StyleListView.qml       \
                ConnectorDropNode.qml

TRANSLATIONS = ./quickqanava-fr_FR.ts
lupdate_only {
    SOURCES = *.qml
}

CONFIG(release, debug|release) {
    linux-g++*:     TARGET    = quickqanava2
    android:        TARGET    = quickqanava2
    win32-msvc*:    TARGET    = quickqanava2
    win32-g++*:     TARGET    = quickqanava2
}

CONFIG(debug, debug|release) {
    linux-g++*:     TARGET    = quickqanava2d
    android:        TARGET    = quickqanava2d
    win32-msvc*:    TARGET    = quickqanava2d
    win32-g++*:     TARGET    = quickqanava2d
}


