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
    static void  insert( T t, QVector<T>& c, int i ) { c.insert( t ); Q_UNUSED(i); }
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

