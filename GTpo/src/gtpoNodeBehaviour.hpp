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

