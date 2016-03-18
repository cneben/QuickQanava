
TEMPLATE	= lib
LANGUAGE	= C++
DEFINES		+= QANAVA
TARGET		= gtpo
DESTDIR		= ../build
CONFIG		+= warn_on thread staticlib c++11
QT		+= core 

DEFINES     += "GTPO_HAVE_PROTOCOL_BUFFER"
INCLUDEPATH += c:/projects/DELIA/libs/protobuf3/src

HEADERS +=  ./gtpoUtils.h               \
            ./gtpoEdge.hpp              \
            ./gtpoGraph.h               \
            ./gtpoGraph.hpp             \
            ./gtpoRandomGraph.h         \
            ./gtpoBehaviour.h           \
            ./gtpoBehaviour.hpp         \
            ./gtpoSerializer.h          \
            ./gtpoGmlSerializer.h       \
            ./gtpoGmlSerializer.hpp     \
            ./gtpoProtoSerializer.h     \
            ./gtpoProtoSerializer.hpp   \
            ./gtpoNode.hpp              \
            ./GTpo.h                    \
            ./GTpoQt.h                  \
            ./GTpoStd.h

OTHER_FILES +=  ./GTpo          \
                ./GTpoStd       \
                ./GTpoQt        \
                ./gtpo.proto

SOURCES +=  ./gtpo.pb.cc                \
            ./pugixml/src/pugixml.cpp

CONFIG(release, debug|release) {
    linux-g++*:     TARGET    = gtpo
    android:        TARGET    = gtpo
    win32-msvc*:    TARGET    = gtpo
    win32-g++*:     TARGET    = gtpo
}

CONFIG(debug, debug|release) {
    linux-g++*:     TARGET    = gtpod
    android:        TARGET    = gtpod
    win32-msvc*:    TARGET    = gtpod
    win32-g++*:     TARGET    = gtpod
}
