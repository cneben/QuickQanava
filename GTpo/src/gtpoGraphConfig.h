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
// This file is a part of the GTpo software library.
//
// \file	gtpoGraphConfig.h
// \author	benoit@destrat.io
// \date	2017 03 04
//-----------------------------------------------------------------------------

#ifndef gtpoGraphConfig_h
#define gtpoGraphConfig_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./gtpoUtils.h"
#include "./gtpoBehaviour.h"
#include "./gtpoContainerAdapter.h"

namespace gtpo { // ::gtpo

template <class Config>
class GenGraph;

template <class Config>
class GenNode;

template <class Config>
class GenEdge;

template <class Config>
class GenGroupEdge;

template <class Config>
class GenGroup;

template <class Config>
class GraphGroupAjacentEdgesBehaviour;

template <class Config>
class GroupAdjacentEdgesBehaviour;

/*! Default configuration for GTpo primitive, containers and behaviours.
 *
 */
struct GraphConfig
{
    //! Define gtpo::GenGraph base class.
    using GraphBase = Empty;
    //! Define gtpo::GenNode base class.
    using NodeBase  = Empty;
    //! Define gtpo::GenEdge base class.
    using EdgeBase  = Empty;
    //! Define gtpo::GenGroup base class.
    using GroupBase = Empty;

    using FinalGroup        = gtpo::GenGroup<GraphConfig>;
    using FinalNode         = gtpo::GenNode<GraphConfig>;
    using FinalEdge         = gtpo::GenEdge<GraphConfig>;
    using FinalGroupEdge    = gtpo::GenGroupEdge<GraphConfig>;

    //! Static behaviours that should be used for node (default to empty node behaviour tuple).
    //using NodeBehaviours = std::tuple<>;

    //! Static behaviours that should be used for graph  (default to empty graph behaviour tuple).
    //using GraphBehaviours = std::tuple<>;
    //using GraphBehaviours = std::tuple< gtpo::GraphGroupAjacentEdgesBehaviour< GraphConfig > >;

    //! Static behaviours that should be used for graph (default to empty group behaviour tuple).
    //using GroupBehaviours = std::tuple<>;
    //using GroupBehaviours = std::tuple< gtpo::GroupAdjacentEdgesBehaviour< GraphConfig > >;

    template <typename T>
    using container_adapter = gtpo::std_container_adapter<T>;

    //! Define the container used to store nodes (default to std::vector).
    template <class...Ts>
    using NodeContainer = std::vector<Ts...>;

    //! Define the container used to store edges (default to std::vector).
    template <class...Ts>
    using EdgeContainer = std::vector<Ts...>;

    //! Define the unordered container used to search for edges and nodes (default to std::unordered_set).
    template <class T>
    using SearchContainer = std::unordered_set<T>;
};


} // ::gtpo

#endif // gtpoGraphConfig_h

