/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// \file	node.hpp
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* node Edges Management *///-----------------------------------------------
template < class config_t >
auto node<config_t>::addOutEdge( weak_edge outEdgePtr ) -> void
{
    assert_throw( !outEdgePtr.expired(), "gtpo::node<>::addOutEdge(): Error: out edge is expired." );
    auto node = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    auto outEdge = outEdgePtr.lock();
    auto outEdgeSrc = outEdge->getSrc().lock();
    if ( outEdge ) {
        if ( !outEdgeSrc || outEdgeSrc != node )  // Out edge source should point to target node
            outEdge->setSrc( node );
        config_t::template container_adapter< weak_edges >::insert( outEdgePtr, _outEdges );
        if ( !outEdge->getDst().expired() ) {
            config_t::template container_adapter< weak_nodes >::insert( outEdge->getDst(), _outNodes );
            this->notify_out_node_inserted( weak_node{node}, outEdge->getDst(), weak_edge{outEdge} );
        }
    }
}

template <class config_t>
auto node<config_t>::addInEdge( weak_edge inEdgePtr ) -> void
{
    assert_throw( !inEdgePtr.expired(), "gtpo::node<>::addInEdge(): Error: in edge is expired." );
    auto node = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    auto inEdge = inEdgePtr.lock( );
    if ( inEdge ) {
        auto inEdgeDst = inEdge->getDst().lock();
        if ( !inEdgeDst ||
             inEdgeDst != node ) // In edge destination should point to target node
            inEdge->setDst( node );
        config_t::template container_adapter< weak_edges >::insert( inEdgePtr, _inEdges );
        if ( !inEdge->getSrc().expired() ) {
            config_t::template container_adapter< weak_nodes >::insert( inEdge->getSrc(), _inNodes );
            this->notify_in_node_inserted( weak_node{node}, inEdge->getSrc(), inEdgePtr );
        }
    }
}

template < class config_t >
auto node<config_t>::removeOutEdge( const weak_edge outEdge ) -> void
{
    gtpo::assert_throw( !outEdge.expired(), "gtpo::node<>::removeOutEdge(): Error: Out edge has expired" );
    auto outEdgePtr = outEdge.lock( );
    auto outEdgeSrcPtr = outEdgePtr->getSrc().lock();
    weak_node node{ std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this()) };
    gtpo::assert_throw( outEdgeSrcPtr != nullptr &&    // Out edge src must be this node
                        outEdgeSrcPtr.get() == this, "gtpo::node<>::removeOutEdge(): Error: Out edge source is expired or different from this node.");

    auto outEdgeDst = outEdgePtr->getDst().lock();
    if ( outEdgeDst != nullptr ) {
        gtpo::assert_throw( outEdgeDst != nullptr, "gtpo::node<>::removeOutEdge(): Error: Out edge destination is expired." );
        this->notify_out_node_removed( node, outEdgePtr->getDst(), outEdge );
    }
    config_t::template container_adapter<weak_edges>::remove( outEdge, _outEdges );
    config_t::template container_adapter<weak_nodes>::remove( outEdgePtr->getDst(), _outNodes );
    if ( getInDegree() == 0 ) {
        graph_t* graph{ this->getGraph() };
        if ( graph != nullptr )
            graph->installRootNode( node );
    }
    this->notify_out_node_removed( node );
}

template < class config_t >
auto node<config_t>::removeInEdge( const weak_edge inEdge ) -> void
{
    gtpo::assert_throw( !inEdge.expired(), "gtpo::node<>::removeInEdge(): Error: In edge has expired" );
    auto nodePtr = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    auto inEdgePtr = inEdge.lock( );
    auto inEdgeDstPtr = inEdgePtr->getDst().lock();
    gtpo::assert_throw( inEdgeDstPtr &&    // in edge dst must be this node
                        inEdgeDstPtr == nodePtr, "gtpo::node<>::removeInEdge(): Error: In edge destination is expired or different from this node.");

    auto inEdgeSrcPtr = inEdgePtr->getSrc().lock();
    gtpo::assert_throw( inEdgeSrcPtr != nullptr, "gtpo::node<>::removeInEdge(): Error: In edge source is expired." );
    this->notify_in_node_removed( weak_node{ nodePtr }, inEdgePtr->getSrc(), inEdge );
    config_t::template container_adapter< weak_edges >::remove( inEdge, _inEdges );
    config_t::template container_adapter< weak_nodes >::remove( inEdgePtr->getSrc(), _inNodes );
    if ( getInDegree() == 0 ) {
        graph_t* graph{ this->getGraph() };
        if ( graph != nullptr )
            graph->installRootNode( weak_node{ nodePtr } );
    }
    this->notify_in_node_removed( weak_node{nodePtr} );
}
//-----------------------------------------------------------------------------

} // ::gtpo
