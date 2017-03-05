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
// \file	gtpoEdge.hpp
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* GenEdge Restricted Hyper Edge Management *///-------------------------------
template < class Config >
auto GenEdge< Config >::addInHEdge( WeakEdge inHEdge ) -> void
{
    if ( inHEdge.expired() )
        throw gtpo::bad_topology_error( "gtpo::GenEdge<>::addInHEdge(): Error: Input hyper edge is null." );
    SharedEdge inHEdgePtr{ inHEdge.lock() };
    if ( inHEdgePtr != nullptr ) {
        if ( inHEdgePtr->getHDst().expired() )
            inHEdgePtr->setHDst( this->shared_from_this() );
        Config::template container_adapter< WeakEdges >::insert( inHEdge, _inHEdges );
        if ( !inHEdgePtr->getSrc().expired() )
            Config::template container_adapter< WeakNodes >::insert( inHEdgePtr->getSrc(), _inHNodes );
    }
}

template < class Config >
auto GenEdge< Config >::removeInHEdge( WeakEdge inHEdge ) -> void
{
    if ( inHEdge.expired() )
        return;                 // Do not throw, removing a null inHEdge let edge in a perfectely valid state
    SharedEdge inHEdgePtr{ inHEdge.lock() };
    if ( inHEdgePtr != nullptr ) {
        inHEdgePtr->setHDst( SharedEdge{} );
        Config::template container_adapter< WeakEdges >::remove( inHEdge, _inHEdges );
        Config::template container_adapter< WeakNodes >::remove( inHEdgePtr->getSrc(), _inHNodes );
    }
}
//-----------------------------------------------------------------------------

} // ::gtpo
