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
// \file	gtpoNodeBehaviour.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpoNodeBehaviour_h
#define gtpoNodeBehaviour_h

// GTpo headers
#include "./gtpoBehaviour.h"

namespace gtpo { // ::gtpo

template < class Config, class ConcreteNode >
class GenNode;

template < class Config >
class GenEdge;

/*! \brief Define a node observer interface.
 *
 */
template < class Config >
class NodeBehaviour : public gtpo::Behaviour<Config>
{
public:
    NodeBehaviour( const std::string& name = {} ) noexcept : gtpo::Behaviour<Config>( name ) { }
    virtual ~NodeBehaviour() = default;
    NodeBehaviour( const NodeBehaviour& ) = delete;
    NodeBehaviour& operator=( const NodeBehaviour& ) = delete;

    using WeakNode          = typename GenNode<Config, typename Config::FinalNode>::Weak;
    using SharedNode        = typename GenNode<Config, typename Config::FinalNode>::Shared;
    using SharedNodes       = typename Config::template NodeContainer< SharedNode >;
    using WeakNodesSearch   = typename Config::template SearchContainer< WeakNode >;
    using WeakEdge          = typename GenEdge<Config>::Weak;

public:
    /*! \brief Called immediatly after an in-edge with source \c weakInNode has been inserted.
     */
    virtual void    inNodeInserted( WeakNode& weakInNode, const WeakEdge& edge )  noexcept { (void)weakInNode; (void)edge; }

    /*! \brief Called when an in-edge with source \c weakInNode is about to be removed.
     */
    virtual void    inNodeRemoved( WeakNode& weakInNode, const WeakEdge& edge )  noexcept { (void)weakInNode; (void)edge; }

    /*! \brief Called immediatly after an in node has been removed.
     */
    virtual void    inNodeRemoved()  noexcept { }

    /*! \brief Called immediatly after an out-edge with destination \c weakOutNode has been inserted.
     */
    virtual void    outNodeInserted( WeakNode& weakOutNode, const WeakEdge& edge )  noexcept { (void)weakOutNode; (void)edge; }

    /*! \brief Called when an out-edge with destination \c weakOutNode is about to be removed.
     */
    virtual void    outNodeRemoved( WeakNode& weakOutNode, const WeakEdge& edge )  noexcept { (void)weakOutNode; (void)edge; }

    /*! \brief Called immediatly after an out-edge has been removed.
     */
    virtual void    outNodeRemoved()  noexcept { }
};


/*! \brief FIXME.
 *
 * \nosubgrouping
 */
template < class Behaviour, class SBehaviours >
class BehaviourableNode : public Behaviourable<Behaviour, SBehaviours>
{
    /*! \name BehaviourableNode Object Management *///-------------------------
    //@{
public:
    BehaviourableNode() noexcept { }
    ~BehaviourableNode() { /* Nil */ }
    BehaviourableNode( const BehaviourableNode& ) = delete;
    BehaviourableNode& operator=( const BehaviourableNode& ) = delete;

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    inline auto     addNodeBehaviour( std::unique_ptr<Behaviour> behaviour ) -> void {
        Behaviourable<Behaviour, SBehaviours>::addBehaviour(std::move(behaviour));
    }

    template < class Node, class Edge  >
    auto    notifyInNodeInserted( Node& node, const Edge& edge ) noexcept -> void;

    template < class Node, class Edge  >
    auto    notifyInNodeRemoved( Node& node, const Edge& edge ) noexcept -> void;

    auto    notifyInNodeRemoved() noexcept -> void;

    template < class Node, class Edge  >
    auto    notifyOutNodeInserted( Node& node, const Edge& edge ) noexcept -> void;

    template < class Node, class Edge >
    auto    notifyOutNodeRemoved( Node& node, const Edge& edge ) noexcept -> void;

    auto    notifyOutNodeRemoved() noexcept -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoNodeBehaviour.hpp"

#endif // gtpoNodeBehaviour_h
