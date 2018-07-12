
CONFIG      += warn_on qt thread c++14
QT          += core

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/utils.h                \
            $$PWD/config.h               \
            $$PWD/edge.h                 \
            $$PWD/edge.hpp               \
            $$PWD/node.h                 \
            $$PWD/node.hpp               \
            $$PWD/group.h                \
            $$PWD/group.hpp              \
            $$PWD/graph.h                \
            $$PWD/graph.hpp              \
            $$PWD/graph_property.h       \
            $$PWD/algorithm.h            \
            $$PWD/algorithm.hpp          \
            $$PWD/generator.h            \
            $$PWD/behaviour.h            \
            $$PWD/behaviourable.h        \
            $$PWD/behaviourable.hpp      \
            $$PWD/graph_behaviour.h      \
            $$PWD/graph_behaviour.hpp    \
            $$PWD/node_behaviour.h       \
            $$PWD/node_behaviour.hpp     \
            $$PWD/group_behaviour.h      \
            $$PWD/group_behaviour.hpp    \
            $$PWD/adjacent_behaviour.h   \
            $$PWD/adjacent_behaviour.hpp \
            $$PWD/container_adapter.h    \
            $$PWD/GTpo.h

OTHER_FILES += $$PWD/GTpo

