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
// This file is a part of the GTpo software library.
//
// \file	functional.h
// \author	benoit@destrat.io
// \date	2018 08 15
//-----------------------------------------------------------------------------

#ifndef gtpo_functional_h
#define gtpo_functional_h

// STD headers
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <unordered_map>

// GTpo headers
#include "./utils.h"
#include "./config.h"
#include "./edge.h"
#include "./node.h"

namespace gtpo { // ::gtpo

template <class config_t>
class graph;


/* Graph Copy & Filter *///----------------------------------------------------

/*! \brief Copy a source graph to destination.
 *
 * \note Precondition: \c dst must be empty (return false if precondition is not met).
 * \note source and destination may be of different types.
 * \note May throw std::bad_alloc
 */
template <typename src_graph_t, typename dst_graph_t>
auto    copy(const src_graph_t& src, dst_graph_t& dst) -> bool
{
    // PRECONDITIONS:
        // dst must be empty.
    if (!dst.is_empty())
        return false;

    // ALGORITHM:
        // 1. Copy all nodes from src to dst.
        //  1.1 Maintain a mapping between src and dst nodes.
        // 2. Iterate over all edges
        //  2.1 Create edge in dst, mapping source edge source and destination nodes to dst

    // 1.
    std::unordered_map<typename src_graph_t::weak_node_t,
                       typename dst_graph_t::weak_node_t> src_to_dst_nodes;
    for (const auto& src_node : src.get_nodes()) {
        auto dst_node = dst.create_node();
        src_to_dst_nodes.insert( {src_node, dst_node} );    // 1.1
    }

    // 2.
    for (const auto src_edge : src.get_edges()) {
        if (!src_edge)
            return false;
        auto dst_src_node = src_to_dst_nodes[src_edge->get_src()];
        auto dst_dst_node = src_to_dst_nodes[src_edge->get_dst()];
        dst.create_edge(dst_src_node, dst_dst_node);
    }
    return true;
}


/*! \brief Filter a source graph to a destination graph using a functor (keep the node when functor return true).
 *
 * \note Precondition: \c dst must be empty (return false if precondition is not met).
 * \note When false is returned, destination is left in an undefined state (destination may be half
 * created).
 * \note May throw std::bad_alloc
 * \return true is \c src has been succesfully copied to \c dst.
 */
template <typename src_graph_t, typename dst_graph_t, typename filter_node_func_t>
auto    filter(const src_graph_t& src, dst_graph_t& dst, filter_node_func_t f) -> bool
{
    // PRECONDITIONS:
        // dst must be empty
    if (!dst.is_empty())
        return false;

    // ALGORITHM:
        // 1. Copy all nodes from src to dst if f(src_node) is true.
            // 1.1 Maintain a searchable container of all selected source nodes.
            // 1.2 Maintain a mapping from source to destination nodes.
        // 2. Iterate over source edges, recreate a subset of topology containing only selected nodes
            // 2.1 If source edge source and destination nodes are selected, create an edge in destination graph.
            // 2.2 To create an edge in dst, map source src and dst nodes to destination graph, then create edge.

    std::unordered_set<typename src_graph_t::weak_node_t> selected_src_nodes;
    std::unordered_map<typename src_graph_t::weak_node_t,
                       typename dst_graph_t::weak_node_t> src_to_dst_nodes;
    // 1.
    for ( const auto& src_node : src.get_nodes() ) {
        if (f(src_node)) {
            auto dst_node = dst.create_node();
            if (dst_node.expired())
                return false;
            // 1.1
            selected_src_nodes.insert(src_node);

            // 1.1
            src_to_dst_nodes.insert( {src_node, dst_node} );
        }
    }

    // 2.
    for ( const auto& src_edge : src.get_edges() ) {
        if (!src_edge)
            return false;
        // 2.1
        auto src_src_node = src_edge->get_src().lock();
        auto src_dst_node = src_edge->get_dst().lock();
        if ( !src_src_node ||
             !src_dst_node )
            return false;
        if (selected_src_nodes.find(src_src_node) != selected_src_nodes.end() &&
            selected_src_nodes.find(src_dst_node) != selected_src_nodes.end() ) {
            // 2.2
            auto dst_src_node = src_to_dst_nodes[src_src_node];
            auto dst_dst_node = src_to_dst_nodes[src_dst_node];
            dst.create_edge(dst_src_node, dst_dst_node);
        }
    }

    return true;
}


/*! \brief Map source graph nodes to a destination graph nodes using a transformation functor.
 *
 * \note Precondition: \c dst must be empty (return false if precondition is not met).
 * \note When false is returned, destination is left in an undefined state (destination may be half
 * created).
 * \note May throw std::bad_alloc
 * \return true is \c src has been succesfully copied to \c dst.
 */
template <typename src_graph_t, typename dst_graph_t, typename map_node_func_t>
auto    map(const src_graph_t& src, dst_graph_t& dst, map_node_func_t f) -> bool
{
    // PRECONDITIONS:
        // dst must be empty
    if (!dst.is_empty())
        return false;

    // ALGORITHM:
        // 1. Iterate over all source nodes:
        //   1.1 Apply map_node_func_t on source node.
        //   1.2 Keep a mapping from source node to destination node.
        // 2. Iterate over all edges:
        //    2.1 Copy source topology to destination using mapping from source to destination nodes.

    std::unordered_map<typename src_graph_t::weak_node_t,
                       typename dst_graph_t::weak_node_t> src_to_dst_nodes;
    // 1.
    for ( const auto& src_node : src.get_nodes() ) {
        //auto dst_node = dst.create_node();
        // 1.1
        auto dst_node = f(src_node);
        if (!dst_node)
            return false;
        dst.insert_node(dst_node);

        // 1.2
        src_to_dst_nodes.insert( {src_node, dst_node} );
    } // for all source graph nodes

    // 2.
    for ( const auto& src_edge : src.get_edges() ) {
        if (!src_edge)
            return false;
        // 2.1
        auto src_src_node = src_edge->get_src().lock();
        auto src_dst_node = src_edge->get_dst().lock();
        if ( !src_src_node ||
             !src_dst_node )
            return false;

        // 2.1
        auto dst_src_node = src_to_dst_nodes[src_src_node];
        auto dst_dst_node = src_to_dst_nodes[src_dst_node];
        dst.create_edge(dst_src_node, dst_dst_node);
    }

    return true;
}


/*! \brief Default mapping functor used to define gtpo::copy() function in term of mapping: map every source node to a new destination node.
 *
 * \warning Default copy "map" functor does not handle polymorphism, created node will always be destination_graph_t::shared_node_t.
 */
template <typename src_node_t, typename dst_node_t>
struct copy_map_node_func_t
{
    std::shared_ptr<dst_node_t> operator()(const src_node_t& src_node) {
        // FIXME: Use copy ctor...
        return std::make_shared<dst_node_t>();
    }
};

template <typename src_graph_t,
          typename dst_graph_t,
          typename map_node_func_t = copy_map_node_func_t<typename src_graph_t::shared_node_t,
                                                          typename dst_graph_t::node_t>
          >
auto    copy2(const src_graph_t& src, dst_graph_t& dst,
              map_node_func_t f = map_node_func_t{}) -> bool
{
    return gtpo::map(src, dst, f);
}
//-----------------------------------------------------------------------------

} // ::gtpo


#endif // gtpo_functional_h

