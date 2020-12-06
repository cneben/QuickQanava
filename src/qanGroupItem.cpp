/*
 Copyright (c) 2008-2020, Benoit AUTHEMAN All rights reserved.

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

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanGroupItem.h"
#include "./qanEdgeItem.h"
#include "./qanGroup.h"
#include "./qanDraggableCtrl.h"

namespace qan { // ::qan

//using GroupDraggableCtrl = qan::DraggableCtrl<qan::Group, qan::GroupItem>;

/* Group Object Management *///------------------------------------------------
GroupItem::GroupItem( QQuickItem* parent ) :
    qan::NodeItem{ parent }
{
    qan::Draggable::configure(this);
    qan::Draggable::setAcceptDrops(true);

    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );

    // Force group connected edges update when the group is moved
    connect( this, &qan::GroupItem::xChanged,
             this, &qan::GroupItem::groupMoved );
    connect( this, &qan::GroupItem::yChanged,
             this, &qan::GroupItem::groupMoved );
    // Update adjacent edges z when group item z is modified.
    connect( this, &qan::GroupItem::zChanged, [this]() { this->groupMoved(); } );

    setItemStyle(qan::Group::style(parent));
    setObjectName(QStringLiteral("qan::GroupItem"));
    // Note: Do not set width and height
}

auto    GroupItem::getGroup() noexcept -> qan::Group* { return _group.data(); }
auto    GroupItem::getGroup() const noexcept -> const qan::Group* { return _group.data(); }
auto    GroupItem::setGroup(qan::Group* group) noexcept -> void
{
    // DraggableCtrl configuration is done in setNode()
    qan::NodeItem::setNode(static_cast<qan::Node*>(group));

    // Configuration specific to group
    _group = group;
    if ( group != nullptr &&            // Warning: Do that after having set _group
         group->getItem() != this )
        group->setItem(this);
}

auto    GroupItem::setRect(const QRectF& r) noexcept -> void
{
    // PRECONDITIONS:
        // r rect must be valid
    if (!r.isValid())
        return;
    setX(r.left());
    setY(r.top());
    setPreferredGroupWidth(r.width());
    setPreferredGroupHeight(r.height());
}
//-----------------------------------------------------------------------------


/* Selection and Sizing Management *///----------------------------------------
void    GroupItem::setPreferredGroupWidth(qreal preferredGroupWidth) noexcept
{
    _preferredGroupWidth = preferredGroupWidth;
    emit preferredGroupWidthChanged();
}

void    GroupItem::setPreferredGroupHeight(qreal preferredGroupHeight) noexcept
{
    _preferredGroupHeight = preferredGroupHeight;
    emit preferredGroupHeightChanged();
}

void    GroupItem::setMinimumGroupWidth(qreal minimumGroupWidth) noexcept
{
    _minimumGroupWidth = minimumGroupWidth;
    emit minimumGroupWidthChanged();
}

void    GroupItem::setMinimumGroupHeight(qreal minimumGroupHeight) noexcept
{
    _minimumGroupHeight = minimumGroupHeight;
    emit minimumGroupHeightChanged();
}
//-----------------------------------------------------------------------------


/* Collapse Management *///----------------------------------------------------
void    GroupItem::setCollapsed(bool collapsed) noexcept
{
    qan::NodeItem::setCollapsed(collapsed);
    // Note: Selection is hidden in base implementation
    if (_group) {
        const auto adjacentEdges = _group->collectAdjacentEdges();
        for (auto edge : adjacentEdges) {    // When a group is collapsed, all adjacent edges shouldbe hidden/shown...
            if (edge &&
                edge->getItem() != nullptr)
                edge->getItem()->setVisible(!getCollapsed());
        }
        if (!getCollapsed())
            groupMoved();   // Force update of all adjacent edges
    }
}

void    GroupItem::collapseAncestors(bool collapsed)
{
    // Do not call base
    // PRECONDITIONS:
        // getNode() can't return nullptr
        // getGraph() can't return nullptr
    const auto graph = getGraph();
    const auto group = getGroup();
    if (graph == nullptr)
        return;
    if (group == nullptr)
        return;

    // ALGORITHM:
        // 1. Collect all ancestors of group
        // 2. Filter from ancestors every nodes that are part of this group
        // 3. Collect adjacent edges of selected nodes
        // 4. Hide selected edges and nodes

    // 1.
    const auto allAncestors = graph->collectAncestorsDfs(*group, true);

    // 2.
    std::vector<qan::Node*> ancestors;
    for (const auto ancestor : allAncestors) {
        if (!group->hasNode(ancestor) &&
            ancestor != group)
            ancestors.push_back(const_cast<qan::Node*>(ancestor));
    }

    // 3.
    std::unordered_set<qan::Edge*> ancestorsEdges;
    for (const auto ancestor: ancestors) {
        const auto edges = ancestor->collectAdjacentEdges0();
        //ancestorsEdges.insert(std::inserter += edges;
        // FIXME use STL std::inserter here ?
        for (const auto edge : edges)
            ancestorsEdges.insert(edge);
    }

    // 4.
    for (const auto ancestorEdge: ancestorsEdges)
        ancestorEdge->getItem()->setVisible(collapsed);
    for (const auto ancestor: ancestors)
        ancestor->getItem()->setVisible(collapsed);
}
//-----------------------------------------------------------------------------

/* Group DnD Management *///---------------------------------------------------
void    GroupItem::groupMoved()
{
    if (getCollapsed())   // Do not update edges when the group is collapsed
        return;

    // Group node adjacent edges must be updated manually since node are children of this group,
    // their x an y position does not change and is no longer monitored by their edges.
    if (_group) {
        auto adjacentEdges = _group->collectAdjacentEdges();
        for (auto edge : adjacentEdges) {
            if (edge != nullptr &&
                edge->getItem() != nullptr)
                edge->getItem()->updateItem(); // Edge is updated even is edge item visible=false, updateItem() will take care of visibility
        }
    }
}

void    GroupItem::groupNodeItem(qan::NodeItem* nodeItem, bool transform)
{
    // PRECONDITIONS:
        // nodeItem can't be nullptr
        // A 'container' must have been configured
    if ( nodeItem == nullptr ||
         getContainer() == nullptr )   // A container must have configured in concrete QML group component
        return;

    // Note: no need for the container to be visible or open.
    auto groupPos = QPointF{nodeItem->x(), nodeItem->y()};
    if (transform) {
        const auto globalPos = nodeItem->mapToGlobal(QPointF{0., 0.});
        groupPos = getContainer()->mapFromGlobal(globalPos);
        nodeItem->setPosition(groupPos);
    }
    nodeItem->setParentItem(getContainer());
    groupMoved();           // Force call to groupMoved() to update group adjacent edges
    endProposeNodeDrop();
}

void    GroupItem::ungroupNodeItem(qan::NodeItem* nodeItem, bool transform)
{
    if (nodeItem == nullptr)   // A container must have configured in concrete QML group component
        return;
    if (getGraph() &&
        getGraph()->getContainerItem() != nullptr) {
        QPointF nodeGlobalPos = mapToItem(getGraph()->getContainerItem(), nodeItem->position());
        nodeItem->setParentItem(getGraph()->getContainerItem());
        if (transform)
            nodeItem->setPosition(nodeGlobalPos);
        nodeItem->setZ(z()+1.);
        nodeItem->setDraggable( true );
        nodeItem->setDroppable( true );
    }
}
void    GroupItem::setContainer(QQuickItem* container) noexcept
{
    // PRECONDITIONS: None, container can be nullptr
    if (container != _container) {
        _container = container;
        emit containerChanged();
    }
}
QQuickItem*         GroupItem::getContainer() noexcept { return _container; }
const QQuickItem*   GroupItem::getContainer() const noexcept { return _container; }

void    GroupItem::mouseDoubleClickEvent(QMouseEvent* event)
{
    qan::NodeItem::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton)
        emit groupDoubleClicked( this, event->localPos());
}

void    GroupItem::mousePressEvent(QMouseEvent* event)
{
    qan::NodeItem::mousePressEvent(event);

    if (event->button() == Qt::LeftButton &&    // Selection management
         getGroup() &&
         isSelectable() &&
         !getCollapsed()) {     // Collapsed group is not selectable
        if (getGraph())
            getGraph()->selectGroup(*getGroup(), event->modifiers());
    }

    if (event->button() == Qt::LeftButton)
        emit groupClicked(this, event->localPos());
    else if (event->button() == Qt::RightButton)
        emit groupRightClicked(this, event->localPos());
}
//-----------------------------------------------------------------------------

} // ::qan
