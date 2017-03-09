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
// \file	gtpoAdjacentBehaviour.hpp
// \author	benoit@destrat.io
// \date	2017 03 09
//-----------------------------------------------------------------------------

#include "./gtpoUtils.h"

namespace gtpo { // ::gtpo

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

template < class Config >
void    GraphGroupAjacentEdgesBehaviour< Config >::edgeRemoved( WeakEdge& weakEdge ) noexcept
{
    auto edge = weakEdge.lock();
    if ( edge ) {
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
//-----------------------------------------------------------------------------

} // ::gtpo

