TEMPLATE    = app
TARGET      = test-40k
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick quickcontrols2

include(../../quickqanava-common.pri)
include(../../src/quickqanava.pri)

RESOURCES   += ./40k.qrc

SOURCES     += ./qan40kSample.cpp

HEADERS     += ./qan40kSample.h     \
               ./40k.qml

OTHER_FILES += ./40k.qml
