/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library.
//
// \file	qanContainerAdapter.h
// \author	benoit@destrat.io
// \date	2017 03 04
//-----------------------------------------------------------------------------

#ifndef qanContainerAdapter_h
#define qanContainerAdapter_h

// Qt headers
#include <QObject>
#include <QList>
#include <QVector>
#include <QSet>

// GTpo headers
#include "gtpoContainerAdapter.h"

// QuickContainers headers
#include "../QuickContainers/src/qcmContainerModel.h"

namespace qan { // ::qan

template < typename T >
struct ContainerAdapter { };

template < typename T >
struct ContainerAdapter< QVector<T> > {
    inline static void  insert( T t, QVector<T>& c ) { c.append( t ); }
    inline static void  insert( T t, QVector<T>& c, int i ) { c.insert( i, t ); }
    inline static void  remove( const T& t, QVector<T>& c ) { c.removeAll(t); }
    inline static   std::size_t size( QVector<T>& c ) { return c.size(); }
    inline static   bool        contains( const QVector<T>& c, const T& t ) { return c.contains(t); }
    inline static   void        reserve( QVector<T>& c, std::size_t s) { c.reserve(s); }
};

template < typename T >
struct ContainerAdapter< QSet<T> > {
    inline static void  insert( T t, QSet<T>& c ) { c.insert( t ); }
    inline static void  insert( T t, QSet<T>& c, int i ) { c.insert( i, t ); }
    inline static void  remove( const T& t, QSet<T>& c ) { c.remove(t); }
    inline static   std::size_t size( QSet<T>& c ) { return c.size(); }
    inline static   bool        contains( const QSet<T>& c, const T& t ) { return c.contains(t); }
    inline static   void        reserve( QSet<T>& c, std::size_t s) { c.reserve(s); }
};

template < template<typename...CArgs> class Container, typename T >
struct ContainerAdapter< qcm::ContainerModel<Container, T> > {
    inline static void  insert( T t, qcm::ContainerModel<Container, T>& c ) { c.append( t ); }
    inline static void  insert( T t, qcm::ContainerModel<Container, T>& c, int i ) { c.insert( t ); }
    inline static void  remove( const T& t, qcm::ContainerModel<Container, T>& c ) { c.remove(t); }
    inline static   std::size_t size( qcm::ContainerModel<Container, T>& c ) { return c.size(); }
    inline static   bool        contains( const qcm::ContainerModel<Container, T>& c, const T& t ) { return c.contains(t); }
    inline static   void        reserve( qcm::ContainerModel<Container, T>& c, std::size_t s) { c.reserve(s); }
};


} // ::qan

#ifndef COMPARE_WEAK_PTR
#define COMPARE_WEAK_PTR
template< typename T >
inline bool operator==(const std::weak_ptr<T>& e1, const std::weak_ptr<T>& e2)
{
    return gtpo::compare_weak_ptr( e1, e2 );
}

template<typename T>
inline uint qHash(const std::weak_ptr<T>& t, uint seed ) /*noexcept( qHash(t) ) )*/
{
    return qHash( static_cast<T*>( t.lock().get() ), seed );
}
#endif

#endif // qanContainerAdapter_h

