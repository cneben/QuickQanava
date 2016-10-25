TEMPLATE	= app
TARGET		= test-topology
CONFIG		+= qt warn_on thread c++14
DEFINES		+= QANAVA  
QT              += widgets core gui qml quick charts quickcontrols2

include(../../common.pri)
DEPENDPATH +=   ../../src/

SOURCES     +=  ./qanTopologySample.cpp \
                ./qanImgNode.cpp        \
                ./topology.pb.cc            # Protocol Buffer generated files
HEADERS     +=  ./qanTopologySample.h \
                ./qanImgNode.h

OTHER_FILES +=  main.qml        \
                ImgNode.qml     \
                gtpo.proto      \
                topology.proto

RESOURCES   +=  ../../QuickGeoGL/src/QuickGeoGL.qrc             \
                ../../QuickProperties/src/QuickProperties.qrc   \
                ../../src/QuickQanava2.qrc                      \
                topology.qrc

CONFIG(release, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lprotobuf -lquickqanava2
    android:        LIBS	+= -L../../build/ -lquickqanava2 -lprotobuf
    win32-msvc*:    PRE_TARGETDEPS +=  ../../build/quickqanava2.lib
    win32-msvc*:    LIBS	+= ../../build/quickqanava2.lib $$PROTOCOL_BUFFER3_LIBDIR_RELEASE/libprotobuf.lib
    win32-g++*:     LIBS	+= -L../../build/ -lquickqanava2 -lprotobuf
}

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lquickqanava2d -lprotobuf
    android:        LIBS	+= -L../../build/ -lquickqanava2d -lprotobuf
    win32-msvc*:    PRE_TARGETDEPS += ../../build/quickqanava2d.lib
    win32-msvc*:    LIBS	+= ../../build/quickqanava2d.lib $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
    win32-g++*:     LIBS	+= -L../../build/ -lquickqanava2d -lprotobuf
}
