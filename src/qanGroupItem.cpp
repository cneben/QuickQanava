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

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanGroupItem.h"
#include "./qanGroup.h"

namespace qan { // ::qan

/* Group Object Management *///------------------------------------------------
GroupItem::GroupItem( QQuickItem* parent ) :
    QQuickItem{ parent }
{
    qan::Draggable::configure(this);
    qan::Draggable::setAcceptDrops(true);

    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );
    _controller.setTargetItem(this);
    //setSelectable(false);

    // Force group connected edges update when the group is moved
    connect( this, &qan::GroupItem::xChanged,
             this, &qan::GroupItem::groupMoved );
    connect( this, &qan::GroupItem::yChanged,
             this, &qan::GroupItem::groupMoved );
}

GroupItem::~GroupItem() { /* Nil */ }

auto    GroupItem::getGroup() noexcept -> qan::Group* { return _group.data(); }
auto    GroupItem::getGroup() const noexcept -> const qan::Group* { return _group.data(); }
auto    GroupItem::setGroup(qan::Group* group) noexcept -> void {
    _group = group;
    if ( group != nullptr ) {
        if ( group->getGraph() != nullptr ) {
            qan::Selectable::configure(this, group->getGraph());
        } else qWarning() << "qan::GroupItem::setGroup(): Warning: Setting a group that has no valid reference to qan::Graph.";
    }
    _controller.setTarget(group);
}

auto    GroupItem::getGraph() const noexcept -> const qan::Graph* { return _group ? _group->getGraph() : nullptr; }
auto    GroupItem::getGraph() noexcept -> qan::Graph* { return _group ? _group->getGraph() : nullptr; }
//-----------------------------------------------------------------------------

/* Group DnD Management *///---------------------------------------------------
void    GroupItem::groupMoved()
{
    qDebug() << "GroupItem::groupMoved()";
    // Group node adjacent edges must be updated manually since node are children of this group,
    // their x an y position does not change and is no longer monitored by their edges.
    if ( _group ) {
        for ( auto weakEdge : _group->getAdjacentEdges() ) {
            qan::Edge* edge = weakEdge.lock().get();
            if ( edge != nullptr && edge->getItem() != nullptr )
                edge->getItem()->updateItem();
        }
    }
}

void    GroupItem::groupNodeItem(qan::NodeItem* nodeItem)
{
    if ( nodeItem == nullptr ||
         getContainer() == nullptr )   // A container must have configured in concrete QML group component
        return;
    nodeItem->setPosition( nodeItem->mapToItem( getContainer(), QPointF{0., 0.} ) );
    nodeItem->setParentItem( getContainer() );
    groupMoved(); // Force call to groupMoved() to update group adjacent edges
    endProposeNodeDrop();
}

void    GroupItem::ungroupNodeItem(qan::NodeItem* nodeItem)
{
    if ( nodeItem == nullptr )   // A container must have configured in concrete QML group component
        return;
    if ( getGraph() &&
         getGraph()->getContainerItem() != nullptr ) {
        QPointF nodeGlobalPos = mapToItem(getGraph()->getContainerItem(), nodeItem->position());
        nodeItem->setParentItem( getGraph()->getContainerItem() );
        nodeItem->setPosition( nodeGlobalPos );
        nodeItem->setDraggable( true );
        nodeItem->setDroppable( true );
    }
}

void    GroupItem::proposeNodeDrop( qan::Node* node )
{
    Q_UNUSED( node);
    emit nodeDragEnter( );
}

void    GroupItem::endProposeNodeDrop()
{
    emit nodeDragLeave( );
}


void    GroupItem::dragEnterEvent( QDragEnterEvent* event )
{
    qDebug() << "qan::GroupItem::dragEnterEvent()";
    if ( ! _controller.handleDragEnterEvent(event) )
        event->ignore();
    QQuickItem::dragEnterEvent( event );
}

void	GroupItem::dragMoveEvent( QDragMoveEvent* event )
{
    qDebug() << "qan::GroupItem::dragMoveEvent()";
    _controller.handleDragMoveEvent(event);
    QQuickItem::dragMoveEvent( event );
}

void	GroupItem::dragLeaveEvent( QDragLeaveEvent* event )
{
    qDebug() << "qan::GroupItem::dragLeaveEvent()";
    _controller.handleDragLeaveEvent(event);
    QQuickItem::dragLeaveEvent( event );
}

void    GroupItem::dropEvent( QDropEvent* event )
{
    _controller.handleDropEvent(event);
    QQuickItem::dropEvent( event );
}

void    GroupItem::mouseDoubleClickEvent(QMouseEvent* event )
{
    qDebug() << "qan::GroupItem::mouseDoubleClickEvent()";
    _controller.handleMouseDoubleClickEvent(event);

    if ( event->button() == Qt::LeftButton )
        emit groupDoubleClicked( this, event->localPos() );
}

void    GroupItem::mouseMoveEvent(QMouseEvent* event )
{
    qDebug() << "qan::GroupItem::mouseMoveEvent()";
    _controller.handleMouseMoveEvent(event);
    QQuickItem::mouseMoveEvent(event);
}

void    GroupItem::mousePressEvent( QMouseEvent* event )
{
    qDebug() << "qan::GroupItem::mousePressEvent()";
    _controller.handleMousePressEvent(event);
}

void    GroupItem::mouseReleaseEvent( QMouseEvent* event )
{
    qDebug() << "qan::GroupItem::mouseReleaseEvent()";
    _controller.handleMouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------

} // ::qan
