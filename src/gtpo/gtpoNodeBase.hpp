/*
 Copyright (c) 2008-2021, Benoit AUTHEMAN All rights reserved.

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
template <class node_base_t,
          class graph_t,
          class node_t,
          class edge_t,
          class group_t>
auto node<node_base_t,
          graph_t,
          node_t,
          edge_t,
          group_t>::add_out_edge(edge_t* outEdge) -> bool
{
    if (outEdge == nullptr)
        return false;
    // FIXME v2
    //assert_throw( !outEdgePtr.expired(), "gtpo::node<>::add_out_edge(): Error: out edge is expired." );
    //auto node = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    //auto outEdge = outEdgePtr.lock();
    auto outEdgeSrc = outEdge->get_src();
    if (!outEdgeSrc || outEdgeSrc != this)  // Out edge source should point to target node
        outEdge->set_src(reinterpret_cast<node_t*>(this));
    // FIXME v2 container
    //config_t::template container_adapter< weak_edges_t >::insert( outEdgePtr, _out_edges );
    if (outEdge->get_dst() != nullptr) {
        // FIXME v2 container
        //config_t::template container_adapter< weak_nodes_t >::insert( outEdge->get_dst(), _out_nodes );
        // FIXME v2 notify
        //this->notify_out_node_inserted( weak_node_t{node}, outEdge->get_dst(), weak_edge_t{outEdge} );
        return true;
    }
    return false;
}

template <class node_base_t,
          class graph_t,
          class node_t,
          class edge_t,
          class group_t>
auto node<node_base_t,
          graph_t,
          node_t,
          edge_t,
          group_t>::add_in_edge(edge_t* in_edge) -> bool
{
    if (in_edge == nullptr)
        return false;
    auto in_edge_dst = in_edge->get_dst();
    if (!in_edge_dst ||
            in_edge_dst != this) // In edge destination should point to target node
        in_edge->set_dst(reinterpret_cast<node_t*>(this));
    container_adapter<edges_t>::insert(in_edge, _in_edges);
    if (in_edge->get_src() != nullptr) {
        container_adapter<nodes_t>::insert(in_edge->get_src(), _in_nodes);
        observable_base_t::notify_in_node_inserted(*reinterpret_cast<node_t*>(this),
                                                   *in_edge->get_src(), *in_edge);
    }
    return true;
}

template <class node_base_t,
          class graph_t,
          class node_t,
          class edge_t,
          class group_t>
auto node<node_base_t,
          graph_t,
          node_t, edge_t,
          group_t>::remove_out_edge(const edge_t* outEdge) -> void
{
    // FIXME v2
    //gtpo::assert_throw( !outEdge.expired(), "gtpo::node<>::remove_out_edge(): Error: Out edge has expired" );
    if (outEdge == nullptr)
        return;
    auto outEdgeSrc = outEdge->get_src();
    //weak_node_t node{ std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this()) };
    //gtpo::assert_throw( outEdgeSrcPtr != nullptr &&    // Out edge src must be this node
    //                    outEdgeSrcPtr.get() == this, "gtpo::node<>::remove_out_edge(): Error: Out edge source is expired or different from this node.");

    auto outEdgeDst = outEdge->get_dst();
    if (outEdgeDst != nullptr) {
        //gtpo::assert_throw( outEdgeDst != nullptr, "gtpo::node<>::remove_out_edge(): Error: Out edge destination is expired." );
        //this->notify_out_node_removed( node, outEdgePtr->get_dst(), outEdge );
    }
    // FIXME v2 container
    //config_t::template container_adapter<weak_edges_t>::remove( outEdge, _out_edges );
    //config_t::template container_adapter<weak_nodes_t>::remove( outEdgePtr->get_dst(), _out_nodes );
    if (get_in_degree() == 0) {
        graph_t* graph = this->get_graph();
        if (graph != nullptr)
            graph->install_root_node(reinterpret_cast<node_t*>(this));
    }
    // FIXME v2 notify
    //this->notify_out_node_removed(this);
}

template <class node_base_t,
          class graph_t,
          class node_t,
          class edge_t,
          class group_t>
auto node<node_base_t,
          graph_t,
          node_t, edge_t,
          group_t>::remove_in_edge(const edge_t* inEdge) -> void
{
    // FIXME v2
    //gtpo::assert_throw( !inEdge.expired(), "gtpo::node<>::remove_in_edge(): Error: In edge has expired" );
    //auto nodePtr = std::static_pointer_cast<typename config_t::final_node_t>(this->shared_from_this());
    //auto inEdgePtr = inEdge.lock( );
    if (inEdge == nullptr)
        return;
    auto in_edge_dst = inEdge->get_dst();
    //gtpo::assert_throw( in_edge_dstPtr &&    // in edge dst must be this node
    //                    in_edge_dstPtr == nodePtr, "gtpo::node<>::remove_in_edge(): Error: In edge destination is expired or different from this node.");

    auto inEdgeSrc = inEdge->get_src();
    //gtpo::assert_throw(inEdgeSrcPtr != nullptr, "gtpo::node<>::remove_in_edge(): Error: In edge source is expired.");
    //this->notify_in_node_removed( weak_node_t{ nodePtr }, inEdgePtr->get_src(), inEdge );
    // FIXME v2 container
    //config_t::template container_adapter< weak_edges_t >::remove( inEdge, _in_edges );
    //config_t::template container_adapter< weak_nodes_t >::remove( inEdgePtr->get_src(), _in_nodes );
    if (get_in_degree() == 0) {
        graph_t* graph = this->get_graph();
        if (graph != nullptr)
            graph->install_root_node(reinterpret_cast<node_t*>(this));
    }

    // FIXME v2 notify
    //this->notify_in_node_removed(this);
}
//-----------------------------------------------------------------------------

/* Group Nodes Management *///-------------------------------------------------
template <class node_base_t,
          class graph_t,
          class node_t,
          class edge_t,
          class group_t>
auto node<node_base_t,
          graph_t,
          node_t, edge_t,
          group_t>::has_node(const node_t* node ) const noexcept -> bool
{
    if (node == nullptr)
        return false;
    auto nodeIter = std::find_if(_nodes.begin(), _nodes.end(),
                                 [=](const node_t* candidate_node){ return (node == candidate_node); } );
    return nodeIter != _nodes.end();
}
//-----------------------------------------------------------------------------

} // ::gtpo
