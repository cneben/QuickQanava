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
 * \note Root nodes could be considered as "mother vertices" as described in graph theory of directed graphs ... but
 * the actual implementation in GTpo has no protection for more than 0 degree cycles... When graph has cycles with
 * degree > 1, we could actually have strongly connected components ... with no root nodes !
 *
 * \note See http://en.cppreference.com/w/cpp/language/dependent_name for
 *       typename X::template T c++11 syntax and using Nodes = typename config_t::template node_container_t< Node* >;
 *
 */
template < class config_t = gtpo::default_config >
class graph : public config_t::graph_base,
              public gtpo::behaviourable_graph< config_t >
{
    /*! \name Graph Management *///--------------------------------------------
    //@{
public:
    using graph_t           = graph<config_t>;

    // Shortcut for user access to graph configuration.
    using final_config_t      = config_t;

    using node_t              = typename config_t::final_node_t;
    using shared_node_t       = typename std::shared_ptr<typename config_t::final_node_t>;
    using weak_node_t         = typename std::weak_ptr<typename config_t::final_node_t>;
    using shared_nodes_t      = typename config_t::template node_container_t< shared_node_t >;
    using weak_nodes_t        = typename config_t::template node_container_t< weak_node_t >;
    using weak_nodes_t_search = typename config_t::template search_container_t< weak_node_t >;

    using weak_edge_t         = typename std::weak_ptr<typename config_t::final_edge_t>;
    using shared_edge_t       = typename std::shared_ptr<typename config_t::final_edge_t>;
    using weak_edges_t        = typename config_t::template edge_container_t< weak_edge_t >;
    using shared_edges_t      = typename config_t::template edge_container_t< shared_edge_t >;
    using weak_edges_search_t = typename config_t::template search_container_t< weak_edge_t >;

    using shared_group_t      = typename std::shared_ptr<typename config_t::final_group_t>;
    using weak_group_t        = typename std::weak_ptr<typename config_t::final_group_t>;
    using shared_groups_t     = typename config_t::template node_container_t< shared_group_t >;

    //! User friendly shortcut to this concrete graph behaviour.
    using behaviour = graph_behaviour< config_t >;
    //! User friendly shortcut type to this concrete graph Behaviourable base type.
    using behaviourable_base = gtpo::behaviourable_graph< config_t >;

public:
    using size_type  = typename shared_nodes_t::size_type;

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

    /*! \brief Test if this graph is empty, and empty graph has no nodes.
     *
     * // FIXME: groups are not taken into account.
     *
     * \return true if the graph is empty, false otherwise.
     */
    auto    is_empty() noexcept -> bool { return get_node_count() == 0; }
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
     *   std::weak_ptr<gtpo::node<>> n1 = g.create_node();
     *   // or preferably
     *   auto n2 = g.create_node();
     * } catch ( const std::runtime_error& e ) { }
     * \endcode
     *
     * With STpo:
     * \code
     * using namespace gtpo;
     * try{
     *   gtpo::graph<> g;
     *   std::weak_ptr<gtpo::Node<>> n1 = g.create_node();
     *   // or preferably
     *   auto n2 = g.create_node();
     * } catch ( const std::runtime_error& e ) { }
     * \endcode
     * \return a reference to the created node (graph has ownership for the node).
     * \throw gtpo::bad_topology_error with an error description if creation fails.
     */
    auto    create_node() noexcept( false ) -> weak_node_t;

    /*! \brief Insert a node created outside of GTpo into the graph.
     *
     * If your nodes must be created outside of GTpo (ie not with the create_node() method),
     * the only way of giving node ownership to GTpo is trought the insert_node method.
     * Example of a node created from a QML component:
     * \code
     *  qan::Node* node = static_cast< qan::Node* >( createFromQmlComponent( nodeComponent ) );
     *  if ( node != nullptr )
     *    graph<QGraphcConfig>::insert_node( std::shared_ptr<qan::Node>{node} );
     * \endcode
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto    insert_node( shared_node_t node ) noexcept( false ) -> weak_node_t;

    /*! \brief Remove node \c node from graph.
     *
     * Complexity depends on config_t::node_container_t.
     * \note If \c weakNode is actually grouped in a group, it will first be ungroup before
     * beeing removed (any group behaviour will also be notified that the node is ungrouped).
     * \throw gtpo::bad_topology_error if node can't be removed (or node is not valid).
     */
    auto    remove_node( weak_node_t weakNode ) noexcept( false ) -> void;

    //! Return the number of nodes actually registered in graph.
    inline auto get_node_count() const -> size_type { return _nodes.size(); }
    //! Return the number of root nodes (actually registered in graph)ie nodes with a zero in degree).
    inline auto get_root_node_count() const -> size_type { return _root_nodes.size(); }

    /*! \brief Install a given \c node in the root node cache.
     *
     * This method should not be directly used by an end user until you have deeply
     * modified graph topology with non gtpo::graph<> methods.
     *
     * \throw gtpo::bad_topology_error if \c node in degree is different from 0.
     */
    auto    install_root_node( weak_node_t node ) noexcept( false ) -> void;
    /*! \brief Test if a given \c node is a root node.
     *
     * This method is safer than testing node->get_in_degree()==0, since it check
     * \c node in degree and its presence in the internal root node cache.
     *
     * \return true if \c node is a root node, false otherwise.
     * \throw gtpo::bad_topology_error if there is a graph cohenrency problem (ie node is in the root node cache but has a zero in degree).
     */
    auto    is_root_node( weak_node_t node ) const noexcept( false ) -> bool;

    //! Use fast search container to find if a given \c node is part of this graph.
    auto    contains( weak_node_t node ) const noexcept -> bool;

    //! Graph main nodes container.
    inline auto     get_nodes() const -> const shared_nodes_t& { return _nodes; }
    //! Return a const begin iterator over graph shared_node_t nodes.
    inline auto     begin() const -> typename shared_nodes_t::const_iterator { return _nodes.begin( ); }
    //! Return a const begin iterator over graph shared_node_t nodes.
    inline auto     end() const -> typename shared_nodes_t::const_iterator { return _nodes.end( ); }

    //! Return a const begin iterator over graph shared_node_t nodes.
    inline auto     cbegin() const -> typename shared_nodes_t::const_iterator { return _nodes.cbegin(); }
    //! Return a const end iterator over graph shared_node_t nodes.
    inline auto     cend() const -> typename shared_nodes_t::const_iterator { return _nodes.cend(); }

    //! Graph root nodes container.
    inline auto     get_root_nodes() const -> const weak_nodes_t& { return _root_nodes; }

private:
    shared_nodes_t        _nodes;
    weak_nodes_t        _root_nodes;
    weak_nodes_t_search _nodes_search;
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
    //template < class edge_t = typename config_t::final_edge_t >
    auto        create_edge( weak_node_t source, weak_node_t destination ) noexcept(false) -> weak_edge_t;

    /*! \brief Insert a directed edge created outside of GTpo into the graph.
     *
     * \param edge must have a valid source and destination set otherwise a bad topology exception will be thrown.
     * \sa insert_node()
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto        insert_edge( shared_edge_t edge ) noexcept( false ) -> weak_edge_t;

    /*! \brief Remove first directed edge found between \c source and \c destination node.
     *
     * If the current graph<> config_t::edge_container_t and config_t::node_container_t allow parrallel edges support, the first
     * edge found between \c source and \c destination will be removed.
     *
     * Complexity is O(edge count) at worst.
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination or edge does not exists).
     */
    auto        remove_edge( weak_node_t source, weak_node_t destination ) noexcept( false ) -> void;

    /*! \brief Remove all directed edge between \c source and \c destination node.
     *
     * If the current graph<> config_t::edge_container_t and config_t::node_container_t allow parrallel edges support, the first
     * edge found between \c source and \c destination will be removed.
     *
     * Worst case complexity is O(edge count).
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination or edge does not exists).
     */
    auto        remove_all_edges( weak_node_t source, weak_node_t destination ) noexcept( false ) -> void;

    /*! \brief Remove directed edge \c edge.
     *
     * Worst case complexity is O(edge count).
     * \throw a gtpo::bad_topology_error if suppression fails (\c edge does not exists).
     */
    auto        remove_edge( weak_edge_t edge ) noexcept( false ) -> void;

    /*! \brief Look for the first directed edge between \c source and \c destination and return it.
     *
     * Worst case complexity is O(edge count).
     * \return A shared reference on edge, en empty shared reference otherwise (result == false).
     * \throw noexcept.
     */
    auto        find_edge( weak_node_t source, weak_node_t destination ) const noexcept -> weak_edge_t;
    /*! \brief Test if a directed edge exists between nodes \c source and \c destination.
     *
     * This method only test a 1 degree relationship (ie a direct edge between \c source
     * and \c destination). Worst case complexity is O(edge count).
     * \throw noexcept.
     */
    auto        has_edge( weak_node_t source, weak_node_t destination ) const noexcept -> bool;
    /*! \brief Look for the first directed restricted hyper edge between \c source node and \c destination edge and return it.
     *
     * Worst case complexity is O(edge count).
     * \return A shared reference on edge, en empty shared reference otherwise (result == false).
     * \throw noexcept.
     */
    auto        find_edge( weak_node_t source, weak_edge_t destination ) const noexcept -> weak_edge_t;

    //! Return the number of edges currently existing in graph.
    auto        get_edge_count() const noexcept -> unsigned int { return static_cast<int>( _edges.size() ); }
    /*! \brief Return the number of (parallel) directed edges between nodes \c source and \c destination.
     *
     * Graph edge_container_t should support multiple insertions (std::vector, std::list) to enable
     * parrallel edge support, otherwise, get_edge_count() will always return 1 or 0.
     *
     * This method only test a 1 degree relationship (ie a direct edge between \c source
     * and \c destination). Worst case complexity is O(edge count).
     * \throw no GTpo exception (might throw a std::bad_weak_ptr).
     */
    auto        get_edge_count( weak_node_t source, weak_node_t destination ) const noexcept( false ) -> unsigned int;

    //! Use fast search container to find if a given \c edge is part of this graph.
    auto        contains( weak_edge_t edge ) const noexcept -> bool;

    //! Graph main edges container.
    inline auto get_edges() const noexcept -> const shared_edges_t& { return _edges; }
private:
    shared_edges_t        _edges;
    weak_edges_search_t   _edges_search;
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
    auto            create_group() noexcept( false ) -> weak_group_t;

    /*! \brief Insert a node group into the graph.
     *
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto            insert_group( shared_group_t group ) noexcept( false ) -> weak_group_t;

    /*! \brief Remove node group \c group.
     *
     * Group content (ie group's nodes) are not removed from the graph, but ungrouped (ie moved from
     * the group to the graph).
     *
     * Worst case complexity is O(group count).
     * \throw a gtpo::bad_topology_error if suppression fails (\c group does not exists).
     */
    auto            remove_group( weak_group_t group ) noexcept( false ) -> void;

    //! Return true if a given group \c group is registered in the graph.
    auto            has_group( const weak_group_t& group ) const -> bool;

    //! Return the number of edges currently existing in graph.
    inline auto     get_group_count() const noexcept -> int { return static_cast<int>( _groups.size() ); }

    //! Graph main edges container.
    inline auto     get_groups() const noexcept -> const shared_groups_t& { return _groups; }

    /*! \brief Insert an existing node \c node in group \c group.
     *
     * \note If a behaviour has been installed with gtpo::group::add_dynamic_group_behaviour(), behaviour's
     * node_inserted() will be called.
     *
     * \note \c node get_group() will return \c group if grouping succeed.
     */
    auto            group_node( weak_node_t node, weak_group_t group) noexcept(false) -> void;

    //! \copydoc group_node()
    auto            group_node( weak_group_t weakGroupNode, weak_group_t weakGroup ) noexcept(false) -> void;

    /*! \brief Insert an existing node \c weakNode in group \c weakGroup group.
     *
     * \note If a behaviour has been installed with gtpo::group::add_dynamic_group_behaviour(), behaviour's
     * node_removed() will be called.
     *
     * \note \c node getGroup() will return an expired weak pointer if ungroup succeed.
     */
    auto            ungroup_node( weak_node_t weakNode, weak_group_t weakGroup ) noexcept(false) -> void;

    //! \copydoc ungroup_node()
    auto            ungroup_node( weak_group_t weakGroupNode, weak_group_t weakGroup ) noexcept(false) -> void;
private:
    shared_groups_t    _groups;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#endif // gtpo_graph_h

