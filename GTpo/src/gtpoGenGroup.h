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
// \file	gtpoGenGroup.h
// \author	benoit@destrat.io
// \date	2017 03 04
//-----------------------------------------------------------------------------

#ifndef gtpoGenGroup_h
#define gtpoGenGroup_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// GTpo headers
#include "./gtpoGraphConfig.h"
#include "./gtpoUtils.h"
#include "./gtpoGenNode.h"
#include "./gtpoGenEdge.h"
#include "./gtpoGroupBehaviour.h"
#include "./gtpoAdjacentBehaviour.h"

namespace gtpo { // ::gtpo


/*! \brief Base class for modelling groups of nodes in a gtpo::GenGraph graph.
 *
 * \nosubgrouping
*/
template <class Config = gtpo::GraphConfig>
class GenGroup : public gtpo::GenNode<Config, typename Config::FinalGroup>,
                 public gtpo::BehaviourableGroup< gtpo::GroupBehaviour< Config >,
                                                  std::tuple< gtpo::GroupAdjacentEdgesBehaviour< Config > > >
                                                    //typename Config::GroupBehaviours>
{
    friend GenGraph<Config>;   // GenGraph need access to setGraph()

    /*! \name Node Management *///---------------------------------------------
    //@{
public:
    using Graph             = GenGraph<Config>;

    using WeakNode          = typename GenNode<Config>::Weak;
    using SharedNode        = typename GenNode<Config>::Shared;
    using WeakNodes         = typename Config::template NodeContainer< WeakNode >;

    using WeakEdge          = typename GenEdge<Config>::Weak;
    using WeakEdgesSearch   = typename Config::template SearchContainer< WeakEdge >;

    using Weak          = std::weak_ptr< typename Config::FinalGroup >;
    using Shared        = std::shared_ptr< typename Config::FinalGroup >;
    using WeakGroup     = Weak;
    using SharedGroup   = Shared;

    GenGroup() noexcept : gtpo::GenNode<Config, typename Config::FinalGroup>() { }
    ~GenGroup() { /* Nil */ }
    GenGroup( const GenGroup& ) = delete;
    GenGroup& operator=( GenGroup const& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Nodes Management *///--------------------------------------
    //@{
public:
    //! Return group's nodes.
    inline auto getNodes() noexcept -> const WeakNodes& { return _nodes; }

    //! Return true if group contains \c node.
    auto        hasNode( const WeakNode& node ) const noexcept -> bool;
    //! Return group registered node count.
    inline auto getNodeCount( ) const noexcept -> int { return static_cast< int >( _nodes.size() ); }
private:
    WeakNodes   _nodes;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Adjacent Edges *///----------------------------------------------
    //@{
public:
    inline auto     getEdges() noexcept -> WeakEdgesSearch& { return _edges; }
    inline auto     getEdges() const noexcept -> const WeakEdgesSearch& { return _edges; }

    inline auto     getAdjacentEdges() noexcept -> WeakEdgesSearch& { return _adjacentEdges; }
    inline auto     getAdjacentEdges() const noexcept -> const WeakEdgesSearch& { return _adjacentEdges; }

protected:
    WeakEdgesSearch _edges;
    WeakEdgesSearch _adjacentEdges;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#endif // gtpoGenGroup_h

