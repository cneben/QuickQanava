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
#include "gtpo/qt_container_adapter.h"

// QuickContainers headers
#include <qcmContainer.h>

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

template < template<typename...CArgs> class C, typename T >
struct ContainerAdapter< qcm::Container<C, T> > {
    inline static void  insert( T t, qcm::Container<C, T>& c ) { c.append( t ); }
    inline static void  insert( T t, qcm::Container<C, T>& c, int i ) { Q_UNUSED(i); c.insert( t ); }
    inline static void  remove( const T& t, qcm::Container<C, T>& c ) { c.removeAll(t); }
    inline static   std::size_t size( qcm::Container<C, T>& c ) { return c.size(); }
    inline static   bool        contains( const qcm::Container<C, T>& c, const T& t ) { return c.contains(t); }
    inline static   void        reserve( qcm::Container<C , T>& c, std::size_t s) { c.reserve(s); }
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

