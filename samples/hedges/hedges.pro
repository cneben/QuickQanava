TEMPLATE	= app
TARGET		= test-hedges
CONFIG		+= qt warn_on thread c++14
QT		+= widgets core gui qml quick quickcontrols2

include(../../quickqanava-common.pri)
include(../../src/quickqanava-conf.pri)

SOURCES	+=  ./hedges.cpp
HEADERS	+=  ./hedges.h
OTHER_FILES +=  main.qml
RESOURCES   +=  hedges.qrc
