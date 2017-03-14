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
