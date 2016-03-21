/*
    The MIT License (MIT)

    Copyright (c) 2016 Benoit AUTHEMAN

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the GTpo software library.
//
// \file	serializer.cpp
// \author	benoit@qanava.org
// \date	2016 03 21
//-----------------------------------------------------------------------------

// STD headers
#include <random>

// GTpo headers
#include "gtpoGmlSerializer.h"
#include "gtpoProtoSerializer.h"
#include "serializer.h"

int	main( int /*argc*/, char** /*argv*/ )
{
    stpo::Graph sg;
    RandomGraph::RandomConfig rc;
    rc.nodeCount = 10;
    rc.outNodeRng = std::make_pair( 1, 3 );
    RandomGraph::generate( sg, rc );

    { // GraphML OUT serialization
        try {
            // Create a std::cout progress notifier and register it to GraphML out serializer
            gtpo::EchoProgressNotifier progressNotifier;
            gtpo::OutGmlSerializer<stpo::SConfig> gmlOut( "gmlout.graphml" );
            gmlOut.serializeOut( sg, &progressNotifier );
            gmlOut.finishOut();
        } catch (...) { }
    }

    { // Protocol Buffer IN/OUT serialization
        stpo::Graph sgi;
        gtpo::ProtoSerializer<stpo::SConfig> ps;
        ps.serializeOutTo( sg, std::string( "test.gtpo" ) );
        ps.serializeInFrom( std::string( "test.gtpo" ), sgi );
    }

    return 0;
}

