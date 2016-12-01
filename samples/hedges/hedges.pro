TEMPLATE	= app
TARGET		= test-hedges
CONFIG		+= qt warn_on thread c++14
QT		+= widgets core gui qml quick charts quickcontrols2

INCLUDEPATH     +=  ../../src
INCLUDEPATH     +=  ../../QuickProperties/src

include(../../GTpo/src/gtpo.pri)
include(../../QuickProperties/src/quickproperties.pri)
include(../../src/quickqanava.pri)
include(../../common.pri)

SOURCES	+=  ./hedges.cpp
HEADERS	+=  ./hedges.h
OTHER_FILES +=  main.qml
RESOURCES   +=  hedges.qrc
