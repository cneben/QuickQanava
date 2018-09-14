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
// \file	group_behaviour.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#pragma once

// GTpo headers
#include "./behaviour.h"
#include "./group.h"
#include "./node.h"

namespace gtpo { // ::gtpo

template < class config_t >
class edge;

template < class config_t >
class group;

/*! \brief Define an observer interface to catch changes in a gtpo::group.
 *
 */
template < class config_t >
class group_behaviour :  public gtpo::behaviour<gtpo::group<config_t>>
{
    /*! \name group_behaviour Object Management *///----------------------------
    //@{
public:
    group_behaviour() noexcept : gtpo::behaviour<gtpo::group<config_t>>{} {}
    ~group_behaviour() noexcept = default;
    group_behaviour( const group_behaviour<config_t>& ) = default;
    group_behaviour& operator=( const group_behaviour<config_t>& ) = default;
    group_behaviour( group_behaviour<config_t>&& ) = default;
    group_behaviour& operator=( group_behaviour<config_t>&& ) = default;

    using weak_node_t      = std::weak_ptr<typename config_t::final_node_t>;
    using weak_group_t     = std::weak_ptr<typename config_t::final_group_t>;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Notification Interface *///--------------------------------
    //@{
    //! Called immediatly after \c weakNode has been inserted into target group.
    void    node_inserted( weak_node_t& weakNode ) noexcept { static_cast<void>(weakNode); }
    //! Called when \c weakNode is about to be removed from target group.
    void    node_removed( weak_node_t& weakNode ) noexcept { static_cast<void>(weakNode); }

    //! Called immediatly after \c weakGroup group has been inserted into target group.
    void    group_inserted( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }
    //! Called when \c weakGroup group is about to be removed from target group.
    void    group_removed( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }
    //@}
    //-------------------------------------------------------------------------
};


/*! \brief Abstract interface for dynamic (virtual) group behaviours.
 *
 */
template < class config_t >
class dynamic_group_behaviour :  public group_behaviour<config_t>
{
    /*! \name group_behaviour Object Management *///----------------------------
    //@{
public:
    dynamic_group_behaviour() noexcept : group_behaviour<config_t>{} {}
    ~dynamic_group_behaviour() noexcept = default;
    dynamic_group_behaviour( const dynamic_group_behaviour<config_t>& ) = delete;
    dynamic_group_behaviour& operator=( const dynamic_group_behaviour<config_t>& ) = delete;
    dynamic_group_behaviour( dynamic_group_behaviour<config_t>&& ) = default;
    dynamic_group_behaviour& operator=( dynamic_group_behaviour<config_t>&& ) = default;

    using weak_node_t      = std::weak_ptr<typename config_t::final_node_t>;
    using weak_group_t     = std::weak_ptr<typename config_t::final_group_t>;

public:
    void    node_inserted( weak_node_t& weakNode ) noexcept { on_node_inserted(weakNode); }
    void    node_removed( weak_node_t& weakNode ) noexcept { on_node_removed(weakNode); }
    void    group_inserted( weak_group_t& weakGroup ) noexcept { on_group_inserted(weakGroup); }
    void    group_removed( weak_group_t& weakGroup ) noexcept { on_group_removed(weakGroup); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Notification Interface *///--------------------------------
    //@{
protected:
    //! Called immediatly after \c weakNode has been inserted into target group.
    virtual void    on_node_inserted( weak_node_t& weakNode ) noexcept { static_cast<void>(weakNode); }
    //! Called when \c weakNode is about to be removed from target group.
    virtual void    on_node_removed( weak_node_t& weakNode ) noexcept { static_cast<void>(weakNode); }

    //! Called immediatly after \c weakGroup group has been inserted into target group.
    virtual void    on_group_inserted( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }
    //! Called when \c weakGroup group is about to be removed from target group.
    virtual void    on_group_removed( weak_group_t& weakGroup ) noexcept { static_cast<void>(weakGroup); }
    //@}
    //-------------------------------------------------------------------------
};

template <class config_t>
class behaviourable_group;

/*! \brief Enable dynamic behaviour (observers) on gtpo::group<>.
 *
 * \note Add enable_group_dynamic_behaviour<> to gtpo::config::group_behaviours tuple to enable dynamic
 * group dynamic behaviours, otherwise, any gtpo::dynamic_group_behaviour added using gtpo::graph::add_dynamic_group_behaviour()
 * method won't be notified of changes in topology.
 */
template < class config_t >
class enable_group_dynamic_behaviour :  public gtpo::group_behaviour<config_t>
{
public:
    enable_group_dynamic_behaviour() noexcept {}
    ~enable_group_dynamic_behaviour() noexcept = default;
    enable_group_dynamic_behaviour( const enable_graph_dynamic_behaviour<config_t>& ) = delete;
    enable_group_dynamic_behaviour& operator=( const enable_graph_dynamic_behaviour<config_t>& ) = delete;

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
        const auto node = weakNode.lock();
        if (!node)
            return;
        const auto node_group = node->get_group().lock();
        const auto group_behaviourable = static_cast<gtpo::behaviourable_group<config_t>*>(node_group.get());
        if ( group_behaviourable != nullptr )
            group_behaviourable->notify_dynamic_behaviours( &dynamic_group_behaviour<config_t>::node_inserted, weakNode );
    }
    void    node_removed( weak_node_t& weakNode ) noexcept {
        const auto node = weakNode.lock();
        if (!node)
            return;
        const auto node_group = node->get_group().lock();
        const auto group_behaviourable = static_cast<gtpo::behaviourable_group<config_t>*>(node_group.get());
        if ( group_behaviourable != nullptr )
            group_behaviourable->notify_dynamic_behaviours( &dynamic_group_behaviour<config_t>::node_removed, weakNode );
    }
    void    group_inserted( weak_group_t& weakGroup ) noexcept {
        const auto group = weakGroup.lock();
        if (!group)
            return;
        const auto group_group = group->get_group().lock();
        const auto group_behaviourable = static_cast<gtpo::behaviourable_group<config_t>*>(group_group.get());
        if ( group_behaviourable != nullptr )
            group_behaviourable->notify_dynamic_behaviours( &dynamic_group_behaviour<config_t>::group_inserted, weakGroup );
    }
    void    group_removed( weak_group_t& weakGroup ) noexcept {
        const auto group = weakGroup.lock();
        if (!group)
            return;
        const auto group_group = group->get_group().lock();
        const auto group_behaviourable = static_cast<gtpo::behaviourable_group<config_t>*>(group_group.get());
        if ( group_behaviourable != nullptr )
            group_behaviourable->notify_dynamic_behaviours( &dynamic_group_behaviour<config_t>::group_removed, weakGroup );
    }
};


/*! \brief Observation interface for gtpo::group.
 *
 * \note notify_node_inserted() and notify_node_removed() are called when a group is
 * inserted into node with gtpo::graph::insert_node(group, node) or
 * gtpo::graph::remove_node(group, node), it has nothing to do with
 * adjacent nodes.
 *
 * \nosubgrouping
 */
template <class config_t>
class behaviourable_group : public behaviourable<dynamic_group_behaviour<config_t>,
                                                 typename config_t::group_behaviours
                                                > // gtpo::behaviourable<>
{
    /*! \name behaviourable_group Object Management *///------------------------
    //@{
public:
    friend enable_group_dynamic_behaviour<config_t>;   // enable_group_dynamic_behaviour<> need access to notify_dynamic_behaviours

    behaviourable_group() noexcept { }
    ~behaviourable_group() noexcept = default;
    behaviourable_group( const behaviourable_group<config_t>& ) = delete;
    behaviourable_group& operator=( const behaviourable_group<config_t>& ) = delete;

    using dynamic_group_behaviour_t = dynamic_group_behaviour<config_t>;
    using group_static_behaviours_t = typename config_t::group_behaviours;

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    inline auto     add_dynamic_group_behaviour( std::unique_ptr<dynamic_group_behaviour_t> behaviour ) -> void {
        behaviourable<dynamic_group_behaviour_t, group_static_behaviours_t>::add_behaviour(std::move(behaviour));
    }

    //! Notify registered behaviours that a group has been inserted into target group.
    template < class group_t >
    auto    notify_group_inserted( group_t& group ) noexcept -> void;

    //! Notify registered behaviours that a group is on the verge of beeing removed from target group.
    template < class group_t >
    auto    notify_group_removed( group_t& group ) noexcept -> void;

    //! Notify registered behaviours that a node has been inserted into target group.
    template < class node_t >
    auto    notify_node_inserted( node_t& node ) noexcept -> void;

    //! Notify registered behaviours that a node is on the verge of beeing removed from target group.
    template < class node_t >
    auto    notify_node_removed( node_t& node ) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./group_behaviour.hpp"

