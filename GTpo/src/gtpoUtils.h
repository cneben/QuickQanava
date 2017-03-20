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
// \file	gtpoUtils.h
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef gtpoUtils_h
#define gtpoUtils_h

// STD headers
#include <algorithm>
#include <exception>    // std::runtime_error
#include <memory>       // std::weak_ptr
#include <iostream>     // std::cout

namespace gtpo { // ::gtpo

//! Empty class
class Empty { };

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
<<<<<<< HEAD
        throw E( message );
=======
        throw E{ message };
>>>>>>> dev
}

//! Compare two std::weak_ptr that must have been checked for expired() and nullptr content (ie use_count()==0).
template < class T >
auto    compare_weak_ptr( const std::weak_ptr<T>& left, const std::weak_ptr<T>& right ) noexcept -> bool const {
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
 * GTpo GenGraph Config configuration struct should inherit from ContainerAccessors since
 * GenGraph access its nodes, edges and groups container trought the generic insert()/remove() interface.
 *
<<<<<<< HEAD
 * For example, if you define class GenGraph< QtConfig >, your Qt config class could inherit
 * from qtpo::ContainerAccessors (include gtpoQtUtils.h), for the standard library, use
 * gtpo::StdContainerAccessors (DefaultConfig inherits gtpo::StdContainerAccessors to provide
 * default implementations for accessors working with most STL containers.
=======
>>>>>>> dev
 */
struct ContainerAccessors {
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
     *   gtpo::DefaultConfig::remove<IntList>::from( l, 42 );
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

#endif // gtpoUtils_h

