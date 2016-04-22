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
// \file	gtpoBehaviour.hpp
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

#include "gtpoUtils.h"

namespace gtpo { // ::gtpo

/* Virtual Behaviours Management *///------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class T >
auto    Behaviourable< Behaviour, SBehaviours >::notifyBehaviours( void (Behaviour::*method)(T&), T& arg ) -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on template template parameter template keyword functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _behaviours )
        if ( behaviour != nullptr )
            ((*behaviour).*method)(arg);
}
//-----------------------------------------------------------------------------


/* Notification Helper Methods *///--------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeInserted( Node& node ) -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeInserted, node );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.nodeInserted( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeRemoved( Node& node ) -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeRemoved, node );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.nodeRemoved( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeModified( Node& node ) -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeModified, node );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.nodeModified( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeInserted( Edge& edge ) -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeInserted, edge );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.edgeInserted( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeRemoved( Edge& edge ) -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeRemoved, edge );
    sNotifyBehaviours( [&](auto behaviour) { behaviour.edgeRemoved( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeModified( Edge& edge ) -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeModified, edge );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.edgeModified( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupInserted( Group& group ) -> void
{
    notifyBehaviours< Group >( &Behaviour::groupInserted, group );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.groupInserted( group ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupRemoved( Group& group ) -> void
{
    notifyBehaviours< Group >( &Behaviour::groupRemoved, group );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.groupRemoved( group ); } );
}

template < class Behaviour, class SBehaviours  >
template < class G >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupModified( G& group ) -> void
{
    notifyBehaviours< G >( &Behaviour::groupModified, group );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.groupModified( group ); } );
}
//-----------------------------------------------------------------------------


/* Group Adjacent Edges Behaviour *///-----------------------------------------
template < class Config >
auto GroupAdjacentEdgesBehaviour< Config >::nodeInserted( WeakNode& weakNode ) -> void
{
    if ( !weakNode.expired() ) {
        SharedNode node = weakNode.lock();
        if ( node != nullptr ) {
            WeakGroup weakGroup = node->getGroup();
            SharedGroup group = weakGroup.lock();
            for ( auto inEdge : node->getInEdges() ) // Add all node in/out edges to adjacent edge set
                Config::template insert<WeakEdgesSearch>::into( group->getAdjacentEdges(), inEdge );
            for ( auto outEdge : node->getOutEdges() )
                Config::template insert<WeakEdgesSearch>::into( group->getAdjacentEdges(), outEdge );
        }
    }
}

template < class Config >
auto GroupAdjacentEdgesBehaviour< Config >::nodeRemoved( WeakNode& weakNode ) -> void
{
    if ( !weakNode.expired() ) {
        SharedNode node = weakNode.lock();
        if ( node != nullptr ) {
            WeakGroup weakGroup = node->getGroup();
            SharedGroup group = weakGroup.lock();
            // Remove all node in/out edges from group adjacent edge set
                // Except if this edge "other" src or dst is still part of the group
            for ( auto inWeakEdge : node->getInEdges() ) {
                SharedEdge inEdge = inWeakEdge.lock();
                WeakNode inEdgeWeakSrc = inEdge->getSrc();
                SharedNode inEdgeSrc = inEdgeWeakSrc.lock();
                if ( !gtpo::compare_weak_ptr( inEdgeSrc->getGroup(), weakGroup ) )
                    Config::template remove<WeakEdgesSearch>::from( group->getAdjacentEdges(), inWeakEdge );
            }
            for ( auto outWeakEdge : node->getOutEdges() ) {
                SharedEdge outEdge = outWeakEdge.lock();
                WeakNode outEdgeWeakDst = outEdge->getDst();
                SharedNode outEdgeDst = outEdgeWeakDst.lock();
                if ( !gtpo::compare_weak_ptr( outEdgeDst->getGroup(), weakGroup ) )
                    Config::template remove<WeakEdgesSearch>::from( group->getAdjacentEdges(), outWeakEdge );
            }
        }
    }
}
//-----------------------------------------------------------------------------

/* Graph Group Edge Set Behaviour *///-----------------------------------------
template < class Config >
auto GraphGroupAjacentEdgesBehaviour< Config >::edgeInserted( WeakEdge& weakEdge ) -> void
{
    if ( !weakEdge.expired() ) {
        SharedEdge edge = weakEdge.lock();
        if ( edge != nullptr ) {
            // If either src or dst is inside a group, add edge to group adjacent edge set
            SharedNode src = edge->getSrc().lock();
            SharedNode dst = edge->getSrc().lock();
            if ( src != nullptr && dst != nullptr ) {
                SharedGroup srcGroup = src->getGroup().lock();
                if ( srcGroup != nullptr )
                    Config::template insert<WeakEdgesSearch>::into( srcGroup->getAdjacentEdges(), weakEdge );
                SharedGroup dstGroup = dst->getGroup().lock();
                if ( dstGroup != nullptr )
                    Config::template insert<WeakEdgesSearch>::into( dstGroup->getAdjacentEdges(), weakEdge );
            }
        }
    }
}

template < class Config >
auto GraphGroupAjacentEdgesBehaviour< Config >::edgeRemoved( WeakEdge& weakEdge ) -> void
{
    if ( !weakEdge.expired() ) {
        SharedEdge edge = weakEdge.lock();
        if ( edge != nullptr ) {
            // If either src or dst is inside a group, remove edge from group adjacent edge set
            SharedNode src = edge->getSrc().lock();
            SharedNode dst = edge->getSrc().lock();
            if ( src != nullptr && dst != nullptr ) {
                SharedGroup srcGroup = src->getGroup().lock();
                if ( srcGroup != nullptr )
                    Config::template remove<WeakEdgesSearch>::from( srcGroup->getAdjacentEdges(), weakEdge );
                SharedGroup dstGroup = dst->getGroup().lock();
                if ( dstGroup != nullptr )
                    Config::template remove<WeakEdgesSearch>::from( dstGroup->getAdjacentEdges(), weakEdge );
            }
        }
    }
}
//-----------------------------------------------------------------------------


} // ::gtpo

