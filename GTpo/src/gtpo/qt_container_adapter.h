/*
    This file is part of QuickContainers library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickContainers library.
//
// \file    qcmContainerConcept.h
// \author  benoit@destrat.io
// \date    2016 11 28
//-----------------------------------------------------------------------------

#ifndef qcmContainerConcept_h
#define qcmContainerConcept_h

// Qt headers
#include <QList>
#include <QVector>
#include <QSet>

// STD headers
#include <memory>       // shared_ptr, weak_ptr
#include <type_traits>  // integral_constant

namespace qcm { // ::qcm

/*template < typename T >
struct insert { };

//! Insert \c item with type \t T in \c container of type \c Container<T>: insert<T>::into<Container>(t, container)
template < typename T >
struct insert< QList<T> > {
    static void  into( T t, QList<T>& c ) { c.append( t ); }
};*/



template < template<typename...CArgs> class Container, typename T >
struct container { };

template < typename T >
struct container< QList, T > {
    static void  insert( T t, QList<T>& c ) { c.append( t ); }
    static constexpr void  insert( T t, QList<T>& c, int i ) {
        /*if ( i == 0 )         // Mwai...
            c.prepend( t );
        else*/
            c.insert( i, t );
    }
    static void  remove( const T& t, QList<T>& c ) { c.remove( t ); }
};

template < typename T >
struct container< QVector, T > {
    static void  insert( T t, QVector<T>& c ) { c.append( t ); }
    static void  insert( T t, QVector<T>& c, int i ) { c.insert( i, t ); }
    static void  remove( const T& t, QVector<T>& c ) { c.removeAll( t ); }
};

template < typename T >
struct container< QSet, T > {
    static void  insert( T t, QVector<T>& c ) { c.append( t ); }
    static void  insert( T t, QVector<T>& c, int i ) { c.insert( t ); }
    static void  remove( const T& t, QVector<T>& c ) { c.remove( t ); }
};


/*
template < typename C, typename T >
struct container< QVector<T>, T > {
    template <typename ItemT>
    static void  insert( QVector<T>& c, ItemT t ) { c.append( t ); }

    template <typename ItemT>
    static void  remove( QVector<T>& c, const ItemT& t ) { c.removeAll( t ); }
};

template <typename C, typename T >
struct container< QSet<T>, T > {
    template <typename ItemT>
    static void  insert( QSet<T>& c, ItemT t ) { c.insert( t ); }

    template <typename ItemT>
    static void  remove( QSet<T>& c, const ItemT& t ) { c.remove( t ); }
};
*/

} // ::qcm

#endif // qcmContainerConcept_h

