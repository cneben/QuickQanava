TEMPLATE	= app
TARGET		= test-groups
CONFIG		+= qt warn_on thread c++14
QT		+= widgets core gui qml quick
INCLUDEPATH     +=  ../../src
INCLUDEPATH     +=  ../../QuickProperties/src

include(../../quickqanava-common.pri)
include(../../src/quickqanava-conf.pri)

SOURCES     += ./groups.cpp
HEADERS     += ./groups.h
OTHER_FILES += ./main.qml
RESOURCES   += ./groups.qrc
