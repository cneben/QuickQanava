TEMPLATE    = app
TARGET      = propsedit
FORMS       =
CONFIG      += qt warn_on thread c++11
LANGUAGE    = C++
QT          += widgets core gui qml quick
INCLUDEPATH +=  ../../src
 
SOURCES	+=  qpsMainWindow.cpp
HEADERS	+=  qpsMainWindow.h

DISTFILES   +=  propsedit.qml

RESOURCES   +=  propsedit.qrc                      \
                ../../src/QuickProperties2.qrc

QT_PRIVATE_HEADERS=

CONFIG(release, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lquickproperties2
    android:        LIBS	+= -L../../build/ -lquickproperties2
    win32-msvc*:    PRE_TARGETDEPS += ../../build/quickproperties2.lib
    win32-msvc*:    LIBS	+= ../../build/quickproperties2.lib
    win32-g++*:     LIBS        += -L../../build/ -lquickproperties2
}

CONFIG(debug, debug|release) {
    linux-g++*:     LIBS	+= -L../../build/ -lquickproperties2d
    android:        LIBS	+= -L../../build/ -lquickproperties2d
    win32-msvc*:    PRE_TARGETDEPS += ../../build/quickproperties2d.lib
    win32-msvc*:    LIBS	+= ../../build/quickproperties2d.lib
    win32-g++*:     LIBS        += -L../../build/ -lquickproperties2d
}
