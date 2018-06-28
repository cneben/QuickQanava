/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// This file is a part of the GTpo software library.
//
// \file	graph.h
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef gtpo_graph_h
#define gtpo_graph_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./utils.h"
#include "./config.h"
#include "./edge.h"
#include "./node.h"
#include "./group.h"
#include "./graph_behaviour.h"
#include "./adjacent_behaviour.h"
#include "./group_behaviour.h"

/*! \brief Main GTpo namespace (\#include \<GTpo\>).
 */
namespace gtpo { // ::gtpo

template <class config_t>
class graph;

template <class config_t>
class graph_group_adjacent_edges_behaviour;

/*! \brief Weighted directed graph using a node-list, edge-list representation.
 *
 * \image html graph-datamodel.png
 *
 * \param BaseClass Optional base class for this graph class
 *
 * \note See http://en.cppreference.com/w/cpp/language/dependent_name for
 *       typename X::template T c++11 syntax and using Nodes = typename config_t::template node_container_t< Node* >;
 *
 */
template < class config_t = gtpo::config<> >
class graph : public config_t::graph_base,
              public gtpo::behaviourable_graph< config_t >
{
    /*! \name Graph Management *///--------------------------------------------
    //@{
public:
    using graph_t           = graph<config_t>;

    // Shortcut for user access to graph configuration.
    using configuration     = config_t;

    using shared_node       = typename std::shared_ptr<typename config_t::final_node_t>;
    using weak_node         = typename std::weak_ptr<typename config_t::final_node_t>;
    using shared_nodes      = typename config_t::template node_container_t< shared_node >;
    using weak_nodes        = typename config_t::template node_container_t< weak_node >;
    using weak_nodes_search = typename config_t::template search_container_t< weak_node >;

    using weak_edge         = typename std::weak_ptr<typename config_t::final_edge_t>;
    using shared_edge       = typename std::shared_ptr<typename config_t::final_edge_t>;
    using weak_edges        = typename config_t::template edge_container_t< weak_edge >;
    using shared_edges      = typename config_t::template edge_container_t< shared_edge >;
    using weak_edges_search = typename config_t::template search_container_t< weak_edge >;

    using shared_group      = typename std::shared_ptr<typename config_t::final_group_t>;
    using weak_group        = typename std::weak_ptr<typename config_t::final_group_t>;
    using shared_groups     = typename config_t::template node_container_t< shared_group >;

    //! User friendly shortcut to this concrete graph behaviour.
    using behaviour = graph_behaviour< config_t >;
    //! User friendly shortcut type to this concrete graph Behaviourable base type.
    using behaviourable_base = gtpo::behaviourable_graph< config_t >;

public:
    using size_type  = typename shared_nodes::size_type;

    graph() noexcept :
        config_t::graph_base{},
        behaviourable_base{} { }

    template < class B >
    explicit graph( B* parent ) noexcept :
        config_t::graph_base{parent},
        behaviourable_base{} { }

    ~graph();

    graph( const graph& ) = delete;
    graph& operator=( const graph& ) = delete;

    /*! \brief Clear the graph from all its content (nodes, edges, groups, behaviours).
     *
     * \note Graph behaviours are cleared after the topology, if you do not want to take into account topology
     * changes when clearing the graph, disable all behaviours before calling clear().
     */
    void    clear() noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Node Management *///---------------------------------------
    //@{
public:
    /*! \brief Create node and insert it in the graph an return a reference to it.
     *
     * \code
     * try{
     *   gtpo::graph<> g;
     *   std::weak_ptr<gtpo::node<>> n1 = g.createNode();
     *   // or preferably
     *   auto n2 = g.createNode();
     * } catch ( const std::runtime_error& e ) { }
     * \endcode
     *
     * With STpo:
     * \code
     * using namespace gtpo;
     * try{
     *   gtpo::graph<> g;
     *   std::weak_ptr<gtpo::Node<>> n1 = g.createNode();
     *   // or preferably
     *   auto n2 = g.createNode();
     * } catch ( const std::runtime_error& e ) { }
     * \endcode
     * \return a reference to the created node (graph has ownership for the node).
     * \throw gtpo::bad_topology_error with an error description if creation fails.
     */
    auto    createNode() noexcept( false ) -> weak_node;

    /*! \brief Insert a node created outside of GTpo into the graph.
     *
     * If your nodes must be created outside of GTpo (ie not with the createNode() method),
     * the only way of giving node ownership to GTpo is trought the insertNode method.
     * Example of a node created from a QML component:
     * \code
     *  qan::Node* node = static_cast< qan::Node* >( createFromQmlComponent( nodeComponent ) );
     *  if ( node != nullptr )
     *    graph<QGraphcConfig>::insertNode( std::shared_ptr<qan::Node>{node} );
     * \endcode
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto    insertNode( shared_node node ) noexcept( false ) -> weak_node;

    /*! \brief Remove node \c node from graph.
     *
     * Complexity depends on config_t::node_container_t.
     * \note If \c weakNode is actually grouped in a group, it will first be ungroup before
     * beeing removed (any group behaviour will also be notified that the node is ungrouped).
     * \throw gtpo::bad_topology_error if node can't be removed (or node is not valid).
     */
    auto    removeNode( weak_node weakNode ) noexcept( false ) -> void;

    //! Return the number of nodes actually registered in graph.
    auto    getNodeCount() const -> size_type { return _nodes.size(); }
    //! Return the number of root nodes (actually registered in graph)ie nodes with a zero in degree).
    auto    getRootNodeCount() const -> size_type { return _rootNodes.size(); }

    /*! \brief Install a given \c node in the root node cache.
     *
     * This method should not be directly used by an end user until you have deeply
     * modified graph topology with non gtpo::graph<> methods.
     *
     * \throw gtpo::bad_topology_error if \c node in degree is different from 0.
     */
    auto    installRootNode( weak_node node ) noexcept( false ) -> void;
    /*! \brief Test if a given \c node is a root node.
     *
     * This method is safer than testing node->getInDegree()==0, since it check
     * \c node in degree and its presence in the internal root node cache.
     *
     * \return true if \c node is a root node, false otherwise.
     * \throw gtpo::bad_topology_error if there is a graph cohenrency problem (ie node is in the root node cache but has a zero in degree).
     */
    auto    isRootNode( weak_node node ) const noexcept( false ) -> bool;

    //! Use fast search container to find if a given \c node is part of this graph.
    auto    contains( weak_node node ) const noexcept -> bool;

    //! Graph main nodes container.
    inline auto     getNodes() const -> const shared_nodes& { return _nodes; }
    //! Return a const begin iterator over graph shared_node nodes.
    inline auto     begin() const -> typename shared_nodes::const_iterator { return _nodes.begin( ); }
    //! Return a const begin iterator over graph shared_node nodes.
    inline auto     end() const -> typename shared_nodes::const_iterator { return _nodes.end( ); }

    //! Return a const begin iterator over graph shared_node nodes.
    inline auto     cbegin() const -> typename shared_nodes::const_iterator { return _nodes.cbegin(); }
    //! Return a const end iterator over graph shared_node nodes.
    inline auto     cend() const -> typename shared_nodes::const_iterator { return _nodes.cend(); }

    //! Graph root nodes container.
    inline auto     getRootNodes() const -> const weak_nodes& { return _rootNodes; }

private:
    shared_nodes         _nodes;
    weak_nodes           _rootNodes;
    weak_nodes_search     _nodesSearch;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Edge Management *///---------------------------------------
    //@{
public:
    /*! \brief Create a directed edge between \c source and \c destination node, then insert it into the graph.
     *
     * Complexity is O(1).
     * \return the inserted edge (if an error occurs edge == false and gtpo::bad_topology_error is thrown).
     * \throw a gtpo::bad_topology_error if creation fails (either \c source or \c destination does not exists).
     */
    template < class Edge_t = edge<config_t> >
    auto        createEdge( weak_node source, weak_node destination ) noexcept(false) -> weak_edge;

    /*! \brief Create a directed hyper edge between \c source node and \c destination edge, then insert it into the graph.
     *
     * Complexity is O(1).
     * \return the inserted edge (if an error occurs edge == false and gtpo::bad_topology_error is thrown).
     * \throw a gtpo::bad_topology_error if creation fails (either \c source or \c destination does not exists).
     */
    auto        createEdge( weak_node source, weak_edge destination ) noexcept(false) -> weak_edge;

    /*! \brief Insert a directed edge created outside of GTpo into the graph.
     *
     * \param edge must have a valid source and destination set otherwise a bad topology exception will be thrown.
     * \sa insertNode()
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto        insertEdge( shared_edge edge ) noexcept( false ) -> weak_edge;

    /*! \brief Remove first directed edge found between \c source and \c destination node.
     *
     * If the current graph<> config_t::edge_container_t and config_t::node_container_t allow parrallel edges support, the first
     * edge found between \c source and \c destination will be removed.
     *
     * Complexity is O(edge count) at worst.
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination or edge does not exists).
     */
    auto        removeEdge( weak_node source, weak_node destination ) noexcept( false ) -> void;

    /*! \brief Remove all directed edge between \c source and \c destination node.
     *
     * If the current graph<> config_t::edge_container_t and config_t::node_container_t allow parrallel edges support, the first
     * edge found between \c source and \c destination will be removed.
     *
     * Worst case complexity is O(edge count).
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination or edge does not exists).
     */
    auto        removeAllEdges( weak_node source, weak_node destination ) noexcept( false ) -> void;

    /*! \brief Remove directed edge \c edge.
     *
     * Worst case complexity is O(edge count).
     * \throw a gtpo::bad_topology_error if suppression fails (\c edge does not exists).
     */
    auto        removeEdge( weak_edge edge ) noexcept( false ) -> void;

    /*! \brief Look for the first directed edge between \c source and \c destination and return it.
     *
     * Worst case complexity is O(edge count).
     * \return A shared reference on edge, en empty shared reference otherwise (result == false).
     * \throw noexcept.
     */
    auto        findEdge( weak_node source, weak_node destination ) const noexcept -> weak_edge;
    /*! \brief Test if a directed edge exists between nodes \c source and \c destination.
     *
     * This method only test a 1 degree relationship (ie a direct edge between \c source
     * and \c destination). Worst case complexity is O(edge count).
     * \throw noexcept.
     */
    auto        hasEdge( weak_node source, weak_node destination ) const noexcept -> bool;
    /*! \brief Look for the first directed restricted hyper edge between \c source node and \c destination edge and return it.
     *
     * Worst case complexity is O(edge count).
     * \return A shared reference on edge, en empty shared reference otherwise (result == false).
     * \throw noexcept.
     */
    auto        findEdge( weak_node source, weak_edge destination ) const noexcept -> weak_edge;
    /*! \brief Test if a directed restricted hyper edge exists between nodes \c source and \c destination.
     *
     * This method only test a 1 degree relationship (ie a direct edge between \c source
     * and \c destination). Worst case complexity is O(edge count).
     * \throw noexcept.
     */
    auto        hasEdge( weak_node source, weak_edge destination ) const noexcept -> bool;

    //! Return the number of edges currently existing in graph.
    auto        getEdgeCount() const noexcept -> unsigned int { return static_cast<int>( _edges.size() ); }
    /*! \brief Return the number of (parallel) directed edges between nodes \c source and \c destination.
     *
     * Graph edge_container_t should support multiple insertions (std::vector, std::list) to enable
     * parrallel edge support, otherwise, getEdgeCount() will always return 1 or 0.
     *
     * This method only test a 1 degree relationship (ie a direct edge between \c source
     * and \c destination). Worst case complexity is O(edge count).
     * \throw no GTpo exception (might throw a std::bad_weak_ptr).
     */
    auto        getEdgeCount( weak_node source, weak_node destination ) const noexcept( false ) -> unsigned int;

    //! Use fast search container to find if a given \c edge is part of this graph.
    auto        contains( weak_edge edge ) const noexcept -> bool;

    //! Graph main edges container.
    inline auto getEdges() const noexcept -> const shared_edges& { return _edges; }
private:
    shared_edges        _edges;
    weak_edges_search   _edgesSearch;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Group Management *///--------------------------------------
    //@{
public:
    /*! \brief Create a new node group with class name \c className and insert it into the graph.
     *
     * Complexity is O(1).
     * \arg className desired class name for the create node group (default to gtpo::Group).
     * \return the inserted group (if an error occurs a gtpo::bad_topology_error is thrown).
     * \throw a gtpo::bad_topology_error if insertion fails.
     */
    auto            createGroup() noexcept( false ) -> weak_group;

    /*! \brief Insert a node group into the graph.
     *
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto            insertGroup( shared_group group ) noexcept( false ) -> weak_group;

    /*! \brief Remove node group \c group.
     *
     * Group content (ie group's nodes) are not removed from the graph, but ungrouped (ie moved from
     * the group to the graph).
     *
     * Worst case complexity is O(group count).
     * \throw a gtpo::bad_topology_error if suppression fails (\c group does not exists).
     */
    auto            removeGroup( weak_group group ) noexcept( false ) -> void;

    //! Return true if a given group \c group is registered in the graph.
    auto            hasGroup( const weak_group& group ) const -> bool;

    //! Return the number of edges currently existing in graph.
    inline auto     getGroupCount() const noexcept -> int { return static_cast<int>( _groups.size() ); }

    //! Graph main edges container.
    inline auto     getGroups() const noexcept -> const shared_groups& { return _groups; }

    /*! \brief Insert an existing node \c weakNode in group \c weakGroup.
     *
     * \note If a behaviour has been installed with gtpo::group::addgroup_behaviour(), behaviour's
     * node_inserted() will be called.
     *
     * \note \c weakNode getGroup() will return \c weakGroup if grouping succeed.
     */
    auto            groupNode( weak_node weakNode, weak_group weakGroup) noexcept(false) -> void
    {
        auto group = weakGroup.lock();
        gtpo::assert_throw( group != nullptr, "gtpo::group<>::groupNode(): Error: trying to insert a node into an expired group." );

        auto node = weakNode.lock();
        gtpo::assert_throw( node != nullptr, "gtpo::group<>::groupNode(): Error: trying to insert an expired node in group." );

        node->setGroup( weakGroup );
        config_t::template container_adapter<weak_nodes>::insert( weakNode, group->_nodes );
        group->notify_node_inserted( weakNode );
    }
    //! \copydoc groupNode()
    auto            groupNode( weak_group weakGroupNode, weak_group weakGroup ) noexcept(false) -> void
    {
        auto group{ weakGroup.lock() };
        gtpo::assert_throw( group != nullptr, "gtpo::group<>::groupNode(): Error: trying to insert a group into an expired group." );

        auto subGroup{ weakGroupNode.lock() };
        gtpo::assert_throw( subGroup != nullptr, "gtpo::group<>::groupNode(): Error: trying to insert an expired group into a group." );

        shared_node subGroupNode = std::static_pointer_cast<node<config_t>>(subGroup);
        groupNode( weak_node{subGroupNode}, weakGroup );
        group->notify_group_inserted( weakGroupNode );
    }

    /*! \brief Insert an existing node \c weakNode in group \c weakGroup group.
     *
     * \note If a behaviour has been installed with gtpo::group::addgroup_behaviour(), behaviour's
     * node_removed() will be called.
     *
     * \note \c node getGroup() will return an expired weak pointer if ungroup succeed.
     */
    auto            ungroupNode( weak_node weakNode, weak_group weakGroup ) noexcept(false) -> void
    {
        auto group = weakGroup.lock();
        gtpo::assert_throw( group != nullptr, "gtpo::group<>::ungroupNode(): Error: trying to ungroup from an expired group." );

        auto node = weakNode.lock();
        gtpo::assert_throw( node != nullptr, "gtpo::group<>::ungroupNode(): Error: trying to ungroup an expired node from a group." );

        gtpo::assert_throw( node->getGroup().lock() == group, "gtpo::group<>::ungroupNode(): Error: trying to ungroup a node that is not part of group." );

        config_t::template container_adapter<weak_nodes>::remove( weakNode, group->_nodes );
        group->notify_node_removed( weakNode );
        node->setGroup( weak_group{} );  // Warning: group must remain valid while notify_node_removed() is called
    }
    //! \copydoc ungroupNode()
    auto            ungroupNode( weak_group weakGroupNode, weak_group weakGroup ) noexcept(false) -> void
    {
        auto group{ weakGroup.lock() };
        gtpo::assert_throw( group != nullptr, "gtpo::group<>::ungroupNode(): Error: trying to ungroup from an expired group." );

        auto subGroup{ weakGroupNode.lock() };
        gtpo::assert_throw( subGroup != nullptr, "gtpo::group<>::ungroupNode(): Error: trying to ungroup an expired group from a group." );

        shared_node subGroupNode = std::static_pointer_cast<node<config_t>>(subGroup);
        group->notify_group_removed( weakGroupNode );
        ungroupNode( weak_node{subGroupNode}, weakGroup );
    }

private:
    shared_groups    _groups;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#endif // gtpo_raph_h

