TEMPLATE = app

QT += qml quick

CONFIG += c++14

include(../../quickqanava-common.pri)
include(../../src/quickqanava.pri)

SOURCES +=  ./navigable.cpp             \
            ../../src/qanNavigable.cpp

HEADERS += ../../src/qanNavigable.h

OTHER_FILES += ./navigable.qml      \
               ./image.qml          \
               ./grid.qml

RESOURCES += navigable.qrc


