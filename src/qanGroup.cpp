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
// This file is a part of the QuickQanava software library.
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
#include "./qanLinear.h"

namespace qan { // ::qan

/* Group Object Management *///------------------------------------------------
Group::Group( QQuickItem* parent ) :
    gtpo::GenGroup< qan::Config >( parent )
{
    setAcceptDrops( true );

    // Force group connected edges update when the group is moved
    connect( this, SIGNAL( xChanged( ) ), this, SLOT( groupMoved( ) ) );
    connect( this, SIGNAL( yChanged( ) ), this, SLOT( groupMoved( ) ) );
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
    return qobject_cast<qan::Graph* >( gtpo::GenGroup< qan::Config >::getGraph() );
}
//-----------------------------------------------------------------------------

/* Group Nodes Management *///-------------------------------------------------
auto    Group::insertNode( qan::Node* node ) -> void
{
    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
        gtpo::GenGroup< qan::Config >::insertNode( weakNode );

        if ( getContainer( ) == nullptr )   // A container must have configured in concrete QML group component
            return;

        // Note 20150907: Set a default position corresponding to the drop position, it will usually be modified
        // if a layout has been configured in this group
        node->setPosition( node->mapToItem( getContainer( ), QPointF( 0., 0. ) ) );
        node->setParentItem( getContainer( ) );
        groupMoved( ); // Force call to groupMoved() to update group adjacent edges

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
    } catch ( std::bad_weak_ptr ) { return; }
      catch ( gtpo::bad_topology_error ) { return; }
}

auto    Group::removeNode( const qan::Node* node ) -> void
{
    if ( node == nullptr )
        return;
    qan::Node* mutableNode = const_cast< qan::Node* >( node );
    WeakNode weakNode;
    try {        
        mutableNode->setParentItem( getGraph() );
        if ( getContainer( ) != nullptr )   // Convert actual node position in group to scene position
            mutableNode->setPosition( getContainer( )->mapToScene( node->position( ) ) ); // Note 20150908: node->mapToScene( 0, 0) don't work on Qt 5.5 beta.
        mutableNode->setDraggable( true );
        mutableNode->setDropable( true );

        weakNode = WeakNode{ const_cast< qan::Node* >( node )->shared_from_this() };
        gtpo::GenGroup< qan::Config >::removeNode( weakNode );
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
    return gtpo::GenGroup< qan::Config >::hasNode( weakNode );
}
//-----------------------------------------------------------------------------

/* Group Appearance Management *///--------------------------------------------
void    Group::setCollapsed( bool collapsed )
{
    if ( collapsed != _collapsed ) {
        _collapsed = collapsed;
        for ( auto weakEdge : getAdjacentEdges() ) {    // When a group is collapsed, all adjacent edges shouldbe hidden/shown...
            qan::Edge* edge = weakEdge.lock().get();
            if ( edge != nullptr )
                edge->setVisible( !collapsed );
        }
        emit collapsedChanged( );
    }
}
//-----------------------------------------------------------------------------

/* Group Behaviour/Layout Management *///--------------------------------------
void    Group::setLayout( qan::Layout* layout )
{
    if ( _layout != nullptr ) {
        // FIXME
        //removeBehaviour( _layout );
        _layout = nullptr;  // gtpo::Behaviourable take care of layout destruction
        emit layoutChanged();
    }
    if ( _layout == nullptr ) {
        addBehaviour( layout );
        //if ( hasLayout( layout ) )
        _layout = layout;
        emit layoutChanged();
    }
}

void    Group::setLinearLayout()
{
    setLayout( new qan::Linear() );
}

void    Group::groupMoved( )
{
    // Group node adjacent edges must be updated manually since node are children of this group,
    // their x an y position does not change and is no longer monitored by their edges.
    for ( auto weakEdge : getAdjacentEdges() ) {
        qan::Edge* edge = weakEdge.lock().get();
        if ( edge != nullptr )
            edge->updateItem();
    }
    if ( getGraph() != nullptr ) {
        WeakGroup weakGroup{ this->shared_from_this( ) };
        getGraph()->notifyGroupModified( weakGroup );
    }
}
//-----------------------------------------------------------------------------

/* Group DnD Management *///---------------------------------------------------
void    Group::proposeNodeDrop( QQuickItem* container, qan::Node* node )
{
    //qDebug( ) << "qan::Group::proposeNodeDrop(): container=" << container << "  node=" << node;
    if ( !getHilightDrag( ) )
        return;

    // If node is already member of the group we are in front of a group "inside" drag and drop,
    // or a group removal via drag and drop, so just remove the node from group and use the normal
    // drag and drop code.
    if( hasNode( node ) )
        removeNode( node ); // removeNode take care or layout() call and node position change...

    // FIXME
//    if ( _layout == nullptr )   // Can't make a drop proposition without a layout configured in the group
//        return;

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
    event->acceptProposedAction( );
    event->accept( );
    emit nodeDragEnter( );
}

void	Group::dragMoveEvent( QDragMoveEvent* event )
{
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
