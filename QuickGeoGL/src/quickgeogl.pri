
CONFIG      += warn_on qt thread c++14
QT          += core widgets gui qml quick

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/QuickGeoGL                \
            $$PWD/QuickGeoGL.h              \
            $$PWD/qglSGLineNode.h           \
            $$PWD/qglSGLineAAMaterial.h     \
            $$PWD/qglSGPolyLineNode.h       \
            $$PWD/qglSGPolyLineAAMaterial.h \
            $$PWD/qglSGConvexPolygonNode.h  \
            $$PWD/qglConvexPolygon.h        \
            $$PWD/qglConvexPolygonBorder.h  \
            $$PWD/qglLine.h                 \
            $$PWD/qglPolyLine.h             \
            $$PWD/qglArrow.h

SOURCES +=  $$PWD/qglSGLineNode.cpp             \
            $$PWD/qglSGLineAAMaterial.cpp       \
            $$PWD/qglSGPolyLineNode.cpp         \
            $$PWD/qglSGPolyLineAAMaterial.cpp   \
            $$PWD/qglSGConvexPolygonNode.cpp    \
            $$PWD/qglConvexPolygon.cpp          \
            $$PWD/qglLine.cpp                   \
            $$PWD/qglPolyLine.cpp               \
            $$PWD/qglArrow.cpp

OTHER_FILES +=  $$PWD/qglLineAAGsh.glsl     \
                $$PWD/qglLineAAVsh.glsl     \
                $$PWD/qglLineAAFsh.glsl     \
                $$PWD/qglPolyLineAAGsh.glsl \
                $$PWD/qglPolyLineAAVsh.glsl \
                $$PWD/qglPolyLineAAFsh.glsl

RESOURCES   +=  $$PWD/QuickGeoGL.qrc

