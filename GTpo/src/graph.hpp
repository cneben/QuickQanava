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
    _edgesSearch.clear();
    _edges.clear();
    for ( auto& node: _nodes )   // Do not maintain topology during node deletion
        node->_graph = nullptr;
    _root_nodes.clear();         // Remove weak_ptr containers first
    _nodes_search.clear();
    _nodes.clear();

    // Clearing groups and behaviours
    for ( auto& group: _groups )
        group->setGraph(nullptr);
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
        node->setGraph( this );
        config_t::template container_adapter< shared_nodes_t >::insert( node, _nodes );
        config_t::template container_adapter< weak_node_ts_search >::insert( weak_node, _nodes_search );
        config_t::template container_adapter< weak_node_ts >::insert( weak_node, _root_nodes );
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
    auto group = node->getGroup().lock();
    if ( group )
        ungroup_node(weak_node, group);

    behaviourable_base::notify_node_removed( weak_node );

    // Removing all orphant edges pointing to node.
    weak_edges_t nodeInEdges; std::copy( node->getInEdges().cbegin(),
                                      node->getInEdges().cend(),
                                      std::back_inserter( nodeInEdges ) );
    for ( auto inEdge: nodeInEdges )
        node->removeInEdge( inEdge );
    for ( auto inEdge: nodeInEdges )
        remove_edge( inEdge );

    weak_edges_t nodeOutEdges; std::copy( node->getOutEdges().cbegin(),
                                      node->getOutEdges().cend(),
                                      std::back_inserter( nodeOutEdges ) );
    for ( auto outEdge: nodeOutEdges )
        node->removeOutEdge( outEdge );
    for ( auto outEdge: nodeOutEdges )
        remove_edge( outEdge );

    // Remove node from main graph containers (it will generate node destruction)
    config_t::template container_adapter<weak_node_ts_search>::remove( weak_node, _nodes_search );
    config_t::template container_adapter<weak_node_ts>::remove( weak_node, _root_nodes );
    node->setGraph( nullptr );
    config_t::template container_adapter<shared_nodes_t>::remove( node, _nodes );
}

template < class config_t >
auto    graph<config_t>::install_root_node( weak_node_t node ) -> void
{
    assert_throw( !node.expired(), "gtpo::graph<>::setRootNode(): Error: node is expired." );
    shared_node_t sharedNode = node.lock();
    assert_throw( sharedNode->getInDegree() == 0, "gtpo::graph<>::setRootNode(): Error: trying to set a node with non 0 in degree as a root node." );

    config_t::template container_adapter<weak_node_ts>::insert( node, _root_nodes );
}

template < class config_t >
auto    graph<config_t>::is_root_node( weak_node_t node ) const -> bool
{
    assert_throw( !node.expired(), "gtpo::graph<>::is_root_node(): Error: node is expired." );
    shared_node_t sharedNode = node.lock();
    if ( sharedNode->getInDegree() != 0 )   // Fast exit when node in degree != 0, it can't be a root node
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
template < class Edge_t >
auto    graph< config_t >::create_edge( weak_node_t source, weak_node_t destination ) -> weak_edge_t
{
    auto sourcePtr = source.lock();
    auto destinationPtr = destination.lock();
    if ( !sourcePtr ||
         !destinationPtr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Node): Insertion of edge failed, either source or destination nodes are expired." );

    auto edge = std::make_shared<Edge_t>();
    edge->setGraph( this );
    config_t::template container_adapter< shared_edges_t >::insert( edge, _edges );
    config_t::template container_adapter< weak_edges_t_search >::insert( edge, _edgesSearch );
    edge->setSrc( source );
    edge->setDst( destination );
    try {
        sourcePtr->addOutEdge( edge );
        destinationPtr->addInEdge( edge );
        if ( sourcePtr.get() != destinationPtr.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
            config_t::template container_adapter<weak_node_ts>::remove( destination, _root_nodes );    // Otherwise destination is no longer a root node
        auto weakEdge = weak_edge_t{edge};
        behaviourable_base::notify_edge_inserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Node): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class config_t >
auto    graph<config_t>::create_edge( weak_node_t source, weak_edge_t destination ) -> weak_edge_t
{
    auto sourcePtr = source.lock();
    auto destinationPtr = destination.lock();
    if ( !sourcePtr ||
         !destinationPtr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Edge): Insertion of edge failed, either source node and/or destination edge is expired." );

    auto edge = std::make_shared< typename config_t::final_edge_t >();
    edge->_graph = this;
    config_t::template container_adapter< shared_edges_t >::insert( edge, _edges );
    config_t::template container_adapter< weak_edges_t_search >::insert( edge, _edgesSearch );
    edge->setSrc( source );
    edge->setHDst( destination );
    try {
        sourcePtr->addOutEdge( edge );
        destinationPtr->addInHEdge( edge );
        // Note: hEdge does not modify root nodes.
        auto weakEdge = weak_edge_t{edge};
        behaviourable_base::notify_edge_inserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Edge): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class config_t >
auto    graph<config_t>::insert_edge( shared_edge_t edge ) -> weak_edge_t
{
    assert_throw( edge != nullptr );
    auto source = edge->getSrc().lock();
    if ( source == nullptr ||
         ( edge->getDst().expired() && edge->getHDst().expired() ) )
        throw gtpo::bad_topology_error( "gtpo::graph<>::insert_edge(): Error: Either source and/or destination nodes are expired." );
    edge->setGraph( this );
    config_t::template container_adapter<shared_edges_t>::insert( edge, _edges );
    config_t::template container_adapter<weak_edges_t_search>::insert( edge, _edgesSearch );
    try {
        source->addOutEdge( edge );
        auto destination = edge->getDst().lock();
        if ( destination != nullptr ) {
            destination->addInEdge( edge );
            if ( source.get() != destination.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
                config_t::template container_adapter<weak_node_ts>::remove( destination, _root_nodes );    // Otherwise destination is no longer a root node
        } else {
            auto hDestination = edge->getHDst().lock();
            if ( hDestination != nullptr )
                hDestination->addInHEdge( edge );
        }
        auto weakEdge = weak_edge_t(edge);
        behaviourable_base::notify_edge_inserted( weakEdge );
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
                                    return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                             compare_weak_ptr<>( e->getDst(), destination ) );
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
void    graph<config_t>::remove_edge( weak_edge_t edge )
{
    shared_edge_t edgePtr = edge.lock();
    if ( edgePtr == nullptr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Error: Edge to be removed is already expired." );
    auto source = edgePtr->getSrc().lock();
    auto destination = edgePtr->getDst().lock();
    auto hDestination = edgePtr->getHDst().lock();
    if ( source == nullptr ||           // Expecting a non null source and either a destination or an hyper destination
         ( destination == nullptr && hDestination == nullptr ) )
        throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Error: Edge source or destination are expired." );
    behaviourable_base::notify_edge_removed( edge );
    source->removeOutEdge( edge );
    if ( destination )      // Remove edge from destination in edges
        destination->removeInEdge( edge );
    if ( hDestination )     // Remove edge from hyper destination in hyper edges
        hDestination->removeInHEdge( edge );

    // If there is in hyper edges, remove them since their destination edge is beeing destroyed
    if ( edgePtr->getInHDegree() > 0 ) {
        auto& inHEdges = edgePtr->getInHEdges();    // Make a deep copy of in hyper edges
        for ( auto& inHEdge : inHEdges )
            remove_edge( inHEdge );
    }
    edgePtr->setGraph( nullptr );
    config_t::template container_adapter<shared_edges_t>::remove( edgePtr, _edges );
    config_t::template container_adapter<weak_edges_t_search>::remove( edge, _edgesSearch );
}

template < class config_t >
auto    graph<config_t>::find_edge( weak_node_t source, weak_node_t destination ) const noexcept -> weak_edge_t
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const shared_edge_t& e ) noexcept {
                                        return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                 compare_weak_ptr<>( e->getDst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : weak_edge_t{} );   // std::shared_ptr::operator* is noexcept
}

template < class config_t >
auto    graph<config_t>::has_edge( weak_node_t source, weak_node_t destination ) const noexcept -> bool
{
    return ( find_edge( source, destination).use_count() != 0 );
}

template < class config_t >
auto    graph<config_t>::find_edge( weak_node_t source, weak_edge_t destination ) const noexcept -> weak_edge_t
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const shared_edge_t& e ) noexcept {
                                        return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                 compare_weak_ptr<>( e->getHDst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : weak_edge_t{} );   // std::shared_ptr::operator* is noexcept
}

template < class config_t >
auto    graph<config_t>::has_edge( weak_node_t source, weak_edge_t destination ) const noexcept -> bool
{
    return ( find_edge( source, destination ).use_count() != 0 );
}

template < class config_t >
auto    graph<config_t>::get_edge_count( weak_node_t source, weak_node_t destination ) const -> unsigned int
{
    unsigned int edgeCount = 0;
    std::for_each( _edges.begin(), _edges.end(), [&](const shared_edge_t& e ) {
                                                        if ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                             compare_weak_ptr<>( e->getDst(), destination ) )
                                                            ++edgeCount;
                                                    } );
    return edgeCount;
}

template < class config_t >
auto    graph<config_t>::contains( weak_edge_t edge ) const noexcept -> bool
{
    if ( edge.expired() )   // Fast exit.
        return false;
    auto edgeIter = std::find_if( _edgesSearch.cbegin(), _edgesSearch.cend(),
                                            [=](const weak_edge_t& n ) { return ( compare_weak_ptr<>( edge, n ) ); } );
    return edgeIter != _edgesSearch.cend();
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
        group->setGraph( this );
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
    group->setGraph( nullptr );
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
//-----------------------------------------------------------------------------

} // ::gtpo

