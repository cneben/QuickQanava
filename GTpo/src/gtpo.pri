
CONFIG      += warn_on qt thread c++14
QT          += core gui

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/gtpoUtils.h               \
            $$PWD/gtpoEdge.hpp              \
            $$PWD/gtpoGraph.h               \
            $$PWD/gtpoGraph.hpp             \
            $$PWD/gtpoRandomGraph.h         \
            $$PWD/gtpoBehaviour.h           \
            $$PWD/gtpoBehaviour.hpp         \
            $$PWD/gtpoProgressNotifier.h    \
            $$PWD/gtpoSerializer.h          \
            $$PWD/gtpoNode.hpp              \
            $$PWD/gtpoGroup.hpp             \
            $$PWD/GTpo.h                    \
            $$PWD/GTpoQt.h                  \
            $$PWD/GTpoStd.h

SOURCES +=

contains(DEFINES, GTPO_HAS_PROTOBUF) {
    message("GTpo: Protocol Buffer v3 support enabled.")
    HEADERS +=  $$PWD/gtpoProtoSerializer.h     \
                $$PWD/gtpoProtoSerializer.hpp   \
                $$PWD/gtpo.pb.h
    SOURCES += $$PWD/gtpo.pb.cc
    OTHER_FILES += $$PWD/gtpo.proto
}

OTHER_FILES += $$PWD/GTpo       \
                $$PWD/GTpoStd   \
                $$PWD/GTpoQt

