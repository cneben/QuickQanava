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
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#include "./gtpoUtils.h"

namespace gtpo { // ::gtpo

/* Virtual Behaviours Management *///------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class T >
auto    Behaviourable< Behaviour, SBehaviours >::notifyBehaviours( void (Behaviour::*method)(T&), T& arg ) noexcept -> void
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

template < class Behaviour, class SBehaviours  >
auto    Behaviourable< Behaviour, SBehaviours >::notifyBehaviours0( void (Behaviour::*method)() ) noexcept -> void
{
    for ( auto& behaviour : _behaviours )
        if ( behaviour != nullptr )
            ((*behaviour).*method)();
}//-----------------------------------------------------------------------------


/* Notification Helper Methods *///--------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeInserted( Node& node ) noexcept -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeInserted, node );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.nodeInserted( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeRemoved( Node& node ) noexcept -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeRemoved, node );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.nodeRemoved( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeModified( Node& node ) noexcept -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeModified, node );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.nodeModified( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeInserted( Edge& edge ) noexcept -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeInserted, edge );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.edgeInserted( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeRemoved( Edge& edge ) noexcept -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeRemoved, edge );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.edgeRemoved( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeModified( Edge& edge ) noexcept -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeModified, edge );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.edgeModified( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupInserted( Group& group ) noexcept -> void
{
    notifyBehaviours< Group >( &Behaviour::groupInserted, group );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.groupInserted( group ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupRemoved( Group& group ) noexcept -> void
{
    notifyBehaviours< Group >( &Behaviour::groupRemoved, group );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.groupRemoved( group ); } );
}

template < class Behaviour, class SBehaviours  >
template < class G >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupModified( G& group ) noexcept -> void
{
    notifyBehaviours< G >( &Behaviour::groupModified, group );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.groupModified( group ); } );
}
//-----------------------------------------------------------------------------


/* Group Adjacent Edges Behaviour *///-----------------------------------------
template < class Config >
void    GroupAdjacentEdgesBehaviour< Config >::nodeInserted( WeakNode& weakNode ) noexcept
{
    SharedNode node = weakNode.lock();
    if ( node ) {
        SharedGroup group = node->getGroup().lock();
        if ( group ) {
            for ( const auto& inEdge : node->getInEdges() ) // Add all node in/out edges to adjacent edge set
                Config::template container_adapter<WeakEdgesSearch>::insert( inEdge, group->getAdjacentEdges() );
            for ( const auto& outEdge : node->getOutEdges() )
                Config::template container_adapter<WeakEdgesSearch>::insert( outEdge, group->getAdjacentEdges() );
        }
    }
}

template < class Config >
void    GroupAdjacentEdgesBehaviour< Config >::nodeRemoved( WeakNode& weakNode ) noexcept
{
    auto node = weakNode.lock();
    if ( node ) {
        auto group = node->getGroup().lock();
        if ( group ) {
            // Remove all node in/out edges from group adjacent edge set
            // Except if this edge "other" src or dst is still part of the group
            for ( const auto& inWeakEdge : node->getInEdges() ) {
                SharedEdge inEdge = inWeakEdge.lock();
                if ( inEdge ) {
                    auto inEdgeSrc = inEdge->getSrc().lock();
                    if ( inEdgeSrc &&
                         inEdgeSrc->getGroup().lock().get() != group.get() )
                        Config::template container_adapter<WeakEdgesSearch>::remove( inWeakEdge, group->getAdjacentEdges() );
                }
            }
            for ( const auto& outWeakEdge : node->getOutEdges() ) {
                SharedEdge outEdge = outWeakEdge.lock();
                if ( outEdge ) {
                    SharedNode outEdgeDst = outEdge->getDst().lock();
                    if ( outEdgeDst &&
                         outEdgeDst->getGroup().lock().get() != group.get() )
                        Config::template container_adapter<WeakEdgesSearch>::remove( outWeakEdge, group->getAdjacentEdges() );
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------

/* Graph Group Edge Set Behaviour *///-----------------------------------------
template < class Config >
void    GraphGroupAjacentEdgesBehaviour< Config >::edgeInserted( WeakEdge& weakEdge ) noexcept
{
    if ( !weakEdge.expired() ) {
        SharedEdge edge = weakEdge.lock();
        if ( edge != nullptr ) {
            // If either src or dst is inside a group, add edge to group adjacent edge set
            SharedNode src = edge->getSrc().lock();
            if ( src != nullptr ) {
                SharedGroup srcGroup = src->getGroup().lock();
                if ( srcGroup != nullptr )
                    Config::template container_adapter<WeakEdgesSearch>::insert( weakEdge, srcGroup->getAdjacentEdges() );
            }
            SharedNode dst = edge->getDst().lock();
            if ( dst != nullptr ) {
                SharedGroup dstGroup = dst->getGroup().lock();
                if ( dstGroup != nullptr )
                    Config::template container_adapter<WeakEdgesSearch>::insert( weakEdge, dstGroup->getAdjacentEdges() );
            }
        }
    }
}

template < class Config >
void    GraphGroupAjacentEdgesBehaviour< Config >::edgeRemoved( WeakEdge& weakEdge ) noexcept
{
    if ( !weakEdge.expired() ) {
        SharedEdge edge = weakEdge.lock();
        if ( edge != nullptr ) {
            // If either src or dst is inside a group, remove edge from group adjacent edge set
            SharedNode src = edge->getSrc().lock();
            if ( src != nullptr ) {
                SharedGroup srcGroup = src->getGroup().lock();
                if ( srcGroup != nullptr )
                    Config::template container_adapter<WeakEdgesSearch>::remove( weakEdge, srcGroup->getAdjacentEdges() );
            }
            SharedNode dst = edge->getDst().lock();
            if ( dst != nullptr ) {
                SharedGroup dstGroup = dst->getGroup().lock();
                if ( dstGroup != nullptr )
                    Config::template container_adapter<WeakEdgesSearch>::remove( weakEdge, dstGroup->getAdjacentEdges() );
            }
        }
    }
}
//-----------------------------------------------------------------------------


} // ::gtpo

