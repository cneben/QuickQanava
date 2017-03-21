
CONFIG      += warn_on qt thread c++14
QT          += core gui

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/gtpoUtils.h               \
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
            $$PWD/gtpoContainerAdapter.h    \
            $$PWD/GTpo.h

OTHER_FILES += $$PWD/GTpo

