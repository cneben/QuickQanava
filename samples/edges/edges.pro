TEMPLATE    = app
TARGET      = test-edges
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick

include(../../src/quickqanava.pri)

SOURCES     +=  edges.cpp

OTHER_FILES +=  edges.qml

RESOURCES   +=  edges.qrc

