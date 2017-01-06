TEMPLATE    = app
TARGET      = gtpoTests
CONFIG      += warn_on thread c++14
QT          += core

DEPENDPATH  += ../src
INCLUDEPATH += ../src

include (../gtpo-common.pri)

# On win32, set Google Test and Mock source and library directories manually
win32-msvc*:GMOCK_DIR       =  c:/projects/DELIA/libs/googletest-master/googlemock
win32-msvc*:GTEST_DIR       =  c:/projects/DELIA/libs/googletest-master/googletest
win32-msvc*:INCLUDEPATH     += $$GTEST_DIR/include $$GMOCK_DIR/include

SOURCES	+=  gtpoTests.cpp
HEADERS	+=  

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../build/ -lgtpod -lgtest -lgmock -lprotobuf
    win32-msvc*:    PRE_TARGETDEPS +=  ../build/gtpod.lib
    win32-msvc*:    LIBS	+= ../build/gtpod.lib $$GTEST_DIR/msvc/x64/Debug/gtestd.lib $$GMOCK_DIR/msvc/x64/Debug/gmock.lib $$PROTOCOL_BUFFER_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../build/ -lgtpod
}

