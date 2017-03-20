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
// \file	gtpoGroupBehaviour.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpoGroupBehaviour_h
#define gtpoGroupBehaviour_h

// GTpo headers
#include "./gtpoBehaviour.h"
#include "./gtpoGenGroup.h"
#include "./gtpoGenNode.h"

namespace gtpo { // ::gtpo

//template < class Config, class ConcreteNode >
//class GenNode;

template < class Config >
class GenEdge;

template < class Config >
class GenGroup;

/*! \brief Define an observer interface to catch changes in a gtpo::GenGroup.
 *
 */
template < class Config >
class GroupBehaviour :  public Behaviour< Config >
{
    /*! \name GroupBehaviour Object Management *///----------------------------
    //@{
public:
    GroupBehaviour() noexcept { }
    virtual ~GroupBehaviour() = default;
    GroupBehaviour( const GroupBehaviour& ) = delete;
    GroupBehaviour& operator=( const GroupBehaviour& ) = delete;

    using WeakNode      = typename GenNode<Config>::Weak;
    using WeakGroup     = std::weak_ptr<GenGroup<Config>>;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Notification Interface *///--------------------------------
    //@{
    //! Called immediatly after \c weakNode has been inserted into target group.
    virtual void    nodeInserted( WeakNode& weakNode ) noexcept { (void)weakNode; }
    //! Called when \c weakNode is about to be removed from target group.
    virtual void    nodeRemoved( WeakNode& weakNode ) noexcept { (void)weakNode; }

    //! Called immediatly after \c weakGroup group has been inserted into target group.
    virtual void    groupInserted( WeakGroup& weakGroup ) noexcept { (void)weakGroup; }
    //! Called when \c weakGroup group is about to be removed from target group.
    virtual void    groupRemoved( WeakGroup& weakGroup ) noexcept { (void)weakGroup; }
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Observation interface for gtpo::GenGroup.
 *
 * \note notifyNodeInserted() and notifyNodeRemoved() are called when a group is
 * inserted into node with gtpo::GenGraph::insertNode(GenGroup, GenNode) or
 * gtpo::GenGraph::removeNode(GenGroup, GenNode), it has nothing to do with
 * adjacent nodes.
 *
 * \nosubgrouping
 */
template < class Behaviour, class SBehaviours >
class BehaviourableGroup : public Behaviourable<Behaviour, SBehaviours>
{
    /*! \name BehaviourableGroup Object Management *///------------------------
    //@{
public:
    BehaviourableGroup() noexcept { }
    ~BehaviourableGroup() { /* Nil */ }
    BehaviourableGroup( const BehaviourableGroup& ) = delete;
    BehaviourableGroup& operator=( const BehaviourableGroup& ) = delete;

    /*! \name Notification Helper Methods *///---------------------------------
    //@{
public:
    inline auto     addGroupBehaviour( std::unique_ptr<Behaviour> behaviour ) -> void {
        Behaviourable<Behaviour, SBehaviours>::addBehaviour(std::move(behaviour));
    }

    //! Notify registered behaviours that a group has been inserted into target group.
    template < class Group >
    auto    notifyGroupInserted( Group& group ) noexcept -> void;

    //! Notify registered behaviours that a group is on the verge of beeing removed from target group.
    template < class Group >
    auto    notifyGroupRemoved( Group& group ) noexcept -> void;

    //! Notify registered behaviours that a node has been inserted into target group.
    template < class Node >
    auto    notifyNodeInserted( Node& node ) noexcept -> void;

    //! Notify registered behaviours that a node is on the verge of beeing removed from target group.
    template < class Node >
    auto    notifyNodeRemoved( Node& node ) noexcept -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoGroupBehaviour.hpp"

#endif // gtpoGroupBehaviour_h
