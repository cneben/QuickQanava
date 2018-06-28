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
// \file	config.h
// \author	benoit@destrat.io
// \date	2017 03 04
//-----------------------------------------------------------------------------

#ifndef gtpo_config_h
#define gtpo_config_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./utils.h"
#include "./behaviour.h"
#include "./container_adapter.h"

namespace gtpo { // ::gtpo

template <class config_t>
class graph;

template <class config_t>
class node;

template <class config_t>
class edge;

template <class config_t>
class group_edge;

template <class config_t>
class group;

template <class config_t>
class graph_group_adjacent_edges_behaviour;

template <class config_t>
class group_adjacent_edges_behaviour;

template <class config_t>
class enable_graph_dynamic_behaviour;

template <class config_t>
class enable_group_dynamic_behaviour;

template <class config_t>
class enable_node_dynamic_behaviour;

/*! \brief Default configuration for GTpo primitive, containers and behaviours.
 *
 */
struct config
{
    //! Define gtpo::graph base class.
    using graph_base = gtpo::empty;
    //! Define gtpo::node base class.
    using node_base  = gtpo::empty;
    //! Define gtpo::edge base class.
    using edge_base  = gtpo::empty;
    //! Define gtpo::group base class.
    using group_base = gtpo::empty;

    using final_group_t        = gtpo::group<config>;
    using final_node_t         = gtpo::node<config>;
    using final_edge_t         = gtpo::edge<config>;
    using final_group_edge_t   = gtpo::group_edge<config>;

    using final_config = config;

    using graph_behaviours = std::tuple< gtpo::graph_group_adjacent_edges_behaviour<final_config>,
                                         gtpo::enable_graph_dynamic_behaviour<final_config>
                                       >; // std::tuple

    using group_behaviours = std::tuple< gtpo::group_adjacent_edges_behaviour<final_config>,
                                         gtpo::enable_group_dynamic_behaviour<final_config>
                                       >; // std::tuple

    using node_behaviours = std::tuple< gtpo::enable_node_dynamic_behaviour<final_config>
                                       >; // std::tuple

    template <typename T>
    using container_adapter = gtpo::std_container_adapter<T>;

    //! Define the container used to store nodes (default to std::vector).
    template <class...Ts>
    using node_container_t = std::vector<Ts...>;

    //! Define the container used to store edges (default to std::vector).
    template <class...Ts>
    using edge_container_t = std::vector<Ts...>;

    //! Define the unordered container used to search for edges and nodes (default to std::unordered_set).
    template <class T>
    using search_container_t = std::unordered_set<T>;
};


} // ::gtpo

#endif // gtpo_config_h

