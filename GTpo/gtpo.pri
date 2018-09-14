
CONFIG      += warn_on qt thread c++14
QT          += core

INCLUDEPATH += $$PWD/src        # Project using this pri could include <gtpo/GTpo> just as if the library was installed in /usr/include/gtpo

HEADERS +=  $$PWD/src/gtpo/utils.h                \
            $$PWD/src/gtpo/config.h               \
            $$PWD/src/gtpo/edge.h                 \
            $$PWD/src/gtpo/edge.hpp               \
            $$PWD/src/gtpo/node.h                 \
            $$PWD/src/gtpo/node.hpp               \
            $$PWD/src/gtpo/group.h                \
            $$PWD/src/gtpo/group.hpp              \
            $$PWD/src/gtpo/graph.h                \
            $$PWD/src/gtpo/graph.hpp              \
            $$PWD/src/gtpo/graph_property.h       \
            $$PWD/src/gtpo/algorithm.h            \
            $$PWD/src/gtpo/algorithm.hpp          \
            $$PWD/src/gtpo/functional.h           \
            $$PWD/src/gtpo/generator.h            \
            $$PWD/src/gtpo/generator.hpp          \
            $$PWD/src/gtpo/behaviour.h            \
            $$PWD/src/gtpo/behaviourable.h        \
            $$PWD/src/gtpo/behaviourable.hpp      \
            $$PWD/src/gtpo/graph_behaviour.h      \
            $$PWD/src/gtpo/graph_behaviour.hpp    \
            $$PWD/src/gtpo/node_behaviour.h       \
            $$PWD/src/gtpo/node_behaviour.hpp     \
            $$PWD/src/gtpo/group_behaviour.h      \
            $$PWD/src/gtpo/group_behaviour.hpp    \
            $$PWD/src/gtpo/adjacent_behaviour.h   \
            $$PWD/src/gtpo/adjacent_behaviour.hpp \
            $$PWD/src/gtpo/container_adapter.h    \
            $$PWD/src/gtpo/GTpo.h

OTHER_FILES += $$PWD/src/gtpo/GTpo

