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
// \file	basic.cpp
// \author	benoit@qanava.org
// \date	2016 01 25
//-----------------------------------------------------------------------------

// STD headers
#include <random>

// GTpo headers
#include "gtpoGmlSerializer.h"
#include "gtpoRandomGraph.h"
#include "basic.h"

int	main( int /*argc*/, char** /*argv*/ )
{
    stpo::Graph sg;
    auto n1 = sg.createNode();

    RandomGraph::RandomConfig rc;
    rc.nodeCount = 10;
    rc.outNodeRng = std::make_pair( 1, 3 );
    RandomGraph::generate( sg, rc );

    stpo::Graph::SharedNode ownedN1 = n1.lock();
    if ( ownedN1 ) {
        double nx = stpo::SConfig::getNodeX( ownedN1 );
        double ny = stpo::SConfig::getNodeY( ownedN1 );
        double nw = stpo::SConfig::getNodeWidth( ownedN1 );
        double nh = stpo::SConfig::getNodeHeight( ownedN1 );
        std::cout << "nx=" << nx << " ny=" << ny << " nw=" << nw << " nh=" << nh << std::endl;
    }
    auto n2 = sg.createNode();
    std::cout << sg.getRootNodeCount() << std::endl;

    //auto e1 = sg.createEdge();
    try {
        //gtpo::OutGmlSerializer<stpo::SConfig> gmlOut( "test.xml" );
        //gmlOut.serializeOut( sg );
        //gmlOut.finishOut();
    } catch (...) { }

/*
    std::cout << "Creating qtpo::Graph..." << std::endl;
    qtpo::Graph qg;
    std::cout << "calling insertNode()" << std::endl;
    auto n = qg.insertNode();
    std::cout << "n->setX()" << std::endl;
    n->setX( 10. );
    n->getX();
    std::cout << "n->getX()=" << n->getX( ) << std::endl;
*/
    return 0;
}

