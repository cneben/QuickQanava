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
// \file	GTpoQt.h
// \author	benoit@destrat.io
// \date	2016 02 05
//-----------------------------------------------------------------------------

#ifndef GTpoQt_h
#define GTpoQt_h

// Qt headers
#include <QObject>
#include <QList>
#include <QVector>
#include <QSet>

// GTpo headers
#include "./gtpoUtils.h"

namespace qtpo { // ::qtpo

/*! Configuration interface for accessing standard library (STL) containers.
 *
 */
struct ContainerAccessors : public gtpo::ContainerAccessors
{
    template < typename C >
    struct insert { };

    template < typename T >
    struct insert< QList<T> > {
        template <typename ItemT>
        static void  into( QList<T>& c, ItemT t ) { c.append( t ); }
    };

    template < typename T >
    struct insert< QVector<T> > {
        template <typename ItemT>
        static void  into( QVector<T>& c, ItemT t ) { c.append( t ); }
    };

    template <typename T >
    struct insert< QSet<T> > {
        template <typename ItemT>
        static void  into( QSet<T>& c, ItemT t ) { c.insert( t ); }
    };

    template < typename T >
    struct remove { };

    template < typename T >
    struct remove< QList<T> > {
        static void  from( QList<T>& c, const T& t ) { c.remove( t ); }
    };

    template < typename T >
    struct remove< QVector<T> > {
        static void  from( QVector<T>& c, const T& t ) { c.removeAll( t ); }
    };

    template < typename T >
    struct remove< QSet<T> > {
        static void  from( QSet<T>& c, const T& t ) { c.remove( t ); }
    };
};

} // ::qtpo

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

#endif // GTpoQt_h

