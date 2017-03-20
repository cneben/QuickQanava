<<<<<<< HEAD
TEMPLATE	= app
TARGET		= test-groups
CONFIG		+= qt warn_on thread c++14
QT		+= widgets core gui qml quick
INCLUDEPATH     +=  ../../src
INCLUDEPATH     +=  ../../QuickContainers/src

include(../../quickqanava-common.pri)
include(../../src/quickqanava-conf.pri)

SOURCES     += ./groups.cpp
HEADERS     += ./groups.h
OTHER_FILES += ./main.qml
=======
TEMPLATE    = app
TARGET      = test-groups
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick
INCLUDEPATH +=  ../../src
INCLUDEPATH +=  ../../QuickContainers/src

include(../../src/quickqanava.pri)

SOURCES     += ./groups.cpp
OTHER_FILES += ./groups.qml
>>>>>>> dev
RESOURCES   += ./groups.qrc
