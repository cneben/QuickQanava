TEMPLATE = app

QT += qml quick

CONFIG += c++14

#include(../../quickqanava-common.pri)
#include(../../src/quickqanava.pri)

SOURCES +=  ./navigable.cpp             \
            ../../src/qanNavigable.cpp  \
            ../../src/qanPointGrid.cpp

HEADERS += ../../src/qanNavigable.h \
           ../../src/qanPointGrid.h

OTHER_FILES += ./navigable.qml      \
               ./image.qml          \
               ./grid.qml

RESOURCES += navigable.qrc


