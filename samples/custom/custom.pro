TEMPLATE	= app
TARGET		= test-custom
FORMS		=
CONFIG		+= qt warn_on thread c++14
DEFINES		+= QANAVA  
LANGUAGE	= C++
QT		+= widgets core gui qml quick quickwidgets

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
    linux-g++*:     LIBS	+= -L../../build/ -lquickqanava2 
    android:        LIBS	+= -L../../build/ -lquickqanava2 
    win32-msvc*:    PRE_TARGETDEPS +=  ../../build/quickqanava2.lib
    win32-msvc*:    LIBS	+= ../../build/quickqanava2.lib
    win32-g++*:     LIBS	+= -L../../build/ -lquickqanava2 
}

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lquickqanava2d
    android:        LIBS	+= -L../../build/ -lquickqanava2d
    win32-msvc*:    PRE_TARGETDEPS += ../../build/quickqanava2d.lib
    win32-msvc*:    LIBS	+=  ../../build/quickqanava2d.lib
    win32-g++*:     LIBS	+= -L../../build/ -lquickqanava2d 
}
