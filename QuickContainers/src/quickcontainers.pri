
CONFIG      += warn_on qt thread c++14
QT          += core widgets gui qml quick quickcontrols2

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/qcmObjectModel.h               \
            $$PWD/qcmContainerModelListReference.h          \
            $$PWD/qcmAbstractContainerModel.h               \
            $$PWD/qcmContainerConcept.h                     \
            $$PWD/qcmContainerModel.h                       \
            $$PWD/qcmContainerModelComposer.h               \
            $$PWD/qcmContainerModelComposerWatcher.h        \
            $$PWD/qcmContainerModelComposerListReference.h  \
            $$PWD/QuickContainers.h

SOURCES +=  $$PWD/qcmObjectModel.cpp             \
            $$PWD/qcmContainerModelComposerWatcher.cpp

OTHER_FILES +=  $$PWD/QuickContainers

RESOURCES   +=  $$PWD/QuickContainers.qrc
