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
// \file	gtpoNodeBehaviour.hpp
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#include "./gtpoUtils.h"

namespace gtpo { // ::gtpo

/* Notification Helper Methods *///--------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class Node, class Edge >
auto    BehaviourableNode< Behaviour, SBehaviours >::notifyInNodeInserted( Node& node, const Edge& edge ) noexcept -> void
{
    this->notifyBehaviours( &Behaviour::inNodeInserted, node, edge );
    this->sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.inNodeInserted( node, edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node, class Edge >
auto    BehaviourableNode< Behaviour, SBehaviours >::notifyInNodeRemoved( Node& node, const Edge& edge ) noexcept -> void
{
    this->notifyBehaviours( &Behaviour::inNodeRemoved, node, edge );
    this->sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.inNodeRemoved( node, edge ); } );
}

template < class Behaviour, class SBehaviours  >
auto    BehaviourableNode< Behaviour, SBehaviours >::notifyInNodeRemoved() noexcept -> void
{
    this->notifyBehaviours0( &Behaviour::inNodeRemoved );
    this->sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.inNodeRemoved(); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node, class Edge >
auto    BehaviourableNode< Behaviour, SBehaviours >::notifyOutNodeInserted( Node& node, const Edge& edge ) noexcept -> void
{
    this->notifyBehaviours( &Behaviour::outNodeInserted, node, edge );
    this->sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.outNodeInserted( node, edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node, class Edge >
auto    BehaviourableNode< Behaviour, SBehaviours >::notifyOutNodeRemoved( Node& node, const Edge& edge ) noexcept -> void
{
    this->notifyBehaviours( &Behaviour::outNodeRemoved, node, edge );
    this->sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.outNodeRemoved( node, edge ); } );
}

template < class Behaviour, class SBehaviours  >
auto    BehaviourableNode< Behaviour, SBehaviours >::notifyOutNodeRemoved() noexcept -> void
{
    this->notifyBehaviours0( &Behaviour::outNodeRemoved );
    this->sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.outNodeRemoved(); } );
}
//-----------------------------------------------------------------------------

} // ::gtpo

