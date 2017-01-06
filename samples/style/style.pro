TEMPLATE	= app
TARGET		= test-style
CONFIG		+= qt warn_on thread c++14
QT              += widgets core gui qml quick quickcontrols2

include(../../quickqanava-common.pri)
include(../../src/quickqanava.pri)

SOURCES     += ./qanStyleSample.cpp
HEADERS     += ./qanStyleSample.h

OTHER_FILES += 	main.qml

RESOURCES   += style.qrc
