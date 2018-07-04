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
// \file	node_behaviour.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpo_node_behaviour_h
#define gtpo_node_behaviour_h

// GTpo headers
#include "./behaviour.h"
#include "./behaviourable.h"

namespace gtpo { // ::gtpo

template <class config_t>
class node;

template <class config_t>
class edge;

/*! \brief Define a static node observer interface.
 *
 */
template <class config_t>
class node_behaviour : public gtpo::behaviour<gtpo::node<config_t>>
{
    /*! \name node_behaviour Object Management *///-----------------------------
    //@{
public:
    node_behaviour() noexcept : gtpo::behaviour<gtpo::node<config_t>>{} { }
    ~node_behaviour() noexcept = default;
    node_behaviour( const node_behaviour<config_t>& ) = delete;
    node_behaviour& operator=( const node_behaviour<config_t>& ) = delete;

    using weak_node_t          = std::weak_ptr<typename config_t::final_node_t>;
    using shared_node_t        = std::shared_ptr<typename config_t::final_node_t>;
    using shared_nodes_t       = typename config_t::template node_container_t< shared_node_t >;
    using weak_nodes_t_search  = typename config_t::template search_container_t< weak_node_t >;

    using weak_edge_t          = std::weak_ptr<typename config_t::final_edge_t>;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Notification Interface *///---------------------------------
public:
    /*! \brief Called immediatly after an in-edge with source \c weakInNode has been inserted.
     */
    void    in_node_inserted( weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called when an in-edge with source \c weakInNode is about to be removed.
     */
    void    in_node_removed( weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an in node has been removed.
     */
    void    in_node_removed(weak_node_t& target)  noexcept { static_cast<void>(target); }

    /*! \brief Called immediatly after an out-edge with destination \c weakOutNode has been inserted.
     */
    void    out_node_inserted( weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called when an out-edge with destination \c weakOutNode is about to be removed.
     */
    void    out_node_removed( weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an out-edge has been removed.
     */
    void    out_node_removed(weak_node_t& target)  noexcept { static_cast<void>(target); }
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Abstract interface for dynamic (virtual) node behaviours.
 *
 */
template <class config_t>
class dynamic_node_behaviour : public node_behaviour<config_t>
{
public:
    dynamic_node_behaviour() noexcept : node_behaviour<config_t>{} { }
    virtual ~dynamic_node_behaviour() = default;
    dynamic_node_behaviour( const dynamic_node_behaviour<config_t>& ) = delete;
    dynamic_node_behaviour& operator=( const dynamic_node_behaviour<config_t>& ) = delete;
    dynamic_node_behaviour( dynamic_node_behaviour<config_t>&& ) = delete;
    dynamic_node_behaviour& operator=( dynamic_node_behaviour<config_t>&& ) = delete;

    using weak_node_t          = std::weak_ptr<typename config_t::final_node_t>;
    using shared_node_t        = std::shared_ptr<typename config_t::final_node_t>;
    using shared_nodes_t       = typename config_t::template node_container_t< shared_node_t >;
    using weak_nodes_t_search  = typename config_t::template search_container_t< weak_node_t >;

    using weak_edge_t          = std::weak_ptr<typename config_t::final_edge_t>;

public:
    void    in_node_inserted( weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge )  noexcept { on_in_node_inserted(target, weakInNode, edge); }
    void    in_node_removed( weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge )  noexcept { on_in_node_removed(target, weakInNode, edge); }
    void    in_node_removed( weak_node_t& target )  noexcept { on_in_node_removed(target); }
    void    out_node_inserted( weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge )  noexcept { on_out_node_inserted(target, weakOutNode, edge); }
    void    out_node_removed( weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge )  noexcept { on_out_node_removed(target, weakOutNode, edge); }
    void    out_node_removed( weak_node_t& target )  noexcept { on_out_node_removed(target); }

protected:
    /*! \brief Called immediatly after an in-edge with source \c weakInNode has been inserted.
     */
    virtual void    on_in_node_inserted( weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called when an in-edge with source \c weakInNode is about to be removed.
     */
    virtual void    on_in_node_removed( weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an in node has been removed.
     */
    virtual void    on_in_node_removed(weak_node_t& target)  noexcept { static_cast<void>(target); }

    /*! \brief Called immediatly after an out-edge with destination \c weakOutNode has been inserted.
     */
    virtual void    on_out_node_inserted( weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called when an out-edge with destination \c weakOutNode is about to be removed.
     */
    virtual void    on_out_node_removed( weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge )  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an out-edge has been removed.
     */
    virtual void    on_out_node_removed(weak_node_t& target)  noexcept { static_cast<void>(target); }
};

template <class config_t>
class behaviourable_node;

/*! \brief Enable dynamic behaviour (observers) on gtpo::node<>.
 *
 * \note Add enable_graph_behaviour<> to gtpo::config::node_behaviours tuple to enable dynamic
 * graph behaviours, otherwise, any gtpo::dynamic_node_behaviour added using gtpo::graph::add_dynamic_node_behaviour()
 * method won't be notified of changes in topology.
 */
template < class config_t >
class enable_node_dynamic_behaviour : public gtpo::node_behaviour<config_t>
{
public:
    enable_node_dynamic_behaviour() noexcept : gtpo::node_behaviour<config_t>{} {}
    ~enable_node_dynamic_behaviour() noexcept = default;

    using weak_node_t = std::weak_ptr<typename config_t::final_node_t>;
    using weak_edge_t = std::weak_ptr<typename config_t::final_edge_t>;
    using base_t    = gtpo::node_behaviour<config_t>;

public:
    void    in_node_inserted( weak_node_t target, weak_node_t weakInNode, const weak_edge_t& edge )  noexcept {
        auto node = target.lock();
        if ( node )
            node->notify_dynamic_behaviours( &gtpo::dynamic_node_behaviour<config_t>::in_node_inserted, target, weakInNode, edge );
    }
    void    in_node_removed( weak_node_t target, weak_node_t weakInNode, const weak_edge_t& edge )  noexcept {
        auto node = target.lock();
        if ( node )
            node->notify_dynamic_behaviours( &gtpo::dynamic_node_behaviour<config_t>::in_node_removed, target, weakInNode, edge );
    }
    void    in_node_removed(weak_node_t target)  noexcept {
        auto node = target.lock();
        if ( node )
            node->notify_dynamic_behaviours( &gtpo::dynamic_node_behaviour<config_t>::in_node_removed, target );
    }
    void    out_node_inserted( weak_node_t target, weak_node_t weakOutNode, const weak_edge_t& edge )  noexcept {
        auto node = target.lock();
        if ( node )
            node->notify_dynamic_behaviours( &gtpo::dynamic_node_behaviour<config_t>::out_node_inserted, target, weakOutNode, edge );
    }
    void    out_node_removed( weak_node_t target, weak_node_t weakOutNode, const weak_edge_t& edge )  noexcept {
        auto node = target.lock();
        if ( node )
            node->notify_dynamic_behaviours( &gtpo::dynamic_node_behaviour<config_t>::out_node_removed, target, weakOutNode, edge );
    }
    void    out_node_removed(weak_node_t target)  noexcept {
        auto node = target.lock();
        if ( node )
            node->notify_dynamic_behaviours( &gtpo::dynamic_node_behaviour<config_t>::out_node_removed, target );
    }
};


/*! \brief Observation interface for gtpo::node.
 *
 * \nosubgrouping
 */
template < class config_t >
class behaviourable_node : public behaviourable<gtpo::dynamic_node_behaviour<config_t>,
                                                typename config_t::node_behaviours
                                               > // gtpo::behaviourable<>
{
    /*! \name behaviourable_node Object Management *///------------------------
    //@{
public:
    friend enable_node_dynamic_behaviour<config_t>;   // enable_node_dynamic_behaviour<> need access to notify_dynamic_behaviours

    behaviourable_node() noexcept { }
    ~behaviourable_node() noexcept = default;
    behaviourable_node( const behaviourable_node<config_t>& ) = delete;
    behaviourable_node& operator=( const behaviourable_node<config_t>& ) = delete;

    using dynamic_node_behaviour_t = dynamic_node_behaviour<config_t>;
    using node_static_behaviours_t = typename config_t::node_behaviours;

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    inline auto     add_dynamic_node_behaviour( std::unique_ptr<dynamic_node_behaviour_t> behaviour ) -> void {
        behaviourable<dynamic_node_behaviour_t, node_static_behaviours_t>::add_behaviour(std::move(behaviour));
    }

    template < class node_t, class edge_t  >
    auto    notify_in_node_inserted( node_t target, node_t node, const edge_t& edge ) noexcept -> void;

    template < class node_t, class edge_t  >
    auto    notify_in_node_removed( node_t target, node_t node, const edge_t& edge ) noexcept -> void;

    template < class node_t >
    auto    notify_in_node_removed( node_t target ) noexcept -> void;

    template < class node_t, class edge_t  >
    auto    notify_out_node_inserted( node_t target, node_t node, const edge_t& edge ) noexcept -> void;

    template < class node_t, class edge_t >
    auto    notify_out_node_removed( node_t target, node_t node, const edge_t& edge ) noexcept -> void;

    template < class node_t >
    auto    notify_out_node_removed( node_t target ) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./node_behaviour.hpp"

#endif // gtpo_node_behaviour_h
