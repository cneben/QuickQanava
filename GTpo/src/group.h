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
// \file	group.h
// \author	benoit@destrat.io
// \date	2017 03 04
//-----------------------------------------------------------------------------

#pragma once

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./config.h"
#include "./utils.h"
#include "./node.h"
#include "./edge.h"
#include "./group_behaviour.h"
#include "./adjacent_behaviour.h"

namespace gtpo { // ::gtpo

/*! \brief Base class for modelling groups of nodes in a gtpo::graph graph.
 *
 * \nosubgrouping
*/
template <class config_t = gtpo::config<>>
class group : public gtpo::node<config_t>,
              public gtpo::behaviourable_group<config_t>
{
    friend graph<config_t>;   // graph need access to set_graph()

    /*! \name Node Management *///---------------------------------------------
    //@{
public:
    using weak_node_t         = std::weak_ptr<typename config_t::final_node_t>;
    using shared_node_t       = std::shared_ptr<typename config_t::final_node_t>;
    using weak_nodes_t        = typename config_t::template node_container_t< weak_node_t >;

    using weak_edge_t         = std::weak_ptr<typename config_t::final_edge_t>;
    using weak_edges_search_t = typename config_t::template search_container_t< weak_edge_t >;

    using weak_group_t        = std::weak_ptr< typename config_t::final_group_t >;
    using shared_group_t      = std::shared_ptr< typename config_t::final_group_t >;

    group() noexcept : gtpo::node<config_t>() { }
    ~group() { /* Nil */ }
    group( const group& ) = delete;
    group& operator=( group const& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Nodes Management *///--------------------------------------
    //@{
public:
    //! Return group's nodes.
    inline auto get_nodes() noexcept -> const weak_nodes_t& { return _nodes; }

    //! Return true if group contains \c node.
    auto        has_node( const weak_node_t& node ) const noexcept -> bool;
    //! Return group registered node count.
    inline auto get_node_count( ) const noexcept -> int { return static_cast< int >( _nodes.size() ); }
private:
    weak_nodes_t   _nodes;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Adjacent Edges *///----------------------------------------------
    //@{
public:
    inline auto     get_edges() noexcept -> weak_edges_search_t& { return _edges; }
    inline auto     get_edges() const noexcept -> const weak_edges_search_t& { return _edges; }

    inline auto     get_adjacent_edges() noexcept -> weak_edges_search_t& { return _adjacentEdges; }
    inline auto     get_adjacent_edges() const noexcept -> const weak_edges_search_t& { return _adjacentEdges; }

protected:
    weak_edges_search_t _edges;
    weak_edges_search_t _adjacentEdges;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

