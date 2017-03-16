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
// \file	qanNodeItem.cpp
// \author	benoit@destrat.io
// \date	2016 03 04
//-----------------------------------------------------------------------------

// Qt headers
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanNode.h"
#include "./qanNodeItem.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
NodeItem::NodeItem(QQuickItem* parent) :
    QQuickItem{parent},
    _defaultStyle{ new qan::NodeStyle{ "", QStringLiteral("qan::Node") } },
    _style{ nullptr }
{
    qan::Draggable::configure(this);
    _draggableCtrl.setTargetItem(this);

    setFlag( QQuickItem::ItemAcceptsDrops, true );
    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );

    setStyle( _defaultStyle.data() );

    connect( this, &qan::NodeItem::widthChanged,
             this, &qan::NodeItem::onWidthChanged );
    connect( this, &qan::NodeItem::heightChanged,
             this, &qan::NodeItem::onHeightChanged );
}

NodeItem::~NodeItem() { /* Nil */ }

auto    NodeItem::getNode() noexcept -> qan::Node* { return _node.data(); }
auto    NodeItem::getNode() const noexcept -> const qan::Node* { return _node.data(); }
auto    NodeItem::setNode(qan::Node* node) noexcept -> void {
    _node = node;
    _draggableCtrl.setTarget(node);
}

auto    NodeItem::setGraph(qan::Graph* graph) noexcept -> void {
    _graph = graph;
    qan::Selectable::configure( this, graph );
}
auto    NodeItem::getGraph() const noexcept -> const qan::Graph* { return _graph.data(); }
auto    NodeItem::getGraph() noexcept -> qan::Graph* { return _graph.data(); }
//-----------------------------------------------------------------------------

/* Selection Management *///---------------------------------------------------
void    NodeItem::onWidthChanged()
{
    qan::Selectable::updateSelectionWidth();
    if ( _complexBoundingShape )            // Invalidate actual bounding shape
        emit requestUpdateBoundingShape();
    else setDefaultBoundingShape();
}

void    NodeItem::onHeightChanged()
{
    qan::Selectable::updateSelectionHeight();
    if ( _complexBoundingShape )            // Invalidate actual bounding shape
        emit requestUpdateBoundingShape();
    else setDefaultBoundingShape();
}
//-----------------------------------------------------------------------------

/* Drag'nDrop Management *///--------------------------------------------------
void    NodeItem::setResizable( bool resizable ) noexcept
{
    if ( resizable != _resizable ) {
        _resizable = resizable;
        emit resizableChanged();
    }
}

void    NodeItem::dragEnterEvent( QDragEnterEvent* event )
{
    if ( ! _draggableCtrl.handleDragEnterEvent(event) )
        event->ignore();
    QQuickItem::dragEnterEvent( event );
}

void	NodeItem::dragMoveEvent( QDragMoveEvent* event )
{
    _draggableCtrl.handleDragMoveEvent(event);
    QQuickItem::dragMoveEvent( event );
}

void	NodeItem::dragLeaveEvent( QDragLeaveEvent* event )
{
    _draggableCtrl.handleDragLeaveEvent(event);
    QQuickItem::dragLeaveEvent( event );
}

void    NodeItem::dropEvent( QDropEvent* event )
{
    _draggableCtrl.handleDropEvent(event);
    QQuickItem::dropEvent( event );
}

void    NodeItem::mouseDoubleClickEvent(QMouseEvent* event )
{
    _draggableCtrl.handleMouseDoubleClickEvent(event);

    if ( event->button() == Qt::LeftButton )
        emit nodeDoubleClicked( this, event->localPos() );
}

void    NodeItem::mouseMoveEvent(QMouseEvent* event )
{
    _draggableCtrl.handleMouseMoveEvent(event);
    QQuickItem::mouseMoveEvent(event);
}

void    NodeItem::mousePressEvent( QMouseEvent* event )
{
    bool accepted = isInsideBoundingShape( event->localPos() );
    if ( accepted ) {
        forceActiveFocus();

        // Selection management
        if ( event->button() == Qt::LeftButton &&
             getNode() &&
             getNode()->getItem() != nullptr &&
             getNode()->getItem()->isSelectable() ) {
            const auto graph = getGraph();
            if ( graph != nullptr )
                graph->selectNode( *getNode(), event->modifiers() );
        }

        // QML notifications
        if ( event->button() == Qt::LeftButton )
            emit nodeClicked( this, event->localPos() );
        else if ( event->button() == Qt::RightButton )
            emit nodeRightClicked( this, event->localPos() );
        event->accept();
    } else
        event->ignore();
    // Note 20160712: Do not call base QQuickItem implementation.
}

void    NodeItem::mouseReleaseEvent( QMouseEvent* event )
{
    _draggableCtrl.handleMouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------

/* Appearance Management *///--------------------------------------------------
void    NodeItem::setStyle( NodeStyle* style )
{
    if ( style == _style )
        return;
    if ( style == nullptr )
        style = _defaultStyle.data();
    if ( _style != nullptr &&
         _style != _defaultStyle.data() )  // Every style that is non default is disconnect from this node
        QObject::disconnect( _style, 0, this, 0 );
    _style = style;
    connect( _style, &QObject::destroyed, this, &NodeItem::styleDestroyed );    // Monitor eventual style destruction
    emit styleChanged( );
}

void    NodeItem::styleDestroyed( QObject* style )
{
    if ( style != nullptr &&
         style != _defaultStyle.data() )
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
}
//-----------------------------------------------------------------------------


/* Intersection Shape Management *///------------------------------------------
void    NodeItem::setComplexBoundingShape( bool complexBoundingShape ) noexcept
{
    if ( complexBoundingShape != _complexBoundingShape ) {
        _complexBoundingShape = complexBoundingShape;
        if ( !complexBoundingShape )
            setDefaultBoundingShape();
        else
            emit requestUpdateBoundingShape();
        emit complexBoundingShapeChanged();
    }
}

QPolygonF   NodeItem::getBoundingShape() noexcept
{
    if ( _boundingShape.isEmpty( ) )
        _boundingShape = generateDefaultBoundingShape( );
    return _boundingShape;
}

QPolygonF    NodeItem::generateDefaultBoundingShape( ) const
{
    // Generate a rounded rectangular intersection shape for this node rect new geometry
    QPainterPath path;
    qreal shapeRadius( 5. );
    path.addRoundedRect( QRectF{ 0., 0., width(), height() }, shapeRadius, shapeRadius );
    return path.toFillPolygon( QTransform{} );
}

void    NodeItem::setDefaultBoundingShape( )
{
    setBoundingShape( generateDefaultBoundingShape() );
}

void    NodeItem::setBoundingShape( QVariantList boundingShape )
{
    QPolygonF shape; shape.resize( boundingShape.size() );
    int p = 0;
    for ( const auto& vp : boundingShape )
        shape[p++] = vp.toPointF( );
    _boundingShape = ( !shape.isEmpty( ) ? shape : generateDefaultBoundingShape() );
    emit boundingShapeChanged();
}

bool    NodeItem::isInsideBoundingShape( QPointF p )
{
    if ( _boundingShape.isEmpty() )
        setBoundingShape( generateDefaultBoundingShape() );
    return _boundingShape.containsPoint( p, Qt::OddEvenFill );
}
//-----------------------------------------------------------------------------

} // ::qan
