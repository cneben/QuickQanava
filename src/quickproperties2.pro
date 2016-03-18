
TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = quickproperties2
DESTDIR     = ../build
CONFIG      += warn_on qt thread staticlib c++11
QT          += core widgets gui qml quick

include(../common.pri)

HEADERS +=  ./qpsLimit.h                \
            ./qpsSerializer.h           \
            ./qpsDataSource.h           \
            ./qpsCSVDataSource.h        \
            ./qpsProperties.h           \
            ./qpsTimeValueMap.h         \
            ./qpsTimedProperties.h      \
            ./qpsContainerListModel.h   \
            ./qpsPropertiesList.h       \
            ./QuickProperties2.h        \
            ./quickproperties.pb.h

SOURCES +=  ./qpsSerializer.cpp             \
            ./qpsDataSource.cpp             \
            ./qpsCSVDataSource.cpp          \
            ./qpsProperties.cpp             \
            ./qpsTimeValueMap.cpp           \
            ./qpsTimedProperties.cpp        \
            ./qpsContainerListModel.cpp     \
            ./qpsPropertiesList.cpp         \
            ./quickproperties.pb.cc

OTHER_FILES += ./QuickProperties2       \
                quickproperties.proto

DISTFILES   +=  ./PropertiesEditor.qml         \
                ./PropertiesListView.qml       \
                ./PropertiesView.qml

CONFIG(release, debug|release) {
    linux-g++*:     TARGET    = quickproperties2
    android:        TARGET    = quickproperties2
    win32-msvc*:    TARGET    = quickproperties2
    win32-g++*:     TARGET    = quickproperties2
}

CONFIG(debug, debug|release) {
    linux-g++*:     TARGET    = quickproperties2d
    android:        TARGET    = quickproperties2d
    win32-msvc*:    TARGET    = quickproperties2d
    win32-g++*:     TARGET    = quickproperties2d
}
