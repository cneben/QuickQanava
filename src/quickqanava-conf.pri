QT += widgets core gui qml quick quickcontrols2

INCLUDEPATH += $$PWD/../QuickGeoGL
RESOURCES   += $$PWD/../QuickGeoGL/src/QuickGeoGL.qrc

DEPENDPATH  += $$PWD/../src
INCLUDEPATH += $$PWD/../src
RESOURCES   += $$PWD/../src/QuickQanava.qrc

INCLUDEPATH += $$PWD/../GTpo/src
INCLUDEPATH += $$PWD/../QuickContainers/src
RESOURCES   += $$PWD/../QuickContainers/src/QuickContainers.qrc

contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
    message("QuickQanava: Protocol Buffer v3 support enabled.")
    DEFINES += GTPO_HAS_PROTOBUF    # Specific GTpo configuration
    win32-msvc*:INCLUDEPATH += $$PROTOCOL_BUFFER3_DIR/include
    win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/lib
    win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR/lib
}

CONFIG(release, debug|release) {
    linux-g++*:     LIBS	+= -L$$PWD/../build/ -lquickqanava
    android:        LIBS	+= -L$$PWD/../build/ -lquickqanava
    win32-msvc*:    PRE_TARGETDEPS += $$shadowed($$PWD)/../build/quickqanava.lib
    win32-msvc*:    LIBS	+= $$shadowed($$PWD)/../build/quickqanava.lib
    win32-g++*:     LIBS	+= -L$$PWD/../build/ -L$$OUT_PWD/../build/ -lquickqanava
}

CONFIG(debug, debug|release) {
message($$shadowed($$PWD))
    linux-g++*:     LIBS	+= -L$$PWD/../build/ -L$$OUT_PWD/../build/ -lquickqanavad
    android:        LIBS	+= -L$$PWD/../build/ -L$$OUT_PWD/../build/ -lquickqanavad
    win32-msvc*:    PRE_TARGETDEPS += $$shadowed($$PWD)/../build/quickqanavad.lib
    win32-msvc*:    LIBS	+= $$shadowed($$PWD)/../build/quickqanavad.lib
    win32-g++*:     LIBS	+= -L$$OUT_PWD/../../build/ -L$$OUT_PWD/../build/ -lquickqanavad
}
contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
    linux-g++*:     LIBS	+= -lprotobuf
    android:        LIBS	+= -lprotobuf
    win32-g++*:     LIBS	+= -lprotobuf
CONFIG(release, debug|release) {
    win32-msvc*:    LIBS	+= $$PROTOCOL_BUFFER3_LIBDIR_RELEASE/libprotobuf.lib
}
CONFIG(debug, debug|release) {
    win32-msvc*:    LIBS	+= $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
}
}
