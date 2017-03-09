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
// \file	gtpoNodeBehaviour.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpoNodeBehaviour_h
#define gtpoNodeBehaviour_h

// GTpo headers
#include "./gtpoBehaviour.h"

namespace gtpo { // ::gtpo

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

    using WeakNode          = std::weak_ptr< typename Config::FinalNode >;
    using WeakEdge          = std::weak_ptr< typename Config::FinalEdge >;
    using SharedNode        = std::shared_ptr< typename Config::FinalNode >;
    using SharedNodes       = typename Config::template NodeContainer< SharedNode >;
    using WeakNodesSearch   = typename Config::template SearchContainer< WeakNode >;

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
