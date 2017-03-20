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
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpoBehaviour_h
#define gtpoBehaviour_h

// STD headers
#include <cstddef>          // std::size_t
#include <functional>       // std::function
#include <vector>
#include <memory>

// GTpo headers
<<<<<<< HEAD
// Nil

namespace gtpo { // ::gtpo

/*! \brief Define a node observer interface.
 *
 */
template < class Config >
class NodeBehaviour
{
public:
    NodeBehaviour( const std::string& name ) noexcept : _name( name ) { }
    virtual ~NodeBehaviour() = default;
    NodeBehaviour( const NodeBehaviour& ) = delete;
    NodeBehaviour& operator=( const NodeBehaviour& ) = delete;

    using WeakNode          = std::weak_ptr< typename Config::Node >;
    using SharedNode        = std::shared_ptr< typename Config::Node >;
    using SharedNodes       = typename Config::template NodeContainer< SharedNode >;
    using WeakNodesSearch   = typename Config::template SearchContainer< WeakNode >;

public:
    inline auto getName() const noexcept -> const std::string& { return _name; }
protected:
    inline auto setName( const std::string& name )  noexcept -> void { _name = name; }
private:
    std::string _name = "";

public:
    /*! \brief Called immediatly after an in-edge with source \c weakInNode has been inserted.
     */
    virtual void    inNodeInserted( WeakNode& weakInNode )  noexcept { (void)weakInNode; }

    /*! \brief Called when an in-edge with source \c weakInNode is about to be removed.
     */
    virtual void    inNodeAboutToBeRemoved( WeakNode& weakInNode )  noexcept { (void)weakInNode; }

    /*! \brief Called immediatly after an in node has been removed.
     */
    virtual void    inNodeRemoved()  noexcept { }

    /*! \brief Called immediatly after an out-edge with destination \c weakOutNode has been inserted.
     */
    virtual void    outNodeInserted( WeakNode& weakOutNode )  noexcept { (void)weakOutNode; }

    /*! \brief Called when an out-edge with destination \c weakOutNode is about to be removed.
     */
    virtual void    outNodeRemoved( WeakNode& weakOutNode )  noexcept { (void)weakOutNode; }

    /*! \brief Called immediatly after an out-edge has been removed.
     */
    virtual void    outNodeRemoved()  noexcept { }

    /*! \brief Called immedialty after \c weakNode has been modified.
     *
     * Creating, inserting or removing a node does not generate a nodeModified() call.
     *
     * A call to nodeModified() usually follow a node property modification trought
     * the gtpo::GenGraph::Config PropertiesConfig interface.
     */
    virtual void    nodeModified( WeakNode& weakNode ) noexcept { (void)weakNode; }
};


/*! \brief Define an edge observer interface.
 *
 */
template < class Config >
class EdgeBehaviour
{
public:
    EdgeBehaviour() noexcept {}
    virtual ~EdgeBehaviour() = default;
    EdgeBehaviour( const EdgeBehaviour& ) = delete;
    EdgeBehaviour& operator=( const EdgeBehaviour& ) = delete;

    using Edge              = typename Config::Edge;
    using SharedEdge        = std::shared_ptr< typename Config::Edge >;
    using WeakEdge          = std::weak_ptr< typename Config::Edge >;

    /*! \brief Called immediatly after \c weakEdge has been modified.
     *
     * Creating, inserting or removing an edge does not generate an edgeModified() call.
     *
     * A call to edgeModified() usually follow an edge property modification trought
     * the gtpo::GenGraph::Config PropertiesConfig interface.
     */
    virtual void    edgeModified( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
};


=======
/* Nil */

namespace gtpo { // ::gtpo

>>>>>>> dev
/*! \brief Base class for concrete behaviours.
 *
 * \image html behaviour-class.png
 *
 */
template <class Config >
class Behaviour
{
public:
<<<<<<< HEAD
    Behaviour() noexcept { }
=======
    Behaviour( const std::string& name = {} ) noexcept : _name{name} { }
>>>>>>> dev
    virtual ~Behaviour() { }

    explicit Behaviour( const Behaviour& ) = delete;
    Behaviour& operator=( const Behaviour& ) = delete;

public:
<<<<<<< HEAD
=======
    inline auto getName() const noexcept -> const std::string& { return _name; }
protected:
    inline auto setName( const std::string& name )  noexcept -> void { _name = name; }
private:
    std::string _name = "";

public:
>>>>>>> dev
    /*! \brief Enable this behaviour until it is disabled again with a call to disable().
     *
     * \note enabling a disabled behaviour does not apply behaviour on changes made
     *       while it was disabled.
     * \sa isEnabled() for enabled state description.
     */
    inline auto     enable() noexcept -> void { _enabled = true; notifyEnabledChanged(); }
    /*! \brief Disable this behaviour until it is enabled back wit a call to enable().
     *
     * \sa isEnabled() for enabled state description.
     */
    inline auto     disable() noexcept -> void { _enabled = false; notifyEnabledChanged(); }
    /*! \brief Return the actual "enabled" state for this behaviour.
     *
     *  This method could be usefull in very specific use cases, such as serialization or insertion of a large number
     *  of nodes in a graph or group where this behaviour is applied.
     *  A behaviour is enabled by default after its creation.
     *
     *  \return true if enable() has been called, false if disable() has been called.
     */
    inline auto     isEnabled( ) const noexcept -> bool { return _enabled; }

    //! Optionnal notifier for the enabled property.
    virtual void    notifyEnabledChanged() { }

protected:
    bool            _enabled = true;
};

<<<<<<< HEAD
/*! \brief Define an observer interface to catch changes in a gtpo::GenGroup.
 *
 */
template < class Config >
class GroupBehaviour :  public Behaviour< Config >
{
    /*! \name GroupBehaviour Object Management *///----------------------------
    //@{
public:
    GroupBehaviour() noexcept { }
    virtual ~GroupBehaviour() = default;
    GroupBehaviour( const GroupBehaviour& ) = delete;
    GroupBehaviour& operator=( const GroupBehaviour& ) = delete;

    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using Group         = typename Config::Group;
    using SharedGroup   = std::shared_ptr< typename Config::Group >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Notification Interface *///---------------------------------
    //@{
    //! Called immediatly after \c weakNode has been inserted.
    virtual void    nodeInserted( WeakNode& weakNode ) noexcept { (void)weakNode; }
    //! Called when \c weakNode is about to be removed.
    virtual void    nodeRemoved( WeakNode& weakNode ) noexcept { (void)weakNode; }
    //! \copydoc gtpo::NodeBehaviour::nodeModified()
    virtual void    nodeModified( WeakNode& weakNode ) noexcept { (void)weakNode; }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Edge Notification Interface *///---------------------------------
    //@{
public:
    //! Called immediatly after \c weakEdge has been inserted.
    virtual void    edgeInserted( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //! Called when \c weakEdge is about to be removed.
    virtual void    edgeRemoved( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //! \copydoc gtpo::EdgeBehaviour::edgeModified()
    virtual void    edgeModified( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Notification Interface *///--------------------------------
    //@{
public:
    /*! \brief Called whenever group \c weakGroup has been modified.
     *
     * Called in the following situations:
     * \li node or edge inserted or removed from group
     * \li group label changed
     */
    virtual void    groupModified( WeakGroup& weakGroup ) noexcept { (void)weakGroup; }
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Define an observer interface to catch changes in a gtpo::GenGraph.
 *
 */
template < class Config >
class GraphBehaviour :  public GroupBehaviour< Config >
{
public:
    GraphBehaviour() noexcept { }
    virtual ~GraphBehaviour() { }
    GraphBehaviour( const GraphBehaviour& ) = delete;
    GraphBehaviour& operator=( const GraphBehaviour& ) = delete;

    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using Group         = typename Config::Group;
    using SharedGroup   = std::shared_ptr< typename Config::Group >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;

    //! Called immediatly after group \c weakGroup has been inserted in graph.
    virtual void    groupInserted( WeakGroup& weakGroup ) noexcept { (void)weakGroup; }
    //! Called immediatly before group \c weakGroup is removed from graph.
    virtual void    groupRemoved( WeakGroup& weakGroup ) noexcept { (void)weakGroup; }

    /*! \name Edge Notification Interface *///---------------------------------
    //@{
public:
    //! Called immediatly after \c weakEdge has been inserted.
    virtual void    edgeInserted( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //! Called when \c weakEdge is about to be removed.
    virtual void    edgeRemoved( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //! \copydoc gtpo::EdgeBehaviour::edgeModified()
    virtual void    edgeModified( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //@}
    //-------------------------------------------------------------------------

};

=======
>>>>>>> dev
// C++14 O(N log(N)) copied from: http://stackoverflow.com/a/26902803
// There is more complex O(N) solutions available on SO
template< class F, class...Ts, std::size_t...Is >
void for_each_in_tuple( std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...> ) noexcept {
    (void)tuple; // Avoid C4100 warning for size 1 std::tuple with MSVC2015 U2
    (void)func;  // Avoid C4100 warning for size 1 std::tuple with MSVC2015 U2
    using expander = int[];
    (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template< class F, class...Ts >
void for_each_in_tuple( std::tuple<Ts...>& tuple, F func ) noexcept {
    for_each_in_tuple( tuple, func, std::make_index_sequence<sizeof...(Ts)>() );
}

//! Empty interface definition for graph primitives supporting behaviour concept.
struct IBehaviorable {
    IBehaviorable() noexcept = default;
    ~IBehaviorable() = default;
    IBehaviorable( const IBehaviorable& ) = delete;
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
    Behaviourable() noexcept { }
    ~Behaviourable() { _behaviours.clear(); }
    Behaviourable( const Behaviourable& ) = delete;
    Behaviourable& operator=( const Behaviourable& ) = delete;

public:
    //! Clear all actually regsitered behaviours (they are automatically deleted).
<<<<<<< HEAD
    inline  auto    clear( ) -> void { _behaviours.clear(); }
=======
    inline  auto    clear() -> void { _behaviours.clear(); }
>>>>>>> dev
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
<<<<<<< HEAD
     *   stpo::Graph sg;
=======
     *   gtpo::GenGraph<> sg;
>>>>>>> dev
     *   auto myBehaviour = std::make_unique< MyBehaviour >( );
     *   sg.addBehaviour( myBehaviour.release() );
     * \endcode
     *
     * \param behaviour Graph will get behaviour ownership.
     */
    inline auto     addBehaviour( std::unique_ptr<Behaviour> behaviour ) -> void {
        _behaviours.emplace_back( std::move( behaviour ) );
    }

    //! std::vector of std::unique_ptr pointers on Behaviour.
    using Behaviours = std::vector< std::unique_ptr< Behaviour > >;

public:
    //! Return true if a behaviours is currently registered (ie getBehaviours().size()>0).
    inline auto    hasBehaviours() const noexcept -> bool { return _behaviours.size() > 0; }

    //! Return a read only container of actually registered behaviours.
    inline auto    getBehaviours() const noexcept -> const Behaviours& { return _behaviours; }

protected:
    /*! \brief Apply a method pointer on all registered behaviours.
     *
     * Example use:
     * \code
     *    // For a given NodeBehaviour method: auto    notifyNodeModified( WeakNode& node ) -> void;
     *    notifyBehaviours< WeakNode >( &Behaviour::nodeModified, node );
     * \endcode
     */
    template < class T >
    auto    notifyBehaviours( void (Behaviour::*method)(T&) noexcept, T& arg ) noexcept -> void;

<<<<<<< HEAD
=======
    template < class T, class T2 >
    auto    notifyBehaviours( void (Behaviour::*method)(T&, T2&) noexcept, T& arg, T2&) noexcept -> void;

>>>>>>> dev
    //! Similar to notifyBahaviours() but without arguments.
    auto    notifyBehaviours0( void (Behaviour::*method)() noexcept ) noexcept -> void;

private:
    Behaviours  _behaviours;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Static Behaviours Management *///--------------------------------
    //@{
public:
    /*! \brief Apply a functor on all registered static behaviours.
     *
     * Example use:
     * \code
     * \endcode
     */
    template < class Functor >
    auto    sNotifyBehaviours( Functor f ) noexcept -> void {
        for_each_in_tuple( _sBehaviours, f );
    }

private:
    SBehaviours  _sBehaviours;
    //@}
    //-------------------------------------------------------------------------
<<<<<<< HEAD

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:

    // Group Behaviours Notification
    template < class Node >
    auto    notifyNodeInserted( Node& node ) noexcept -> void;

    template < class Node >
    auto    notifyNodeRemoved( Node& node ) noexcept -> void;

    //! Notify all behaviors that node \c node inside this group has been modified.
    template < class Node >
    auto    notifyNodeModified( Node& node ) noexcept -> void;

    template < class Edge >
    auto    notifyEdgeInserted( Edge& edge ) noexcept -> void;

    template < class Edge >
    auto    notifyEdgeRemoved( Edge& edge ) noexcept -> void;

    //! Notify all behaviors that node \c node inside this group has been modified.
    template < class Edge >
    auto    notifyEdgeModified( Edge& edge ) noexcept -> void;

    // GraphBehaviours Notification
    template < class Group >
    auto    notifyGroupInserted( Group& group ) noexcept -> void;

    template < class Group >
    auto    notifyGroupRemoved( Group& group ) noexcept -> void;

    //! Notify all behaviors that group \c group has been modified.
    template < class G >
    auto    notifyGroupModified( G& group ) noexcept -> void;
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
    GroupAdjacentEdgesBehaviour() noexcept = default;
    virtual ~GroupAdjacentEdgesBehaviour() = default;
    GroupAdjacentEdgesBehaviour( const GroupAdjacentEdgesBehaviour& ) = delete;
    GroupAdjacentEdgesBehaviour& operator=( const GroupAdjacentEdgesBehaviour& ) = delete;

    using SharedNode    = std::shared_ptr< typename Config::Node >;
    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using SharedEdge    = std::shared_ptr< typename Config::Edge >;
    using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;
    using WeakGroup     = std::weak_ptr< typename Config::Group >;
    using SharedGroup   = std::shared_ptr< typename Config::Group >;

public:
    virtual void    groupModified( WeakGroup& weakGroup ) noexcept override { (void)weakGroup; }
    virtual void    nodeInserted( WeakNode& weakNode ) noexcept override;
    virtual void    nodeRemoved( WeakNode& weakNode ) noexcept override;
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
    GraphGroupAjacentEdgesBehaviour() noexcept { }
    virtual ~GraphGroupAjacentEdgesBehaviour() { }
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
    virtual void    edgeInserted( WeakEdge& weakEdge ) noexcept override;
    virtual void    edgeRemoved( WeakEdge& weakEdge ) noexcept override;
    //-------------------------------------------------------------------------
=======
>>>>>>> dev
};

} // ::gtpo

#include "./gtpoBehaviour.hpp"

#endif // gtpoBehaviour_h
