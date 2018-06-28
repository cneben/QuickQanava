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
// \file	behaviourable.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpo_behaviourable_h
#define gtpo_behaviourable_h

// STD headers
#include <cstddef>          // std::size_t
#include <functional>       // std::function
#include <vector>
#include <memory>
#include <utility>          // c++14 std::index_sequence

// GTpo headers
#include "./behaviour.h"

namespace gtpo { // ::gtpo

namespace impl { // ::gtpo::impl

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

} // ::gtpo::impl

/*! \brief Empty interface definition for graph primitives supporting behaviour concept.
 */
class abstract_behaviourable {
public:
    abstract_behaviourable() {}
    ~abstract_behaviourable() = default;

    abstract_behaviourable( const abstract_behaviourable& ) = default;
    abstract_behaviourable& operator=( const abstract_behaviourable& ) = default;
    abstract_behaviourable( abstract_behaviourable&& ) = default;
    abstract_behaviourable& operator=( abstract_behaviourable&& ) = default;
};

/*! \brief Base class for all type supporting behaviours (actually gtpo::graph and gtpo::group).
 *
 * \nosubgrouping
 */
template < class behaviour_t, typename static_behaviours_t >
class behaviourable : public abstract_behaviourable
{
    /*! \name behaviourable Object Management *///-----------------------------
    //@{
public:
    behaviourable() : abstract_behaviourable() { }
    ~behaviourable() noexcept { _dynamic_behaviours.clear(); }
    behaviourable( const behaviourable<behaviour_t, static_behaviours_t>& ) = default;
    behaviourable& operator=( const behaviourable<behaviour_t, static_behaviours_t>& ) = default;

public:
    //! Clear all registered behaviours (they are automatically deleted).
    inline  auto    clear() -> void { _dynamic_behaviours.clear(); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Dynamic (virtual) Behaviours Management *///---------------------
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
    inline auto     add_behaviour( std::unique_ptr<behaviour_t> behaviour ) -> void {
        _dynamic_behaviours.emplace_back( std::move( behaviour ) );
    }

    //! std::vector of std::unique_ptr pointers on behaviour.
    using behaviours_t = std::vector<std::unique_ptr<behaviour_t>>;

public:
    //! Return true if a behaviours is currently registered (ie getBehaviours().size()>0).
    inline auto    hasBehaviours() const noexcept -> bool { return _dynamic_behaviours.size() > 0; }

    //! Return a read only container of actually registered behaviours.
    inline auto    getBehaviours() const noexcept -> const behaviours_t& { return _dynamic_behaviours; }

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
    auto    notify_dynamic_behaviours( void (behaviour_t::*method)(T&) /*noexcept*/, T& arg ) noexcept -> void;

    template < class T, class T2 >
    auto    notify_dynamic_behaviours( void (behaviour_t::*method)(T&, T2&) /*noexcept*/, T& arg, T2&) noexcept -> void;

    template < class T, class T2, class T3 >
    auto    notify_dynamic_behaviours( void (behaviour_t::*method)(T&, T2&, const T3&) /*noexcept*/, T&, T2&, const T3&) noexcept -> void;

    //! Similar to notifyBahaviours() but without arguments.
    auto    notify_dynamic_behaviours0( void (behaviour_t::*method)() /*noexcept*/ ) noexcept -> void;

private:
    behaviours_t    _dynamic_behaviours;
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
    auto    notify_static_behaviours( Functor f ) noexcept -> void {
        impl::for_each_in_tuple( _static_behaviours, f );
    }

private:
    static_behaviours_t _static_behaviours;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./behaviourable.hpp"

#endif // gtpo_behaviourable_h
