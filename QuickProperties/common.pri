
# Comment to remove Protocol Buffer serialization support
DEFINES += "QUICKPROPERTIES_HAVE_PROTOCOL_BUFFER"

# Optionnal Protocol Buffer v3 configuration for win32
win32-msvc*:PROTOCOL_BUFFER3_DIR=C:/projects/DELIA/libs/protobuf3
win32-msvc*:INCLUDEPATH     	+= $$PROTOCOL_BUFFER3_DIR\src
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_RELEASE  = $$PROTOCOL_BUFFER3_DIR/cmake/build/Release
win32-msvc*:PROTOCOL_BUFFER3_LIBDIR_DEBUG    = $$PROTOCOL_BUFFER3_DIR\cmake/build/Debug
