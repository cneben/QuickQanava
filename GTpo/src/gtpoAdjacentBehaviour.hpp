/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

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
        auto group = std::static_pointer_cast<GenGroup<Config>>(node->getGroup().lock());
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
        auto group = std::static_pointer_cast<GenGroup<Config>>(node->getGroup().lock());
        if ( group ) {
            // Remove all node in/out edges from group adjacent edge set
            // Except if this edge "other" src or dst is still part of the group
            for ( const auto& inWeakEdge : node->getInEdges() ) {
                using WeakEdge = typename GenEdge<Config>::Weak;
                using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;
                auto inEdge = inWeakEdge.lock();
                if ( inEdge ) {
                    auto inEdgeSrc = inEdge->getSrc().lock();
                    if ( inEdgeSrc &&
                         inEdgeSrc->getGroup().lock().get() != group.get() )
                        Config::template container_adapter<WeakEdgesSearch>::remove( inWeakEdge, group->getAdjacentEdges() );
                }
            }
            for ( const auto& outWeakEdge : node->getOutEdges() ) {
                auto outEdge = outWeakEdge.lock();
                if ( outEdge ) {
                    auto outEdgeDst = outEdge->getDst().lock();
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
    auto edge = weakEdge.lock();
    if ( edge != nullptr ) {
        // If either src or dst is inside a group, add edge to group adjacent edge set
        auto src = edge->getSrc().lock();
        if ( src != nullptr ) {
            auto srcGroup = std::static_pointer_cast<Group>(src->getGroup().lock());
            if ( srcGroup != nullptr )
                Config::template container_adapter<WeakEdgesSearch>::insert( weakEdge, srcGroup->getAdjacentEdges() );
        }
        auto dst = edge->getDst().lock();
        if ( dst != nullptr ) {
            auto dstGroup = std::static_pointer_cast<Group>(dst->getGroup().lock());
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
        auto src = edge->getSrc().lock();
        if ( src != nullptr ) {
            auto srcGroup = std::static_pointer_cast<Group>(src->getGroup().lock());
            if ( srcGroup != nullptr )
                Config::template container_adapter<WeakEdgesSearch>::remove( weakEdge, srcGroup->getAdjacentEdges() );
        }
        auto dst = edge->getDst().lock();
        if ( dst != nullptr ) {
            auto dstGroup = std::static_pointer_cast<Group>(dst->getGroup().lock());
            if ( dstGroup != nullptr )
                Config::template container_adapter<WeakEdgesSearch>::remove( weakEdge, dstGroup->getAdjacentEdges() );
        }
    }
}
//-----------------------------------------------------------------------------

} // ::gtpo

