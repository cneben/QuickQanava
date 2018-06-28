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
    _rootNodes.clear();         // Remove weak_ptr containers first
    _nodesSearch.clear();
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
auto graph<config_t>::createNode( ) -> weak_node
{
    weak_node weakNode;
    try {
        auto node = std::make_shared< typename config_t::final_node_t >();
        weakNode = insertNode( node );
    } catch (...) { gtpo::assert_throw( false, "graph<>::createNode(): Error: can't insert node in graph." ); }
    return weakNode;
}

template < class config_t >
auto    graph<config_t>::insertNode( shared_node node ) -> weak_node
{
    assert_throw( node != nullptr, "gtpo::graph<>::insertNode(): Error: Trying to insert a nullptr node in graph." );
    weak_node weakNode;
    try {
        weakNode = node;
        node->setGraph( this );
        config_t::template container_adapter< shared_nodes >::insert( node, _nodes );
        config_t::template container_adapter< weak_nodes_search >::insert( weakNode, _nodesSearch );
        config_t::template container_adapter< weak_nodes >::insert( weakNode, _rootNodes );
        behaviourable_base::notify_node_inserted( weakNode );
    } catch (...) { gtpo::assert_throw( false, "gtpo::graph<>::insertNode(): Error: can't insert node in graph." ); }
    return weakNode;
}

template < class config_t >
auto    graph<config_t>::removeNode( weak_node weakNode ) -> void
{
    gtpo::assert_throw( !weakNode.expired(), "gtpo::graph<>::removeNode(): Error: trying to remove an expired node." );
    auto node = weakNode.lock();
    if ( !node )
        gtpo::assert_throw( false, "gtpo::graph<>::removeNode(): Error: node is expired." );

    // Eventually, ungroup node
    auto group = node->getGroup().lock();
    if ( group )
        ungroupNode(weakNode, group);

    behaviourable_base::notify_node_removed( weakNode );

    // Removing all orphant edges pointing to node.
    weak_edges nodeInEdges; std::copy( node->getInEdges().cbegin(),
                                      node->getInEdges().cend(),
                                      std::back_inserter( nodeInEdges ) );
    for ( auto inEdge: nodeInEdges )
        node->removeInEdge( inEdge );
    for ( auto inEdge: nodeInEdges )
        removeEdge( inEdge );

    weak_edges nodeOutEdges; std::copy( node->getOutEdges().cbegin(),
                                      node->getOutEdges().cend(),
                                      std::back_inserter( nodeOutEdges ) );
    for ( auto outEdge: nodeOutEdges )
        node->removeOutEdge( outEdge );
    for ( auto outEdge: nodeOutEdges )
        removeEdge( outEdge );    

    // Remove node from main graph containers (it will generate node destruction)
    config_t::template container_adapter<weak_nodes_search>::remove( weakNode, _nodesSearch );
    config_t::template container_adapter<weak_nodes>::remove( weakNode, _rootNodes );
    node->setGraph( nullptr );
    config_t::template container_adapter<shared_nodes>::remove( node, _nodes );
}

template < class config_t >
auto    graph<config_t>::installRootNode( weak_node node ) -> void
{
    assert_throw( !node.expired(), "gtpo::graph<>::setRootNode(): Error: node is expired." );
    shared_node sharedNode = node.lock();
    assert_throw( sharedNode->getInDegree() == 0, "gtpo::graph<>::setRootNode(): Error: trying to set a node with non 0 in degree as a root node." );

    config_t::template container_adapter<weak_nodes>::insert( node, _rootNodes );
}

template < class config_t >
auto    graph<config_t>::isRootNode( weak_node node ) const -> bool
{
    assert_throw( !node.expired(), "gtpo::graph<>::isRootNode(): Error: node is expired." );
    shared_node sharedNode = node.lock();
    if ( sharedNode->getInDegree() != 0 )   // Fast exit when node in degree != 0, it can't be a root node
        return false;
    return gtpo::find_weak_ptr( _rootNodes, node );
}

template < class config_t >
auto    graph<config_t>::contains( weak_node node ) const noexcept -> bool
{
    if ( node.expired() )   // Fast exit.
        return false;
    auto nodeIter = std::find_if( _nodesSearch.cbegin(), _nodesSearch.cend(),
                                            [=](const weak_node& n ) { return ( compare_weak_ptr<>( node, n ) ); } );
    return nodeIter != _nodesSearch.cend();
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template < class config_t >
template < class Edge_t >
auto    graph< config_t >::createEdge( weak_node source, weak_node destination ) -> weak_edge
{
    auto sourcePtr = source.lock();
    auto destinationPtr = destination.lock();
    if ( !sourcePtr ||
         !destinationPtr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::createEdge(Node,Node): Insertion of edge failed, either source or destination nodes are expired." );

    auto edge = std::make_shared<Edge_t>();
    edge->setGraph( this );
    config_t::template container_adapter< shared_edges >::insert( edge, _edges );
    config_t::template container_adapter< weak_edges_search >::insert( edge, _edgesSearch );
    edge->setSrc( source );
    edge->setDst( destination );
    try {
        sourcePtr->addOutEdge( edge );
        destinationPtr->addInEdge( edge );
        if ( sourcePtr.get() != destinationPtr.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
            config_t::template container_adapter<weak_nodes>::remove( destination, _rootNodes );    // Otherwise destination is no longer a root node
        auto weakEdge = weak_edge{edge};
        behaviourable_base::notify_edge_inserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::createEdge(Node,Node): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class config_t >
auto    graph<config_t>::createEdge( weak_node source, weak_edge destination ) -> weak_edge
{
    auto sourcePtr = source.lock();
    auto destinationPtr = destination.lock();
    if ( !sourcePtr ||
         !destinationPtr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::createEdge(Node,Edge): Insertion of edge failed, either source node and/or destination edge is expired." );

    auto edge = std::make_shared< typename config_t::final_edge_t >();
    edge->_graph = this;
    config_t::template container_adapter< shared_edges >::insert( edge, _edges );
    config_t::template container_adapter< weak_edges_search >::insert( edge, _edgesSearch );
    edge->setSrc( source );
    edge->setHDst( destination );
    try {
        sourcePtr->addOutEdge( edge );
        destinationPtr->addInHEdge( edge );
        // Note: hEdge does not modify root nodes.
        auto weakEdge = weak_edge{edge};
        behaviourable_base::notify_edge_inserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::createEdge(Node,Edge): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class config_t >
auto    graph<config_t>::insertEdge( shared_edge edge ) -> weak_edge
{
    assert_throw( edge != nullptr );
    auto source = edge->getSrc().lock();
    if ( source == nullptr ||
         ( edge->getDst().expired() && edge->getHDst().expired() ) )
        throw gtpo::bad_topology_error( "gtpo::graph<>::insertEdge(): Error: Either source and/or destination nodes are expired." );
    edge->setGraph( this );
    config_t::template container_adapter<shared_edges>::insert( edge, _edges );
    config_t::template container_adapter<weak_edges_search>::insert( edge, _edgesSearch );
    try {
        source->addOutEdge( edge );
        auto destination = edge->getDst().lock();
        if ( destination != nullptr ) {
            destination->addInEdge( edge );
            if ( source.get() != destination.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
                config_t::template container_adapter<weak_nodes>::remove( destination, _rootNodes );    // Otherwise destination is no longer a root node
        } else {
            auto hDestination = edge->getHDst().lock();
            if ( hDestination != nullptr )
                hDestination->addInHEdge( edge );
        }
        auto weakEdge = weak_edge(edge);
        behaviourable_base::notify_edge_inserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::createEdge(): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class config_t >
void    graph<config_t>::removeEdge( weak_node source, weak_node destination )
{
    if ( source.expired() ||
         destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::graph<>::removeEdge(): Topology error." );

    // Find the edge associed with source / destination
    if ( _edges.size() == 0 )
        return; // Fast exit
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                 [=](const shared_edge& e ){
                                    return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                             compare_weak_ptr<>( e->getDst(), destination ) );
                                    }
                                 ); // std::find_if
    if ( edgeIter != _edges.end() )
        removeEdge( *edgeIter );
}

template < class config_t >
void    graph<config_t>::removeAllEdges( weak_node source, weak_node destination )
{
    if ( source.expired() ||
         destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::graph<>::removeEdge(): Topology error." );

    while ( getEdgeCount( source, destination ) > 0 )
        removeEdge( source, destination );
}

template < class config_t >
void    graph<config_t>::removeEdge( weak_edge edge )
{
    shared_edge edgePtr = edge.lock();
    if ( edgePtr == nullptr )
        throw gtpo::bad_topology_error( "gtpo::graph<>::removeEdge(): Error: Edge to be removed is already expired." );
    auto source = edgePtr->getSrc().lock();
    auto destination = edgePtr->getDst().lock();
    auto hDestination = edgePtr->getHDst().lock();
    if ( source == nullptr ||           // Expecting a non null source and either a destination or an hyper destination
         ( destination == nullptr && hDestination == nullptr ) )
        throw gtpo::bad_topology_error( "gtpo::graph<>::removeEdge(): Error: Edge source or destination are expired." );
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
            removeEdge( inHEdge );
    }
    edgePtr->setGraph( nullptr );
    config_t::template container_adapter<shared_edges>::remove( edgePtr, _edges );
    config_t::template container_adapter<weak_edges_search>::remove( edge, _edgesSearch );
}

template < class config_t >
auto    graph<config_t>::findEdge( weak_node source, weak_node destination ) const noexcept -> weak_edge
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const shared_edge& e ) noexcept {
                                        return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                 compare_weak_ptr<>( e->getDst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : weak_edge{} );   // std::shared_ptr::operator* is noexcept
}

template < class config_t >
auto    graph<config_t>::hasEdge( weak_node source, weak_node destination ) const noexcept -> bool
{
    return ( findEdge( source, destination).use_count() != 0 );
}

template < class config_t >
auto    graph<config_t>::findEdge( weak_node source, weak_edge destination ) const noexcept -> weak_edge
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const shared_edge& e ) noexcept {
                                        return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                 compare_weak_ptr<>( e->getHDst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : weak_edge{} );   // std::shared_ptr::operator* is noexcept
}

template < class config_t >
auto    graph<config_t>::hasEdge( weak_node source, weak_edge destination ) const noexcept -> bool
{
    return ( findEdge( source, destination ).use_count() != 0 );
}

template < class config_t >
auto    graph<config_t>::getEdgeCount( weak_node source, weak_node destination ) const -> unsigned int
{
    unsigned int edgeCount = 0;
    std::for_each( _edges.begin(), _edges.end(), [&](const shared_edge& e ) {
                                                        if ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                             compare_weak_ptr<>( e->getDst(), destination ) )
                                                            ++edgeCount;
                                                    } );
    return edgeCount;
}

template < class config_t >
auto    graph<config_t>::contains( weak_edge edge ) const noexcept -> bool
{
    if ( edge.expired() )   // Fast exit.
        return false;
    auto edgeIter = std::find_if( _edgesSearch.cbegin(), _edgesSearch.cend(),
                                            [=](const weak_edge& n ) { return ( compare_weak_ptr<>( edge, n ) ); } );
    return edgeIter != _edgesSearch.cend();
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
template < class config_t >
auto graph<config_t>::createGroup() -> weak_group
{
    weak_group weakGroup;
    try {
        auto group = std::make_shared< typename config_t::final_group_t >();
        weakGroup = insertGroup( group );
    } catch (...) { gtpo::assert_throw( false, "gtpo::graph<>::createGroup(): Error: can't insert group in graph." ); }
    return weakGroup;
}

template < class config_t >
auto    graph<config_t>::insertGroup( shared_group group ) noexcept( false ) -> weak_group
{
    assert_throw( group != nullptr, "gtpo::graph<>::insertGroup(): Error: trying to insert a nullptr shared_group" );
    weak_group weakGroup;
    try {
        weakGroup = group;
        group->setGraph( this );
        config_t::template container_adapter<shared_groups>::insert( group, _groups );
        this->notify_group_inserted( weakGroup );
    } catch (...) { throw gtpo::bad_topology_error( "gtpo::graph<>::insertGroup(): Insertion of group failed" ); }
    return weakGroup;
}

template < class config_t >
auto    graph<config_t>::removeGroup( weak_group weakGroup ) noexcept( false ) -> void
{
    gtpo::assert_throw( !weakGroup.expired(), "gtpo::graph<>::removeGroup(): Error: trying to remove an expired group." );
    shared_group group = weakGroup.lock();
    if ( !group )
        gtpo::assert_throw( false, "graph<>::removeGroup(): Error: trying to remove and expired group." );

    // Remove group (it will be automatically deallocated)
    this->notify_group_removed( weakGroup );
    group->setGraph( nullptr );
    config_t::template container_adapter<shared_groups>::remove( group, _groups );
}

template < class config_t >
auto    graph<config_t>::hasGroup( const weak_group& group ) const -> bool
{
    if ( group.expired() )
        return false;
    auto groupIter = std::find_if( _groups.begin(), _groups.end(),
                                        [=](const weak_group& graphGroup  ){ return ( compare_weak_ptr<>( group, graphGroup ) ); } );
    return groupIter != _groups.end();
}
//-----------------------------------------------------------------------------

} // ::gtpo

