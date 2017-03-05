TEMPLATE	= app
TARGET		= test-progress
CONFIG		+= qt warn_on thread c++14
QT              += widgets core gui qml quick

include(../../quickqanava-common.pri)
include(../../src/quickqanava-conf.pri)

SOURCES     +=  ./qanProgressSample.cpp 
HEADERS     +=  ./qanProgressSample.h

OTHER_FILES +=  ./main.qml

RESOURCES   +=  ./progress.qrc


