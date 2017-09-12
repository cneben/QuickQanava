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
