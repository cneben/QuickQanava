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

