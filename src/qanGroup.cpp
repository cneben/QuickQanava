/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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
// \author	benoit@destrat.io
// \date	2016 03 22
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanGroup.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Group Object Management *///------------------------------------------------
Group::Group( QObject* parent ) :
    gtpo::GenGroup< qan::GraphConfig >{}
{
}

Group::~Group()
{
    // Force remove all children who hae no cpp ownership
    // FIXME QAN3
    /*for ( auto child: childItems() ) {
        if ( QQmlEngine::objectOwnership( child ) != QQmlEngine::CppOwnership )
            child->setParent( nullptr );
    }*/
}
//-----------------------------------------------------------------------------

/* Group Nodes Management *///-------------------------------------------------
auto    Group::insertNode( WeakNode weakNode ) -> void
{
    SharedNode node = weakNode.lock();
    if ( node != nullptr ) {
        qan::Node* qanNode = qobject_cast< qan::Node* >( node.get() );
        if ( qanNode != nullptr )
            insertNode( qanNode, false );  // If the node is a QuickQanava qan::Node, insert it "graphically" before adding it to graph topology.
        else    // Should not happen
            gtpo::GenGroup< qan::GraphConfig >::insertNode( weakNode );
    }
}

auto    Group::insertNode( qan::Node* node, bool drop ) -> void
{
    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
        gtpo::GenGroup< qan::GraphConfig >::insertNode( weakNode );

        // FIXME QAN3
        /*if ( getContainer( ) == nullptr )   // A container must have configured in concrete QML group component
            return;

        // Note 20150907: Set a default position corresponding to the drop position, it will usually be modified
        // if a layout has been configured in this group
        if ( drop ) // Do not map position if the insertion didn't result from a drag and drop action (for example when inserting a node for in serialization)
            node->setPosition( node->mapToItem( getContainer(), QPointF{ 0., 0. } ) );
        node->setParentItem( getContainer() );
        groupMoved(); // Force call to groupMoved() to update group adjacent edges
        */
        endProposeNodeDrop();
    } catch ( std::bad_weak_ptr ) { return; }
      catch ( gtpo::bad_topology_error ) { return; }
}

auto    Group::removeNode( const qan::Node* node ) -> void
{
    // FIXME QAN3
/*    if ( node == nullptr )
        return;
    qan::Node* mutableNode = const_cast< qan::Node* >( node );
    WeakNode weakNode;
    try {
        QPointF nodePos{ node->position() };
        mutableNode->setParentItem( getGraph()->getContainerItem() );
        mutableNode->setPosition( nodePos + position() );
        mutableNode->setDraggable( true );
        mutableNode->setDropable( true );

        weakNode = WeakNode{ const_cast< qan::Node* >( node )->shared_from_this() };
        gtpo::GenGroup< qan::GraphConfig >::removeNode( weakNode );
    } catch ( std::bad_weak_ptr ) { return; }
    */
}

bool    Group::hasNode( qan::Node* node ) const
{
    if ( node == nullptr )
        return false;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return false; }
    return gtpo::GenGroup< qan::GraphConfig >::hasNode( weakNode );
}
//-----------------------------------------------------------------------------

/* Group Appearance Management *///--------------------------------------------
void    Group::setCollapsed( bool collapsed )
{
    if ( collapsed != _collapsed ) {
        _collapsed = collapsed;
        for ( auto weakEdge : getAdjacentEdges() ) {    // When a group is collapsed, all adjacent edges shouldbe hidden/shown...
            qan::Edge* edge = weakEdge.lock().get();
            // FIXME QAN3
            /*
            if ( edge != nullptr )
                edge->setVisible( !collapsed );
            */
        }
        emit collapsedChanged( );
    }
}
//-----------------------------------------------------------------------------

/* Group Behaviour/Layout Management *///--------------------------------------
/*void    Group::setLayout( qan::Layout* layout )
{
    if ( _layout != nullptr ) {
        // FIXME
        //removeBehaviour( _layout );
        _layout = nullptr;  // gtpo::Behaviourable take care of layout destruction
        emit layoutChanged();
    }
    if ( _layout == nullptr ) {
        //addBehaviour( layout );       // FIXME: 20160714 removed because of g++5.2 compilation problems, reactivate group layout later...
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
*/
//-----------------------------------------------------------------------------

/* Group DnD Management *///---------------------------------------------------
void    Group::proposeNodeDrop( QQuickItem* container, qan::Node* node )
{
    Q_UNUSED( container );
    Q_UNUSED( node);

    emit nodeDragEnter( );
    if ( !getHilightDrag( ) )
        return;

    // FIXME 20160710: Reactivate this code with layouts
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

void    Group::endProposeNodeDrop()
{
    /*
    emit nodeDragLeave( );
    if ( getHilightDrag( ) &&
         _shadowDropNode != nullptr &&
         hasNode( _shadowDropNode ) )
    {
        removeNode( _shadowDropNode );
        getGraph()->removeNode( _shadowDropNode );
    }
    */
}
//-----------------------------------------------------------------------------

} // ::qan
