/*
 Copyright (c) 2008-2020, Benoit AUTHEMAN All rights reserved.

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
// \file	behaviourable.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#pragma once

// STD headers
#include <cstddef>          // std::size_t
#include <functional>       // std::function
#include <vector>
#include <memory>
#include <utility>          // c++14 std::index_sequence

// FIXME v2
// GTpo headers
//#include "./behaviour.h"

namespace gtpo { // ::gtpo

/*! \brief Empty interface definition for graph primitives supporting behaviour concept.
 */
class abstract_observable {
public:
    abstract_observable() {}
    ~abstract_observable() = default;

    abstract_observable(const abstract_observable&) = default;
    abstract_observable& operator=( const abstract_observable&) = default;
    abstract_observable(abstract_observable&&) = default;
    abstract_observable& operator=(abstract_observable&&) = default;
};

/*! \brief Base class for all type supporting behaviours (actually gtpo::graph and gtpo::group).
 *
 * \nosubgrouping
 */
template <class behaviour_t>
class observable : public abstract_observable
{
    /*! \name behaviourable Object Management *///-----------------------------
    //@{
public:
    observable() : abstract_observable() { }
    ~observable() noexcept { _observers.clear(); }
    observable(const observable<behaviour_t>&) = default;
    observable& operator=(const observable<behaviour_t>&) = default;

public:
    //! Clear all registered behaviours (they are automatically deleted).
    inline  auto    clear() -> void { _observers.clear(); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Dynamic (virtual) Observer Management *///-----------------------
    //@{
public:
    /*! \brief Add a behaviour to the primitive inheriting from this behaviourable (observable) interface.
     *
     * \note This is a sink method, Behaviourable get ownership for \c behaviour, since it is
     * difficult to "downcast" unique_ptr, use the following code for calling:
     * \code
     *   gtpo::graph<> sg;
     *   sg.addBehaviour( std::make_unique< MyBehaviour >( ) );
     * \endcode
     */
    inline auto     add_observer(std::unique_ptr<behaviour_t> observer) -> void {
        _observers.emplace_back(std::move(observer));
    }

    //! std::vector of std::unique_ptr pointers on behaviour.
    using behaviours_t = std::vector<std::unique_ptr<behaviour_t>>;

public:
    //! Return true if an observer is currently registered (ie getObservers().size() > 0).
    inline auto    hasObservers() const noexcept -> bool { return _observers.size() > 0; }

    //! Return a read only container of actually registered observers.
    inline auto    getObservers() const noexcept -> const behaviours_t& { return _observers; }

protected:
    /*! \brief Call a pointer on a method on all registered dynamic behaviours.
     *
     * Example use:
     * \code
     *    // For a given node_behaviour method: auto    notifyNodeModified( weak_node_t& node ) -> void;
     *    notify_behaviours< weak_node_t >( &behaviour::nodeModified, node );
     * \endcode
     */
    template < class T >
    auto    notify_observers( void (behaviour_t::*method)(T&) /*noexcept*/, T& arg ) noexcept -> void;

    template < class T, class T2 >
    auto    notify_observers( void (behaviour_t::*method)(T&, T2&) /*noexcept*/, T& arg, T2&) noexcept -> void;

    template < class T, class T2, class T3 >
    auto    notify_observers( void (behaviour_t::*method)(T&, T2&, const T3&) /*noexcept*/, T&, T2&, const T3&) noexcept -> void;

    //! Similar to notifyBahaviours() but without arguments.
    auto    notify_observers0( void (behaviour_t::*method)() /*noexcept*/ ) noexcept -> void;

private:
    behaviours_t    _observers;
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Abstract interface for dynamic (virtual) node behaviours.
 *
 */
template <class node_t, edge_t>
class node_observer : public node_observer<config_t>
{
public:
    node_observer() noexcept : node_behaviour<config_t>{} { }
    virtual ~node_observer() = default;
    node_observer(const node_observer<config_t>& ) = delete;
    node_observer& operator=(const node_observer<config_t>&) = delete;
    node_observer(node_observer<config_t>&&) = delete;
    node_observer& operator=(node_observer<config_t>&&) = delete;

public:
    void    in_node_inserted(node_t& target, node_t& inNode, const edge_t& edge )  noexcept { on_in_node_inserted(target, weakInNode, edge); }
    void    in_node_removed(node_t& target, node_t& inNode, const edge_t& edge )  noexcept { on_in_node_removed(target, weakInNode, edge); }
    void    in_node_removed(node_t& target)  noexcept { on_in_node_removed(target); }
    void    out_node_inserted(node_t& target, node_t& weakOutNode, const edge_t& edge )  noexcept { on_out_node_inserted(target, weakOutNode, edge); }
    void    out_node_removed(node_t& target, node_t& weakOutNode, const edge_t& edge )  noexcept { on_out_node_removed(target, weakOutNode, edge); }
    void    out_node_removed(node_t& target)  noexcept { on_out_node_removed(target); }

protected:
    /*! \brief Called immediatly after an in-edge with source \c weakInNode has been inserted.
     */
    virtual void    on_in_node_inserted(weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called when an in-edge with source \c weakInNode is about to be removed.
     */
    virtual void    on_in_node_removed(weak_node_t& target, weak_node_t& weakInNode, const weak_edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an in node has been removed.
     */
    virtual void    on_in_node_removed(weak_node_t& target)  noexcept { static_cast<void>(target); }

    /*! \brief Called immediatly after an out-edge with destination \c weakOutNode has been inserted.
     */
    virtual void    on_out_node_inserted(weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called when an out-edge with destination \c weakOutNode is about to be removed.
     */
    virtual void    on_out_node_removed(weak_node_t& target, weak_node_t& weakOutNode, const weak_edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an out-edge has been removed.
     */
    virtual void    on_out_node_removed(weak_node_t& target)  noexcept { static_cast<void>(target); }
};


/*! \brief Observation interface for gtpo::node.
 *
 * \nosubgrouping
 */
template <class node_t, class edge_t>
class behaviourable_node : public behaviourable<gtpo::node_observer<config_t>,
                                                typename config_t::node_behaviours
                                               > // gtpo::behaviourable<>
{
    /*! \name behaviourable_node Object Management *///------------------------
    //@{
public:
    behaviourable_node() noexcept { }
    ~behaviourable_node() noexcept = default;
    behaviourable_node(const behaviourable_node<node_t, edge_t>&) = delete;
    behaviourable_node& operator=(const behaviourable_node<node_t, edge_t>&) = delete;

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    auto    add_node_observer(std::unique_ptr<node_observer_t> behaviour) -> void {
        behaviourable<node_observer_t, node_static_behaviours_t>::add_behaviour(std::move(behaviour));
    }

    auto    notify_in_node_inserted(node_t target, node_t node, const edge_t& edge) noexcept -> void;

    auto    notify_in_node_removed(node_t target, node_t node, const edge_t& edge) noexcept -> void;

    auto    notify_in_node_removed(node_t target) noexcept -> void;

    auto    notify_out_node_inserted(node_t target, node_t node, const edge_t& edge) noexcept -> void;

    auto    notify_out_node_removed(node_t* target, node_t* node, const edge_t& edge) noexcept -> void;

    auto    notify_out_node_removed(node_t* target) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------
};


} // ::gtpo

#include "./behaviourable.hpp"
