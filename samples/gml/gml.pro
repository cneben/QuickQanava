TEMPLATE	= app
TARGET		= test-gml
FORMS		=
CONFIG		+= qt warn_on thread c++11
DEFINES		+= QANAVA  
LANGUAGE	= C++
QT		+= widgets core gui qml quick quickwidgets
INCLUDEPATH     +=  ../../src
INCLUDEPATH     +=  ../../QuickProperties/src

include(../../common.pri)
GTPO_DIR        =  ../../GTpo/src
INCLUDEPATH	+= $$GTPO_DIR

SOURCES	+=  ./gml.cpp
HEADERS	+=  ./gml.h

OTHER_FILES   +=  main.qml

RESOURCES   +=  ../../src/QuickQanava2.qrc                       \
                gml.qrc

QT_PRIVATE_HEADERS=

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
    win32-msvc*:    LIBS	+= ../../build/quickqanava2d.lib
    win32-g++*:     LIBS	+= -L../../build/ -lquickqanava2d 
}




