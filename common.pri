
# Win32 MSVC specific Protocol Buffer configuration
win32-msvc*:PROTOCOL_BUFFER3_DIR=C:/projects/DELIA/hlg5-libs/protobuf-3.0.2
win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR/include
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/lib
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR/lib

# Remove C4100 unreferenced formal parameter
win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34100

# Remove C4267 conversion from size_t to int (protobuf)
win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267

# Used internally, override if you want to use a different working copy of GTpo
INCLUDEPATH	+= $$PWD/GTpo/src
GTPO_DIR        =  $$PWD/\GTpo/src
INCLUDEPATH	+= $$GTPO_DIR
DEFINES         += "GTPO_HAVE_PROTOCOL_BUFFER"

# Add the QuickQanava2 source directory to include path
INCLUDEPATH     += $$PWD/src
INCLUDEPATH     += $$PWD/QuickProperties/src

CONFIG(release, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_RELEASE/libprotobuf.lib
}

CONFIG(debug, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
}
