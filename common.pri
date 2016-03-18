
# Win32 MSVC specific Protocol Buffer configuration
win32-msvc*:PROTOCOL_BUFFER3_DIR=C:/projects/DELIA/libs/protobuf3
win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR\src
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/cmake/build/Release
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR\cmake/build/Debug

# Remove C4100 unreferenced formal parameter
win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34100

# Used internally, override if you want to use a different working copy of GTpo
INCLUDEPATH	+= $$PWD/GTpo/src
GTPO_DIR        =  $$PWD/\GTpo/src
INCLUDEPATH	+= $$GTPO_DIR
DEFINES         += "GTPO_HAVE_PROTOCOL_BUFFER"

# Add the QuickQanava2 source directory to include path
INCLUDEPATH     += $$PWD/src
INCLUDEPATH     += $$PWD/QuickProperties/src
