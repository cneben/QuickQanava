
CONFIG      += warn_on qt thread c++14
QT          += core gui

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/gtpoUtils.h               \
<<<<<<< HEAD
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
=======
            $$PWD/gtpoGraphConfig.h         \
            $$PWD/gtpoGenEdge.h             \
            $$PWD/gtpoGenEdge.hpp           \
            $$PWD/gtpoGenNode.h             \
            $$PWD/gtpoGenNode.hpp           \
            $$PWD/gtpoGenGroup.h            \
            $$PWD/gtpoGenGroup.hpp          \
            $$PWD/gtpoGenGraph.h            \
            $$PWD/gtpoGenGraph.hpp          \
            $$PWD/gtpoBehaviour.h           \
            $$PWD/gtpoBehaviour.hpp         \
            $$PWD/gtpoGraphBehaviour.h      \
            $$PWD/gtpoGraphBehaviour.hpp    \
            $$PWD/gtpoNodeBehaviour.h       \
            $$PWD/gtpoNodeBehaviour.hpp     \
            $$PWD/gtpoGroupBehaviour.h      \
            $$PWD/gtpoGroupBehaviour.hpp    \
            $$PWD/gtpoAdjacentBehaviour.h   \
            $$PWD/gtpoAdjacentBehaviour.hpp \
            $$PWD/gtpoProgressNotifier.h    \
            $$PWD/gtpoSerializer.h          \
            $$PWD/gtpoContainerAdapter.h    \
            $$PWD/gtpoSerializer.h          \
            $$PWD/GTpo.h
>>>>>>> dev

SOURCES +=

contains(DEFINES, GTPO_HAS_PROTOBUF) {
    message("GTpo: Protocol Buffer v3 support enabled.")
<<<<<<< HEAD
    HEADERS +=  $$PWD/gtpoProtoSerializer.h     \
                $$PWD/gtpoProtoSerializer.hpp   \
                $$PWD/gtpo.pb.h
    SOURCES += $$PWD/gtpo.pb.cc
    OTHER_FILES += $$PWD/gtpo.proto
}

OTHER_FILES += $$PWD/GTpo       \
                $$PWD/GTpoStd   \
                $$PWD/GTpoQt
=======
    HEADERS +=  $$PWD/protobuf/gtpoProtoSerializer.h     \
                $$PWD/protobuf/gtpoProtoSerializer.hpp   \
                $$PWD/protobuf/gtpo.pb.h
    SOURCES += $$PWD/protobuf/gtpo.pb.cc
    OTHER_FILES += $$PWD/protobuf/gtpo.proto
}

OTHER_FILES += $$PWD/GTpo
>>>>>>> dev

