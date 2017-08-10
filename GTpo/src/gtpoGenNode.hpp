/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the GTpo software.
//
// \file	gtpoNode.hpp
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* GenNode Edges Management *///-----------------------------------------------
template < class Config, class ConcreteNode >
auto GenNode< Config, ConcreteNode >::addOutEdge( WeakEdge outEdgePtr ) -> void
{
    assert_throw( !outEdgePtr.expired(), "gtpo::GenNode<>::addOutEdge(): Error: out edge is expired." );
    auto node = this->shared_from_this();
    auto outEdge = outEdgePtr.lock( );
    auto outEdgeSrc = outEdge->getSrc().lock();
    if ( outEdge ) {
        if ( !outEdgeSrc || outEdgeSrc != node )  // Out edge source should point to target node
            outEdge->setSrc( node );
        Config::template container_adapter< WeakEdges >::insert( outEdgePtr, _outEdges );
        if ( !outEdge->getDst().expired() ) {
            Config::template container_adapter< WeakNodes >::insert( outEdge->getDst(), _outNodes );
            this->notifyOutNodeInserted( outEdge->getDst(), WeakEdge{outEdge} );
        }
    }
}

template < class Config, class ConcreteNode >
auto GenNode< Config, ConcreteNode >::addInEdge( WeakEdge inEdgePtr ) -> void
{
    assert_throw( !inEdgePtr.expired(), "gtpo::GenNode<>::addInEdge(): Error: in edge is expired." );
    auto node = this->shared_from_this();
    auto inEdge = inEdgePtr.lock( );
    if ( inEdge ) {
        auto inEdgeDst = inEdge->getDst().lock();
        if ( !inEdgeDst ||
             inEdgeDst != node ) // In edge destination should point to target node
            inEdge->setDst( node );
        Config::template container_adapter< WeakEdges >::insert( inEdgePtr, _inEdges );
        if ( !inEdge->getSrc().expired() ) {
            Config::template container_adapter< WeakNodes >::insert( inEdge->getSrc(), _inNodes );
            this->notifyInNodeInserted( inEdge->getSrc(), inEdgePtr );
        }
    }
}

template < class Config, class ConcreteNode >
auto GenNode< Config, ConcreteNode >::removeOutEdge( const WeakEdge outEdge ) -> void
{
    gtpo::assert_throw( !outEdge.expired(), "gtpo::GenNode<>::removeOutEdge(): Error: Out edge has expired" );
    auto outEdgePtr = outEdge.lock( );
    auto outEdgeSrcPtr = outEdgePtr->getSrc().lock();
    gtpo::assert_throw( outEdgeSrcPtr != nullptr &&    // Out edge src must be this node
                        outEdgeSrcPtr.get() == this, "gtpo::GenNode<>::removeOutEdge(): Error: Out edge source is expired or different from this node.");

    auto outEdgeDst = outEdgePtr->getDst().lock();
    if ( outEdgeDst != nullptr ) {
        gtpo::assert_throw( outEdgeDst != nullptr, "gtpo::GenNode<>::removeOutEdge(): Error: Out edge destination is expired." );
        this->notifyOutNodeRemoved( outEdgePtr->getDst(), outEdge );
    }
    Config::template container_adapter<WeakEdges>::remove( outEdge, _outEdges );
    Config::template container_adapter<WeakNodes>::remove( outEdgePtr->getDst(), _outNodes );
    if ( getInDegree() == 0 ) {
        Graph* graph{ getGraph() };
        if ( graph != nullptr )
            graph->installRootNode( WeakNode{ this->shared_from_this() } );
    }
    this->notifyOutNodeRemoved();
}

template < class Config, class ConcreteNode >
auto GenNode< Config, ConcreteNode >::removeInEdge( const WeakEdge inEdge ) -> void
{
    gtpo::assert_throw( !inEdge.expired(), "gtpo::GenNode<>::removeInEdge(): Error: In edge has expired" );
    auto nodePtr = this->shared_from_this();
    auto inEdgePtr = inEdge.lock( );
    auto inEdgeDstPtr = inEdgePtr->getDst().lock();
    gtpo::assert_throw( inEdgeDstPtr &&    // in edge dst must be this node
                        inEdgeDstPtr == nodePtr, "gtpo::GenNode<>::removeInEdge(): Error: In edge destination is expired or different from this node.");

    auto inEdgeSrcPtr = inEdgePtr->getSrc().lock();
    gtpo::assert_throw( inEdgeSrcPtr != nullptr, "gtpo::GenNode<>::removeInEdge(): Error: In edge source is expired." );
    this->notifyInNodeRemoved( inEdgePtr->getSrc(), inEdge );
    Config::template container_adapter< WeakEdges >::remove( inEdge, _inEdges );
    Config::template container_adapter< WeakNodes >::remove( inEdgePtr->getSrc(), _inNodes );
    if ( getInDegree() == 0 ) {
        Graph* graph{ getGraph() };
        if ( graph != nullptr )
            graph->installRootNode( WeakNode{ nodePtr } );
    }
    this->notifyInNodeRemoved();
}
//-----------------------------------------------------------------------------

} // ::gtpo
