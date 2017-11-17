
TEMPLATE    = lib
TARGET      = quickcontainers
DESTDIR     = ../build
CONFIG      += staticlib c++14
QT          += core widgets gui qml quick
QT          += charts datavisualization     # Force building charts and dataviz adapters

include(../quickcontainers-common.pri)

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

CONFIG(release, debug|release) {
    linux-g++*:     TARGET    = quickcontainers
    android:        TARGET    = quickcontainers
    win32-msvc*:    TARGET    = quickcontainers
    win32-g++*:     TARGET    = quickcontainers
}

CONFIG(debug, debug|release) {
    linux-g++*:     TARGET    = quickcontainers
    android:        TARGET    = quickcontainers
    win32-msvc*:    TARGET    = quickcontainers
    win32-g++*:     TARGET    = quickcontainers
}
