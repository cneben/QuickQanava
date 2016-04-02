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
// \file	basic.h
// \author	benoit@qanava.org
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef basic_h
#define basic_h

// Qt headers
#include <QObject>
#include <QList>
#include <QVector>
#include <QSet>

// GTpo headers
//#include <GTpo>
#include <GTpoStd>

namespace qtpo {
/*
class Node;
class Edge;

class QGraphConfig final : public gtpo::DefaultConfig {
public:
    using Node = qtpo::Node;
    using Edge = qtpo::Edge;

    template <class...Args>
    using NodeContainer = QVector<Args...>;

    template <class...Args>
    using EdgeContainer = QVector<Args...>;

    template< class GraphConfig >
    using Edges = EdgeContainer< gtpo::GenEdge< GraphConfig >* >;

    template <class...Args>
    using SearchContainer = QSet<Args...>;

    using GraphBase = QObject;
    using NodeBase = QObject;
    using EdgeBase = QObject;

    template < typename N >
    static inline double   getNodeX( const N& n ) { return static_cast< const QGraphConfig::Node&>( n ).getQX( ); }

    template < typename N >
    static inline void     setNodeX( N& n, double x ) { return static_cast< Node&>( n ).setQX( x ); }

    template <typename C>
    struct insert { };

    template <typename T >
    struct insert<QList<T>> {
        template <typename T>
        static void  into( QList<T>& c, T t ) { c.append( t ); }
    };
    template <typename T >
    struct insert<QVector<T>> {
        template <typename T>
        static void  into( QVector<T>& c, T t ) { c.append( t ); }
    };
    template <typename T >
    struct insert<QSet<T>> {
        template <typename T>
        static void  into( QSet<T>& c, T t ) { c.insert( t ); }
    };

    template <typename T>
    struct remove { };
    template <typename T>
    struct remove< QList<T> > {
        static void  from( QList<T>& c, T t ) { c.remove( t ); }
    };
    template <typename T>
    struct remove< QVector<T> > {
        static void  from( QVector<T>& c, T t ) { c.removeAll( t ); }
    };
    template <typename T>
    struct remove< QSet<T> > {
        static void  from( QSet<T>& c, T t ) { c.remove( t ); }
    };
};
*/
/*class Edge : public gtpo::GenEdge< QGraphConfig >
{
Q_OBJECT
public:
    explicit Edge( QObject* parent = nullptr ) : gtpo::GenEdge<typename QGraphConfig>( parent ) { }
    virtual ~Edge( ) { }
};
*/

/*class Node : public gtpo::GenNode< QGraphConfig >
{
Q_OBJECT
public:
    explicit Node( QObject* parent = nullptr ) : gtpo::GenNode<typename QGraphConfig>( parent ) { }
    virtual ~Node( ) { }

public:
    //Q_PROPERTY( qreal qx READ getQX WRITE setQX NOTIFY qXChanged )
    //qreal   getQX( ) const { return _qx; }
    //void    setQX( qreal qx ) { _qx = qx; _graph->notifyXChanged( *this ); emit qXChanged(); }
    // FIXME 20160204 Simplifier l'interface de notify... (proposer un notifyXChanged() dans gtpo::GenNode<>)...
    // FIXME: ***lock**** FIXME FIXME FIXME FIXME FIXME FIXME
    //void    setQX( qreal qx ) { _qx = qx; getGraph()->notifyXChanged( shared_from_this() ); emit qXChanged(); }
private:
    //qreal   _qx = 0.;
signals:
    //void    qXChanged( );
};

class Graph : public gtpo::GenGraph< QGraphConfig >
{
    Q_OBJECT
public:
    explicit Graph( QObject* parent = nullptr ) : gtpo::GenGraph<QGraphConfig>( parent ) { }

public:
    //Q_PROPERTY( qreal x READ getX WRITE setX NOTIFY xChanged )
    //qreal   getX( ) { return _x; }
    //void    setX( qreal x ) { _x = x; _graph->notifyXChanged( *this ); }
private:
    //qreal   _x = 0.;
signals:
    //void    xChanged( );
};*/

} // ::qtpo Qt Topo

#endif // basic_h

