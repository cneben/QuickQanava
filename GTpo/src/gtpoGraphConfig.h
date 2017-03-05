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
// \file	gtpoGraphConfig.h
// \author	benoit@qanava.org
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
class GenGroup;

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

    using FinalNode     = gtpo::GenNode<GraphConfig>;
    using FinalEdge     = gtpo::GenEdge<GraphConfig>;
    using FinalGroup    = gtpo::GenGroup<GraphConfig>;

    //! Static behaviours that should be used for node (default to empty node behaviour tuple).
    using NodeBehaviours = std::tuple<>;

    //! Static behaviours that should be used for graph  (default to empty graph behaviour tuple).
    using GraphBehaviours = std::tuple<>;

    //! Static behaviours that should be used for graph (default to empty group behaviour tuple).
    using GroupBehaviours = std::tuple<>;

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

