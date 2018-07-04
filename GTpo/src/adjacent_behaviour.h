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
// \file	adjacent_behaviour.h
// \author	benoit@destrat.io
// \date	2017 03 09
//-----------------------------------------------------------------------------

#pragma once

// GTpo headers
#include "./graph_behaviour.h"
#include "./group.h"
#include "./group_behaviour.h"

namespace gtpo { // ::gtpo

/*! \brief Collect and maintain a group adjacent edge set when a node is removed or inserted in a group.
 *
 *  This behaviour should be used with graph_group_adjacent_edges_behaviour.
 */
template <class config_t>
class group_adjacent_edges_behaviour : public group_behaviour<config_t>
{
    /* Group Edge Set behaviour *///-------------------------------------------
public:
    //group_adjacent_edges_behaviour() noexcept = default;
    group_adjacent_edges_behaviour() = default;
    ~group_adjacent_edges_behaviour() noexcept = default;
    group_adjacent_edges_behaviour( const group_adjacent_edges_behaviour& ) = delete;
    group_adjacent_edges_behaviour& operator=( const group_adjacent_edges_behaviour& ) = delete;

    using weak_node_t         = std::weak_ptr<typename config_t::final_node_t>;
    using shared_node_t       = std::shared_ptr<typename config_t::final_node_t>;

    using weak_edge_t         = std::weak_ptr<typename config_t::final_edge_t>;
    using shared_group_t      = std::shared_ptr<typename config_t::final_group_t>;

    using group_t           = typename config_t::final_group_t;
    using weak_edges_search_t = typename config_t::template search_container_t< weak_edge_t >;

public:
    void    node_inserted( weak_node_t& weakNode ) noexcept;
    void    node_removed( weak_node_t& weakNode ) noexcept;
    //-------------------------------------------------------------------------
};

/*! \brief Collect and maintain a group adjacent edge set and group edge set.
 *
 */
template < class config_t >
class graph_group_adjacent_edges_behaviour : public graph_behaviour< config_t >
{
    /* Group Edge Set behaviour *///-------------------------------------------
public:
    graph_group_adjacent_edges_behaviour() noexcept : graph_behaviour<config_t>{} { }
    ~graph_group_adjacent_edges_behaviour() noexcept = default;
    graph_group_adjacent_edges_behaviour( const graph_group_adjacent_edges_behaviour& ) = default;
    graph_group_adjacent_edges_behaviour& operator=( const graph_group_adjacent_edges_behaviour& ) = delete;

    using weak_edge_t         = std::weak_ptr<typename config_t::final_edge_t>;
    using group_t           = typename config_t::final_group_t;
    using weak_edges_search_t = typename config_t::template search_container_t< weak_edge_t >;

public:
    void    edge_inserted( weak_edge_t& weakEdge ) noexcept;
    void    edge_removed( weak_edge_t& weakEdge ) noexcept;
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./adjacent_behaviour.hpp"
