TEMPLATE	= app
TARGET		= test-custom
CONFIG		+= qt warn_on thread c++14
QT		+= widgets core gui qml quick

include(../../quickqanava-common.pri)
include(../../src/quickqanava.pri)

SOURCES	+=  custom.cpp

OTHER_FILES   +=  custom.qml        \
                  CustomNode.qml    \
                  ControlNode.qml   \
                  DiamondNode.qml

RESOURCES   +=  custom.qrc

