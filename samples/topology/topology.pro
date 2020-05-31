TEMPLATE    = app
TARGET      = test-topology
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick quickcontrols2

include(../../src/quickqanava.pri)

RESOURCES   += ./topology.qrc           \
               ./theme/qan/qan-theme.qrc

SOURCES     +=  ./qanTopologySample.cpp \
                ./qanFaceNode.cpp

HEADERS     +=  ./qanTopologySample.h   \
                ./qanFaceNode.h         \
                ./main.qml

OTHER_FILES +=  main.qml                \
                FaceNode.qml
