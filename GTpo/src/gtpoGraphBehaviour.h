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
