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
// This file is a part of the QuickQanava software library.
//
// \file	qanNodeItem.cpp
// \author	benoit@destrat.io
// \date	2016 03 04
//-----------------------------------------------------------------------------

// Std headers
#include <algorithm>    // std::for_each
#include <utility>      // std::as_const

// Qt headers
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanNode.h"
#include "./qanNodeItem.h"
#include "./qanGraph.h"
#include "./qanDraggableCtrl.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
NodeItem::NodeItem(QQuickItem* parent) :
    QQuickItem{parent}
{
    setStyle( qan::Node::style() );
    setObjectName( QStringLiteral("qan::NodeItem") );

    qan::Draggable::configure(this);
    _draggableCtrl = std::unique_ptr<AbstractDraggableCtrl>{std::make_unique<qan::DraggableCtrl>()};
    const auto nodeDraggableCtrl = static_cast<qan::DraggableCtrl*>(_draggableCtrl.get());
    nodeDraggableCtrl->setTargetItem(this);

    setFlag( QQuickItem::ItemAcceptsDrops, true );
    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );

    connect( this, &qan::NodeItem::widthChanged,
             this, &qan::NodeItem::onWidthChanged );
    connect( this, &qan::NodeItem::heightChanged,
             this, &qan::NodeItem::onHeightChanged );
}

NodeItem::~NodeItem()
{
    // Delete all dock items
    for ( auto& dockItem : _dockItems ) {
        if ( dockItem )
            dockItem->deleteLater();
    }

    // Delete all ports
    for (auto& port : _ports) {
        if ( port && port->parent() == nullptr)
            port->deleteLater();
    }
}

qan::AbstractDraggableCtrl& NodeItem::draggableCtrl() { Q_ASSERT(_draggableCtrl!=nullptr); return *_draggableCtrl; }
//-----------------------------------------------------------------------------

/* Topology Management *///----------------------------------------------------
auto    NodeItem::getNode() noexcept -> qan::Node* { return _node.data(); }
auto    NodeItem::getNode() const noexcept -> const qan::Node* { return _node.data(); }
auto    NodeItem::setNode(qan::Node* node) noexcept -> void {
    _node = node;
    const auto nodeDraggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    nodeDraggableCtrl->setTarget(node);
}

auto    NodeItem::setGraph(qan::Graph* graph) noexcept -> void {
    _graph = graph;
    qan::Selectable::configure( this, graph );
}
auto    NodeItem::getGraph() const noexcept -> const qan::Graph* { return _graph.data(); }
auto    NodeItem::getGraph() noexcept -> qan::Graph* { return _graph.data(); }

auto    NodeItem::setRect(const QRectF& r) noexcept -> void
{
    // PRECONDITIONS:
        // r rect must be valid
    if (!r.isValid())
        return;
    setX(r.left());
    setY(r.top());
    setWidth(r.width());
    setHeight(r.height());
}
//-----------------------------------------------------------------------------

/* Selection Management *///---------------------------------------------------
void    NodeItem::onWidthChanged()
{
    configureSelectionItem();
    if ( _complexBoundingShape )            // Invalidate actual bounding shape
        emit requestUpdateBoundingShape();
    else setDefaultBoundingShape();
}

void    NodeItem::onHeightChanged()
{
    configureSelectionItem();
    if ( _complexBoundingShape )            // Invalidate actual bounding shape
        emit requestUpdateBoundingShape();
    else setDefaultBoundingShape();
}
//-----------------------------------------------------------------------------

/* Node Configuration *///-----------------------------------------------------
void    NodeItem::setResizable( bool resizable ) noexcept
{
    if ( resizable != _resizable ) {
        _resizable = resizable;
        emit resizableChanged();
    }
}

void    NodeItem::setRatio(qreal ratio) noexcept
{
    _ratio = ratio;
    emit ratioChanged();
}

void    NodeItem::setConnectable( Connectable connectable ) noexcept
{
    if ( _connectable != connectable ) {
        _connectable = connectable;
        emit connectableChanged();
    }
}
//-----------------------------------------------------------------------------

/* Draggable Management *///---------------------------------------------------
void    NodeItem::dragEnterEvent( QDragEnterEvent* event )
{
    if (getNode() != nullptr &&
        getNode()->getLocked()) {
        event->setAccepted(false);
        QQuickItem::dragEnterEvent(event);
        return;
    }
    const auto draggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    if ( !draggableCtrl->handleDragEnterEvent(event) )
        event->ignore();
    QQuickItem::dragEnterEvent(event);
}

void	NodeItem::dragMoveEvent( QDragMoveEvent* event )
{
    if (getNode() != nullptr &&
        getNode()->getLocked()) {
        event->setAccepted(false);
        QQuickItem::dragMoveEvent(event);
        return;
    }
    const auto draggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    draggableCtrl->handleDragMoveEvent(event);
    QQuickItem::dragMoveEvent(event);
}

void	NodeItem::dragLeaveEvent( QDragLeaveEvent* event )
{
    if (getNode() != nullptr &&
        getNode()->getLocked()) {
        event->setAccepted(false);
        QQuickItem::dragLeaveEvent(event);
        return;
    }
    const auto draggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    draggableCtrl->handleDragLeaveEvent(event);
    QQuickItem::dragLeaveEvent(event);
}

void    NodeItem::dropEvent( QDropEvent* event )
{
    const auto draggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    draggableCtrl->handleDropEvent(event);
    QQuickItem::dropEvent( event );
}

void    NodeItem::mouseDoubleClickEvent(QMouseEvent* event )
{
    const auto draggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    draggableCtrl->handleMouseDoubleClickEvent(event);
    if ( event->button() == Qt::LeftButton )
        emit nodeDoubleClicked( this, event->localPos() );
}

void    NodeItem::mouseMoveEvent(QMouseEvent* event )
{
    if (getNode() != nullptr &&
        getNode()->getLocked()) {
        QQuickItem::mouseMoveEvent(event);
        return;
    }
    const auto draggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    if ( draggableCtrl->handleMouseMoveEvent(event) )
        event->accept();
    else
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
             isSelectable() &&
             !getNode()->getLocked() ) {
            if ( _graph )
                _graph->selectNode( *getNode(), event->modifiers() );
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
    const auto draggableCtrl = static_cast<DraggableCtrl*>(_draggableCtrl.get());
    draggableCtrl->handleMouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------


/* Style Management *///-------------------------------------------------------
void    NodeItem::setStyle( qan::NodeStyle* style ) noexcept
{
    if ( style != _style ) {
        if ( _style )  // Every style that is non default is disconnect from this node
            QObject::disconnect( _style, Q_NULLPTR, this, Q_NULLPTR );
        _style = style;
        if ( _style )
            connect( _style,    &QObject::destroyed,    // Monitor eventual style destruction
                     this,      &NodeItem::styleDestroyed );
        emit styleChanged( );
    }
}

void    NodeItem::setItemStyle( qan::Style* style ) noexcept
{
    auto nodeStyle = qobject_cast<qan::NodeStyle*>(style);
    if ( nodeStyle != nullptr )
        setStyle( nodeStyle );
}

void    NodeItem::styleDestroyed( QObject* style )
{
    if ( style != nullptr )
        setStyle( nullptr );   // Set default style when current style is destroyed
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

/* Port/Dock Management *///---------------------------------------------------
qan::PortItem*  NodeItem::findPort(const QString& portId) const noexcept
{
    for ( const auto port : qAsConst(_ports) ) {   // Note: std::as_const is officially c++17
        const auto portItem = qobject_cast<qan::PortItem*>(port);
        if ( portItem &&
             portItem->getId() == portId )
            return portItem;
    }
    return nullptr;
}

void    NodeItem::setLeftDock( QQuickItem* leftDock ) noexcept
{
    if ( leftDock != _dockItems[static_cast<std::size_t>(Dock::Left)].data() ) {
        if ( leftDock != nullptr ) {
            configureDock(*leftDock, Dock::Left);
            QQmlEngine::setObjectOwnership(leftDock, QQmlEngine::CppOwnership);
        }
        _dockItems[static_cast<std::size_t>(Dock::Left)] = leftDock;
        emit leftDockChanged();
    }
}

void    NodeItem::setTopDock( QQuickItem* topDock ) noexcept
{
    if ( topDock != _dockItems[static_cast<std::size_t>(Dock::Top)].data() ) {
        if ( topDock != nullptr ) {
            configureDock(*topDock, Dock::Top);
            QQmlEngine::setObjectOwnership(topDock, QQmlEngine::CppOwnership);
        }
        _dockItems[static_cast<std::size_t>(Dock::Top)] = topDock;
        emit topDockChanged();
    }
}

void    NodeItem::setRightDock( QQuickItem* rightDock ) noexcept
{
    if ( rightDock != _dockItems[static_cast<std::size_t>(Dock::Right)].data() ) {
        if ( rightDock != nullptr ) {
            configureDock(*rightDock, Dock::Right);
            QQmlEngine::setObjectOwnership(rightDock, QQmlEngine::CppOwnership);
        }
        _dockItems[static_cast<std::size_t>(Dock::Right)] = rightDock;
        emit rightDockChanged();
    }
}

void    NodeItem::setBottomDock( QQuickItem* bottomDock ) noexcept
{
    if ( bottomDock != _dockItems[static_cast<std::size_t>(Dock::Bottom)].data() ) {
        if ( bottomDock != nullptr ) {
            configureDock(*bottomDock, Dock::Bottom);
            QQmlEngine::setObjectOwnership(bottomDock, QQmlEngine::CppOwnership);
        }
        _dockItems[static_cast<std::size_t>(Dock::Bottom)] = bottomDock;
        emit bottomDockChanged();
    }
}

void    NodeItem::setDock(Dock dock, QQuickItem* dockItem) noexcept
{
    if ( dockItem != nullptr )
        configureDock(*dockItem, dock);
    switch ( dock ) {
        case Dock::Left: setLeftDock(dockItem); break;
        case Dock::Top: setTopDock(dockItem); break;
        case Dock::Right: setRightDock(dockItem); break;
        case Dock::Bottom: setBottomDock(dockItem); break;
    };
}

void    NodeItem::configureDock(QQuickItem& dockItem, const Dock dock) noexcept
{
    dockItem.setParentItem(this);
    dockItem.setProperty("hostNodeItem",
                         QVariant::fromValue(this));
    dockItem.setProperty("dockType",
                         QVariant::fromValue(dock));
}
//-----------------------------------------------------------------------------

} // ::qan
