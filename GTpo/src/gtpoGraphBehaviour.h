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
// \file	gtpoGraphBehaviour.h
// \author	benoit@destrat.io
// \date	2017 03 09
//-----------------------------------------------------------------------------

#ifndef gtpoGraphBehaviour_h
#define gtpoGraphBehaviour_h

// GTpo headers
#include "./gtpoBehaviour.h"
#include "./gtpoGenNode.h"
#include "./gtpoGenEdge.h"

namespace gtpo { // ::gtpo

/*! \brief Define an observer interface to catch changes in a gtpo::GenGraph.
 *
 */
template < class Config >
class GraphBehaviour :  public Behaviour< Config >
{
public:
    GraphBehaviour() noexcept {}
    virtual ~GraphBehaviour() {}
    GraphBehaviour( const GraphBehaviour& ) = delete;
    GraphBehaviour& operator=( const GraphBehaviour& ) = delete;

    using WeakNode      = typename GenNode<Config>::Weak;
    using WeakEdge      = typename GenEdge<Config>::Weak;
    using WeakGroup     = typename GenGroup<Config>::Weak;

public:
    //! Called immediatly after node \c weakNode has been inserted in graph.
    virtual void    nodeInserted( WeakNode& weakNode ) noexcept { (void)weakNode; }
    //! Called immediatly before node \c weakNode is removed from graph.
    virtual void    nodeRemoved( WeakNode& weakNode) noexcept { (void)weakNode; }

    //! Called immediatly after group \c weakGroup has been inserted in graph.
    virtual void    groupInserted( WeakGroup& weakGroup ) noexcept { (void)weakGroup; }
    //! Called immediatly before group \c weakGroup is removed from graph.
    virtual void    groupRemoved( WeakGroup& weakGroup ) noexcept { (void)weakGroup; }

    /*! \name Edge Notification Interface *///---------------------------------
    //@{
public:
    //! Called immediatly after \c weakEdge has been inserted.
    virtual void    edgeInserted( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //! Called when \c weakEdge is about to be removed.
    virtual void    edgeRemoved( WeakEdge& weakEdge ) noexcept { (void)weakEdge; }
    //@}
    //-------------------------------------------------------------------------

};


/*! \brief FIXME.
 *
 * \nosubgrouping
 */
template < class Behaviour, class SBehaviours >
class BehaviourableGraph : public Behaviourable<Behaviour, SBehaviours>
{
    /*! \name BehaviourableGraph Object Management *///------------------------
    //@{
public:
    BehaviourableGraph() noexcept { }
    ~BehaviourableGraph() { /* Nil */ }
    BehaviourableGraph( const BehaviourableGraph& ) = delete;
    BehaviourableGraph& operator=( const BehaviourableGraph& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    inline auto     addGraphBehaviour( std::unique_ptr<Behaviour> behaviour ) -> void {
        Behaviourable<Behaviour, SBehaviours>::addBehaviour(std::move(behaviour));
    }

    template < class Node >
    auto    notifyNodeInserted( Node& node ) noexcept -> void;

    template < class Node >
    auto    notifyNodeRemoved( Node& node ) noexcept -> void;

    template < class Edge >
    auto    notifyEdgeInserted( Edge& node ) noexcept -> void;

    template < class Edge >
    auto    notifyEdgeRemoved( Edge& node ) noexcept -> void;

    template < class Group >
    auto    notifyGroupInserted( Group& group ) noexcept -> void;

    template < class Group >
    auto    notifyGroupRemoved( Group& group ) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoGraphBehaviour.hpp"

#endif // gtpoGraphBehaviour_h
