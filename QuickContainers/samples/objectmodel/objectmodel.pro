TEMPLATE    = app
TARGET      = objectmodel
FORMS       =
CONFIG      += qt warn_on thread c++14
LANGUAGE    = C++
QT          += widgets core gui qml quick charts quickcontrols2
INCLUDEPATH +=  ../../src

include(../../quickcontainers-common.pri)
include(../../src/quickcontainers.pri)
 
SOURCES	+=  qcmObjectModelSample.cpp
HEADERS	+=  qcmObjectModelSample.h

DISTFILES   +=  objectmodel.qml

RESOURCES   +=  objectmodel.qrc
