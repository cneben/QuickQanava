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
// \file	gtpoBehaviour.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpoBehaviour_h
#define gtpoBehaviour_h

// STD headers
#include <cstddef>          // std::size_t
#include <functional>       // std::function
#include <vector>
#include <memory>
#include <utility>          // c++14 std::index_sequence

// GTpo headers
/* Nil */

namespace gtpo { // ::gtpo

/*! \brief Base class for concrete behaviours.
 *
 * \image html behaviour-class.png
 *
 */
template <class Config >
class Behaviour
{
public:
    Behaviour( const std::string& name = {} ) noexcept : _name{name} { }
    virtual ~Behaviour() { }

    explicit Behaviour( const Behaviour& ) = delete;
    Behaviour& operator=( const Behaviour& ) = delete;

public:
    inline auto getName() const noexcept -> const std::string& { return _name; }
protected:
    inline auto setName( const std::string& name )  noexcept -> void { _name = name; }
private:
    std::string _name = "";

public:
    /*! \brief Enable this behaviour until it is disabled again with a call to disable().
     *
     * \note enabling a disabled behaviour does not apply behaviour on changes made
     *       while it was disabled.
     * \sa isEnabled() for enabled state description.
     */
    inline auto     enable() noexcept -> void { _enabled = true; notifyEnabledChanged(); }
    /*! \brief Disable this behaviour until it is enabled back wit a call to enable().
     *
     * \sa isEnabled() for enabled state description.
     */
    inline auto     disable() noexcept -> void { _enabled = false; notifyEnabledChanged(); }
    /*! \brief Return the actual "enabled" state for this behaviour.
     *
     *  This method could be usefull in very specific use cases, such as serialization or insertion of a large number
     *  of nodes in a graph or group where this behaviour is applied.
     *  A behaviour is enabled by default after its creation.
     *
     *  \return true if enable() has been called, false if disable() has been called.
     */
    inline auto     isEnabled( ) const noexcept -> bool { return _enabled; }

    //! Optionnal notifier for the enabled property.
    virtual void    notifyEnabledChanged() { }

protected:
    bool            _enabled = true;
};

// C++14 O(N log(N)) copied from: http://stackoverflow.com/a/26902803
// There is more complex O(N) solutions available on SO
template< class F, class...Ts, std::size_t...Is >
void for_each_in_tuple( std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...> ) noexcept {
    (void)tuple; // Avoid C4100 warning for size 1 std::tuple with MSVC2015 U2
    (void)func;  // Avoid C4100 warning for size 1 std::tuple with MSVC2015 U2
    using expander = int[];
    (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template< class F, class...Ts >
void for_each_in_tuple( std::tuple<Ts...>& tuple, F func ) noexcept {
    for_each_in_tuple( tuple, func, std::make_index_sequence<sizeof...(Ts)>() );
}

//! Empty interface definition for graph primitives supporting behaviour concept.
struct IBehaviorable {
    IBehaviorable() noexcept = default;
    ~IBehaviorable() = default;
    IBehaviorable( const IBehaviorable& ) = delete;
    IBehaviorable& operator=( const IBehaviorable& ) = delete;
};

/*! \brief Base class for all type supporting behaviours (actually gtpo::GenGraph and gtpo::GenGroup).
 *
 * \nosubgrouping
 */
template < class Behaviour, class SBehaviours >
class Behaviourable : public IBehaviorable
{
    /*! \name Behaviorable Object Management *///------------------------------
    //@{
public:
    Behaviourable() noexcept { }
    ~Behaviourable() { _behaviours.clear(); }
    Behaviourable( const Behaviourable& ) = delete;
    Behaviourable& operator=( const Behaviourable& ) = delete;

public:
    //! Clear all actually regsitered behaviours (they are automatically deleted).
    inline  auto    clear() -> void { _behaviours.clear(); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Virtual Behaviours Management *///-------------------------------
    //@{
public:
    /*! \brief Add a behaviour to this graph (graph get ownership for behaviour).
     *
     * \note This is a sink method, Behaviourable get ownership for \c behaviour, since it is
     * difficult to "downcast" unique_ptr, use the following code for calling:
     * \code
     *   gtpo::GenGraph<> sg;
     *   auto myBehaviour = std::make_unique< MyBehaviour >( );
     *   sg.addBehaviour( myBehaviour.release() );
     * \endcode
     *
     * \param behaviour Graph will get behaviour ownership.
     */
    inline auto     addBehaviour( std::unique_ptr<Behaviour> behaviour ) -> void {
        _behaviours.emplace_back( std::move( behaviour ) );
    }

    //! std::vector of std::unique_ptr pointers on Behaviour.
    using Behaviours = std::vector< std::unique_ptr< Behaviour > >;

public:
    //! Return true if a behaviours is currently registered (ie getBehaviours().size()>0).
    inline auto    hasBehaviours() const noexcept -> bool { return _behaviours.size() > 0; }

    //! Return a read only container of actually registered behaviours.
    inline auto    getBehaviours() const noexcept -> const Behaviours& { return _behaviours; }

protected:
    /*! \brief Apply a method pointer on all registered behaviours.
     *
     * Example use:
     * \code
     *    // For a given NodeBehaviour method: auto    notifyNodeModified( WeakNode& node ) -> void;
     *    notifyBehaviours< WeakNode >( &Behaviour::nodeModified, node );
     * \endcode
     */
    template < class T >
    auto    notifyBehaviours( void (Behaviour::*method)(T&) noexcept, T& arg ) noexcept -> void;

    template < class T, class T2 >
    auto    notifyBehaviours( void (Behaviour::*method)(T&, T2&) noexcept, T& arg, T2&) noexcept -> void;

    //! Similar to notifyBahaviours() but without arguments.
    auto    notifyBehaviours0( void (Behaviour::*method)() noexcept ) noexcept -> void;

private:
    Behaviours  _behaviours;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Static Behaviours Management *///--------------------------------
    //@{
public:
    /*! \brief Apply a functor on all registered static behaviours.
     *
     * Example use:
     * \code
     * \endcode
     */
    template < class Functor >
    auto    sNotifyBehaviours( Functor f ) noexcept -> void {
        for_each_in_tuple( _sBehaviours, f );
    }

private:
    SBehaviours  _sBehaviours;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoBehaviour.hpp"

#endif // gtpoBehaviour_h
