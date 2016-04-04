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
// \file	qanNode.cpp
// \author	benoit@qanava.org
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>
#include <QPainterPath>

// Qanava headers
#include "./qanNode.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
Node::Node( QQuickItem* parent ) :
    gtpo::GenNode< qan::Config >( parent ),
    _defaultStyle{ new qan::NodeStyle{ "", "qan::Node" } },
    _style{ nullptr }
{
    setAcceptDrops( true );
    setStyle( _defaultStyle.data() );
}

Node::~Node( )
{
/*    for ( auto child: childItems() ) {
        if ( QQmlEngine::objectOwnership( child ) != QQmlEngine::CppOwnership ) {
            child->setParent( nullptr );
        }
    }*/
}

qan::Graph* Node::getGraph()
{
    return qobject_cast< qan::Graph* >( gtpo::GenNode< qan::Config >::getGraph() );
}
//-----------------------------------------------------------------------------

void    Node::installBehaviour( qan::NodeBehaviour* behaviour )
{
    addBehaviour( behaviour );
}

/* Content Management *///-----------------------------------------------------
void    Node::setStyle( NodeStyle* style )
{
    if ( style == _style )
        return;
    if ( style == nullptr )
        style = _defaultStyle.data();
    if ( _style != nullptr && _style != _defaultStyle.data() )  // Every style that is non default is disconnect from this node
        QObject::disconnect( _style, 0, this, 0 );
    _style = style;
    connect( _style, &QObject::destroyed, this, &Node::styleDestroyed );    // Monitor eventual style destruction
    emit styleChanged( );

    // FIXME 20160209
    //emitNodeModified( );
}

void    Node::styleDestroyed( QObject* style )
{
    if ( style != nullptr && style != _defaultStyle.data() )
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
}
//-----------------------------------------------------------------------------


/* Intersection Shape Management *///------------------------------------------
void    Node::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry )
{
    QQuickItem::geometryChanged( newGeometry, oldGeometry );
    if ( _boundingShape.isEmpty( ) )
        generateDefaultBoundingShape( );
}

QPolygonF   Node::getBoundingShape( )
{
    if ( _boundingShape.isEmpty( ) )
        return generateDefaultBoundingShape( );
    return _boundingShape;
}

const QPolygonF Node::getBoundingShape( ) const
{
    if ( _boundingShape.isEmpty( ) )
        return generateDefaultBoundingShape( );
    return _boundingShape;
}

QPolygonF    Node::generateDefaultBoundingShape( ) const
{
    // Generate a rounded rectangular intersection shape for this node rect new geometry
    QPainterPath path;
    qreal shapeRadius( 5. );
    path.addRoundedRect( QRectF( QPointF( 0., 0. ), QSizeF( width( ), height( ) ) ),
                         shapeRadius, shapeRadius );
    return path.toFillPolygon( QTransform( ) );
}

void    Node::setBoundingShape( QVariantList boundingShape )
{
    QPolygonF shape;
    foreach ( QVariant vp, boundingShape )
        shape.append( vp.toPointF( ) );
    if ( !shape.isEmpty( ) )
        _boundingShape = shape;
}
bool    Node::isInsideBoundingShape( QPointF p )
{
    return _boundingShape.containsPoint( p, Qt::OddEvenFill );
}
//-----------------------------------------------------------------------------

/* Node Group Management *///--------------------------------------------------
void    Node::dropNode( QQuickItem* target )
{
    // FIXME
//    emitNodeModified( );    // dropNode() is called everytime the node has been dragged.
    if ( target == nullptr )
        return;

    qan::Group* group = qobject_cast< qan::Group* >( target );
    if ( group != nullptr && getGraph()->hasGroup( group ) )
        group->insertNode( this );
}

void    Node::proposeNodeDrop( QQuickItem* target )
{
    // FIXME
    //    emitNodeModified( );    // proposeNodeDrop() is called everytime the node has been dragged.
    if ( target == nullptr )
        return;

    qan::Group* group = qobject_cast< qan::Group* >( target );
    if ( group != nullptr && getGraph()->hasGroup( group ) )
        group->proposeNodeDrop( group->getContainer( ), this );
}

void    Node::ungroup( )
{
    if ( getQanGroup() != nullptr && getGraph()->hasGroup( getQanGroup() ) )
        getQanGroup()->removeNode( this );
    // FIXME
    //    emitNodeModified( );
}

qan::Group* Node::getQanGroup( )
{
    WeakGroup weakGroup = gtpo::GenNode<qan::Config>::getGroup();
    if ( weakGroup.expired() )
        return nullptr;
    return weakGroup.lock().get();
}
//-----------------------------------------------------------------------------

/* Drag'nDrop Management *///--------------------------------------------------
void    Node::dragEnterEvent( QDragEnterEvent* event )
{
    if ( getAcceptDrops() ) {
        if ( event->source() == nullptr ) {
            event->accept(); // This is propably a drag initated with type=Drag.Internal, for exemple a connector drop node, accept by default...
            QQuickItem::dragEnterEvent( event );
            return;
        }

        if ( event->source() != nullptr ) { // Get the source item from the quick drag attached object received
            QVariant source = event->source()->property( "source" );
            if ( source.isValid() ) {
                QQuickItem* sourceItem = source.value< QQuickItem* >( );
                QVariant draggedStyle = sourceItem->property( "draggedNodeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
                if ( draggedStyle.isValid() ) {
                    event->accept();
                    return;
                }
            }
        }
        event->ignore();
        QQuickItem::dragEnterEvent( event );
    }
}

void	Node::dragMoveEvent( QDragMoveEvent* event )
{
    if ( getAcceptDrops() ) {
        event->acceptProposedAction();
        event->accept();
    }
    QQuickItem::dragMoveEvent( event );
}

void	Node::dragLeaveEvent( QDragLeaveEvent* event )
{
    if ( getAcceptDrops() )
        event->ignore();
    QQuickItem::dragLeaveEvent( event );
}

void    Node::dropEvent( QDropEvent* event )
{
    if ( getAcceptDrops() && event->source() != nullptr ) { // Get the source item from the quick drag attached object received
        QVariant source = event->source()->property( "source" );
        if ( source.isValid() ) {
            QQuickItem* sourceItem = source.value< QQuickItem* >( );
            QVariant draggedStyle = sourceItem->property( "draggedNodeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
            if ( draggedStyle.isValid() ) {
                qan::NodeStyle* draggedNodeStyle = draggedStyle.value< qan::NodeStyle* >( );
                if ( draggedNodeStyle != nullptr )
                    setStyle( draggedNodeStyle );
            }
        }
    }
    QQuickItem::dropEvent( event );
}
//-----------------------------------------------------------------------------

} // ::qan
