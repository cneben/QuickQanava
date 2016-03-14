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
// \file	stpo.h
// \author	benoit@qanava.org
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef stpo_h
#define stpo_h

#include "./gtpo.h"

/*! GTpo gtpo graph generated with standard library containers.
 *
 * \note include "stpo.h" to use GTpo stpo module.
 */
namespace stpo {

class Node;
class Edge;
class Graph;

template < typename Node, typename Edge >
struct PropertiesConfig {
    using SharedNode = std::shared_ptr< typename Node >;
    using SharedEdge = std::shared_ptr< typename Edge >;

    static inline double   getNodeX( const SharedNode& n ) { return n->getX(); }
    static inline void     setNodeX( SharedNode& n, double x ) { n->setX( x ); }

    static inline double   getNodeY( const SharedNode& n ) { return n->getY();; }
    static inline void     setNodeY( SharedNode& n, double y ) { n->setY( y ); }

    static inline double   getNodeWidth( const SharedNode& n ) { return n->getWidth();; }
    static inline void     setNodeWidth( SharedNode& n, double w ) { n->setWidth( w ); }

    static inline double   getNodeHeight( const SharedNode& n ) { return n->getHeight(); }
    static inline void     setNodeHeight( SharedNode& n, double h ) { n->setHeight( h ); }

    static inline double    getEdgeWeight( const Edge& e ) { return 0.; }
    static inline void      setEdgeWeight( const SharedEdge& e, double w ) { e->setWeight( w ); }
    static inline void      setEdgeWeight( Edge& e, double w ) { e->setWeight( x ); }
};


class SConfig final :   public gtpo::BaseConfig,
                        public gtpo::StdContainerConfig,
                        public stpo::PropertiesConfig<stpo::Node,
                                                      stpo::Edge>
{
public:
    using Node = stpo::Node;
    using Edge = stpo::Edge;

    template <class...Ts>
    using NodeContainer = std::vector<Ts...>;

    template <class...Ts>
    using EdgeContainer = std::vector<Ts...>;

    template <class T>
    using SearchContainer = std::unordered_set<T>;
};

class Graph : public gtpo::GenGraph< SConfig >
{

};

class Edge : public gtpo::GenEdge< SConfig >
{
public:
    qreal   getWeight( ) const { return _weight; }
    void    setWeight( qreal weight ) {
        _weight = weight;
        getGraph()->notifyEdgeModified( shared_from_this() );
    }
private:
    qreal   _weight = 0.;
};

class Node : public gtpo::GenNode< SConfig >
{
public:    
    qreal   getX( ) const { return _x; }
    void    setX( qreal x ) {
        _x = x;
        getGraph()->notifyNodeModified( shared_from_this() );
    }

    qreal   getY( ) const { return _y; }
    void    setY( qreal y ) {
        _y = y;
        getGraph()->notifyNodeModified( shared_from_this() );
    }

    qreal   getWidth( ) const { return _width; }
    void    setWidth( qreal width ) {
        _width = width;
        getGraph()->notifyNodeModified( shared_from_this() );
    }

    qreal   getHeight( ) const { return _height; }
    void    setHeight( qreal height ) {
        _height = height;
        getGraph()->notifyNodeModified( shared_from_this() );
    }

private:
    qreal   _x  = 0.;
    qreal   _y  = 0.;
    qreal   _width  = 0.;
    qreal   _height = 0.;
};

} // ::stpo STD Topo

#endif // stpo_h
