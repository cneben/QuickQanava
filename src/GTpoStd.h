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
class Group;

template < typename Node, typename Edge >
struct PropertiesConfig {
    using SharedNode = std::shared_ptr< Node >;
    using SharedEdge = std::shared_ptr< Edge >;

    static inline const std::string& getNodeLabel( const SharedNode& n ) { return n->getLabel(); }
    static inline void               setNodeLabel( SharedNode& n, const std::string& label ) { n->setLabel( label ); }

    static inline double    getNodeX( const SharedNode& n ) { return n->getX(); }
    static inline void      setNodeX( SharedNode& n, double x ) { n->setX( x ); }

    static inline double    getNodeY( const SharedNode& n ) { return n->getY(); }
    static inline void      setNodeY( SharedNode& n, double y ) { n->setY( y ); }

    static inline double    getNodeWidth( const SharedNode& n ) { return n->getWidth(); }
    static inline void      setNodeWidth( SharedNode& n, double w ) { n->setWidth( w ); }

    static inline double    getNodeHeight( const SharedNode& n ) { return n->getHeight(); }
    static inline void      setNodeHeight( SharedNode& n, double h ) { n->setHeight( h ); }

    static inline double    getEdgeWeight( const SharedEdge& e ) { return e->getWeight(); }
    static inline void      setEdgeWeight( const SharedEdge& e, double w ) { e->setWeight( w ); }
    static inline void      setEdgeWeight( Edge& e, double w ) { e.setWeight( w ); }
};


class SConfig final :   public gtpo::BaseConfig,
                        public gtpo::StdContainerAccessors,
                        public stpo::PropertiesConfig<stpo::Node,
                                                      stpo::Edge>
{
public:
    using Node = stpo::Node;
    using Edge = stpo::Edge;
    using Group = stpo::Group;

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

class Group : public gtpo::GenGroup< SConfig >
{
public:
};

class Edge : public gtpo::GenEdge< SConfig >
{
public:
    double   getWeight( ) const { return _weight; }
    void    setWeight( double weight ) {
        _weight = weight;
        getGraph()->notifyEdgeModified( WeakEdge{ this->shared_from_this() } );
    }
private:
    double   _weight = 0.;
};

class Node : public gtpo::GenNode< SConfig >
{
public:    
    const std::string&  getLabel( ) const { return _label; }
    void                setLabel( const std::string& label ) {
        _label = label;
        getGraph()->notifyNodeModified( WeakNode{ this->shared_from_this() } );
    }

    double  getX( ) const { return _x; }
    void    setX( double x ) {
        _x = x;
        getGraph()->notifyNodeModified( WeakNode{ shared_from_this() } );
    }

    double  getY( ) const { return _y; }
    void    setY( double y ) {
        _y = y;
        getGraph()->notifyNodeModified( WeakNode{ shared_from_this() } );
    }

    double  getWidth( ) const { return _width; }
    void    setWidth( double width ) {
        _width = width;
        getGraph()->notifyNodeModified( WeakNode{ shared_from_this() } );
    }

    double  getHeight( ) const { return _height; }
    void    setHeight( double height ) {
        _height = height;
        getGraph()->notifyNodeModified( WeakNode{ shared_from_this() } );
    }

private:
    std::string _label = "";
    double      _x  = 0.;
    double      _y  = 0.;
    double      _width  = 0.;
    double      _height = 0.;
};

} // ::stpo STD Topo

#endif // stpo_h
