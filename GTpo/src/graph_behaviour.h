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
// \file	graph_behaviour.h
// \author	benoit@destrat.io
// \date	2017 03 09
//-----------------------------------------------------------------------------

#pragma once

// GTpo headers
#include "./behaviour.h"
#include "./node.h"
#include "./edge.h"
#include "./group.h"

namespace gtpo { // ::gtpo

/*! \brief Define an observer interface to catch changes in a gtpo::graph.
 *
 */
template < class config_t >
class graph_behaviour :  public gtpo::behaviour<gtpo::graph<config_t>>
{
public:
    graph_behaviour() noexcept : gtpo::behaviour<gtpo::graph<config_t>>{} {}
    ~graph_behaviour() noexcept = default;
    graph_behaviour( const graph_behaviour<config_t>& ) = delete;
    graph_behaviour& operator=( const graph_behaviour<config_t>& ) = delete;

    using weak_node_t      = std::weak_ptr<typename config_t::final_node_t>;
    using weak_edge_t      = std::weak_ptr<typename config_t::final_edge_t>;
    using weak_group_t     = std::weak_ptr<typename config_t::final_group_t>;

    /*! \name Graph Notification Interface *///--------------------------------
    //@{
public:
    //! Called immediatly after node \c weakNode has been inserted in graph.
    void    node_inserted( weak_node_t& weakNode ) noexcept { static_cast<void>(weakNode); }
    //! Called immediatly before node \c weakNode is removed from graph.
    void    node_removed( weak_node_t& weakNode) noexcept { static_cast<void>(weakNode); }

public:
    //! Called immediatly after group \c weakGroup has been inserted in graph.
    void    group_inserted( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }
    //! Called immediatly before group \c weakGroup is removed from graph.
    void    group_removed( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }

public:
    //! Called immediatly after \c weakEdge has been inserted.
    void    edge_inserted( weak_edge_t& weakEdge ) noexcept { static_cast<void>(weakEdge); }
    //! Called when \c weakEdge is about to be removed.
    void    edge_removed( weak_edge_t& weakEdge ) noexcept { static_cast<void>(weakEdge); }
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Abstract interface for dynamic (virtual) graph behaviours.
 *
 */
template < class config_t >
class dynamic_graph_behaviour :  public graph_behaviour<config_t>
{
public:
    dynamic_graph_behaviour() noexcept : graph_behaviour<config_t>{} {}
    virtual ~dynamic_graph_behaviour() noexcept = default;
    dynamic_graph_behaviour( const dynamic_graph_behaviour<config_t>& ) = delete;
    dynamic_graph_behaviour& operator=( const dynamic_graph_behaviour<config_t>& ) = delete;
    dynamic_graph_behaviour( dynamic_graph_behaviour<config_t>&& ) = default;
    dynamic_graph_behaviour& operator=( dynamic_graph_behaviour<config_t>&& ) = default;

    using weak_node_t      = std::weak_ptr<typename config_t::final_node_t>;
    using weak_edge_t      = std::weak_ptr<typename config_t::final_edge_t>;
    using weak_group_t     = std::weak_ptr<typename config_t::final_group_t>;

public:
    void    node_inserted( weak_node_t& weakNode ) noexcept { on_node_inserted(weakNode); }
    void    node_removed( weak_node_t& weakNode) noexcept { on_node_removed(weakNode); }
    void    group_inserted( weak_group_t& weakGroup ) noexcept { on_group_inserted(weakGroup); }
    void    group_removed( weak_group_t& weakGroup ) noexcept { on_group_removed(weakGroup); }
    void    edge_inserted( weak_edge_t& weakEdge ) noexcept { on_edge_inserted(weakEdge); }
    void    edge_removed( weak_edge_t& weakEdge ) noexcept { on_edge_removed(weakEdge); }

    /*! \name Graph Dynamic (virtual) Notification Interface *///--------------
    //@{
protected:
    //! Called immediatly after node \c weakNode has been inserted in graph.
    virtual void    on_node_inserted( weak_node_t& weakNode ) noexcept { static_cast<void>(weakNode); }
    //! Called immediatly before node \c weakNode is removed from graph.
    virtual void    on_node_removed( weak_node_t& weakNode) noexcept { static_cast<void>(weakNode); }

    //! Called immediatly after group \c weakGroup has been inserted in graph.
    virtual void    on_group_inserted( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }
    //! Called immediatly before group \c weakGroup is removed from graph.
    virtual void    on_group_removed( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }

protected:
    //! Called immediatly after \c weakEdge has been inserted.
    virtual void    on_edge_inserted( weak_edge_t& weakEdge ) noexcept { static_cast<void>(weakEdge); }
    //! Called when \c weakEdge is about to be removed.
    virtual void    on_edge_removed( weak_edge_t& weakEdge ) noexcept { static_cast<void>(weakEdge); }
    //@}
    //-------------------------------------------------------------------------
};


/*! \brief Enable dynamic behaviour (observers) on gtpo::Graph<>.
 *
 * \note Add enable_graph_behaviour<> to gtpo::config::graph_behaviours tuple to enable dynamic
 * graph behaviours, otherwise, any gtpo::dynamic_graph_behaviour added using gtpo::graph::add_dynamic_graph_behaviour()
 * method won't be notified of changes in topology.
 */
template < class config_t >
class enable_graph_dynamic_behaviour :  public gtpo::graph_behaviour<config_t>
{
public:
    enable_graph_dynamic_behaviour() noexcept : gtpo::graph_behaviour<config_t>{} {}
    ~enable_graph_dynamic_behaviour() noexcept = default;
    enable_graph_dynamic_behaviour( const enable_graph_dynamic_behaviour<config_t>& ) = delete;
    enable_graph_dynamic_behaviour& operator=( const enable_graph_dynamic_behaviour<config_t>& ) = delete;

    using weak_node_t      = std::weak_ptr<typename config_t::final_node_t>;
    using weak_edge_t      = std::weak_ptr<typename config_t::final_edge_t>;
    using weak_group_t     = std::weak_ptr<typename config_t::final_group_t>;

public:
    template < class primitive_t >
    auto get_primitive_graph(std::weak_ptr<primitive_t>& weak_primitive) -> gtpo::graph<config_t>*
    {
        auto primitive = weak_primitive.lock();
        return ( primitive ? primitive->get_graph() : nullptr );
    }

    void    node_inserted( weak_node_t& weakNode ) noexcept {
        const auto graph = get_primitive_graph(weakNode);
        if ( graph != nullptr )
            graph->notify_dynamic_behaviours( &dynamic_graph_behaviour<config_t>::node_inserted, weakNode );
    }
    void    node_removed( weak_node_t& weakNode) noexcept {
        const auto graph = get_primitive_graph(weakNode);
        if ( graph != nullptr )
            graph->notify_dynamic_behaviours( &dynamic_graph_behaviour<config_t>::node_removed, weakNode );
    }
    void    group_inserted( weak_group_t& weakGroup ) noexcept {
        const auto graph = get_primitive_graph(weakGroup);
        if ( graph != nullptr )
            graph->notify_dynamic_behaviours( &dynamic_graph_behaviour<config_t>::group_inserted, weakGroup );
    }
    void    group_removed( weak_group_t& weakGroup ) noexcept {
        const auto graph = get_primitive_graph(weakGroup);
        if ( graph != nullptr )
            graph->notify_dynamic_behaviours( &dynamic_graph_behaviour<config_t>::group_removed, weakGroup );
    }
    void    edge_inserted( weak_edge_t& weakEdge ) noexcept {
        const auto graph = get_primitive_graph(weakEdge);
        if ( graph != nullptr )
            graph->notify_dynamic_behaviours( &dynamic_graph_behaviour<config_t>::edge_inserted, weakEdge );
    }
    void    edge_removed( weak_edge_t& weakEdge ) noexcept {
        const auto graph = get_primitive_graph(weakEdge);
        if ( graph != nullptr )
            graph->notify_dynamic_behaviours( &dynamic_graph_behaviour<config_t>::edge_removed, weakEdge );
    }
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Enable static and dynamic behaviour support for gtpo::Graph.
 *
 * \nosubgrouping
 */
template <class config_t>
class behaviourable_graph : public behaviourable<dynamic_graph_behaviour<config_t>,
                                                 typename config_t::graph_behaviours
                                                > // gtpo::behaviourable<>
{
    /*! \name behaviourable_graph Object Management *///------------------------
    //@{
public:
    friend enable_graph_dynamic_behaviour<config_t>;   // enable_graph_dynamic_behaviour<> need access to notify_dynamic_behaviours

    using dynamic_graph_behaviour_t = dynamic_graph_behaviour<config_t>;
    using graph_static_behaviours_t = typename config_t::graph_behaviours;
    using behaviourable_base = behaviourable<dynamic_graph_behaviour<config_t>, typename config_t::graph_behaviours>;

    behaviourable_graph() : behaviourable_base{} { }
    ~behaviourable_graph() noexcept { /* Nil */ }
    behaviourable_graph( const behaviourable_graph<config_t>& ) = delete;
    behaviourable_graph& operator=( const behaviourable_graph<config_t>& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    inline auto     add_dynamic_graph_behaviour( std::unique_ptr<dynamic_graph_behaviour_t> behaviour ) -> void {
        behaviourable<dynamic_graph_behaviour_t, graph_static_behaviours_t>::add_behaviour(std::move(behaviour));
    }

    template < class node_t >
    auto    notify_node_inserted( node_t& node ) noexcept -> void;

    template < class node_t >
    auto    notify_node_removed( node_t& node ) noexcept -> void;

    template < class edge_t >
    auto    notify_edge_inserted( edge_t& node ) noexcept -> void;

    template < class edge_t >
    auto    notify_edge_removed( edge_t& node ) noexcept -> void;

    template < class group_t >
    auto    notify_group_inserted( group_t& group ) noexcept -> void;

    template < class group_t >
    auto    notify_group_removed( group_t& group ) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./graph_behaviour.hpp"
