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
// This file is a part of the GTpo software.
//
// \file	gtpoNode.hpp
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* GenNode Edges Management *///-----------------------------------------------
template < class Config >
auto GenNode< Config >::addOutEdge( WeakEdge outEdgePtr ) -> void
{
    assert_throw( !outEdgePtr.expired(), "gtpo::GenNode<>::addOutEdge(): Error: out edge is expired." );
    auto node = this->shared_from_this();
    auto outEdge = outEdgePtr.lock( );
    auto outEdgeSrc = outEdge->getSrc().lock();
    if ( outEdge ) {
        if ( !outEdgeSrc || outEdgeSrc != node )  // Out edge source should point to target node
            outEdge->setSrc( node );
        Config::template insert< WeakEdges >::into( _outEdges, outEdgePtr );
        if ( !outEdge->getDst().expired() ) {
            Config::template insert< WeakNodes >::into( _outNodes, outEdge->getDst() );
            notifyOutNodeInserted( outEdge->getDst() );
        }
    }
}

template < class Config >
auto GenNode< Config >::addInEdge( WeakEdge inEdgePtr ) -> void
{
    assert_throw( !inEdgePtr.expired(), "gtpo::GenNode<>::addInEdge(): Error: in edge is expired." );
    auto node = this->shared_from_this();
    auto inEdge = inEdgePtr.lock( );
    auto inEdgeDst = inEdge->getDst().lock();
    if ( inEdge ) {
        if ( !inEdgeDst || inEdgeDst != node ) // In edge destination should point to target node
            inEdge->setDst( node );
        Config::template insert< WeakEdges >::into( _inEdges, inEdgePtr );
        if ( !inEdge->getSrc().expired() ) {
            Config::template insert< WeakNodes >::into( _inNodes, inEdge->getSrc() );
            notifyInNodeInserted( inEdge->getSrc() );
        }
    }
}

template < class Config >
auto GenNode< Config >::removeOutEdge( const WeakEdge outEdge ) -> void
{
    gtpo::assert_throw( !outEdge.expired(), "gtpo::GenNode<>::removeOutEdge(): Error: Out edge has expired" );
    auto outEdgePtr = outEdge.lock( );
    auto outEdgeSrcPtr = outEdgePtr->getSrc().lock();
    gtpo::assert_throw( outEdgeSrcPtr != nullptr &&    // Out edge src must be this node
                        outEdgeSrcPtr.get() == this, "gtpo::GenNode<>::removeOutEdge(): Error: Out edge source is expired or different from this node.");

    auto outEdgeDst = outEdgePtr->getDst().lock();
    if ( outEdgeDst != nullptr ) {
        gtpo::assert_throw( outEdgeDst != nullptr, "gtpo::GenNode<>::removeOutEdge(): Error: Out edge destination is expired." );
        notifyOutNodeRemoved( outEdgePtr->getDst() );
    }
    Config::template remove< WeakEdges >::from( _outEdges, outEdge );
    Config::template remove< WeakNodes >::from( _outNodes, outEdgePtr->getDst() );
    if ( getInDegree() == 0 ) {
        Graph* graph{ getGraph() };
        if ( graph != nullptr )
            graph->installRootNode( WeakNode{ this->shared_from_this() } );
    }
}

template < class Config >
auto GenNode< Config >::removeInEdge( const WeakEdge inEdge ) -> void
{
    gtpo::assert_throw( !inEdge.expired(), "gtpo::GenNode<>::removeInEdge(): Error: In edge has expired" );
    auto nodePtr = this->shared_from_this();
    auto inEdgePtr = inEdge.lock( );
    auto inEdgeDstPtr = inEdgePtr->getDst().lock();
    gtpo::assert_throw( inEdgeDstPtr &&    // in edge dst must be this node
                        inEdgeDstPtr == nodePtr, "gtpo::GenNode<>::removeInEdge(): Error: In edge destination is expired or different from this node.");

    auto inEdgeSrcPtr = inEdgePtr->getSrc().lock();
    gtpo::assert_throw( inEdgeSrcPtr != nullptr, "gtpo::GenNode<>::removeInEdge(): Error: In edge source is expired." );
    notifyInNodeAboutToBeRemoved( inEdgePtr->getSrc() );
    Config::template remove< WeakEdges >::from( _inEdges, inEdge );
    Config::template remove< WeakNodes >::from( _inNodes, inEdgePtr->getSrc() );
    if ( getInDegree() == 0 ) {
        Graph* graph{ getGraph() };
        if ( graph != nullptr )
            graph->installRootNode( WeakNode{ nodePtr } );
    }
    notifyInNodeRemoved();
}
//-----------------------------------------------------------------------------

/* GenNode Behaviour Notifications *///----------------------------------------
template < class Config >
auto    GenNode< Config >::notifyInNodeInserted( WeakNode& inNode ) noexcept -> void
{
    BehaviourableBase::notifyBehaviours( &gtpo::NodeBehaviour<Config>::inNodeInserted, inNode );
    this->sNotifyBehaviours( [&](auto& behaviour) { behaviour.inNodeInserted( inNode ); } );
}

template < class Config >
auto    GenNode< Config >::notifyInNodeAboutToBeRemoved( WeakNode& inNode ) noexcept -> void
{
    BehaviourableBase::notifyBehaviours( &gtpo::NodeBehaviour<Config>::inNodeAboutToBeRemoved, inNode );
    this->sNotifyBehaviours( [&](auto& behaviour) { behaviour.inNodeAboutToBeRemoved( inNode ); } );
}

template < class Config >
auto    GenNode< Config >::notifyInNodeRemoved() noexcept -> void
{
    BehaviourableBase::notifyBehaviours0( &gtpo::NodeBehaviour<Config>::inNodeRemoved );
    this->sNotifyBehaviours( [&](auto& behaviour) { behaviour.inNodeRemoved(); } );
}

template < class Config >
auto    GenNode< Config >::notifyOutNodeInserted( WeakNode& outNode ) noexcept -> void
{
    BehaviourableBase::notifyBehaviours( &gtpo::NodeBehaviour<Config>::outNodeInserted, outNode );
    this->sNotifyBehaviours( [&](auto& behaviour) { behaviour.outNodeInserted( outNode ); } );
}

template < class Config >
auto    GenNode< Config >::notifyOutNodeRemoved( WeakNode& outNode ) noexcept -> void
{
    BehaviourableBase::notifyBehaviours( &gtpo::NodeBehaviour<Config>::outNodeRemoved, outNode );
    this->sNotifyBehaviours( [&](auto& behaviour) { behaviour.outNodeRemoved( outNode ); } );
}
//-----------------------------------------------------------------------------

} // ::gtpo
