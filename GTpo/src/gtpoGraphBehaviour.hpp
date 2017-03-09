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
// \file	gtpoGraphBehaviour.hpp
// \author	benoit@destrat.io
// \date	2017 03 09
//-----------------------------------------------------------------------------

#include "./gtpoUtils.h"

namespace gtpo { // ::gtpo

/* Notification Helper Methods *///--------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class Node >
auto    BehaviourableGraph< Behaviour, SBehaviours >::notifyNodeInserted( Node& node ) noexcept -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeInserted, node );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.nodeInserted( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node >
auto    BehaviourableGraph< Behaviour, SBehaviours >::notifyNodeRemoved( Node& node ) noexcept -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeRemoved, node );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.nodeRemoved( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    BehaviourableGraph< Behaviour, SBehaviours >::notifyEdgeInserted( Edge& edge ) noexcept -> void
{
    // FIXME: could be implicit probably...
    notifyBehaviours< Edge >( &Behaviour::edgeInserted, edge );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.edgeInserted( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    BehaviourableGraph< Behaviour, SBehaviours >::notifyEdgeRemoved( Edge& edge ) noexcept -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeRemoved, edge );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.edgeRemoved( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    BehaviourableGraph< Behaviour, SBehaviours >::notifyGroupInserted( Group& group ) noexcept -> void
{
    notifyBehaviours< Group >( &Behaviour::groupInserted, group );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.groupInserted( group ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    BehaviourableGraph< Behaviour, SBehaviours >::notifyGroupRemoved( Group& group ) noexcept -> void
{
    notifyBehaviours< Group >( &Behaviour::groupRemoved, group );
    sNotifyBehaviours( [&](auto& behaviour) noexcept { behaviour.groupRemoved( group ); } );
}
//-----------------------------------------------------------------------------

} // ::gtpo

