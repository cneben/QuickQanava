TEMPLATE	= app
TARGET		= test-style
FORMS		=
CONFIG		+= qt warn_on thread c++11
DEFINES		+= QANAVA  
LANGUAGE	= C++
QT              += widgets core gui qml quick quickwidgets
INCLUDEPATH     +=  ../../src
INCLUDEPATH     +=  ../../QuickProperties2

GTPO_LIBDIR_DEBUG   =c:\projects\DELIA\build-GTpo-Desktop_Qt_5_6_0_MSVC2015_64bit-Debug\build
GTPO_DIR            =c:\projects\DELIA\GTpo\src
INCLUDEPATH	+=$$GTPO_DIR
DEFINES         += "GTPO_HAVE_PROTOCOL_BUFFER"

PROTOCOL_BUFFER3_DIR=C:/projects/DELIA/libs/protobuf3
INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR\src
PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/cmake/build/Release
PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR\cmake/build/Debug

SOURCES	+=  ./qanStyleSample.cpp
HEADERS	+=  ./qanStyleSample.h

OTHER_FILES   +=  main.qml

RESOURCES   +=  ../../QuickProperties/src/QuickProperties2.qrc      \
                ../../src/QuickQanava2.qrc                          \
                style.qrc

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
    win32-msvc*:    LIBS	+= ../../build/quickqanava2d.lib $(GTPO_DEBUGLIBDIR)/gtpod.lib $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../../build/ -lquickqanava2d 
}




