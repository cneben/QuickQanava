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
