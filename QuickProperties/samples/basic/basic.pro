QT += core qml
QT -= gui
CONFIG += c++11

TARGET = basic
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += qpsBasic.cpp
HEADERS += qpsBasic.h

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lquickproperties2d
    win32-msvc*:    PRE_TARGETDEPS +=  ../../build/quickproperties2d.lib
    win32-msvc*:    LIBS	+= ../../build/quickproperties2d.lib
    win32-g++*:     LIBS	+= -L../../build/ -lquickproperties2d
}
