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
// \author	benoit@destrat.io
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

template < typename Node, typename Edge, typename Group >
struct PropertiesAccessors : public gtpo::PropertiesAccessors<Node, Edge, Group> {
    // Node accessors
    static inline const std::string  getLabel( const Node* n ) noexcept { return n->getLabel(); }
    static inline void               setLabel( Node* n, const std::string& label ) noexcept { n->setLabel( label ); }

    static inline double    getX( const Node* n ) noexcept { return n->getX(); }
    static inline void      setX( Node* n, double x ) noexcept { n->setX( x ); }

    static inline double    getY( const Node* n ) noexcept { return n->getY(); }
    static inline void      setNodeY( Node* n, double y ) noexcept { n->setY( y ); }

    static inline double    getWidth( const Node* n ) noexcept { return n->getWidth(); }
    static inline void      setWidth( Node* n, double w ) noexcept { n->setWidth( w ); }

    static inline double    getHeight( const Node* n ) noexcept { return n->getHeight(); }
    static inline void      setHeight( Node* n, double h ) noexcept { n->setHeight( h ); }

    // Edge accessors
    static inline double    getWeight( const Edge* e ) noexcept { return e->getWeight(); }
    static inline void      setWeight( Edge* e, double w ) noexcept { e->setWeight( w ); }

    // Group accessors
    static inline const std::string     getLabel( const Group* g ) noexcept { return g->getLabel(); }
    static inline void                  setLabel( Group* g, const std::string& l ) noexcept { g->setLabel( l ); }

    static inline double        getX( const Group* g ) noexcept { return g->getX(); }
    static inline void          setX( Group* g, double x ) noexcept { g->setX( x ); }

    static inline double        getY( const Group* g ) noexcept { return g->getY(); }
    static inline void          setY( Group* g, double y ) noexcept { g->setY( y ); }

    static inline double        getWidth( const Group* g ) noexcept { return g->getWidth(); }
    static inline void          setWidth( Group* g, double width ) noexcept { g->setWidth( width ); }

    static inline double        getHeight( const Group* g ) noexcept { return g->getHeight(); }
    static inline void          setHeight( Group* g, double height ) noexcept { g->setHeight( height ); }

    static inline bool          getGroupCollapsed( const Group* g ) noexcept { return g->getCollapsed(); }
    static inline void          setGroupCollapsed( Group* g, bool collapsed ) noexcept { g->setCollapsed( collapsed ); }
};


class Config final :   public gtpo::GraphConfig,
                       public gtpo::StdContainerAccessors,
                       public stpo::PropertiesAccessors<stpo::Node, stpo::Edge, stpo::Group>
{
public:
    using Node = stpo::Node;
    using Edge = stpo::Edge;
    using Group = stpo::Group;

    using GraphBehaviours = std::tuple< gtpo::GraphGroupAjacentEdgesBehaviour< Config > >;
    using GroupBehaviours = std::tuple< gtpo::GroupAdjacentEdgesBehaviour< Config > >;

    template <class...Ts>
    using NodeContainer = std::vector<Ts...>;

    template <class...Ts>
    using EdgeContainer = std::vector<Ts...>;

    template <class T>
    using SearchContainer = std::unordered_set<T>;
};

class Graph : public gtpo::GenGraph< stpo::Config >
{

};

class Group : public gtpo::GenGroup< stpo::Config >
{
public:
    inline const std::string&    getLabel( ) const  { return _label; }
    inline void                  setLabel( const std::string& l ) { _label = l; }

    inline double   getX( ) const { return _x; }
    inline void     setX( double x ) { _x = x; }

    inline double   getY( ) const { return _y; }
    inline void     setY( double y ) { _y = y; }

    inline double   getWidth( ) const { return _width; }
    inline void     setWidth( double width ) { _width = width; }

    inline double   getHeight( ) const { return _height; }
    inline void     setHeight( double height ) { _height = height ; }

    inline bool     getCollapsed( ) const { return _collapsed; }
    inline void     setCollapsed( bool collapsed ) { _collapsed = collapsed; }

protected:
    std::string _label;
    double      _x = 0.;
    double      _y = 0.;
    double      _width = 0.;
    double      _height = 0.;
    bool        _collapsed = false;
};

class Edge : public gtpo::GenEdge< stpo::Config >
{
public:
    double   getWeight( ) const { return _weight; }
    void    setWeight( double weight ) {
        _weight = weight;
        WeakEdge edge{ this->shared_from_this() };
        getGraph()->notifyEdgeModified( edge );
    }
private:
    double   _weight = 0.;
};

class Node : public gtpo::GenNode< stpo::Config >
{
public:    
    const std::string&  getLabel( ) const { return _label; }
    void                setLabel( const std::string& label ) {
        _label = label;
        WeakNode node{ this->shared_from_this() };
        getGraph()->notifyNodeModified( node );
    }

    double  getX( ) const { return _x; }
    void    setX( double x ) {
        _x = x;
        WeakNode node{ this->shared_from_this() };
        getGraph()->notifyNodeModified( node );
    }

    double  getY( ) const { return _y; }
    void    setY( double y ) {
        _y = y;
        WeakNode node{ this->shared_from_this() };
        getGraph()->notifyNodeModified( node );
    }

    double  getWidth( ) const { return _width; }
    void    setWidth( double width ) {
        _width = width;
        WeakNode node{ this->shared_from_this() };
        getGraph()->notifyNodeModified( node );
    }

    double  getHeight( ) const { return _height; }
    void    setHeight( double height ) {
        _height = height;
        WeakNode node{ this->shared_from_this() };
        getGraph()->notifyNodeModified( node );
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
