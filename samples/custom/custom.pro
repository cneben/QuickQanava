TEMPLATE	= app
TARGET		= test-custom
FORMS		=
CONFIG		+= qt warn_on thread c++14
DEFINES		+= QANAVA  
LANGUAGE	= C++
QT		+= widgets core gui qml quick charts

include(../../GTpo/src/gtpo.pri)
include(../../QuickProperties/src/quickproperties.pri)
include(../../QuickProperties/src/quickpropertiespb.pri)
include(../../src/quickqanava.pri)
include(../../common.pri)

SOURCES	+=  qanMainWindow.cpp
HEADERS	+=  qanMainWindow.h

OTHER_FILES   +=  main.qml          \
                  CustomNode.qml    \
                  ControlNode.qml   \
                  DiamondNode.qml

RESOURCES   +=  ../../src/QuickQanava2.qrc                       \
                custom.qrc


