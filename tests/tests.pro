TEMPLATE    = app
TARGET      = qps2Tests
CONFIG      += warn_on thread c++11
LANGUAGE    = C++
QT          +=  core gui qml quick
INCLUDEPATH += ../src

GMOCK_DIR       =  c:/projects/DELIA/libs/googletest-master/googlemock
GTEST_DIR       =  c:/projects/DELIA/libs/googletest-master/googletest
INCLUDEPATH     += $$GTEST_DIR/include $$GMOCK_DIR/include

DEFINES             += "QUICKPROPERTIES_HAVE_PROTOCOL_BUFFER"
PROTOCOL_BUFFER3_DIR=C:/projects/DELIA/libs/protobuf3
INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR\src
PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/cmake/build/Release
PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR\cmake/build/Debug

SOURCES	+=  qps2Tests.cpp
HEADERS	+=  qps2Tests.h

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../build/ -lquickproperties2d 
    win32-msvc*:    PRE_TARGETDEPS +=  ../build/quickproperties2d.lib
    win32-msvc*:    LIBS	+= ../build/quickproperties2d.lib $$GTEST_DIR/msvc/x64/Debug/gtestd.lib $$GMOCK_DIR/msvc/x64/Debug/gmock.lib $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../build/ -lquickproperties2d
}




