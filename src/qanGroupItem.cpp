/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

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
// \file	qanGroup.cpp
// \author	benoit@destrat.io
// \date	2016 03 22
//-----------------------------------------------------------------------------

// Qt headers

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanGroupItem.h"
#include "./qanEdgeItem.h"
#include "./qanGroup.h"
#include "./qanDraggableCtrl.h"

namespace qan { // ::qan

using GroupDraggableCtrl = qan::DraggableCtrl<qan::Group, qan::GroupItem>;

/* Group Object Management *///------------------------------------------------
GroupItem::GroupItem( QQuickItem* parent ) :
    QQuickItem{ parent }
{
    qan::Draggable::configure(this);
    qan::Draggable::setAcceptDrops(true);
    _draggableCtrl = std::unique_ptr<AbstractDraggableCtrl>{std::make_unique<GroupDraggableCtrl>()};
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->setTargetItem(this);

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

qan::AbstractDraggableCtrl& GroupItem::draggableCtrl() { Q_ASSERT(_draggableCtrl != nullptr); return *_draggableCtrl; }

auto    GroupItem::getGroup() noexcept -> qan::Group* { return _group.data(); }
auto    GroupItem::getGroup() const noexcept -> const qan::Group* { return _group.data(); }
auto    GroupItem::setGroup(qan::Group* group) noexcept -> void {
    _group = group;
    if ( group != nullptr &&            // Warning: Do that after having set _group
         group->getItem() != this )
        group->setItem(this);
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->setTarget(group);
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
    qan::Selectable::configureSelectionItem();
}

void    GroupItem::onHeightChanged()
{
    qan::Selectable::configureSelectionItem();
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

        if ( !collapsed )
            groupMoved();   // Force update of all adjacent edges
        emit collapsedChanged();
    }
}
//-----------------------------------------------------------------------------

/* Group DnD Management *///---------------------------------------------------
void    GroupItem::groupMoved()
{
    if ( _collapsed )   // Do not update edges when the group is collapsed
        return;
    // Group node adjacent edges must be updated manually since node are children of this group,
    // their x an y position does not change and is no longer monitored by their edges.
    if ( _group ) {
        for ( auto weakEdge : _group->getAdjacentEdges() ) {
            qan::Edge* edge = weakEdge.lock().get();
            if ( edge != nullptr &&
                 edge->getItem() != nullptr &&
                 edge->getItem()->isVisible() )
                edge->getItem()->updateItem();
        }
    }
}

void    GroupItem::groupNodeItem(qan::NodeItem* nodeItem, bool transformPosition )
{
    if ( nodeItem == nullptr ||
         getContainer() == nullptr )   // A container must have configured in concrete QML group component
        return;
    if ( transformPosition )
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
        nodeItem->setZ(z()+1.);
        nodeItem->setDraggable( true );
        nodeItem->setDroppable( true );
    }
}

void    GroupItem::dragEnterEvent( QDragEnterEvent* event )
{
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    if ( ! groupDraggableCtrl->handleDragEnterEvent(event) )
        event->ignore();
    QQuickItem::dragEnterEvent( event );
}

void	GroupItem::dragMoveEvent( QDragMoveEvent* event )
{
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->handleDragMoveEvent(event);
    QQuickItem::dragMoveEvent( event );
}

void	GroupItem::dragLeaveEvent( QDragLeaveEvent* event )
{
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->handleDragLeaveEvent(event);
    QQuickItem::dragLeaveEvent( event );
}

void    GroupItem::dropEvent( QDropEvent* event )
{
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->handleDropEvent(event);
    QQuickItem::dropEvent( event );
}

void    GroupItem::mouseDoubleClickEvent(QMouseEvent* event )
{
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->handleMouseDoubleClickEvent(event);

    if ( event->button() == Qt::LeftButton )
        emit groupDoubleClicked( this, event->localPos() );
}

void    GroupItem::mouseMoveEvent(QMouseEvent* event )
{
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->handleMouseMoveEvent(event);
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
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->handleMousePressEvent(event);

    if ( event->button() == Qt::LeftButton )
        emit groupClicked( this, event->localPos() );
    else if ( event->button() == Qt::RightButton )
        emit groupRightClicked( this, event->localPos() );
}

void    GroupItem::mouseReleaseEvent( QMouseEvent* event )
{
    const auto groupDraggableCtrl = static_cast<GroupDraggableCtrl*>(_draggableCtrl.get());
    groupDraggableCtrl->handleMouseReleaseEvent(event);
}
//-----------------------------------------------------------------------------

} // ::qan
