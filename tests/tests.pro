TEMPLATE    = app
TARGET      = quickanava_tests
CONFIG      += qt warn_on thread c++14
QT          += widgets core gui qml quick quickcontrols2

DEPENDPATH  += ../src
INCLUDEPATH += ../src

include (../src/quickqanava.pri)

SOURCES	+=  ./tests.cpp             \
            ./topology_tests.cpp    \
            ./observers_tests.cpp   \
            ./groups_tests.cpp

CONFIG(debug, debug|release) {
    linux-g++*: LIBS += -L../build/ -lgtest -lgmock
}

