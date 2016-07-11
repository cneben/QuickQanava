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
#include <QMimeData>

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
    setResizable( true );

    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );
}

Node::~Node( ) { }

qan::Graph* Node::getGraph()
{
    return qobject_cast< qan::Graph* >( gtpo::GenNode< qan::Config >::getGraph() );
}

bool    Node::operator==( const qan::Node& right ) const
{
    return getLabel() == right.getLabel();
}
//-----------------------------------------------------------------------------

/* Behaviours Management *///--------------------------------------------------
auto    Node::installBehaviour( qan::NodeBehaviour* behaviour ) -> void
{
    addBehaviour( behaviour );
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

void    Node::mouseDoubleClickEvent(QMouseEvent* event )
{
    emit nodeDoubleClicked( this, event->localPos() );
}

void    Node::mouseMoveEvent(QMouseEvent* event )
{
    if ( getDraggable() &&
         event->buttons() |  Qt::LeftButton &&
         getDragActive() ) {

        if ( getQanGroup() != nullptr ) {
            getQanGroup()->removeNode( this );
            _dragInitialMousePos = event->windowPos();  // Note 20160811: Resetting position cache since the node has changed parent and
                                                        // thus position (same scene pos but different local pos)
            _dragInitialPos = parentItem() != nullptr ? parentItem()->mapToScene( position() ) : position();
        }

        // Inspired by void QQuickMouseArea::mouseMoveEvent(QMouseEvent *event)
        // https://code.woboq.org/qt5/qtdeclarative/src/quick/items/qquickmousearea.cpp.html#47curLocalPos
        // Coordinate mapping in qt quick is even more a nightmare than with graphics view...
        // BTW, this code is probably buggy for deep quick item hierarchy.
        QPointF startLocalPos;
        QPointF curLocalPos;
        if ( parentItem() != nullptr ) {
            startLocalPos = parentItem()->mapFromScene( _dragInitialMousePos );
            curLocalPos = parentItem()->mapFromScene( event->windowPos() );
        } else {
            startLocalPos = _dragInitialMousePos;
            curLocalPos = event->windowPos();
        }
        QPointF startPos = parentItem() != nullptr ? parentItem()->mapFromScene( _dragInitialPos ) : _dragInitialPos;
        setX( startPos.x() + curLocalPos.x() - startLocalPos.x() );
        setY( startPos.y() + curLocalPos.y() - startLocalPos.y() );

        qan::Group* group = getGraph()->groupAt( position(), { width(), height() } );
        if ( group != nullptr &&
             getDropable() ) {
                group->proposeNodeDrop( group->getContainer(), this );
                _lastProposedGroup = group;
        }
        if ( group == nullptr && _lastProposedGroup != nullptr ) {
            _lastProposedGroup->endProposeNodeDrop();
            _lastProposedGroup = nullptr;
        }
    }
}

void    Node::mousePressEvent(QMouseEvent* event )
{
//    bool accepted = isInsideBoundingShape( event->localPos() );
//    if ( accepted ) {
        if ( event->button() == Qt::LeftButton )
            emit nodeClicked( this, event->localPos() );
        else if ( event->button() == Qt::RightButton )
            emit nodeRightClicked( this, event->localPos() );

        if ( getDraggable() &&
             event->button() == Qt::LeftButton ) {
            setDragActive( true );
            _dragInitialMousePos = event->windowPos();
            _dragInitialPos = parentItem() != nullptr ? parentItem()->mapToScene( position() ) : position();
        }
 //   }
}

void    Node::mouseReleaseEvent(QMouseEvent* event )
{
    if ( getDropable() ) {
        qan::Group* group = getGraph()->groupAt( position(), { width(), height() } );
        if ( group != nullptr )
            group->insertNode( this );
    }

    setDragActive( false );
    _dragInitialMousePos = { 0., 0. }; // Invalid all cached coordinates when button is released
    _dragInitialPos = { 0., 0. };
    _lastProposedGroup = nullptr;
}

//-----------------------------------------------------------------------------

/* Appearance Management *///--------------------------------------------------
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
void    Node::ungroup( )
{
    if ( getQanGroup() != nullptr && getGraph()->hasGroup( getQanGroup() ) )
        getQanGroup()->removeNode( this );
}

qan::Group* Node::getQanGroup( )
{
    WeakGroup weakGroup = gtpo::GenNode<qan::Config>::getGroup();
    if ( weakGroup.expired() )
        return nullptr;
    return weakGroup.lock().get();
}
//-----------------------------------------------------------------------------

} // ::qan
