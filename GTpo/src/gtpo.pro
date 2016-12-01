
TEMPLATE	= lib
LANGUAGE	= C++
TARGET		= gtpo
DESTDIR		= ../build
CONFIG		+= warn_on thread staticlib c++14
QT		+= core 

include (../common-gtpo.pri)

HEADERS +=  ./gtpoUtils.h               \
            ./gtpoGraph.h               \
            ./gtpoGraph.hpp             \
            ./gtpoEdge.hpp              \
            ./gtpoNode.hpp              \
            ./gtpoGroup.hpp             \
            ./gtpoRandomGraph.h         \
            ./gtpoBehaviour.h           \
            ./gtpoBehaviour.hpp         \
            ./gtpoSerializer.h          \
            ./gtpoProgressNotifier.h    \
            ./gtpoGmlSerializer.h       \
            ./gtpoGmlSerializer.hpp     \
            ./gtpoProtoSerializer.h     \
            ./gtpoProtoSerializer.hpp   \
            ./GTpo.h                    \
            ./GTpoQt.h                  \
            ./GTpoStd.h

OTHER_FILES +=  ./GTpo          \
                ./GTpoStd       \
                ./GTpoQt        \
                ./gtpo.proto    \
                ../common.pri   \
                ./GTpo.pri

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
