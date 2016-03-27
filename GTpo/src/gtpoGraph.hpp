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
// \author	benoit@qanava.org
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
void    GenGraph< Config >::clear()
{
    // Note 20160104: First edges, then nodes (it helps maintaining topology if
    // womething went wrong during destruction
    for ( auto edge: _edges )   // Do not maintain topology during edge deletion
        edge->_graph = nullptr;
    _edges.clear();
    for ( auto node: _nodes )   // Do not maintain topology during node deletion
        node->_graph = nullptr;
    _rootNodes.clear();         // Remove weak_ptr containers first
    _nodesSearch.clear();
    _nodes.clear();

    // Clearing groups and behaviours
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
        auto node = std::make_shared< typename Config::Node >();
        weakNode = insertNode( node );
    } catch (...) { gtpo::assert_throw( false, "GenGraph<>::createNode(): Error: can't insert node in graph." ); }
    return weakNode;
}

template < class Config >
auto    GenGraph< Config >::createNode( const std::string& nodeClassName ) noexcept( false ) -> WeakNode
{
    if ( nodeClassName == "gtpo::Node" )    // FIXME 20160212
        return createNode();
    return WeakNode();
}

template < class Config >
auto    GenGraph< Config >::insertNode( SharedNode node ) -> WeakNode
{
    assert_throw( node != nullptr, "gtpo::GenGraph<>::insertNode(): Error: Trying to insert a nullptr node in graph." );
    WeakNode weakNode;
    try {
        weakNode = node;
        node->setGraph( this );
        Config::template insert< SharedNodes >::into( _nodes, node );
        Config::template insert< WeakNodesSearch >::into( _nodesSearch, weakNode );
        Config::template insert< WeakNodes >::into( _rootNodes, weakNode );
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
    Config::template remove<WeakNodesSearch>::from( _nodesSearch, weakNode );
    Config::template remove<WeakNodes>::from( _rootNodes, weakNode );
    node->setGraph( nullptr );
    Config::template remove<SharedNodes>::from( _nodes, node );
}

template < class Config >
auto    GenGraph< Config >::installRootNode( WeakNode node ) -> void
{
    assert_throw( !node.expired(), "gtpo::GenGraph<>::setRootNode(): Error: node is expired." );
    SharedNode sharedNode = node.lock();
    assert_throw( sharedNode->getInDegree() == 0, "gtpo::GenGraph<>::setRootNode(): Error: trying to set a node with non 0 in degree as a root node." );

    Config::template insert<WeakNodes>::into( _rootNodes, node );
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
auto    GenGraph< Config >::containsNode( WeakNode node ) const noexcept -> bool
{
    auto nodeIter = std::find_if( _nodesSearch.cbegin(), _nodesSearch.cend(),
                                            [=](const WeakNode& n ) { return ( compare_weak_ptr<>( node, n ) ); } );
    return nodeIter != _nodesSearch.cend();
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template < class Config >
auto    GenGraph< Config >::createEdge( WeakNode src, WeakNode dst ) -> WeakEdge
{
    WeakEdge edge;
    try {
        auto sharedEdge = std::make_shared< typename Config::Edge >();
        sharedEdge->setGraph( this );
        Config::template insert< SharedEdges >::into( _edges, sharedEdge );
        sharedEdge->setSrc( src );
        sharedEdge->setDst( dst );
        edge = sharedEdge;

        try {
            SharedNode source = src.lock();
            if ( source )
                source->addOutEdge( edge );
            SharedNode destination = dst.lock();
            if ( destination )
                destination->addInEdge( edge );

            if ( source != destination ) // If edge define is a trivial circuit, do not remove destination from root nodes
                Config::template remove<WeakNodes>::from( _rootNodes, dst );    // Otherwise destination is no longer a root node
            BehaviourableBase::notifyEdgeInserted( edge );
        } catch ( ... ) {
            throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(): Insertion of edge failed, source or destination nodes topology can't be modified." );
        }
    } catch (...) { throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(): Insertion of edge failed, source or destination nodes are probably expired." ); }
    return edge;
}

template < class Config >
auto    GenGraph< Config >::createEdge( const std::string& className, WeakNode src, WeakNode dst ) -> WeakEdge
{
    if ( className == "gtpo::Edge" )
        return createEdge( src, dst );
    return WeakEdge();
}

template < class Config >
auto    GenGraph< Config >::insertEdge( SharedEdge sharedEdge ) -> WeakEdge
{
    assert_throw( sharedEdge != nullptr );
    WeakNode source{ sharedEdge->getSrc() };
    WeakNode destination{ sharedEdge->getDst() };
    SharedNode ownedSource{ source.lock() };
    SharedNode ownedDestination{ destination.lock() };
    assert_throw( ownedSource != nullptr &&
                  ownedDestination != nullptr, "gtpo::GenGraph<>::insertEdge(): Error: Can't lock source and/or destination nodes." );

    WeakEdge edge;
    try {
        sharedEdge->setGraph( this );
        Config::template insert<SharedEdges>::into( _edges, sharedEdge );
        edge = sharedEdge;
        try {
            ownedSource->addOutEdge( edge );
            ownedDestination->addInEdge( edge );
            if ( ownedSource != ownedDestination ) // If edge define is a trivial circuit, do not remove destination from root nodes
                Config::template remove<WeakNodes>::from( _rootNodes, destination );    // Otherwise destination is no longer a root node
            notifyEdgeInserted( edge );
        } catch ( ... ) {
            throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(): Insertion of edge failed, source or destination nodes topology can't be modified." );
        }
    } catch (...) { throw gtpo::bad_topology_error( "gtpo::GenGraph<>::createEdge(): Insertion of edge failed, source or destination nodes are probably expired." ); }
    return edge;
}

template < class Config >
void    GenGraph< Config >::removeEdge( WeakNode source, WeakNode destination )
{
    if ( source.expired() || destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Topology error." );

    // Find the edge associed with source / destination
    auto edgeIter = _edges.end();
    if ( _edges.begin() != _edges.end( ) ) {
        edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                 [=](const SharedEdge& e ){
                                    return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                             compare_weak_ptr<>( e->getDst(), destination ) );
                                    }
        );
    }
    if ( edgeIter != _edges.end() )
        removeEdge( *edgeIter );
}

template < class Config >
void    GenGraph< Config >::removeAllEdges( WeakNode source, WeakNode destination )
{
    if ( source.expired() || destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Topology error." );

    while ( getEdgeCount( source, destination ) > 0 )
        removeEdge( source, destination );
}

template < class Config >
void    GenGraph< Config >::removeEdge( WeakEdge edge )
{
    SharedEdge sharedEdge = edge.lock();
    if ( !sharedEdge )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Error: Edge to be removed is already expired." );

    SharedNode source = sharedEdge->getSrc().lock();
    SharedNode destination = sharedEdge->getDst().lock();
    if ( !source || !destination )
        throw gtpo::bad_topology_error( "gtpo::GenGraph<>::removeEdge(): Error: Edge source or destination are expired." );
    BehaviourableBase::notifyEdgeRemoved( edge );
    source->removeOutEdge( edge );
    destination->removeInEdge( edge );
    sharedEdge->setGraph( nullptr );
    Config::template remove<SharedEdges>::from( _edges, sharedEdge );
}

template < class Config >
auto    GenGraph< Config >::findEdge( WeakNode source, WeakNode destination ) const -> WeakEdge
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                    [=](const SharedEdge& e ){
                                        return ( compare_weak_ptr<>( e->getSrc(), source ) &&
                                                 compare_weak_ptr<>( e->getDst(), destination ) );
                                    } );
    return ( edgeIter != _edges.end() ? *edgeIter : WeakEdge() );
}

template < class Config >
auto    GenGraph< Config >::hasEdge( WeakNode source, WeakNode destination ) const -> bool
{
    return ( findEdge( source, destination).use_count() != 0 );
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
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
template < class Config >
auto    GenGraph< Config >::createGroup( ) noexcept( false ) -> WeakGroup
{
    WeakGroup weakGroup;
    try {
        auto group = std::make_shared< typename Config::Group >();
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
        Config::template insert<SharedGroups>::into( _groups, group );
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
    Config::template remove<SharedGroups>::from( _groups, group );
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

