# Global GTpo configuration file

# Comment if you do not intent to serialize graph to Protocol Buffer
DEFINES     += "GTPO_HAS_PROTOBUF"

# On win32, manually set the path to protocol buffer source and library directories (on linux,
# GTpo will usually find Protocol Buffer directly if "make install" has been run
win32:INCLUDEPATH += C:/projects/DELIA/hlg5-libs/protobuf-3.0.2/include
win32:PROTOCOL_BUFFER_LIBDIR_RELEASE = C:/projects/DELIA/hlg5-libs/protobuf-3.0.2/lib
win32:PROTOCOL_BUFFER_LIBDIR_DEBUG = C:/projects/DELIA/hlg5-libs/protobuf-3.0.2/lib
