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
    _draggableCtrl.setTargetItem(this);

    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );

    // Force group connected edges update when the group is moved
    connect( this, &qan::GroupItem::xChanged,
             this, &qan::GroupItem::groupMoved );
    connect( this, &qan::GroupItem::yChanged,
             this, &qan::GroupItem::groupMoved );
    // Update adjacent edges z when group item z is modified.
    connect( this, &qan::GroupItem::zChanged, [this]() { this->groupMoved(); } );

    connect( this, &qan::GroupItem::widthChanged,
             this, &qan::GroupItem::onWidthChanged );
    connect( this, &qan::GroupItem::heightChanged,
             this, &qan::GroupItem::onHeightChanged );

    setStyle( qan::Group::style() );
    setObjectName( QStringLiteral("qan::GroupItem") );
}

GroupItem::~GroupItem() { /* Nil */ }

auto    GroupItem::getGroup() noexcept -> qan::Group* { return _group.data(); }
auto    GroupItem::getGroup() const noexcept -> const qan::Group* { return _group.data(); }
auto    GroupItem::setGroup(qan::Group* group) noexcept -> void {
    _group = group;
    _draggableCtrl.setTarget(group);
}

auto    GroupItem::setGraph(qan::Graph* graph) noexcept -> void {
    _graph = graph;
    qan::Selectable::configure( this, graph );
}
auto    GroupItem::getGraph() const noexcept -> const qan::Graph* { return _graph.data(); }
auto    GroupItem::getGraph() noexcept -> qan::Graph* { return _graph.data(); }
//-----------------------------------------------------------------------------

/* Style Management *///-------------------------------------------------------
void    GroupItem::setStyle( qan::Style* style ) noexcept
{
    if ( style != _style ) {
        if ( _style != nullptr )  // Every style that is non default is disconnect from this node
            QObject::disconnect( _style, 0, this, 0 );
        _style = style;
        if ( _style )
            connect( _style,    &QObject::destroyed,    // Monitor eventual style destruction
                     this,      &GroupItem::styleDestroyed );
        emit styleChanged( );
    }
}

void    GroupItem::setItemStyle( qan::Style* style ) noexcept { setStyle( style ); }

void    GroupItem::styleDestroyed( QObject* style )
{
    if ( style != nullptr )
        setStyle( nullptr );   // Set default style when current style is destroyed
}
//-----------------------------------------------------------------------------

/* Selection Management *///---------------------------------------------------
void    GroupItem::onWidthChanged()
{
    qan::Selectable::updateSelectionWidth();
}

void    GroupItem::onHeightChanged()
{
    qan::Selectable::updateSelectionHeight();
}
//-----------------------------------------------------------------------------

/* Collapse Management *///----------------------------------------------------
void    GroupItem::setCollapsed( bool collapsed ) noexcept
{
    if ( _group &&
         collapsed != _collapsed ) {
        _collapsed = collapsed;
        for ( auto weakEdge : _group->getAdjacentEdges() ) {    // When a group is collapsed, all adjacent edges shouldbe hidden/shown...
            const auto edge = weakEdge.lock() ;
            if ( edge &&
                 edge->getItem() != nullptr )
                edge->getItem()->setVisible( !collapsed );
        }
        if ( qan::Selectable::getSelectionItem() )              // Hide selection item when group is collapsed
            qan::Selectable::getSelectionItem()->setVisible(!_collapsed && getSelected());
        emit collapsedChanged();
    }
}
//-----------------------------------------------------------------------------

/* Group DnD Management *///---------------------------------------------------
void    GroupItem::groupMoved()
{
    // Group node adjacent edges must be updated manually since node are children of this group,
    // their x an y position does not change and is no longer monitored by their edges.
    if ( _group ) {
        qDebug() << "qan::GroupItem::groupMoved(): getAdjacentEdges.size()=" << _group->getAdjacentEdges().size();
        for ( auto weakEdge : _group->getAdjacentEdges() ) {
            qan::Edge* edge = weakEdge.lock().get();
            if ( edge != nullptr &&
                 edge->getItem() != nullptr )
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

void    GroupItem::dragEnterEvent( QDragEnterEvent* event )
{
    if ( ! _draggableCtrl.handleDragEnterEvent(event) )
        event->ignore();
    QQuickItem::dragEnterEvent( event );
}

void	GroupItem::dragMoveEvent( QDragMoveEvent* event )
{
    _draggableCtrl.handleDragMoveEvent(event);
    QQuickItem::dragMoveEvent( event );
}

void	GroupItem::dragLeaveEvent( QDragLeaveEvent* event )
{
    _draggableCtrl.handleDragLeaveEvent(event);
    QQuickItem::dragLeaveEvent( event );
}

void    GroupItem::dropEvent( QDropEvent* event )
{
    _draggableCtrl.handleDropEvent(event);
    QQuickItem::dropEvent( event );
}

void    GroupItem::mouseDoubleClickEvent(QMouseEvent* event )
{
    _draggableCtrl.handleMouseDoubleClickEvent(event);

    if ( event->button() == Qt::LeftButton )
        emit groupDoubleClicked( this, event->localPos() );
}

void    GroupItem::mouseMoveEvent(QMouseEvent* event )
{
    _draggableCtrl.handleMouseMoveEvent(event);
    QQuickItem::mouseMoveEvent(event);
}

void    GroupItem::mousePressEvent( QMouseEvent* event )
{
    forceActiveFocus();

    // Selection management
    if ( event->button() == Qt::LeftButton &&
         getGroup() &&
         isSelectable() ) {
        if ( _graph )
            _graph->selectGroup( *getGroup(), event->modifiers() );
    }

    _draggableCtrl.handleMousePressEvent(event);

    if ( event->button() == Qt::LeftButton )
        emit groupClicked( this, event->localPos() );
    else if ( event->button() == Qt::RightButton )
        emit groupRightClicked( this, event->localPos() );
}

void    GroupItem::mouseReleaseEvent( QMouseEvent* event )
{
    _draggableCtrl.handleMouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------

} // ::qan
