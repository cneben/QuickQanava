/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the Qanava software.
//
// \file	qanGroup.cpp
// \author	benoit@qanava.org
// \date	2016 03 22
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>
#include <QPainterPath>

// Qanava headers
#include "./qanGroup.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Group Object Management *///------------------------------------------------
Group::Group( QQuickItem* parent ) :
    gtpo::GenGroup< QGraphConfig >( parent )
{
    setAcceptDrops( true );
}

Group::~Group( )
{
    // Force remove all children who hae no cpp ownership
    for ( auto child: childItems() ) {
        if ( QQmlEngine::objectOwnership( child ) != QQmlEngine::CppOwnership )
            child->setParent( nullptr );
    }
}

qan::Graph* Group::getGraph()
{
    return qobject_cast<qan::Graph* >( gtpo::GenGroup< QGraphConfig >::getGraph() );
}
//-----------------------------------------------------------------------------

/* Group Nodes Management *///-------------------------------------------------
auto    Group::insertNode( qan::Node* node ) -> void
{
    qDebug() << "qan::Group::insertNode(): node=" << node;

    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
        gtpo::GenGroup< QGraphConfig >::insertNode( weakNode );

        if ( getContainer( ) == nullptr )   // A container must have configured in concrete QML group component
            return;
        //qDebug( ) << "qan::Group::insertNode(): container found...";

        // Note 20150907: Set a default position corresponding to the drop position, it will usually be modified
        // if a layout has been configured in this group
        node->setPosition( node->mapToItem( getContainer( ), QPointF( 0., 0. ) ) );
        node->setParentItem( getContainer( ) );
        //node->setGroup( this );

        //qDebug( ) << "qan::Group::insertNode(): node configured...";

        // Be carrefull of not inserting _shadowNode which is already inserted...
        if ( getHilightDrag( ) &&
             node != _shadowDropNode &&
             _shadowDropNode != nullptr &&
             hasNode( _shadowDropNode ) )
        {
            // If shadow drop node is configured, then layout has found a specific place for inserted node
            //_nodes.insert( _nodes.indexOf( _shadowDropNode ), node );

            // Remove shadow node...
            //removeNode( _shadowDropNode );
            //_graph->removeNode( _shadowDropNode );
            //delete _shadowDropNode;
            //_shadowDropNode = nullptr;
        }
        //else
            //_nodes.append( node );  // Layout will occurs automatically after node insertion

        //connect( node, SIGNAL( destroyed( QObject* ) ), this, SLOT( nodeDestroyed( QObject* ) ) );  // Automatically check for node suppression

        // FIXME
        //emit _graph->nodeModified( node );  // Update graph observation signals
        //emit _graph->groupModified( this );

    }
    catch ( std::bad_weak_ptr ) { return; }
    catch ( gtpo::bad_topology_error ) { return; }
}

auto    Group::removeNode( const qan::Node* node ) -> void
{
    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ const_cast< qan::Node* >( node )->shared_from_this() };
        gtpo::GenGroup< QGraphConfig >::removeNode( weakNode );
    } catch ( std::bad_weak_ptr ) { return; }
}

bool    Group::hasNode( qan::Node* node ) const
{
    if ( node == nullptr )
        return false;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return false; }
    return gtpo::GenGroup< QGraphConfig >::hasNode( weakNode );
}
//-----------------------------------------------------------------------------

/* Drag'nDrop Management *///--------------------------------------------------
void    Group::proposeNodeDrop( QQuickItem* container, qan::Node* node )
{
    qDebug( ) << "qan::Group::proposeNodeDrop(): container=" << container << "  node=" << node;
    if ( !getHilightDrag( ) )
        return;

    // FIXME
//    if ( _layout == nullptr )   // Can't make a drop proposition without a layout configured in the group
//        return;

    // If node is already member of the group we are in front of a group "inside" drag and drop,
    // or a group removal via drag and drop, so just remove the node from group and use the normal
    // drag and drop code.
//    if( _nodes.contains( node ) )
//        removeNode( node ); // removeNode take care or layout() call and node position change...

    // Configure a shadow drop node if it has still not been inserted
    if ( _shadowDropNode == nullptr )
    {
//        _shadowDropNode = _graph->insertNode( "" );
//        _shadowDropNode->setAcceptDrops( false );   // Note 20160104: Necessary to avoid drops in a "dummy/shadow" node.
        if ( _shadowDropNode != nullptr )
        {
            // Copy node topology (restricted to this group nodes)
            // Note 20150907: created edges will be automatically removed from graph during node destruction

            /*qan::Node::List inNodes; qan::Layout::collectNodeGroupInNodes( _nodes, *node, inNodes );
            foreach ( qan::Node* inNode, inNodes )
                _graph->insertEdge( inNode, _shadowDropNode );
            qan::Node::List outNodes; qan::Layout::collectNodeGroupOutNodes( _nodes, *node, outNodes );
            foreach ( qan::Node* outNode, outNodes )
                _graph->insertEdge( _shadowDropNode, outNode );
            _shadowDropNode->setSize( node->boundingRect( ).size( ) );

            insertNode( _shadowDropNode );  // Insert the shadow node in the group, layout will occurs automatically (and insertNode() will detect that we try to insert a specific shadow node)
            _shadowDropNode->setOpacity( 0.1 );*/
        }
    }

    // Configure node, so that layout could do an adequat proposition
    //if ( _shadowDropNode != nullptr )
    //    _layout->proposeNodeDrop( container, node, _shadowDropNode );
}

void    Group::dragEnterEvent( QDragEnterEvent* event )
{
    //qDebug( ) << "Group::dragEnterEvent() " << event->source( );
    event->acceptProposedAction( );
    event->accept( );
    emit nodeDragEnter( );
}

void	Group::dragMoveEvent( QDragMoveEvent* event )
{
    //qDebug( ) << "Group::dragMoveEvent(): event->source=" << event->source( );
    event->acceptProposedAction( );
    event->accept();

    // Note 20150907: Actually, drag move event is not used except to accept drop move event,
    // because it is impossible to have an access to the node currently beeing dragged (event->target is null...),
    // so a quite costly mechanism does it manually in QanRectNode.qml by calling proposeNodeDrop(), and dropNode()
    // methods in qan::Node which in turn call group drop methods...
}

void	Group::dragLeaveEvent( QDragLeaveEvent* event )
{
    //qDebug( ) << "Group::dragLeaveEvent()";
    event->ignore();
    emit nodeDragLeave( );

    if ( getHilightDrag( ) &&
           _shadowDropNode != nullptr &&
           hasNode( _shadowDropNode ) )
    {
        removeNode( _shadowDropNode );
        getGraph()->removeNode( _shadowDropNode );
    }
}
//-----------------------------------------------------------------------------

} // ::qan
