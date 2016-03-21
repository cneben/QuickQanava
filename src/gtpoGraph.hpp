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
    _behaviours.clear();
    clear();
}

template < class Config >
void    GenGraph< Config >::clear()
{
    // Note 20160104: First edges, then nodes (help maintaining topology if
    // womething went wrong during destruction
    for ( auto edge: _edges )   // Do not maintain topology during edge deletion
        edge->_graph = nullptr;
    _edges.clear();
    for ( auto node: _nodes )   // Do not maintain topology during node deletion
        node->_graph = nullptr;
    _rootNodes.clear();         // Remove weak_ptr containers first
    _nodesSearch.clear();
    _nodes.clear();

    _behaviours.clear();
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
    assert_throw( node != nullptr );
    WeakNode weakNode;
    try {
        weakNode = node;
        node->setGraph( this );
        Config::template insert< SharedNodes >::into( _nodes, node );
        Config::template insert< WeakNodesSearch >::into( _nodesSearch, weakNode );
        Config::template insert< WeakNodes >::into( _rootNodes, weakNode );
        notifyBehaviours( &Behaviour::nodeInserted, weakNode );
    } catch (...) { gtpo::assert_throw( false, "GenGraph<>::insertNode(): Error: can't insert node in graph." ); }
    return weakNode;
}

template < class Config >
auto    GenGraph< Config >::removeNode( WeakNode weakNode ) -> void
{
    gtpo::assert_throw( !weakNode.expired(), "GenGraph<>::removeNode(): Error: node is expired." );
    SharedNode node = weakNode.lock();
    if ( !node )
        gtpo::assert_throw( false, "GenGraph<>::removeNode(): Error: node is expired." );

    notifyBehaviours( &Behaviour::nodeRemoved, weakNode );

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
    auto rootNodeIter = std::find_if( _rootNodes.begin(), _rootNodes.end(),
                                            [=](const WeakNode& rootNode ){ return ( compare_weak_ptr<>( node, rootNode ) ); } );
    return ( sharedNode->getInDegree() == 0 && rootNodeIter != _rootNodes.end() );
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
            notifyBehaviours( &Behaviour::edgeInserted, edge );
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
            notifyBehaviours( &Behaviour::edgeInserted, edge );
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
    notifyBehaviours( &Behaviour::edgeRemoved, edge );
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

/* Behaviours Management *///--------------------------------------------------
template < class Config >
auto    GenGraph< Config >::notifyNodeModified( WeakNode node ) -> void
{
    notifyBehaviours( &Behaviour::nodeModified, node );
}

template < class Config >
auto    GenGraph< Config >::notifyEdgeModified( WeakEdge edge ) -> void
{
    notifyBehaviours( &Behaviour::edgeModified, edge );
}

template < class Config >
auto    GenGraph< Config >::notifyBehaviours( void (Behaviour::*method)(WeakNode), WeakNode arg ) -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on memg++ template template parameter template keywordber functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _behaviours )
        if ( behaviour != nullptr )
            ((*behaviour).*method)(arg);
}

template < class Config >
auto    GenGraph< Config >::notifyBehaviours( void (Behaviour::*method)(WeakEdge), WeakEdge arg ) -> void
{
    // Note 20160314: See note for notifyBehaviours( Node )
    for ( auto& behaviour : _behaviours )
        if ( behaviour != nullptr )
            ((*behaviour).*method)(arg);
}
//-----------------------------------------------------------------------------

/* Serialization Management *///-----------------------------------------------
template < class Config >
auto    GenGraph< Config >::serializeToGml( std::string fileName ) -> void
{
    if ( fileName.size() == 0 )
        return;
    try {
        // FIXME
        //gtpo::OutGmlPugiSerializer<Config> gmlOut( fileName );
        //gmlOut.serializeOut( *this );
        //gmlOut.finishOut();
    } catch (...) { }
}
//-----------------------------------------------------------------------------

} // ::gtpo

