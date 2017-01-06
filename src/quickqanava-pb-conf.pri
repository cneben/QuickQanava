
win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR/include
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/lib
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR/lib

# Remove C4100 unreferenced formal parameter
win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34100

# Remove C4267 conversion from size_t to int (protobuf)
win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267

# Used internally, override if you want to use a different working copy of GTpo
INCLUDEPATH	+= $$GTPO_DIR

contains(DEFINES, QUICKQANAVA_HAS_PROTOBUF) {
CONFIG(release, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_RELEASE/libprotobuf.lib
}
CONFIG(debug, debug|release) {
    win32-msvc*: QMAKE_CXXFLAGS_WARN_ON -= -w34267 -w34100  # Remove C4100 unreferenced formal parameter
    win32:      LIBS	+=  $$PROTOCOL_BUFFER3_LIBDIR_DEBUG/libprotobufd.lib
}
}

INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/qanProtoSerializer.h      \       # Protocol Buffer serialization
            $$PWD/quickqanava.pb.h

SOURCES +=  $$PWD/qanProtoSerializer.cpp    \   # Protocol Buffer serialization
            $$PWD/quickqanava.pb.cc
			
OTHER_FILES +=  $$PWD/quickqanava.proto
