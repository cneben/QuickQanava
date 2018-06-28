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
// \file	adjacent_behaviour.hpp
// \author	benoit@destrat.io
// \date	2017 03 09
//-----------------------------------------------------------------------------

#include "./utils.h"

namespace gtpo { // ::gtpo

/* Group Adjacent Edges behaviour *///-----------------------------------------
template < class config_t >
void    group_adjacent_edges_behaviour<config_t>::node_inserted( weak_node& weakNode ) noexcept
{
    shared_node node = weakNode.lock();
    if ( node ) {
        auto group = std::static_pointer_cast<gtpo::group<config_t>>(node->getGroup().lock());
        if ( group ) {
            for ( const auto& inEdge : node->getInEdges() ) // Add all node in/out edges to adjacent edge set
                config_t::template container_adapter<weak_edges_search>::insert( inEdge, group->getAdjacentEdges() );
            for ( const auto& outEdge : node->getOutEdges() )
                config_t::template container_adapter<weak_edges_search>::insert( outEdge, group->getAdjacentEdges() );
        }
    }
}

template < class config_t >
void    group_adjacent_edges_behaviour<config_t>::node_removed( weak_node& weakNode ) noexcept
{
    auto node = weakNode.lock();
    if ( node ) {
        auto group = std::static_pointer_cast<gtpo::group<config_t>>(node->getGroup().lock());
        if ( group ) {
            // Remove all node in/out edges from group adjacent edge set
            // Except if this edge "other" src or dst is still part of the group
            for ( const auto& inWeakEdge : node->getInEdges() ) {
                using weak_edge = typename edge<config_t>::weak;
                using weak_edges_search   = typename config_t::template search_container_t< weak_edge >;
                auto inEdge = inWeakEdge.lock();
                if ( inEdge ) {
                    auto inEdgeSrc = inEdge->getSrc().lock();
                    if ( inEdgeSrc &&
                         inEdgeSrc->getGroup().lock().get() != group.get() )
                        config_t::template container_adapter<weak_edges_search>::remove( inWeakEdge, group->getAdjacentEdges() );
                }
            }
            for ( const auto& outWeakEdge : node->getOutEdges() ) {
                auto outEdge = outWeakEdge.lock();
                if ( outEdge ) {
                    auto outEdgeDst = outEdge->getDst().lock();
                    if ( outEdgeDst &&
                         outEdgeDst->getGroup().lock().get() != group.get() )
                        config_t::template container_adapter<weak_edges_search>::remove( outWeakEdge, group->getAdjacentEdges() );
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------

/* Graph Group Edge Set behaviour *///-----------------------------------------
template < class config_t >
void    graph_group_adjacent_edges_behaviour<config_t>::edge_inserted( weak_edge& weakEdge ) noexcept
{
    auto edge = weakEdge.lock();
    if ( edge != nullptr ) {
        // If either src or dst is inside a group, add edge to group adjacent edge set
        auto src = edge->getSrc().lock();
        if ( src != nullptr ) {
            auto srcGroup = std::static_pointer_cast<group_t>(src->getGroup().lock());
            if ( srcGroup != nullptr )
                config_t::template container_adapter<weak_edges_search>::insert( weakEdge, srcGroup->getAdjacentEdges() );
        }
        auto dst = edge->getDst().lock();
        if ( dst != nullptr ) {
            auto dstGroup = std::static_pointer_cast<group_t>(dst->getGroup().lock());
            if ( dstGroup != nullptr )
                config_t::template container_adapter<weak_edges_search>::insert( weakEdge, dstGroup->getAdjacentEdges() );
        }
    }
}

template < class config_t >
void    graph_group_adjacent_edges_behaviour<config_t>::edge_removed( weak_edge& weakEdge ) noexcept
{
    auto edge = weakEdge.lock();
    if ( edge ) {
        // If either src or dst is inside a group, remove edge from group adjacent edge set
        auto src = edge->getSrc().lock();
        if ( src != nullptr ) {
            auto srcGroup = std::static_pointer_cast<group_t>(src->getGroup().lock());
            if ( srcGroup != nullptr )
                config_t::template container_adapter<weak_edges_search>::remove( weakEdge, srcGroup->getAdjacentEdges() );
        }
        auto dst = edge->getDst().lock();
        if ( dst != nullptr ) {
            auto dstGroup = std::static_pointer_cast<group_t>(dst->getGroup().lock());
            if ( dstGroup != nullptr )
                config_t::template container_adapter<weak_edges_search>::remove( weakEdge, dstGroup->getAdjacentEdges() );
        }
    }
}
//-----------------------------------------------------------------------------

} // ::gtpo

