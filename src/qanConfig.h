/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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
// This file is a part of the Qanava software.
//
// \file	qanConfig.h
// \author	benoit@qanava.org
// \date	2016 02 04
//-----------------------------------------------------------------------------

#ifndef qanConfig_h
#define qanConfig_h

#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#endif

// QT headers
#include <QObject>
#include <QList>
#include <QVector>
#include <QSet>
#include <QQuickItem>
#include <QQuickPaintedItem>   // qan::Edge

// GTpo headers
#include <GTpo>
#include <GTpoQt>

// QuickProperties headers
#include "../QuickProperties/src/qpsContainerListModel.h"

namespace qan { // ::qan

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

    template < typename T >
    struct insert< qps::ContainerListModel<QVector, T> > {
        template <typename ItemT>
        static void  into( qps::ContainerListModel<QVector, T>& c, ItemT t ) { c.append( t ); }
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
    struct remove< qps::ContainerListModel<QVector, T> > {
        template <typename ItemT>
        static void  from( qps::ContainerListModel<QVector, T>& c, ItemT t ) { c.removeAll( t ); }
    };

    template < typename T >
    struct remove< QSet<T> > {
        static void  from( QSet<T>& c, const T& t ) { c.remove( t ); }
    };
};

/*! \brief Exception thrown by QuickQanava to notify runtime error (nullptr assert, etc.).
 *
 * Use what() to have a detailled error description.
 */
class runtime_error : public std::runtime_error
{
public:
    explicit runtime_error (const std::string& what_arg) : std::runtime_error( what_arg ) { }
    explicit runtime_error (const char* what_arg) : std::runtime_error( what_arg ) { }
    runtime_error () : runtime_error( "QuickQanava runtime unrecoverable error." ) { }
};

class Node;
class Edge;
class Group;

template < typename Node, typename Edge, typename Group >
struct PropertiesAccessors : public gtpo::PropertiesAccessors< Node, Edge, Group > {

    static inline std::string   getNodeLabel( const Node* n ) { return n->getLabel().toStdString(); }
    static inline void          setNodeLabel( Node* n, const std::string& s ) { n->setLabel( QString::fromStdString( s ) ); }

    static inline double   getNodeX( const Node* n ) { return n->x(); }
    static inline void     setNodeX( Node* n, double x ) { n->setX( x ); }

    static inline double   getNodeY( const Node* n ) { return n->y();; }
    static inline void     setNodeY( Node* n, double y ) { n->setY( y ); }

    static inline double   getNodeWidth( const Node* n ) { return n->width();; }
    static inline void     setNodeWidth( Node* n, double w ) { n->setWidth( w ); }

    static inline double   getNodeHeight( const Node* n ) { return n->height(); }
    static inline void     setNodeHeight( Node* n, double h ) { n->setHeight( h ); }

    static inline double   getEdgeWeight( const Edge* e ) { Q_UNUSED( e ); return 0.; }
    static inline void     setEdgeWeight( Edge* e, double w ) { Q_UNUSED( e ); Q_UNUSED( w ); }

    static inline std::string   getGroupLabel( const Group* n ) { return n->getLabel().toStdString(); }
    static inline void          setGroupLabel( Node* n, const std::string& s ) { n->setLabel( QString::fromStdString( s ) ); }
};

class Config final : public gtpo::GraphConfig,
                     public qan::ContainerAccessors,
                     public qan::PropertiesAccessors<typename qan::Node, typename qan::Edge, typename qan::Group >
{
public:
    // Base config
    using GraphBase = QQuickItem;
    using NodeBase = QQuickItem;
    using EdgeBase = QQuickPaintedItem;
    using GroupBase = QQuickItem;

    using GraphBehaviours = std::tuple<>;
    using GroupBehaviours = std::tuple<>;

    using Node  = qan::Node;
    using Edge  = qan::Edge;
    using Group = qan::Group;

    template <class ...Args>
    using NodeContainer = qps::ContainerListModel< QVector, Args... >;

    template <class ...Args>
    using EdgeContainer = qps::ContainerListModel< QVector, Args... >;

    template < class ...Args >
    using SearchContainer = QSet< Args... >;
};

} // ::qan

#endif // qanConfig_h

