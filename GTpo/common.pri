# Global GTpo configuration file

# Comment if you do not intent to serialize graph to Protocol Buffer
DEFINES     += "GTPO_HAVE_PROTOCOL_BUFFER"

# On win32, manually set the path to protocol buffer source and library directories (on linux,
# GTpo will usually find Ptocol Buffer directly if "make install" has been run
INCLUDEPATH += c:/projects/DELIA/libs/protobuf3/src
PROTOCOL_BUFFER_LIBDIR_RELEASE = C:/projects/DELIA/libs/protobuf3/cmake/build/Release
PROTOCOL_BUFFER_LIBDIR_DEBUG = C:/projects/DELIA/libs/protobuf3/cmake/build/Debug
