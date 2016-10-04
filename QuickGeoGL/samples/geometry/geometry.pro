TEMPLATE	= app
TARGET		= sample-geometry
CONFIG		+= qt warn_on thread c++14
QT		+= widgets core gui qml quick

include(../../src/quickgeogl.pri)

INCLUDEPATH +=  ../../src

SOURCES +=  ./geometry.cpp
			
HEADERS	+=  ./geometry.h

DISTFILES   +=  geometry.qml   	\
                lines.qml       \
                polylines.qml   \
                arrows.qml      \
                polygons.qml

RESOURCES   +=  geometry.qrc                  \
                ../../src/QuickGeoGL.qrc

QT_PRIVATE_HEADERS=





