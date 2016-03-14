TEMPLATE    = app
TARGET      = test-behaviour
FORMS       =
CONFIG      += qt warn_on thread c++11
QT          += core

DEPENDPATH +=   ../../src
INCLUDEPATH +=  ../../src

SOURCES	+=  behaviour.cpp
HEADERS	+=  behaviour.h

DEFINES     += "GTPO_HAVE_PROTOCOL_BUFFER"
INCLUDEPATH +=C:/projects/DELIA/libs/protobuf3/src
PROTOCOL_BUFFER_LIBDIR_RELEASE = C:/projects/DELIA/libs/protobuf3/cmake/build/Release
PROTOCOL_BUFFER_LIBDIR_DEBUG = C:/projects/DELIA/libs/protobuf3/cmake/build/Debug

CONFIG(release, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lgtpo
    android:        LIBS	+= -L../../build/ -lgtpo
    win32-msvc*:    PRE_TARGETDEPS += ../../build/gtpo.lib
    win32-msvc*:    LIBS	+= ../../build/gtpo.lib $$PROTOCOL_BUFFER_LIBDIR_RELEASE/libprotobuf.lib
    win32-g++*:     LIBS	+= -L../../build/ -lgtpo
}

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lgtpod
    android:        LIBS	+= -L../../build/ -lgtpod
    win32-msvc*:    PRE_TARGETDEPS +=  ../../build/gtpod.lib
    win32-msvc*:    LIBS	+= ../../build/gtpod.lib $$PROTOCOL_BUFFER_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../../build/ -lgtpod
}




