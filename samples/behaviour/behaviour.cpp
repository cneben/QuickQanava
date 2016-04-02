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
// \file	behaviour.cpp
// \author	benoit@qanava.org
// \date	2016 03 14
//-----------------------------------------------------------------------------

// STD headers
#include <random>

// GTpo headers
#include "gtpoGmlSerializer.h"
#include "gtpoRandomGraph.h"
#include "behaviour.h"

class EchoBehaviour : public stpo::Graph::Behaviour
{
public:
    void    nodeInserted( stpo::Graph::WeakNode& node ) override { std::cout << "EchoBehaviour::nodeInserted() node=" << node.lock() << std::endl; }
    void    nodeRemoved( stpo::Graph::WeakNode& node ) override { std::cout << "EchoBehaviour::nodeRemoved() node=" << node.lock() << std::endl; }
    void    nodeModified( stpo::Graph::WeakNode& node ) override { std::cout << "EchoBehaviour::nodeModified() node=" << node.lock() << std::endl; }
    void    edgeInserted( stpo::Graph::WeakEdge& edge ) override { std::cout << "EchoBehaviour::edgeInserted() edge=" << edge.lock() << std::endl; }
    void    edgeRemoved( stpo::Graph::WeakEdge& edge ) override { std::cout << "EchoBehaviour::edgeRemoved() edge=" << edge.lock() << std::endl; }
    void    edgeModified( stpo::Graph::WeakEdge& edge ) override { std::cout << "EchoBehaviour::edgeModified() edge=" << edge.lock() << std::endl; }
};

int	main( int /*argc*/, char** /*argv*/ )
{
    stpo::Graph sg;

    auto echoBehaviour = std::make_unique< EchoBehaviour >( );
    sg.addBehaviour( echoBehaviour.release() );

    std::cout << "Expecting nodeInserted() call" << std::endl;
    auto n1 = sg.createNode();
    stpo::Graph::SharedNode sharedN1 = n1.lock();
    if ( sharedN1 ) {
        std::cout << "Expecting nodeModified() call" << std::endl;
        stpo::Config::setNodeX( sharedN1.get(), 1.0 );
        std::cout << "Expecting nodeModified() call" << std::endl;
        stpo::Config::setNodeY( sharedN1.get(), 1.0 );
        std::cout << "Expecting nodeModified() call" << std::endl;
        stpo::Config::setNodeWidth( sharedN1.get(), 1.0 );
        std::cout << "Expecting nodeModified() call" << std::endl;
        stpo::Config::setNodeHeight( sharedN1.get(), 1.0 );
    }

    std::cout << "Expecting nodeInserted() call" << std::endl;
    auto n2 = sg.createNode();
    std::cout << sg.getRootNodeCount() << std::endl;

    std::cout << "Expecting edgeInserted() call" << std::endl;
    auto e1 = sg.createEdge( n1, n2 );
    std::cout << "Expecting edgeModified() call" << std::endl;
    stpo::Graph::SharedEdge sharedE1 = e1.lock();
    stpo::Config::setEdgeWeight( sharedE1.get(), 1.0 );

    std::cout << "Expecting edgeRemoved() call" << std::endl;
    sg.removeEdge( e1 );

    return 0;
}

