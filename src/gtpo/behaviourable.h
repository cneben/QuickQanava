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
template <class observer_t>
class observable : public abstract_observable
{
    /*! \name behaviourable Object Management *///-----------------------------
    //@{
public:
    observable() : abstract_observable() { }
    ~observable() noexcept { _observers.clear(); }
    observable(const observable<observer_t>&) = default;
    observable& operator=(const observable<observer_t>&) = default;

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
    inline auto     add_observer(std::unique_ptr<observer_t> observer) -> void {
        _observers.emplace_back(std::move(observer));
    }

    //! std::vector of std::unique_ptr pointers on behaviour.
    using observers_t = std::vector<std::unique_ptr<observer_t>>;

public:
    //! Return true if an observer is currently registered (ie getObservers().size() > 0).
    inline auto    hasObservers() const noexcept -> bool { return _observers.size() > 0; }

    //! Return a read only container of actually registered observers.
    inline auto    getObservers() const noexcept -> const observer_t& { return _observers; }

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
    auto    notify_observers( void (observer_t::*method)(T&) /*noexcept*/, T& arg ) noexcept -> void;

    template < class T, class T2 >
    auto    notify_observers( void (observer_t::*method)(T&, T2&) /*noexcept*/, T& arg, T2&) noexcept -> void;

    template < class T, class T2, class T3 >
    auto    notify_observers( void (observer_t::*method)(T&, T2&, const T3&) /*noexcept*/, T&, T2&, const T3&) noexcept -> void;

    //! Similar to notifyBahaviours() but without arguments.
    auto    notify_observers0( void (observer_t::*method)() /*noexcept*/ ) noexcept -> void;

private:
    observers_t    _observers;
    //@}
    //-------------------------------------------------------------------------
};


template <class node_t, class edge_t>
class node_observer;

/*! \brief Observation interface for gtpo::node.
 *
 * \nosubgrouping
 */
template <class node_t, class edge_t>
class observable_node : public observable<gtpo::node_observer<node_t, edge_t> >
{
    /*! \name behaviourable_node Object Management *///------------------------
    //@{
public:
    observable_node() noexcept { }
    ~observable_node() noexcept = default;
    observable_node(const observable_node<node_t, edge_t>&) = delete;
    observable_node& operator=(const observable_node<node_t, edge_t>&) = delete;

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    using super_t = observable<gtpo::node_observer<node_t, edge_t> >;
    auto    add_node_observer(std::unique_ptr<gtpo::node_observer<node_t, edge_t>> observer) -> void {
        super_t::add_behaviour(std::move(observer));
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

/*! \brief Base class for concrete observers.
 */
template <class target_t>
class observer
{
public:
    observer() noexcept { }
    ~observer() noexcept { }

   observer(const observer& rhs) = default;
   observer& operator=(const observer& rhs) = default;
   observer(observer&& rhs) noexcept = default;
   observer& operator=(observer&& rhs) noexcept = default;

public:
    target_t*   get_target() noexcept { return _target; }
    void        set_target(target_t* target) { _target = target; }
protected:
    target_t*   _target = nullptr;

public:
    inline auto getName() const noexcept -> const std::string& { return _name; }
protected:
    inline auto setName(const std::string& name)  noexcept -> void { _name = name; }
private:
    std::string _name = "";

public:
    /*! \brief Enable this behaviour until it is disabled again with a call to disable().
     *
     * \note enabling a disabled behaviour does not apply behaviour on changes made
     *       while it was disabled.
     * \sa isEnabled() for enabled state description.
     */
    inline auto     enable() noexcept -> void { _enabled = true; }
    /*! \brief Disable this behaviour until it is enabled back wit a call to enable().
     *
     * \sa isEnabled() for enabled state description.
     */
    inline auto     disable() noexcept -> void { _enabled = false; }
    /*! \brief Return the actual "enabled" state for this behaviour.
     *
     *  This method could be usefull in very specific use cases, such as serialization or insertion of a large number
     *  of nodes in a graph or group where this behaviour is applied.
     *  A behaviour is enabled by default after its creation.
     *
     *  \return true if enable() has been called, false if disable() has been called.
     */
    inline auto     isEnabled() const noexcept -> bool { return _enabled; }

protected:
    bool            _enabled = true;
};


/*! \brief Abstract interface for dynamic (virtual) node behaviours.
 *
 */
template <class node_t, class edge_t>
class node_observer : public observer<node_t>
{
public:
    node_observer() noexcept : observer<node_t>{} { }
    virtual ~node_observer() = default;
    node_observer(const node_observer<node_t, edge_t>& ) = delete;
    node_observer& operator=(const node_observer<node_t, edge_t>&) = delete;
    node_observer(node_observer<node_t, edge_t>&&) = delete;
    node_observer& operator=(node_observer<node_t, edge_t>&&) = delete;

public:
    void    in_node_inserted(node_t& target, node_t& in_node, const edge_t& edge )  noexcept { on_in_node_inserted(target, in_node, edge); }
    void    in_node_removed(node_t& target, node_t& in_node, const edge_t& edge )  noexcept { on_in_node_removed(target, in_node, edge); }
    void    in_node_removed(node_t& target)  noexcept { on_in_node_removed(target); }
    void    out_node_inserted(node_t& target, node_t& out_node, const edge_t& edge )  noexcept { on_out_node_inserted(target, out_node, edge); }
    void    out_node_removed(node_t& target, node_t& out_node, const edge_t& edge )  noexcept { on_out_node_removed(target, out_node, edge); }
    void    out_node_removed(node_t& target)  noexcept { on_out_node_removed(target); }

protected:
    /*! \brief Called immediatly after an in-edge with source \c weakInNode has been inserted.
     */
    virtual void    on_in_node_inserted(node_t& target, node_t& weakInNode, const edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called when an in-edge with source \c weakInNode is about to be removed.
     */
    virtual void    on_in_node_removed(node_t& target, node_t& weakInNode, const edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakInNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an in node has been removed.
     */
    virtual void    on_in_node_removed(node_t& target)  noexcept { static_cast<void>(target); }

    /*! \brief Called immediatly after an out-edge with destination \c weakOutNode has been inserted.
     */
    virtual void    on_out_node_inserted(node_t& target, node_t& weakOutNode, const edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called when an out-edge with destination \c weakOutNode is about to be removed.
     */
    virtual void    on_out_node_removed(node_t& target, node_t& weakOutNode, const edge_t& edge)  noexcept { static_cast<void>(target); static_cast<void>(weakOutNode); static_cast<void>(edge); }

    /*! \brief Called immediatly after an out-edge has been removed.
     */
    virtual void    on_out_node_removed(node_t& target)  noexcept { static_cast<void>(target); }
};

} // ::gtpo

#include "./behaviourable.hpp"
