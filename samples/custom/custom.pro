TEMPLATE	= app
TARGET		= test-custom
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

CONFIG(release, debug|release) {
    linux-g++*:     LIBS	+= -lprotobuf
    android:        LIBS	+= -lprotobuf
    win32-msvc*:    LIBS	+= $$PROTOCOL_BUFFER3_LIBDIR_RELEASE/libprotobuf.lib
    win32-g++*:     LIBS	+= -lprotobuf
}

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -lprotobuf
    android:        LIBS	+= -lprotobuf
    win32-msvc*:    LIBS	+= $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -lprotobuf
}
