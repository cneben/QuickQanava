TEMPLATE    = app
TARGET      = gtpoBenchmarks
CONFIG      += warn_on thread c++14
QT          += core

DEPENDPATH  += ../src
INCLUDEPATH += ../src

include (../common.pri)

# On win32, set Google Benchmarks source and library directories manually
win32-msvc*:GBENCHMARK_DIR =  c:/projects/DELIA/libs/benchmark
win32-msvc*:INCLUDEPATH     += $$GBENCHMARK_DIR/include

SOURCES	+=  gtpoBenchmarks.cpp
HEADERS	+=  

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../build/ -lbenchmark -lprotobuf
    win32-msvc*:    PRE_TARGETDEPS +=  ../build/gtpod.lib
    win32-msvc*:    LIBS	+= ../build/gtpod.lib $$GBENCHMARK_DIR/src/Debug/benchmark.lib Shlwapi.lib $$PROTOCOL_BUFFER_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../build/ -lgtpod
}

