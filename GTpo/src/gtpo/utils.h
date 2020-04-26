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
// \file	utils.h
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef gtpo_utils_h
#define gtpo_utils_h

// STD headers
#include <algorithm>
#include <exception>    // std::runtime_error
#include <memory>       // std::weak_ptr
#include <iostream>     // std::cout
#include <utility>      // std::index_sequence / std::make_index_sequence

namespace gtpo { // ::gtpo

//! Empty class
class empty { };

/*! \brief Exception thrown by GTpo to notify user that a topology related error occurs.
 *
 * Use what() to have a detailled error description.
 */
class bad_topology_error : public std::runtime_error
{
public:
    explicit bad_topology_error (const std::string& what_arg) : std::runtime_error( what_arg ) { }
    explicit bad_topology_error (const char* what_arg) : std::runtime_error( what_arg ) { }
    bad_topology_error () : bad_topology_error( "GTpo topology unrecoverable error." ) { }
};

/*! Standard GTpo utility to assert an expression  \c expr and throw an exception if \c expr is *false*.
 *
 * Used with no template parameter, assert_throw will throw a gtpo::bad_topology_error with a given \c message.
 *
 * \note This function does not rely on N_DEBUG, the test will be run even in release mode.
 */
template < class E = gtpo::bad_topology_error >
auto    assert_throw( bool expr, std::string message = std::string{ "" } ) noexcept( false ) -> void {
    if ( !expr )
        throw E{ message };
}

//! Compare two std::weak_ptr that must have been checked for expired() and nullptr content (ie use_count()==0).
template < class T >
auto    compare_weak_ptr( const std::weak_ptr<T>& left, const std::weak_ptr<T>& right ) noexcept -> bool {
    return !left.owner_before( right ) && !right.owner_before( left );
}

/*! Find a weak_ptr<> element in a STL standard container.
 *
 * \return true if \c element is found in \c container, false otherwise.
 * \throw gtpo::bad_topology_error if the searched element weak_ptr is expired.
 */
template < class C, class T >
auto    find_weak_ptr( const C& container, const T& element ) -> bool {
    assert_throw( !element.expired(), "gtpo::find_weak_ptr<>(): Error: element is expired." );
    auto iter = std::find_if( container.begin(), container.end(),
                                            [=](const T& containerElement ){ return ( compare_weak_ptr<>( containerElement, element ) ); } );
    return ( iter != container.end() );
}


/*! Configuration interface for accessing graph containers.
 *
 * GTpo graph config configuration struct should inherit from ContainerAccessors since
 * graph access its nodes, edges and groups container trought the generic insert()/remove() interface.
 *
 */
struct container_accessors {
    /*! \brief Generic "item inserter" for list/set/vector, specialize for your custom container types.
     */
    template <typename T>
    struct insert { };

    /*! \brief Generic "item remover" for list/set/vector, specialize for your custom container types.
     *
     * \code
     *   using IntList = std::list< int >;
     *   IntList l;
     *   l.push_back( 42 );
     *   // Same interface for std::list/set/vector
     *   gtpo::Defaultconfig::remove<IntList>::from( l, 42 );
     * \endcode
     */
    template <typename T>
    struct remove { };
};

} // ::gtpo

namespace std
{
    //! Specialization of std::hash for weak_ptr type.
    template< typename T >
    struct hash< std::weak_ptr< T > >
    {
        typedef std::weak_ptr< T > argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& key ) const {
            return std::hash< typename std::shared_ptr<T> >()( key.lock() );
        }
    };

    //! Specialization of std::equal_to for weak_ptr type.
    template< typename T >
    struct equal_to< std::weak_ptr< T > >
    {
        typedef bool result_type;
        typedef std::weak_ptr< T > first_argument_type;
        typedef std::weak_ptr< T > second_argument_type;
        result_type operator()( const first_argument_type& left, const second_argument_type& right ) const
        {
           return !left.owner_before( right ) && !right.owner_before( left );
        }
    };
}

#endif // gtpo_utils_h

