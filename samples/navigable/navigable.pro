TEMPLATE = app

QT += qml quick quickcontrols2

CONFIG += c++14

SOURCES +=  ./navigable.cpp             \
            ../../src/qanNavigable.cpp  \
            ../../src/qanPointGrid.cpp

HEADERS += ../../src/qanNavigable.h \
           ../../src/qanPointGrid.h

OTHER_FILES += ./navigable.qml      \
               ./image.qml          \
               ./grid.qml

RESOURCES += navigable.qrc


