TEMPLATE    = app
TARGET      = test-style
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick quickcontrols2

include(../../src/quickqanava.pri)

SOURCES     += ./style.cpp

OTHER_FILES += 	style.qml

RESOURCES   += style.qrc
