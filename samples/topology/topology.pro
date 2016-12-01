TEMPLATE	= app
TARGET		= test-topology
CONFIG		+= qt warn_on thread c++14
QT              += widgets core gui qml quick quickcontrols2

include(../../quickqanava-common.pri)
include(../../src/quickqanava-conf.pri)

RESOURCES   += ./topology.qrc

SOURCES     +=  ./qanTopologySample.cpp \
                ./qanImgNode.cpp

HEADERS     +=  ./qanTopologySample.h   \
                ./qanImgNode.h          \
                ./main.qml              \
                ./ImgNode.qml

contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
SOURCES     +=  ./topology.pb.cc            # Protocol Buffer generated files
HEADERS     +=  ./topology.pb.h
}

OTHER_FILES +=  main.qml        \
                ImgNode.qml     \
                topology.proto

