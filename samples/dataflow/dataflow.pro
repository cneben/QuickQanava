TEMPLATE    = app
TARGET      = test-dataflow
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick

include(../../src/quickqanava.pri)

SOURCES     +=  dataflow.cpp    \
                qanFlowNode.cpp
				
HEADERS     +=  qanFlowNode.h

OTHER_FILES +=  dataflow.qml FlowNode.qml PercentageNode.qml OperationNode.qml ImageNode.qml TintNode.qml

RESOURCES   +=  dataflow.qrc

