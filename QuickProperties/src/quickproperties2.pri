
CONFIG      += warn_on qt thread c++11
QT          += core widgets gui qml quick

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/qpsLimit.h                \
            $$PWD/qpsSerializer.h           \
            $$PWD/qpsDataSource.h           \
            $$PWD/qpsCSVDataSource.h        \
            $$PWD/qpsProperties.h           \
            $$PWD/qpsTimeValueMap.h         \
            $$PWD/qpsTimedProperties.h      \
            $$PWD/qpsContainerListModel.h   \
            $$PWD/qpsPropertiesList.h       \
            $$PWD/QuickProperties2.h        \
            $$PWD/quickproperties.pb.h

SOURCES +=  $$PWD/qpsSerializer.cpp             \
            $$PWD/qpsDataSource.cpp             \
            $$PWD/qpsCSVDataSource.cpp          \
            $$PWD/qpsProperties.cpp             \
            $$PWD/qpsTimeValueMap.cpp           \
            $$PWD/qpsTimedProperties.cpp        \
            $$PWD/qpsContainerListModel.cpp     \
            $$PWD/qpsPropertiesList.cpp         \
            $$PWD/quickproperties.pb.cc

OTHER_FILES += ./QuickProperties2

DISTFILES   +=  $$PWD/PropertiesEditor.qml         \
                $$PWD/PropertiesListView.qml       \
                $$PWD/PropertiesView.qml

RESOURCES   +=  $$PWD/QuickProperties2.qrc
