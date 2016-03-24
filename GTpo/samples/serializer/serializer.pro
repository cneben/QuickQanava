TEMPLATE    = app
TARGET      = test-serializer
FORMS       =
CONFIG      += warn_on thread c++11
QT          += core

DEPENDPATH +=   ../../src
INCLUDEPATH +=  ../../src

SOURCES	+=  serializer.cpp
HEADERS	+=  serializer.h

include (../../common.pri)

CONFIG(release, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lgtpo -lprotobuf
    android:        LIBS	+= -L../../build/ -lgtpo -lprotobuf
    win32-msvc*:    PRE_TARGETDEPS += ../../build/gtpo.lib
    win32-msvc*:    LIBS	+= ../../build/gtpo.lib $$PROTOCOL_BUFFER_LIBDIR_RELEASE/libprotobuf.lib
    win32-g++*:     LIBS	+= -L../../build/ -lgtpo
}

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lgtpod -lprotobuf
    android:        LIBS	+= -L../../build/ -lgtpod -lprotobuf
    win32-msvc*:    PRE_TARGETDEPS +=  ../../build/gtpod.lib
    win32-msvc*:    LIBS	+= ../../build/gtpod.lib $$PROTOCOL_BUFFER_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../../build/ -lgtpod
}




