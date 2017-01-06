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
// This file is a part of the QuickQanava software library.
//
// \file	qanConfig.h
// \author	benoit@destrat.io
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

// QuickContainers headers
#include "../QuickContainers/src/qcmContainerModel.h"

// QuickQanava headers
#include "./qanNavigable.h"

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
    struct insert< qcm::ContainerModel<QVector, T> > {
        template <typename ItemT>
        static void  into( qcm::ContainerModel<QVector, T>& c, ItemT t ) { c.append( t ); }
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
    struct remove< qcm::ContainerModel<QVector, T> > {
        template <typename ItemT>
        static void  from( qcm::ContainerModel<QVector, T>& c, ItemT t ) { c.removeAll( t ); }
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

    // Node accessors
    // 20160329: Warning, do not return by reference in Qan2
    static inline std::string  getLabel( const Node* n ) noexcept { return n->getLabel().toStdString(); }
    static inline void         setLabel( Node* n, const std::string& s ) noexcept { n->setLabel( QString::fromStdString( s ) ); }

    static inline double   getX( const Node* n ) noexcept { return n->x(); }
    static inline void     setX( Node* n, double x ) noexcept { n->setX( x ); }

    static inline double   getY( const Node* n ) noexcept { return n->y();; }
    static inline void     setNodeY( Node* n, double y ) noexcept { n->setY( y ); }

    static inline double   getZ( const Node* n ) noexcept { return n->z();; }
    static inline void     setZ( Node* n, double z ) noexcept { n->setZ( z ); }

    static inline double   getWidth( const Node* n ) noexcept { return n->width();; }
    static inline void     setWidth( Node* n, double w ) noexcept { n->setWidth( w ); }

    static inline double   getHeight( const Node* n ) noexcept { return n->height(); }
    static inline void     setHeight( Node* n, double h ) noexcept { n->setHeight( h ); }

    // Edge accessors
    static inline double   getWeight( const Edge* e ) noexcept { Q_UNUSED( e ); return 0.; }
    static inline void     setWeight( Edge* e, double w ) noexcept { Q_UNUSED( e ); Q_UNUSED( w ); }

    static inline double   getZ( const Edge* e ) noexcept { return e->z(); }
    static inline void     setZ( Edge* e, double z ) noexcept { e->setZ( z ); }

    // Group accessors
    // 20160329: Warning, do not return by reference in Qan2
    static inline std::string   getLabel( const Group* g ) noexcept { return g->getLabel().toStdString(); }
    static inline void          setLabel( Group* g, const std::string& s ) noexcept { g->setLabel( QString::fromStdString( s ) ); }

    static inline double        getX( const Group* g ) noexcept { return g->x(); }
    static inline void          setX( Group* g, double x ) noexcept { g->setX( x ); }

    static inline double        getY( const Group* g ) noexcept { return g->y(); }
    static inline void          setY( Group* g, double y ) noexcept { g->setY( y ); }

    static inline double        getZ( const Group* g ) noexcept { return g->z(); }
    static inline void          setZ( Group* g, double z ) noexcept { g->setZ( z ); }

    static inline double        getWidth( const Group* g ) noexcept { return g->width(); }
    static inline void          setWidth( Group* g, double width ) noexcept { g->setWidth( width ); }

    static inline double        getHeight( const Group* g ) noexcept { return g->height(); }
    static inline void          setHeight( Group* g, double height ) noexcept { g->setHeight( height ); }

    static inline bool          getGroupCollapsed( const Group* g ) noexcept { return g->getCollapsed(); }
    static inline void          setGroupCollapsed( Group* g, bool collapsed ) noexcept { g->setCollapsed( collapsed ); }
};

class Config final : public gtpo::GraphConfig,
                     public qan::ContainerAccessors,
                     public qan::PropertiesAccessors<typename qan::Node, typename qan::Edge, typename qan::Group >
{
public:
    // Base config
    using GraphBase = QQuickItem;
    using NodeBase  = QQuickItem;
    using EdgeBase  = QQuickItem;
    using GroupBase = QQuickItem;

    using GraphBehaviours = std::tuple< gtpo::GraphGroupAjacentEdgesBehaviour< Config > >;
    using GroupBehaviours = std::tuple< gtpo::GroupAdjacentEdgesBehaviour< Config > >;

    using Node  = qan::Node;
    using Edge  = qan::Edge;
    using Group = qan::Group;

    template <class ...Args>
    using NodeContainer = qcm::ContainerModel< QVector, Args... >;

    template <class ...Args>
    using EdgeContainer = qcm::ContainerModel< QVector, Args... >;

    template < class ...Args >
    using SearchContainer = QSet< Args... >;
};

} // ::qan

#endif // qanConfig_h

