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
// \file	gtpoBehaviour.hpp
// \author	benoit@qanava.org
// \date	2016 02 08
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* Virtual Behaviours Management *///------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class T >
auto    Behaviourable< Behaviour, SBehaviours >::notifyBehaviours( void (Behaviour::*method)(T&), T& arg ) -> void
{
    // Note 20160314: See http://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
    // For calling pointer on memg++ template template parameter template keywordber functions.
    // std::unique_ptr has no overload for .* or ->* operator
    // (*behaviour) == (*behaviour.get())
    // Equivalent to: ((*behaviour.get()).*method)(arg)
    for ( auto& behaviour : _behaviours )
        if ( behaviour != nullptr )
            ((*behaviour).*method)(arg);
}
//-----------------------------------------------------------------------------


/* Notification Helper Methods *///--------------------------------------------
template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeInserted( Node& node ) -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeInserted, node );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.nodeInserted( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeRemoved( Node& node ) -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeRemoved, node );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.nodeRemoved( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Node >
auto    Behaviourable< Behaviour, SBehaviours >::notifyNodeModified( Node& node ) -> void
{
    notifyBehaviours< Node >( &Behaviour::nodeModified, node );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.nodeModified( node ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeInserted( Edge& edge ) -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeInserted, edge );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.edgeInserted( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeRemoved( Edge& edge ) -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeRemoved, edge );
    sNotifyBehaviours( [&](auto behaviour) { behaviour.edgeRemoved( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Edge >
auto    Behaviourable< Behaviour, SBehaviours >::notifyEdgeModified( Edge& edge ) -> void
{
    notifyBehaviours< Edge >( &Behaviour::edgeModified, edge );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.edgeModified( edge ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupInserted( Group& group ) -> void
{
    notifyBehaviours< Group >( &Behaviour::groupInserted, group );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.groupInserted( group ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupRemoved( Group& group ) -> void
{
    notifyBehaviours< Group >( &Behaviour::groupRemoved, group );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.groupRemoved( group ); } );
}

template < class Behaviour, class SBehaviours  >
template < class Group >
auto    Behaviourable< Behaviour, SBehaviours >::notifyGroupModified( Group& group ) -> void
{
    notifyBehaviours< Group >( &Behaviour::groupModified, group );
    sNotifyBehaviours( [&](auto& behaviour) { behaviour.groupModified( group ); } );
}
//-----------------------------------------------------------------------------


/* Group Edge Set Behaviour *///-------------------------------------------
template < class Config >
auto GroupEdgeSetBehaviour< Config >::nodeInserted( WeakNode& node ) -> void
{
    if ( !node.expired() ) {
        SharedNode sharedNode = node.lock();
        if ( sharedNode != nullptr ) {
            /*WeakGroup group = node.getGroup();
            // Add all node edges to adjacent edge set
            for ( auto inEdge : sharedNode.getInEdges() )
                group.addAdjacentEdge( group.getAdjacentEdgeSet(), inEdge );*/
        }
    }
}

template < class Config >
auto GroupEdgeSetBehaviour< Config >::nodeRemoved( WeakNode& node ) -> void
{
}
//-----------------------------------------------------------------------------

} // ::gtpo

