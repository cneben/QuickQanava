TEMPLATE    = app
TARGET      = test-topology
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick quickcontrols2

include(../../src/quickqanava.pri)

<<<<<<< HEAD
RESOURCES   += ./topology.qrc
=======
RESOURCES   += ./topology.qrc           \
               ./theme/qan/qan-theme.qrc
>>>>>>> devel

SOURCES     +=  ./qanTopologySample.cpp \
                ./qanFaceNode.cpp

HEADERS     +=  ./qanTopologySample.h   \
                ./qanFaceNode.h         \
                ./main.qml

<<<<<<< HEAD
OTHER_FILES +=  main.qml        \
=======
OTHER_FILES +=  main.qml                \
>>>>>>> devel
                FaceNode.qml
