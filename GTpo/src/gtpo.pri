
CONFIG      += warn_on qt thread c++11
QT          += core gui

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/gtpoUtils.h               \
            $$PWD/gtpoEdge.hpp              \
            $$PWD/gtpoGraph.h               \
            $$PWD/gtpoGraph.hpp             \
            $$PWD/gtpoRandomGraph.h         \
            $$PWD/gtpoBehaviour.h           \
            $$PWD/gtpoBehaviour.hpp         \
            $$PWD/gtpoSerializer.h          \
            $$PWD/gtpoGmlSerializer.h       \
            $$PWD/gtpoGmlSerializer.hpp     \
            $$PWD/gtpoProtoSerializer.h     \
            $$PWD/gtpoProtoSerializer.hpp   \
            $$PWD/gtpoNode.hpp              \
            $$PWD/GTpo.h                    \
            $$PWD/GTpoQt.h                  \
            $$PWD/GTpoStd.h

SOURCES +=  $$PWD/gtpoIDInterface.cpp       \
            $$PWD/gtpo.pb.cc                \
            $$PWD/pugixml/src/pugixml.cpp

OTHER_FILES += $$PWD/GTpo          \
                $$PWD/GTpoStd       \
                $$PWD/GTpoQt        \
                $$PWD/gtpo.proto
