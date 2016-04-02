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
// \file	gtpoBehaviour.h
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpoBehaviour_h
#define gtpoBehaviour_h

// STD headers
#include <functional>       // std::function
#include <vector>

// GTpo headers
// Nil

namespace gtpo { // ::gtpo

/*! \brief Define a node observer interface.
 *
 */
template < class Config >
class NodeBehaviour
{
public:
    NodeBehaviour() = default;
    virtual ~NodeBehaviour() = default;
    explicit NodeBehaviour( const NodeBehaviour& ) = default;
    NodeBehaviour& operator=( const NodeBehaviour& ) = default;

    using WeakNode          = std::weak_ptr< typename Config::Node >;
    using SharedNode        = std::shared_ptr< typename Config::Node >;
    using SharedNodes       = typename Config::template NodeContainer< SharedNode >;
    using WeakNodesSearch   = typename Config::template SearchContainer< WeakNode >;

    //! Called immediatly after \c weakNode has been inserted.
    virtual auto nodeInserted( WeakNode& weakNode ) -> void { (void)weakNode; }
    //! Called when \c weakNode is about to be removed.
    virtual auto nodeRemoved( WeakNode& weakNode ) -> void { (void)weakNode; }
    /*! \brief Called immedialty after \c weakNode has been modified.
     *
     * Creating, inserting or removing a node does not generate a nodeModified() call.
     *
     * A call to nodeModified() usually follow a node property modification trought
     * the gtpo::GenGraph::Config PropertiesConfig interface.
     */
    virtual auto    nodeModified( WeakNode& weakNode ) -> void { (void)weakNode; }
};


/*! \brief Define an edge observer interface.
 *
 */
template < class Config >
class EdgeBehaviour
{
public:
    EdgeBehaviour() = default;
    virtual ~EdgeBehaviour() = default;
    explicit EdgeBehaviour( const EdgeBehaviour& ) = default;
    EdgeBehaviour& operator=( const EdgeBehaviour& ) = default;

    using Edge              = typename Config::Edge;
    using SharedEdge        = std::shared_ptr< typename Config::Edge >;
    using WeakEdge          = std::weak_ptr< typename Config::Edge >;
    using WeakEdges         = typename Config::template EdgeContainer< WeakEdge >;
    using SharedEdges       = typename Config::template EdgeContainer< SharedEdge >;
    using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;

    //! Called immediatly after \c weakEdge has been inserted.
    virtual auto    edgeInserted( WeakEdge& weakEdge ) -> void { (void)weakEdge; }
    //! Called when \c weakEdge is about to be removed.
    virtual auto    edgeRemoved( WeakEdge& weakEdge ) -> void { (void)weakEdge; }
    /*! \brief Called immediatly after \c weakEdge has been modified.
     *
     * Creating, inserting or removing an edge does not generate an edgeModified() call.
     *
     * A call to edgeModified() usually follow an edge property modification trought
     * the gtpo::GenGraph::Config PropertiesConfig interface.
     */
    virtual auto    edgeModified( WeakEdge& weakEdge ) -> void { (void)weakEdge; }
};


/*! \brief Base class for concrete behaviours.
 *
 * \image html behaviour-class.png
 *
 */
template <class Config >
class Behaviour
{
public:
    Behaviour() = default;
    virtual ~Behaviour() { }

    explicit Behaviour( const Behaviour& ) = default;
    Behaviour& operator=( const Behaviour& ) = delete;

public:
    /*! Enable this behaviour until it is disabled again with a call to disable().
     *
     * \note enabling a disabled behaviour does not apply behaviour on changes made
     *       while it was disabled.
     * \sa isEnabled() for enabled state description.
     */
    auto    enable( ) -> void { _enabled = true; notifyEnabledChanged(); }
    /*! Disable this behaviour until it is enabled back wit a call to enable().
     *
     * \sa isEnabled() for enabled state description.
     */
    auto    disable( ) -> void { _enabled = false; notifyEnabledChanged(); }
    /*! Return the actual "enabled" state for this behaviour.
     *
     *  This method could be usefull in very specific use cases, such as serialization or insertion of a large number
     *  of nodes in a graph or group where this behaviour is applied.
     *  A behaviour is enabled by default after its creation.
     *
     *  \return true if enable() has been called, false if disable() has been called.
     */
    auto    isEnabled( ) const -> bool { return _enabled; }

    //! Optionnal notifier for the enabled property.
    virtual void    notifyEnabledChanged() { }

protected:
    bool            _enabled = true;
};

/*! \brief Define an observer interface to catch changes in a gtpo::GenGroup.
 *
 */
template < class Config >
class GroupBehaviour :  public Behaviour< Config >,
                        public NodeBehaviour< Config >,
                        public EdgeBehaviour< Config >
{
public:
    GroupBehaviour() = default;
    virtual ~GroupBehaviour() = default;
    explicit GroupBehaviour( const GroupBehaviour& ) = default;
    GroupBehaviour& operator=( const GroupBehaviour& ) = delete;

    using Group         = typename Config::Group;
    using SharedGroup   = std::shared_ptr< typename Config::Group >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;

public:
    /*! Called whenever group \c weakGroup has been modified.
     *
     * Called in the following situations:
     * \li node or edge inserted or removed from group
     * \li group label changed
     */
    virtual auto    groupModified( WeakGroup& weakGroup ) -> void { (void)weakGroup; }
};

/*! \brief Define an observer interface to catch changes in a gtpo::GenGraph.
 *
 */
template < class Config >
class GraphBehaviour :  public GroupBehaviour< Config >
{
public:
    GraphBehaviour() = default;
    virtual ~GraphBehaviour() = default;
    explicit GraphBehaviour( const GraphBehaviour& ) = default;
    GraphBehaviour& operator=( const GraphBehaviour& ) = delete;

    using WeakGroup     = std::weak_ptr< typename Config::Group >;

    //! Called immediatly after group \c weakGroup has been inserted in graph.
    virtual auto    groupInserted( WeakGroup& weakGroup ) -> void { (void)weakGroup; }
    //! Called immediatly before group \c weakGroup has been removed from graph.
    virtual auto    groupRemoved( WeakGroup& weakGroup ) -> void { (void)weakGroup; }
};

// C++14 O(N log(N)) copied from: http://stackoverflow.com/a/26902803
// There is more complex O(N) solutions available on SO
template< class F, class...Ts, std::size_t...Is >
void for_each_in_tuple( std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...> ) {
    (void)tuple; // Avoid C4100 warning for size 1 std::tuple with MSVC2015
    (void)func;  // Avoid C4100 warning for size 1 std::tuple with MSVC2015
    using expander = int[];
    (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template< class F, class...Ts >
void for_each_in_tuple( std::tuple<Ts...>& tuple, F func ) {
    for_each_in_tuple( tuple, func, std::make_index_sequence<sizeof...(Ts)>() );
}

//! Empty interface definition for graph primitives supporting behaviour concept.
struct IBehaviorable {
    IBehaviorable() = default;
    ~IBehaviorable() = default;
    explicit IBehaviorable( const IBehaviorable& ) = default;
    IBehaviorable& operator=( const IBehaviorable& ) = delete;
};

/*! \brief Base class for all type supporting behaviours (actually gtpo::GenGraph and gtpo::GenGroup).
 *
 * \nosubgrouping
 */
template < class Behaviour, class SBehaviours >
class Behaviourable : public IBehaviorable
{
    /*! \name Behaviorable Object Management *///------------------------------
    //@{
public:
    Behaviourable() = default;
    ~Behaviourable() { _behaviours.clear(); }
    explicit Behaviourable( const Behaviourable& ) = default;
    Behaviourable& operator=( const Behaviourable& ) = delete;

public:
    //! Clear all actually regsitered behaviours (they are automatically deleted).
    inline  auto    clear( ) -> void { _behaviours.clear(); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Virtual Behaviours Management *///-------------------------------
    //@{
public:
    /*! \brief Add a behaviour to this graph (graph get ownership for behaviour).
     *
     * \note This is a sink method, Behaviourable get ownership for \c behaviour, since it is
     * difficult to "downcast" unique_ptr, use the following code for calling:
     * \code
     *   stpo::Graph sg;
     *   auto myBehaviour = std::make_unique< MyBehaviour >( );
     *   sg.addBehaviour( myBehaviour.release() );
     * \endcode
     *
     * \param behaviour Graph will get behaviour ownership.
     */
    auto    addBehaviour( Behaviour* behaviour ) -> void {
        _behaviours.emplace_back( std::move( behaviour ) );
    }

    //! std::vector of std::unique_ptr pointers on Behaviour.
    using Behaviours = std::vector< std::unique_ptr< Behaviour > >;

public:
    //! Return true if a behaviours is currently registered (ie getBehaviours().size()>0).
    auto    hasBehaviours() const -> bool { return _behaviours.size() > 0; }

    //! Return a read only container of actually registered behaviours.
    auto    getBehaviours() const -> Behaviours& { return _behaviours; }

protected:
    /*! Apply a method pointer on all registered behaviours.
     *
     * Example use:
     * \code
     *    // For a given NodeBehaviour method: auto    notifyNodeModified( WeakNode& node ) -> void;
     *    notifyBehaviours< WeakNode >( &Behaviour::nodeModified, node );
     * \endcode
     */
    template < class T >
    auto    notifyBehaviours( void (Behaviour::*method)(T&), T& arg ) -> void;

private:
    Behaviours  _behaviours;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Static Behaviours Management *///--------------------------------
    //@{
public:
    /*! Apply a functor on all registered static behaviours.
     *
     * Example use:
     * \code
     * \endcode
     */
    template < class Functor >
    auto    sNotifyBehaviours( Functor f ) -> void {
        for_each_in_tuple( _sBehaviours, f );
    }

private:
    SBehaviours  _sBehaviours;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    template < class Node >
    auto    notifyNodeInserted( Node& node ) -> void;

    template < class Node >
    auto    notifyNodeRemoved( Node& node ) -> void;

    //! Notify all behaviors that node \c node inside this group has been modified.
    template < class Node >
    auto    notifyNodeModified( Node& node ) -> void;

    template < class Edge >
    auto    notifyEdgeInserted( Edge& edge ) -> void;

    template < class Edge >
    auto    notifyEdgeRemoved( Edge& edge ) -> void;

    //! Notify all behaviors that node \c node inside this group has been modified.
    template < class Edge >
    auto    notifyEdgeModified( Edge& edge ) -> void;

    template < class Group >
    auto    notifyGroupInserted( Group& group ) -> void;

    template < class Group >
    auto    notifyGroupRemoved( Group& group ) -> void;

    //! Notify all behaviors that group \c group has been modified.
    template < class G >
    auto    notifyGroupModified( G& group ) -> void;
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Collect and maintain a group adjacent edge set when a node is removed or inserted in a group.
 *
 *  This behaviour should be used with GraphGroupAjacentEdgesBehaviour.
 */
template < class Config >
class GroupAdjacentEdgesBehaviour : public GroupBehaviour< Config >
{
    /* Group Edge Set Behaviour *///-------------------------------------------
public:
    GroupAdjacentEdgesBehaviour() = default;
    virtual ~GroupAdjacentEdgesBehaviour() = default;
    explicit GroupAdjacentEdgesBehaviour( const GroupAdjacentEdgesBehaviour& ) = default;
    GroupAdjacentEdgesBehaviour& operator=( const GroupAdjacentEdgesBehaviour& ) = delete;

    using SharedNode    = std::shared_ptr< typename Config::Node >;
    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using SharedEdge    = std::shared_ptr< typename Config::Edge >;
    using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;
    using SharedGroup   = std::shared_ptr< typename Config::Group >;

public:
    virtual auto    groupModified( WeakGroup& weakGroup ) -> void override { (void)weakGroup; }
    virtual auto    nodeInserted( WeakNode& weakNode ) -> void override;
    virtual auto    nodeRemoved( WeakNode& weakNode ) -> void override;
    //-------------------------------------------------------------------------
};

/*! \brief Collect and maintain a group adjacent edge set and group edge set.
 *
 */
template < class Config >
class GraphGroupAjacentEdgesBehaviour : public GraphBehaviour< Config >
{
    /* Group Edge Set Behaviour *///-------------------------------------------
public:
    GraphGroupAjacentEdgesBehaviour() = default;
    virtual ~GraphGroupAjacentEdgesBehaviour() = default;
    GraphGroupAjacentEdgesBehaviour( const GraphGroupAjacentEdgesBehaviour& ) = default;
    GraphGroupAjacentEdgesBehaviour& operator=( const GraphGroupAjacentEdgesBehaviour& ) = delete;

    using SharedNode    = std::shared_ptr< typename Config::Node >;
    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using SharedEdge    = std::shared_ptr< typename Config::Edge >;
    using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;
    using SharedGroup   = std::shared_ptr< typename Config::Group >;

public:
    virtual auto    edgeInserted( WeakEdge& weakEdge ) -> void override;
    virtual auto    edgeRemoved( WeakEdge& weakEdge ) -> void override;
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoBehaviour.hpp"

#endif // gtpoBehaviour_h
