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
// \file	gtpoGraph.hpp
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* Graph Node Management *///--------------------------------------------------
template < class Config >
GenGraph< Config >::~GenGraph()
{
    clear();
}

template < class Config >
void    GenGraph< Config >::clear() noexcept
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
        group->_graph = nullptr;
    _groups.clear();
    BehaviourableBase::clear();
}
//-----------------------------------------------------------------------------

/* Graph Node Management *///--------------------------------------------------
template < class Config >
auto GenGraph< Config >::createNode( ) -> WeakNode
{
    WeakNode weakNode;
    try {
        auto node = std::make_shared< typename Config::FinalNode >();
        weakNode = insertNode( node );
    } catch (...) { gtpo::assert_throw( false, "GenGraph<>::createNode(): Error: can't insert node in graph." ); }
    return weakNode;
}

template < class Config >
auto    GenGraph< Config >::createNode( const std::string& className ) noexcept( false ) -> WeakNode
{
    if ( className == "gtpo::Node" )
        return createNode();
    return WeakNode{};
}

template < class Config >
auto    GenGraph< Config >::insertNode( SharedNode node ) -> WeakNode
{
    assert_throw( node != nullptr, "gtpo::GenGraph<>::insertNode(): Error: Trying to insert a nullptr node in graph." );
    WeakNode weakNode;
    try {
        weakNode = node;
        node->setGraph( this );
        Config::template container_adapter< SharedNodes >::insert( node, _nodes );
        Config::template container_adapter< WeakNodesSearch >::insert( weakNode, _nodesSearch );
        Config::template container_adapter< WeakNodes >::insert( weakNode, _rootNodes );
        BehaviourableBase::notifyNodeInserted( weakNode );
    } catch (...) { gtpo::assert_throw( false, "gtpo::GenGraph<>::insertNode(): Error: can't insert node in graph." ); }
    return weakNode;
}

template < class Config >
auto    GenGraph< Config >::removeNode( WeakNode weakNode ) -> void
{
    gtpo::assert_throw( !weakNode.expired(), "gtpo::GenGraph<>::removeNode(): Error: trying to remove an expired node." );
    SharedNode node = weakNode.lock();
    if ( !node )
        gtpo::assert_throw( false, "gtpo::GenGraph<>::removeNode(): Error: node is expired." );

    BehaviourableBase::notifyNodeRemoved( weakNode );

    // Removing all orphant edges pointing to node.
    WeakEdges nodeInEdges; std::copy( node->getInEdges().cbegin(),
                                      node->getInEdges().cend(),
                                      std::back_inserter( nodeInEdges ) );
    for ( auto inEdge: nodeInEdges )
        node->removeInEdge( inEdge );
    for ( auto inEdge: nodeInEdges )
        removeEdge( inEdge );

    WeakEdges nodeOutEdges; std::copy( node->getOutEdges().cbegin(),
                                      node->getOutEdges().cend(),
                                      std::back_inserter( nodeOutEdges ) );
    for ( auto outEdge: nodeOutEdges )
        node->removeOutEdge( outEdge );
    for ( auto outEdge: nodeOutEdges )
        removeEdge( outEdge );

    // Remove node from main graph containers (it will generate node destruction)
    Config::template container_adapter<WeakNodesSearch>::remove( weakNode, _nodesSearch );
    Config::template container_adapter<WeakNodes>::remove( weakNode, _rootNodes );
    node->setGraph( nullptr );
    Config::template container_adapter<SharedNodes>::remove( node, _nodes );
}

template < class Config >
auto    GenGraph< Config >::installRootNode( WeakNode node ) -> void
{
    assert_throw( !node.expired(), "gtpo::GenGraph<>::setRootNode(): Error: node is expired." );
    SharedNode sharedNode = node.lock();
    assert_throw( sharedNode->getInDegree() == 0, "gtpo::GenGraph<>::setRootNode(): Error: trying to set a node with non 0 in degree as a root node." );

    Config::template container_adapter<WeakNodes>::insert( node, _rootNodes );
}

template < class Config >
auto    GenGraph< Config >::isRootNode( WeakNode node ) const -> bool
{
    assert_throw( !node.expired(), "gtpo::GenGraph<>::isRootNode(): Error: node is expired." );
    SharedNode sharedNode = node.lock();
    if ( sharedNode->getInDegree() != 0 )   // Fast exit when node in degree != 0, it can't be a root node
        return false;
    return gtpo::find_weak_ptr( _rootNodes, node );
}

template < class Config >
auto    GenGraph< Config >::contains( WeakNode node ) const noexcept -> bool
{
    if ( node.expired() )   // Fast exit.
        return false;
    auto nodeIter = std::find_if( _nodesSearch.cbegin(), _nodesSearch.cend(),
                                            [=](const WeakNode& n ) { return ( compare_weak_ptr<>( node, n ) ); } );
    return nodeIter != _nodesSearch.cend();
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template < class Config >
template < class Edge_t >
auto    GenGraph< Config >::createEdge( WeakNode source, WeakNode destination ) -> WeakEdge
{
    auto sourcePtr = source.lock();
    auto destinationPtr = destination.lock();
    if ( !sourcePtr ||
         !destinationPtr )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(Node,Node): Insertion of edge failed, either source or destination nodes are expired." );

    auto edge = std::make_shared< typename Edge_t >();
    edge->setGraph( this );
    Config::template container_adapter< SharedEdges >::insert( edge, _edges );
    Config::template container_adapter< WeakEdgesSearch >::insert( edge, _edgesSearch );
    edge->setSrc( source );
    edge->setDst( destination );
    try {
        sourcePtr->addOutEdge( edge );
        destinationPtr->addInEdge( edge );
        if ( sourcePtr.get() != destinationPtr.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
            Config::template container_adapter<WeakNodes>::remove( destination, _rootNodes );    // Otherwise destination is no longer a root node
        auto weakEdge = WeakEdge{edge};
        BehaviourableBase::notifyEdgeInserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(Node,Node): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class Config >
auto    GenGraph< Config >::createEdge( WeakNode source, WeakEdge destination ) -> WeakEdge
{
    auto sourcePtr = source.lock();
    auto destinationPtr = destination.lock();
    if ( !sourcePtr ||
         !destinationPtr )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(Node,Edge): Insertion of edge failed, either source node and/or destination edge is expired." );

    auto edge = std::make_shared< typename Config::FinalEdge >();
    edge->_graph = this;
    Config::template container_adapter< SharedEdges >::insert( edge, _edges );
    Config::template container_adapter< WeakEdgesSearch >::insert( edge, _edgesSearch );
    edge->setSrc( source );
    edge->setHDst( destination );
    try {
        sourcePtr->addOutEdge( edge );
        destinationPtr->addInHEdge( edge );
        // Note: hEdge does not modify root nodes.
        auto weakEdge = WeakEdge{edge};
        BehaviourableBase::notifyEdgeInserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(Node,Edge): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class Config >
auto    GenGraph< Config >::insertEdge( SharedEdge edge ) -> WeakEdge
{
    assert_throw( edge != nullptr );
    auto source{ edge->getSrc().lock() };
    if ( source == nullptr ||
         ( edge->getDst().expired() && edge->getHDst().expired() ) )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::insertEdge(): Error: Either source and/or destination nodes are expired." );
    edge->setGraph( this );
    Config::template container_adapter<SharedEdges>::insert( edge, _edges );
    Config::template container_adapter<WeakEdgesSearch>::insert( edge, _edgesSearch );
    try {
        source->addOutEdge( edge );
        auto destination{ edge->getDst().lock() };
        if ( destination != nullptr ) {
            destination->addInEdge( edge );
            if ( source.get() != destination.get() ) // If edge define is a trivial circuit, do not remove destination from root nodes
                Config::template container_adapter<WeakNodes>::remove( destination, _rootNodes );    // Otherwise destination is no longer a root node
        } else {
            auto hDestination{ edge->getHDst().lock() };
            if ( hDestination != nullptr )
                hDestination->addInHEdge( edge );
        }
        auto weakEdge{WeakEdge{edge}};
        BehaviourableBase::notifyEdgeInserted( weakEdge );
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template < class Config >
void    GenGraph< Config >::removeEdge( WeakNode source, WeakNode destination )
{
    if ( source.expired() ||
         destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Topology error." );

    // Find the edge associed with source / destination
    if ( _edges.size() == 0 )
        return; // Fast exit
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                 [=](const SharedEdge& e ){
                                    return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                             compare_weak_ptr<>( e->getDst(), destination ) );
                                    }
                                 ); // std::find_if
    if ( edgeIter != _edges.end() )
        removeEdge( *edgeIter );
}

template < class Config >
void    GenGraph< Config >::removeAllEdges( WeakNode source, WeakNode destination )
{
    if ( source.expired() ||
         destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Topology error." );

    while ( getEdgeCount( source, destination ) > 0 )
        removeEdge( source, destination );
}

template < class Config >
void    GenGraph< Config >::removeEdge( WeakEdge edge )
{
    SharedEdge edgePtr = edge.lock();
    if ( edgePtr == nullptr )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Error: Edge to be removed is already expired." );
    auto source = edgePtr->getSrc().lock();
    auto destination = edgePtr->getDst().lock();
    auto hDestination = edgePtr->getHDst().lock();
    if ( source == nullptr ||           // Expecting a non null source and either a destination or an hyper destination
         ( destination == nullptr && hDestination == nullptr ) )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Error: Edge source or destination are expired." );
    BehaviourableBase::notifyEdgeRemoved( edge );
    source->removeOutEdge( edge );
    if ( destination )      // Remove edge from destination in edges
        destination->removeInEdge( edge );
    if ( hDestination )     // Remove edge from hyper destination in hyper edges
        hDestination->removeInHEdge( edge );

    // If there is in hyper edges, remove them since their destination edge is beeing destroyed
    if ( edgePtr->getInHDegree() > 0 ) {
        auto& inHEdges{ edgePtr->getInHEdges() };    // Make a deep copy of in hyper edges
        for ( auto& inHEdge : inHEdges )
            removeEdge( inHEdge );
    }
    edgePtr->setGraph( nullptr );
    Config::template container_adapter<SharedEdges>::remove( edgePtr, _edges );
    Config::template container_adapter<WeakEdgesSearch>::remove( edge, _edgesSearch );
}

template < class Config >
auto    GenGraph< Config >::findEdge( WeakNode source, WeakNode destination ) const noexcept -> WeakEdge
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const SharedEdge& e ) noexcept {
                                        return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                 compare_weak_ptr<>( e->getDst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : WeakEdge{} );   // std::shared_ptr::operator* is noexcept
}

template < class Config >
auto    GenGraph< Config >::hasEdge( WeakNode source, WeakNode destination ) const noexcept -> bool
{
    return ( findEdge( source, destination).use_count() != 0 );
}

template < class Config >
auto    GenGraph< Config >::findEdge( WeakNode source, WeakEdge destination ) const noexcept -> WeakEdge
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const SharedEdge& e ) noexcept {
                                        return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                 compare_weak_ptr<>( e->getHDst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : WeakEdge{} );   // std::shared_ptr::operator* is noexcept
}

template < class Config >
auto    GenGraph< Config >::hasEdge( WeakNode source, WeakEdge destination ) const noexcept -> bool
{
    return ( findEdge( source, destination ).use_count() != 0 );
}

template < class Config >
auto    GenGraph< Config >::getEdgeCount( WeakNode source, WeakNode destination ) const -> unsigned int
{
    unsigned int edgeCount = 0;
    std::for_each( _edges.begin(), _edges.end(), [&](const SharedEdge& e ) {
                                                        if ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                             compare_weak_ptr<>( e->getDst(), destination ) )
                                                            ++edgeCount;
                                                    } );
    return edgeCount;
}

template < class Config >
auto    GenGraph< Config >::contains( WeakEdge edge ) const noexcept -> bool
{
    if ( edge.expired() )   // Fast exit.
        return false;
    auto edgeIter = std::find_if( _edgesSearch.cbegin(), _edgesSearch.cend(),
                                            [=](const WeakEdge& n ) { return ( compare_weak_ptr<>( edge, n ) ); } );
    return edgeIter != _edgesSearch.cend();
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
template < class Config >
auto GenGraph< Config >::createGroup() -> WeakGroup
{
    WeakGroup weakGroup;
    try {
        auto group = std::make_shared< typename Config::FinalGroup >();
        weakGroup = insertGroup( group );
    } catch (...) { gtpo::assert_throw( false, "gtpo::GenGraph<>::createGroup(): Error: can't insert group in graph." ); }
    return weakGroup;
}

template < class Config >
auto    GenGraph< Config >::insertGroup( SharedGroup group ) noexcept( false ) -> WeakGroup
{
    assert_throw( group != nullptr, "gtpo::GenGraph<>::insertGroup(): Error: trying to insert a nullptr SharedGroup" );
    WeakGroup weakGroup;
    try {
        weakGroup = group;
        group->setGraph( this );
        Config::template container_adapter<SharedGroups>::insert( group, _groups );
        this->notifyGroupInserted( weakGroup );
    } catch (...) { throw gtpo::bad_topology_error( "gtpo::GenGraph<>::insertGroup(): Insertion of group failed" ); }
    return weakGroup;
}

template < class Config >
auto    GenGraph< Config >::removeGroup( WeakGroup weakGroup ) noexcept( false ) -> void
{
    gtpo::assert_throw( !weakGroup.expired(), "gtpo::GenGraph<>::removeGroup(): Error: trying to remove an expired group." );
    SharedGroup group = weakGroup.lock();
    if ( !group )
        gtpo::assert_throw( false, "GenGraph<>::removeGroup(): Error: trying to remove and expired group." );

    // Remove group (it will be automatically deallocated)
    this->notifyGroupRemoved( weakGroup );
    group->setGraph( nullptr );
    Config::template container_adapter<SharedGroups>::remove( group, _groups );
}

template < class Config >
auto    GenGraph< Config >::hasGroup( const WeakGroup& group ) const -> bool
{
    if ( group.expired() )
        return false;
    auto groupIter = std::find_if( _groups.begin(), _groups.end(),
                                        [=](const WeakGroup& graphGroup  ){ return ( compare_weak_ptr<>( group, graphGroup ) ); } );
    return groupIter != _groups.end();
}
//-----------------------------------------------------------------------------

} // ::gtpo

