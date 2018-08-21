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
// \file	graph.hpp
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* Graph Node Management *///--------------------------------------------------
template <class config_t>
graph<config_t>::~graph()
{
    clear();
}

template <class config_t>
void    graph<config_t>::clear() noexcept
{
    // Note 20160104: First edges, then nodes (it helps maintaining topology if
    // womething went wrong during destruction
    for ( auto& edge: _edges )   // Do not maintain topology during edge deletion
        edge->_graph = nullptr;
    _edges_search.clear();
    _edges.clear();
    for ( auto& node: _nodes )   // Do not maintain topology during node deletion
        node->_graph = nullptr;
    _root_nodes.clear();         // Remove weak_ptr containers first
    _nodes_search.clear();
    _nodes.clear();

    // Clearing groups and behaviours
    for ( auto& group: _groups )
        group->set_graph(nullptr);
    _groups.clear();
    behaviourable_base::clear();
}
//-----------------------------------------------------------------------------

/* Graph Node Management *///--------------------------------------------------
template < class config_t >
auto graph<config_t>::create_node( ) -> weak_node_t
{
    weak_node_t node;
    try {
        node = insert_node( std::make_shared< typename config_t::final_node_t >() );
    } catch (...) { gtpo::assert_throw( false, "graph<>::create_node(): Error: can't insert node in graph." ); }
    return node;
}

template < class config_t >
auto    graph<config_t>::insert_node( shared_node_t node ) -> weak_node_t
{
    assert_throw( node != nullptr, "gtpo::graph<>::insert_node(): Error: Trying to insert a nullptr node in graph." );
    weak_node_t weak_node;
    try {
        weak_node = node;
        node->set_graph( this );
        config_t::template container_adapter< shared_nodes_t >::insert( node, _nodes );
        config_t::template container_adapter< weak_nodes_t_search >::insert( weak_node, _nodes_search );
        config_t::template container_adapter< weak_nodes_t >::insert( weak_node, _root_nodes );
        behaviourable_base::notify_node_inserted( weak_node );
    } catch (...) { gtpo::assert_throw( false, "gtpo::graph<>::insert_node(): Error: can't insert node in graph." ); }
    return weak_node;
}

template < class config_t >
auto    graph<config_t>::remove_node( weak_node_t weak_node ) -> void
{
    auto node = weak_node.lock();
    if ( !node )
        gtpo::assert_throw( false, "gtpo::graph<>::remove_node(): Error: node is expired." );

    // Eventually, ungroup node
    auto group = node->get_group().lock();
    if ( group )
        ungroup_node(weak_node, group);

    behaviourable_base::notify_node_removed( weak_node );

    // Removing all orphant edges pointing to node.
    weak_edges_t nodeInEdges; std::copy( node->get_in_edges().cbegin(),
                                      node->get_in_edges().cend(),
                                      std::back_inserter( nodeInEdges ) );
    for ( auto inEdge: nodeInEdges )
        node->remove_in_edge( inEdge );
    for ( auto inEdge: nodeInEdges )
        remove_edge( inEdge );

    weak_edges_t nodeOutEdges; std::copy( node->get_out_edges().cbegin(),
                                      node->get_out_edges().cend(),
                                      std::back_inserter( nodeOutEdges ) );
    for ( auto outEdge: nodeOutEdges )
        node->remove_out_edge( outEdge );
    for ( auto outEdge: nodeOutEdges )
        remove_edge( outEdge );

    // Remove node from main graph containers (it will generate node destruction)
    config_t::template container_adapter<weak_nodes_t_search>::remove( weak_node, _nodes_search );
    config_t::template container_adapter<weak_nodes_t>::remove( weak_node, _root_nodes );
    node->set_graph( nullptr );
    config_t::template container_adapter<shared_nodes_t>::remove( node, _nodes );
}

template < class config_t >
auto    graph<config_t>::install_root_node( weak_node_t node ) -> void
{
    assert_throw( !node.expired(), "gtpo::graph<>::setRootNode(): Error: node is expired." );
    shared_node_t sharedNode = node.lock();
    assert_throw( sharedNode->get_in_degree() == 0, "gtpo::graph<>::setRootNode(): Error: trying to set a node with non 0 in degree as a root node." );

    config_t::template container_adapter<weak_nodes_t>::insert( node, _root_nodes );
}

template < class config_t >
auto    graph<config_t>::is_root_node( weak_node_t node ) const -> bool
{
    assert_throw( !node.expired(), "gtpo::graph<>::is_root_node(): Error: node is expired." );
    shared_node_t sharedNode = node.lock();
    if ( sharedNode->get_in_degree() != 0 )   // Fast exit when node in degree != 0, it can't be a root node
        return false;
    return gtpo::find_weak_ptr( _root_nodes, node );
}

template < class config_t >
auto    graph<config_t>::contains( weak_node_t node ) const noexcept -> bool
{
    if ( node.expired() )   // Fast exit.
        return false;
    auto nodeIter = std::find_if( _nodes_search.cbegin(), _nodes_search.cend(),
                                            [=](const weak_node_t& n ) { return ( compare_weak_ptr<>( node, n ) ); } );
    return nodeIter != _nodes_search.cend();
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template < class config_t >
//template < class Edge_t >
auto    graph< config_t >::create_edge( weak_node_t source, weak_node_t destination ) -> weak_edge_t
{
    auto source_ptr = source.lock();
    auto destination_ptr = destination.lock();
    if ( !source_ptr ||
         !destination_ptr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Node): Insertion of edge failed, either source or destination nodes are expired." );

    auto edge = std::make_shared<typename config_t::final_edge_t>();
    edge->set_graph( this );
    config_t::template container_adapter< shared_edges_t >::insert( edge, _edges );
    config_t::template container_adapter< weak_edges_search_t >::insert( edge, _edges_search );
    edge->set_src( source );
    edge->set_dst( destination );
    try {
        source_ptr->add_out_edge( edge );
        destination_ptr->add_in_edge( edge );
        if ( source_ptr.get() != destination_ptr.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
            config_t::template container_adapter<weak_nodes_t>::remove( destination, _root_nodes );    // Otherwise destination is no longer a root node
        auto weak_edge = weak_edge_t{edge};
        behaviourable_base::notify_edge_inserted( weak_edge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Node): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class config_t >
auto    graph<config_t>::insert_edge( shared_edge_t edge ) -> weak_edge_t
{
    assert_throw( edge != nullptr );
    auto source = edge->get_src().lock();
    if ( source == nullptr ||
         edge->get_dst().expired() )
        throw gtpo::bad_topology_error( "gtpo::graph<>::insert_edge(): Error: Either source and/or destination nodes are expired." );
    edge->set_graph( this );
    config_t::template container_adapter<shared_edges_t>::insert( edge, _edges );
    config_t::template container_adapter<weak_edges_search_t>::insert( edge, _edges_search );
    try {
        source->add_out_edge( edge );
        auto destination = edge->get_dst().lock();
        if ( destination != nullptr ) {
            destination->add_in_edge( edge );
            if ( source.get() != destination.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
                config_t::template container_adapter<weak_nodes_t>::remove( destination, _root_nodes );    // Otherwise destination is no longer a root node
        }
        auto weak_edge = weak_edge_t(edge);
        behaviourable_base::notify_edge_inserted( weak_edge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class config_t >
void    graph<config_t>::remove_edge( weak_node_t source, weak_node_t destination )
{
    if ( source.expired() ||
         destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Topology error." );

    // Find the edge associed with source / destination
    if ( _edges.size() == 0 )
        return; // Fast exit
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                 [=](const shared_edge_t& e ){
                                    return ( compare_weak_ptr<>( e->get_src(), source ) &&
                                             compare_weak_ptr<>( e->get_dst(), destination ) );
                                    }
                                 ); // std::find_if
    if ( edgeIter != _edges.end() )
        remove_edge( *edgeIter );
}

template < class config_t >
void    graph<config_t>::remove_all_edges( weak_node_t source, weak_node_t destination )
{
    if ( source.expired() ||
         destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Topology error." );

    while ( get_edge_count( source, destination ) > 0 )
        remove_edge( source, destination );
}

template < class config_t >
void    graph<config_t>::remove_edge( weak_edge_t weak_edge )
{
    shared_edge_t edge = weak_edge.lock();
    if ( !edge )
        throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Error: Edge to be removed is already expired." );
    auto source = edge->get_src().lock();
    auto destination = edge->get_dst().lock();
    if ( source == nullptr      ||           // Expecting a non null source and either a destination or an hyper destination
         destination == nullptr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Error: Edge source or destination are expired." );
    behaviourable_base::notify_edge_removed( weak_edge );
    source->remove_out_edge( weak_edge );
    if ( destination )      // Remove edge from destination in edges
        destination->remove_in_edge( weak_edge );

    edge->set_graph( nullptr );
    config_t::template container_adapter<shared_edges_t>::remove( edge, _edges );
    config_t::template container_adapter<weak_edges_search_t>::remove( weak_edge, _edges_search );
}

template < class config_t >
auto    graph<config_t>::find_edge( weak_node_t source, weak_node_t destination ) const noexcept -> weak_edge_t
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const shared_edge_t& e ) noexcept {
                                        return ( compare_weak_ptr<>( e->get_src(), source ) &&
                                                 compare_weak_ptr<>( e->get_dst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : weak_edge_t{} );   // std::shared_ptr::operator* is noexcept
}

template < class config_t >
auto    graph<config_t>::has_edge( weak_node_t source, weak_node_t destination ) const noexcept -> bool
{
    return ( find_edge( source, destination).use_count() != 0 );
}

template < class config_t >
auto    graph<config_t>::get_edge_count( weak_node_t source, weak_node_t destination ) const -> unsigned int
{
    unsigned int edgeCount = 0;
    std::for_each( _edges.begin(), _edges.end(), [&](const shared_edge_t& e ) {
                                                        if ( compare_weak_ptr<>( e->get_src(), source ) &&
                                                             compare_weak_ptr<>( e->get_dst(), destination ) )
                                                            ++edgeCount;
                                                    } );
    return edgeCount;
}

template < class config_t >
auto    graph<config_t>::contains( weak_edge_t edge ) const noexcept -> bool
{
    if ( edge.expired() )   // Fast exit.
        return false;
    auto edgeIter = std::find_if( _edges_search.cbegin(), _edges_search.cend(),
                                            [=](const weak_edge_t& n ) { return ( compare_weak_ptr<>( edge, n ) ); } );
    return edgeIter != _edges_search.cend();
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
template < class config_t >
auto graph<config_t>::create_group() -> weak_group_t
{
    weak_group_t weakGroup;
    try {
        auto group = std::make_shared< typename config_t::final_group_t >();
        weakGroup = insert_group( group );
    } catch (...) { gtpo::assert_throw( false, "gtpo::graph<>::create_group(): Error: can't insert group in graph." ); }
    return weakGroup;
}

template < class config_t >
auto    graph<config_t>::insert_group( shared_group_t group ) noexcept( false ) -> weak_group_t
{
    assert_throw( group != nullptr, "gtpo::graph<>::insert_group(): Error: trying to insert a nullptr shared_group_t" );
    weak_group_t weakGroup;
    try {
        weakGroup = group;
        group->set_graph( this );
        config_t::template container_adapter<shared_groups_t>::insert( group, _groups );
        this->notify_group_inserted( weakGroup );
    } catch (...) { throw gtpo::bad_topology_error( "gtpo::graph<>::insert_group(): Insertion of group failed" ); }
    return weakGroup;
}

template < class config_t >
auto    graph<config_t>::remove_group( weak_group_t weakGroup ) noexcept( false ) -> void
{
    gtpo::assert_throw( !weakGroup.expired(), "gtpo::graph<>::remove_group(): Error: trying to remove an expired group." );
    shared_group_t group = weakGroup.lock();
    if ( !group )
        gtpo::assert_throw( false, "graph<>::remove_group(): Error: trying to remove and expired group." );

    // Remove group (it will be automatically deallocated)
    this->notify_group_removed( weakGroup );
    group->set_graph( nullptr );
    config_t::template container_adapter<shared_groups_t>::remove( group, _groups );
}

template < class config_t >
auto    graph<config_t>::has_group( const weak_group_t& group ) const -> bool
{
    if ( group.expired() )
        return false;
    auto groupIter = std::find_if( _groups.begin(), _groups.end(),
                                        [=](const weak_group_t& graphGroup  ){ return ( compare_weak_ptr<>( group, graphGroup ) ); } );
    return groupIter != _groups.end();
}

template < class config_t >
auto    graph<config_t>::group_node( weak_node_t node, weak_group_t group) noexcept(false) -> void
{
    auto group_ptr = group.lock();
    gtpo::assert_throw( group_ptr != nullptr, "gtpo::group<>::group_node(): Error: trying to insert a node into an expired group." );

    auto node_ptr = node.lock();
    gtpo::assert_throw( node_ptr != nullptr, "gtpo::group<>::group_node(): Error: trying to insert an expired node in group." );

    node_ptr->set_group( group );
    config_t::template container_adapter<weak_nodes_t>::insert( node, group_ptr->_nodes );
    group_ptr->notify_node_inserted( node );
}

template < class config_t >
auto    graph<config_t>::group_node( weak_group_t weakGroupNode, weak_group_t weakGroup ) noexcept(false) -> void
{
    auto group{ weakGroup.lock() };
    gtpo::assert_throw( group != nullptr, "gtpo::group<>::group_node(): Error: trying to insert a group into an expired group." );

    auto subGroup{ weakGroupNode.lock() };
    gtpo::assert_throw( subGroup != nullptr, "gtpo::group<>::group_node(): Error: trying to insert an expired group into a group." );

    shared_node_t subGroupNode = std::static_pointer_cast<node<config_t>>(subGroup);
    group_node( weak_node_t{subGroupNode}, weakGroup );
    group->notify_group_inserted( weakGroupNode );
}

template < class config_t >
auto    graph<config_t>::ungroup_node( weak_node_t weakNode, weak_group_t weakGroup ) noexcept(false) -> void
{
    auto group = weakGroup.lock();
    gtpo::assert_throw( group != nullptr, "gtpo::group<>::ungroup_node(): Error: trying to ungroup from an expired group." );

    auto node = weakNode.lock();
    gtpo::assert_throw( node != nullptr, "gtpo::group<>::ungroup_node(): Error: trying to ungroup an expired node from a group." );

    gtpo::assert_throw( node->get_group().lock() == group, "gtpo::group<>::ungroup_node(): Error: trying to ungroup a node that is not part of group." );

    config_t::template container_adapter<weak_nodes_t>::remove( weakNode, group->_nodes );
    group->notify_node_removed( weakNode );
    node->set_group( weak_group_t{} );  // Warning: group must remain valid while notify_node_removed() is called
}

template < class config_t >
auto    graph<config_t>::ungroup_node( weak_group_t weakGroupNode, weak_group_t weakGroup ) noexcept(false) -> void
{
    auto group{ weakGroup.lock() };
    gtpo::assert_throw( group != nullptr, "gtpo::group<>::ungroup_node(): Error: trying to ungroup from an expired group." );

    auto subGroup{ weakGroupNode.lock() };
    gtpo::assert_throw( subGroup != nullptr, "gtpo::group<>::ungroup_node(): Error: trying to ungroup an expired group from a group." );

    shared_node_t subGroupNode = std::static_pointer_cast<node<config_t>>(subGroup);
    group->notify_group_removed( weakGroupNode );
    ungroup_node( weak_node_t{subGroupNode}, weakGroup );
}

//-----------------------------------------------------------------------------

} // ::gtpo

