TEMPLATE    = app
TARGET      = gtpoTests
CONFIG      += warn_on thread c++11
QT          += core

DEPENDPATH  += ../src
INCLUDEPATH += ../src

DEFINES     += "GTPO_HAVE_PROTOCOL_BUFFER"
INCLUDEPATH += c:/projects/DELIA/libs/protobuf3/src

GMOCK_DIR       =  c:/projects/DELIA/libs/googletest-master/googlemock
GTEST_DIR       =  c:/projects/DELIA/libs/googletest-master/googletest
INCLUDEPATH     += $$GTEST_DIR/include $$GMOCK_DIR/include
PROTOCOL_BUFFER_LIBDIR_RELEASE = C:/projects/DELIA/libs/protobuf3/cmake/build/Release
PROTOCOL_BUFFER_LIBDIR_DEBUG = C:/projects/DELIA/libs/protobuf3/cmake/build/Debug

SOURCES	+=  gtpoTests.cpp
HEADERS	+=  

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../build/ -lgtpod 
    win32-msvc*:    PRE_TARGETDEPS +=  ../build/gtpod.lib
    win32-msvc*:    LIBS	+= ../build/gtpod.lib $$GTEST_DIR/msvc/x64/Debug/gtestd.lib $$GMOCK_DIR/msvc/x64/Debug/gmock.lib $$PROTOCOL_BUFFER_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../build/ -lgtpod
}




