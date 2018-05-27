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
// \file	gtpoAdjacentBehaviour.h
// \author	benoit@destrat.io
// \date	2017 03 09
//-----------------------------------------------------------------------------

#ifndef gtpoAdjacentBehaviour_h
#define gtpoAdjacentBehaviour_h

// GTpo headers
#include "./gtpoGraphBehaviour.h"
#include "./gtpoGenGroup.h"
#include "./gtpoGroupBehaviour.h"

namespace gtpo { // ::gtpo

/*! \brief Collect and maintain a group adjacent edge set when a node is removed or inserted in a group.
 *
 *  This behaviour should be used with GraphGroupAjacentEdgesBehaviour.
 */
template < class Config >
class GroupAdjacentEdgesBehaviour : public GroupBehaviour< Config >
{
    /* Group Edge Set Behaviour *///-------------------------------------------
public:
    GroupAdjacentEdgesBehaviour() noexcept = default;
    virtual ~GroupAdjacentEdgesBehaviour() = default;
    GroupAdjacentEdgesBehaviour( const GroupAdjacentEdgesBehaviour& ) = delete;
    GroupAdjacentEdgesBehaviour& operator=( const GroupAdjacentEdgesBehaviour& ) = delete;

    using WeakNode      = typename GenNode<Config>::Weak;
    using SharedNode    = typename GenNode<Config>::Shared;

    using WeakEdge          = typename GenEdge<Config>::Weak;
    using SharedGroup       = std::shared_ptr< GenGroup<Config> >;
    using Group             = typename Config::FinalGroup;
    using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;
public:
    virtual void    nodeInserted( WeakNode& weakNode ) noexcept override;
    virtual void    nodeRemoved( WeakNode& weakNode ) noexcept override;
    //-------------------------------------------------------------------------
};

/*! \brief Collect and maintain a group adjacent edge set and group edge set.
 *
 */
template < class Config >
class GraphGroupAjacentEdgesBehaviour : public GraphBehaviour< Config >
{
    /* Group Edge Set Behaviour *///-------------------------------------------
public:
    GraphGroupAjacentEdgesBehaviour() noexcept { }
    virtual ~GraphGroupAjacentEdgesBehaviour() { }
    GraphGroupAjacentEdgesBehaviour( const GraphGroupAjacentEdgesBehaviour& ) = default;
    GraphGroupAjacentEdgesBehaviour& operator=( const GraphGroupAjacentEdgesBehaviour& ) = delete;

    using WeakEdge  = typename GenEdge<Config>::Weak;
    using Group     = typename Config::FinalGroup;
    using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;

public:
    virtual void    edgeInserted( WeakEdge& weakEdge ) noexcept override;
    virtual void    edgeRemoved( WeakEdge& weakEdge ) noexcept override;
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoAdjacentBehaviour.hpp"

#endif // gtpoAdjacentBehaviour_h
