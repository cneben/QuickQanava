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
auto node<config_t>::add_out_edge( weak_edge_t outEdgePtr ) -> void
{
    assert_throw( !outEdgePtr.expired(), "gtpo::node<>::add_out_edge(): Error: out edge is expired." );
    auto node = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    auto outEdge = outEdgePtr.lock();
    auto outEdgeSrc = outEdge->get_src().lock();
    if ( outEdge ) {
        if ( !outEdgeSrc || outEdgeSrc != node )  // Out edge source should point to target node
            outEdge->set_src( node );
        config_t::template container_adapter< weak_edges_t >::insert( outEdgePtr, _outEdges );
        if ( !outEdge->get_dst().expired() ) {
            config_t::template container_adapter< weak_nodes_t >::insert( outEdge->get_dst(), _outNodes );
            this->notify_out_node_inserted( weak_node_t{node}, outEdge->get_dst(), weak_edge_t{outEdge} );
        }
    }
}

template <class config_t>
auto node<config_t>::add_in_edge( weak_edge_t inEdgePtr ) -> void
{
    assert_throw( !inEdgePtr.expired(), "gtpo::node<>::add_in_edge(): Error: in edge is expired." );
    auto node = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    auto inEdge = inEdgePtr.lock( );
    if ( inEdge ) {
        auto inEdgeDst = inEdge->get_dst().lock();
        if ( !inEdgeDst ||
             inEdgeDst != node ) // In edge destination should point to target node
            inEdge->set_dst( node );
        config_t::template container_adapter< weak_edges_t >::insert( inEdgePtr, _inEdges );
        if ( !inEdge->get_src().expired() ) {
            config_t::template container_adapter< weak_nodes_t >::insert( inEdge->get_src(), _inNodes );
            this->notify_in_node_inserted( weak_node_t{node}, inEdge->get_src(), inEdgePtr );
        }
    }
}

template < class config_t >
auto node<config_t>::remove_out_edge( const weak_edge_t outEdge ) -> void
{
    gtpo::assert_throw( !outEdge.expired(), "gtpo::node<>::remove_out_edge(): Error: Out edge has expired" );
    auto outEdgePtr = outEdge.lock( );
    auto outEdgeSrcPtr = outEdgePtr->get_src().lock();
    weak_node_t node{ std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this()) };
    gtpo::assert_throw( outEdgeSrcPtr != nullptr &&    // Out edge src must be this node
                        outEdgeSrcPtr.get() == this, "gtpo::node<>::remove_out_edge(): Error: Out edge source is expired or different from this node.");

    auto outEdgeDst = outEdgePtr->get_dst().lock();
    if ( outEdgeDst != nullptr ) {
        gtpo::assert_throw( outEdgeDst != nullptr, "gtpo::node<>::remove_out_edge(): Error: Out edge destination is expired." );
        this->notify_out_node_removed( node, outEdgePtr->get_dst(), outEdge );
    }
    config_t::template container_adapter<weak_edges_t>::remove( outEdge, _outEdges );
    config_t::template container_adapter<weak_nodes_t>::remove( outEdgePtr->get_dst(), _outNodes );
    if ( get_in_degree() == 0 ) {
        graph_t* graph{ this->get_graph() };
        if ( graph != nullptr )
            graph->install_root_node( node );
    }
    this->notify_out_node_removed( node );
}

template < class config_t >
auto node<config_t>::remove_in_edge( const weak_edge_t inEdge ) -> void
{
    gtpo::assert_throw( !inEdge.expired(), "gtpo::node<>::remove_in_edge(): Error: In edge has expired" );
    auto nodePtr = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    auto inEdgePtr = inEdge.lock( );
    auto inEdgeDstPtr = inEdgePtr->get_dst().lock();
    gtpo::assert_throw( inEdgeDstPtr &&    // in edge dst must be this node
                        inEdgeDstPtr == nodePtr, "gtpo::node<>::remove_in_edge(): Error: In edge destination is expired or different from this node.");

    auto inEdgeSrcPtr = inEdgePtr->get_src().lock();
    gtpo::assert_throw( inEdgeSrcPtr != nullptr, "gtpo::node<>::remove_in_edge(): Error: In edge source is expired." );
    this->notify_in_node_removed( weak_node_t{ nodePtr }, inEdgePtr->get_src(), inEdge );
    config_t::template container_adapter< weak_edges_t >::remove( inEdge, _inEdges );
    config_t::template container_adapter< weak_nodes_t >::remove( inEdgePtr->get_src(), _inNodes );
    if ( get_in_degree() == 0 ) {
        graph_t* graph{ this->get_graph() };
        if ( graph != nullptr )
            graph->install_root_node( weak_node_t{ nodePtr } );
    }
    this->notify_in_node_removed( weak_node_t{nodePtr} );
}
//-----------------------------------------------------------------------------

} // ::gtpo
