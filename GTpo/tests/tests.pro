TEMPLATE    = app
TARGET      = gtpoTests
CONFIG      += warn_on thread c++14
QT          += core

DEPENDPATH  += ../src
INCLUDEPATH += ../src

include (../src/gtpo.pri)

# On win32, set Google Test and Mock source and library directories manually
#win32-msvc*:GMOCK_DIR       =  path/to/gmock
#win32-msvc*:GTEST_DIR       =  path/to/gtest
#win32-msvc*:INCLUDEPATH     += $$GTEST_DIR/include $$GMOCK_DIR/include

SOURCES	+=  ./gtpoTests.cpp             \
            ./gtpo_behaviours_tests.cpp \
            ./gtpo_config_tests.cpp     #\
            #./gtpoContainers.cpp    \
            #./gtpoTopology.cpp      \
            #./gtpoGraph.cpp         \
            #./gtpoGroups.cpp        \
            #./gtpoConcrete.cpp

HEADERS	+=  

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../build/ -lgtest -lgmock

    win32-msvc*:    PRE_TARGETDEPS +=  ../build/gtpod.lib
    win32-msvc*:    LIBS	+= ../build/gtpod.lib $$GTEST_DIR/msvc/x64/Debug/gtestd.lib $$GMOCK_DIR/msvc/x64/Debug/gmock.lib
}

