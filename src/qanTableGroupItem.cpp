/*
 Copyright (c) 2008-2022, Benoit AUTHEMAN All rights reserved.

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
// \file	qanTableGroupItem.cpp
// \author	benoit@destrat.io
// \date	2023 01 26
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanGroupItem.h"
#include "./qanTableGroupItem.h"

namespace qan { // ::qan

/* TableGroupItem Object Management *///---------------------------------------
TableGroupItem::TableGroupItem(QQuickItem* parent) :
    qan::GroupItem{parent}
{
    setObjectName(QStringLiteral("qan::TableGroupItem"));

    connect(this,   &QQuickItem::widthChanged,
            this,   &TableGroupItem::onResized);
    connect(this,   &QQuickItem::heightChanged,
            this,   &TableGroupItem::onResized);

    setItemStyle(qan::TableGroup::style(parent));
    setStrictDrop(false);  // Top left corner of a node is enought to allow a drop
}

TableGroupItem::~TableGroupItem()
{
    for (const auto cell: _cells)
        if (cell)
            cell->deleteLater();
    for (const auto border: _verticalBorders)
        if (border)
            border->deleteLater();
    for (const auto border: _horizontalBorders)
        if (border)
            border->deleteLater();
}

void    TableGroupItem::componentComplete() { /* Nil */ }

void    TableGroupItem::classBegin()
{
    auto engine = qmlEngine(this);
    if (engine == nullptr) {
        qWarning() << "qan::TableGroupItem::classBegin(): Error, no QML engine.";
        return;
    }
    auto cellComponent = new QQmlComponent(engine, "qrc:/QuickQanava/TableCell.qml",
                                           QQmlComponent::PreferSynchronous, nullptr);

    const auto createFromComponent = [this](QQmlComponent& component) -> QQuickItem* {
        if (!component.isReady()) {
            qWarning() << "qan::TableGroupItem::classBegin(): createTableCell(): Error table cell component is not ready.";
            qWarning() << component.errorString();
            return nullptr;
        }
        const auto rootContext = qmlContext(this);
        if (rootContext == nullptr) {
            qWarning() << "qan::TableGroupItem::classBegin(): createTableCell(): Error, no QML context.";
            return nullptr;
        }
        QQuickItem* item = nullptr;
        QObject* object = component.beginCreate(rootContext);
        if (object == nullptr ||
            component.isError()) {
            if (object != nullptr)
                object->deleteLater();
            return nullptr;
        }
        component.completeCreate();
        if (!component.isError()) {
            QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
            item = qobject_cast<QQuickItem*>(object);
        } // Note: There is no leak until cpp ownership is set
        if (item != nullptr)
            item->setVisible(true);
        return item;
    };

    // FIXME #190 Add fixed 3x3 cells
    const int cols = 3;
    const int rows = 3;

    for (auto r = 0; r < rows; r++)
        for (auto c = 0; c < cols; c++) {
            auto cell = qobject_cast<qan::TableCell*>(createFromComponent(*cellComponent));
            if (cell != nullptr) {
                _cells.push_back(cell);
                cell->setParentItem(this);
                cell->setVisible(true);
            }
        }

    auto borderComponent = new QQmlComponent(engine, "qrc:/QuickQanava/TableBorder.qml",
                                             QQmlComponent::PreferSynchronous, nullptr);

    // Notes:
    // - There is no "exterior" borders:
    //    - So there is  cols-1 vertical borders
    //    - And there is rows-1 horizontal borders
    //    - For exemplae 6 cells == 4 borders
    // - There is rows*cols cells for (rows-1) + (cols-1) borders.
    //
    // Internal cells vector is indexed row major:
    //   cell1 | cell2 | cell3
    //   ------+-------+------
    //   cell4 | cell5 | cell6
    //   ------+-------+------
    //   cell7 | cell8 | cell9
    //
    // So cell index in _cells at (col=c, row=r) is _cells[(r * cols) + c]

    qan::TableBorder* prevBorder = nullptr;
    for (auto c = 1; c <= (cols - 1); c++) {
        auto border = qobject_cast<qan::TableBorder*>(createFromComponent(*borderComponent));
        if (border != nullptr) {
            border->setTableGroup(getTableGroup());
            border->setOrientation(Qt::Vertical);
            border->setParentItem(this);
            border->setVisible(true);
            border->setPrevBorder(prevBorder);
            for (int r = 0; r < rows; r++) {
                border->addPrevCell(_cells[(r * cols) + c-1]);
                border->addNextCell(_cells[(r * cols) + c]);
            }
            _verticalBorders.push_back(border);

            if (prevBorder != nullptr)  // Audacious initialization of prevBorder nextBorder
                prevBorder->setNextBorder(border);  // with this border
            prevBorder = border;
        }
    }
    prevBorder = nullptr;
    for (auto r = 1; r <= (rows - 1); r++) {
        auto border = qobject_cast<qan::TableBorder*>(createFromComponent(*borderComponent));
        if (border != nullptr) {
            border->setTableGroup(getTableGroup());
            border->setOrientation(Qt::Horizontal);
            border->setParentItem(this);
            border->setVisible(true);
            border->setPrevBorder(prevBorder);
            for (int c = 0; c < cols; c++) {
                border->addPrevCell(_cells[((r-1) * cols)     + c]);
                border->addNextCell(_cells[(r * cols) + c]);
            }
            _horizontalBorders.push_back(border);

            if (prevBorder != nullptr)  // Audacious initialization of prevBorder nextBorder
                prevBorder->setNextBorder(border);  // with this border
            prevBorder = border;
        }
    }

    layoutTable();
}
//-----------------------------------------------------------------------------


/* Cells Management *///-------------------------------------------------------
void    TableGroupItem::setGroup(qan::Group* group) noexcept
{
    qan::GroupItem::setGroup(group);
    auto tableGroup = qobject_cast<qan::TableGroup*>(group);
    if (tableGroup != nullptr) {
        // Set borders reference to group
        for (auto border: _horizontalBorders)
            if (border)
                border->setTableGroup(tableGroup);
        for (auto border: _verticalBorders)
            if (border)
                border->setTableGroup(tableGroup);
        layoutTable();  // Force new layout with actual table group settings
    }
}

const qan::TableGroup*  TableGroupItem::getTableGroup() const { return qobject_cast<const qan::TableGroup*>(getGroup()); }
qan::TableGroup*        TableGroupItem::getTableGroup() { return qobject_cast<qan::TableGroup*>(getGroup()); };


void    TableGroupItem::layoutTable()
{
    const int cols = 3;
    const int rows = 3;

    const auto tableGroup = getTableGroup();
    const auto spacing = tableGroup != nullptr ? tableGroup->getCellSpacing() :
                                                 5.;
    const auto padding = tableGroup != nullptr ? tableGroup->getTablePadding() :
                                                 2.;

    if (cols <= 0 || rows <= 0) {
        qWarning() << "qan::TableGroupItem::layoutTable(): Error, rows and columns count can't be <= 0.";
        return;
    }
    if (spacing < 0 || padding < 0) {
        qWarning() << "qan::TableGroupItem::layoutTable(): Error, padding and spacing can't be < 0.";
        return;
    }

    const auto cellWidth = width() > 0. ? (width()
                                           - (2 * padding)
                                           - ((cols - 1) * spacing)) / cols :
                                          0.;
    const auto cellHeight = height() > 0. ? (height()
                                             - (2 * padding)
                                             - ((cols - 1) * spacing)) / rows :
                                            0.;

    //qWarning() << "cellWidth=" << cellWidth;
    //qWarning() << "cellHeight=" << cellHeight;

    if (cellWidth < 0. || cellHeight < 0.) {
        qWarning() << "qan::TableGroupItem::layoutTable(): Error, invalid cell width/height.";
        return;
    }
    // Note: cells are laid out by their borders, do not set their geometry
    // Layout in space

    // Layout vertical borders:
    // |             cell         |         cell         |         cell             |
    // | padding |   cell   |   border  |   cell   |   border  |   cell   | padding |
    //                       <-spacing->            <-spacing->
    if (_verticalBorders.size() == cols - 1) {
        const auto borderWidth = 3.;    // All easy mouse resize handling
        const auto borderWidth2 = borderWidth / 2.;
        for (auto c = 1; c <= (cols - 1); c++) {
            auto* verticalBorder = _verticalBorders[c-1];
            const auto x = padding +
                           ((c - 1) * spacing) +
                           (c * cellWidth) +
                           (spacing / 2.);
            verticalBorder->setX(x - borderWidth2);
            verticalBorder->setY(0.);
            verticalBorder->setWidth(borderWidth);
            verticalBorder->setHeight(height());
        }
    } else
        qWarning() << "qan::TableGoupItem::layoutTable(): Invalid vertical border count.";

    // Layout horizontal borders
    if (_horizontalBorders.size() == rows - 1) {
        const auto borderHeight = 3.;    // All easy mouse resize handling
        const auto borderHeight2 = borderHeight / 2.;
        for (auto r = 1; r <= (rows - 1); r++) {
            auto* horizontalBorder = _horizontalBorders[r-1];
            const auto y = padding +
                           ((r - 1) * spacing) +
                           (r * cellHeight) +
                           (spacing / 2.);
            horizontalBorder->setX(0.);
            horizontalBorder->setY(y - borderHeight2);
            horizontalBorder->setWidth(width());
            horizontalBorder->setHeight(borderHeight);
        }
    } else
        qWarning() << "qan::TableGoupItem::layoutTable(): Invalid horizontal border count.";

    // Note: There is no need to manually call borders layoutCells() method
    // it will be called automatically when border are moved.
}

void    TableGroupItem::onResized()
{
    layoutTable();
}
//-----------------------------------------------------------------------------


/* TableGroupItem DnD Management *///------------------------------------------
void    TableGroupItem::groupNodeItem(qan::NodeItem* nodeItem, bool transform)
{
    // PRECONDITIONS:
        // nodeItem can't be nullptr
        // A 'container' must have been configured
    if (nodeItem == nullptr ||
        getContainer() == nullptr)   // A container must have configured in concrete QML group component
        return;

    // Note: no need for the container to be visible or open.
    auto groupPos = QPointF{nodeItem->x(), nodeItem->y()};
    if (transform) {
        const auto globalPos = nodeItem->mapToGlobal(QPointF{0., 0.});
        groupPos = getContainer()->mapFromGlobal(globalPos);
        // Find cell at groupPos and attach node to cell
        //qWarning() << "_cells.size()=" << _cells.size();
        //qWarning() << "groupPos=" << groupPos;
        for (const auto& cell: _cells) {
            //qWarning() << cell-> boundingRect();
            const auto cellBr = cell->boundingRect().translated(cell->position());
            //qWarning() << "  cellBr=" << cellBr;
            if (cellBr.contains(groupPos)) {
                cell->setItem(nodeItem);
                break;
            }
        }
    }
    groupMoved();           // Force call to groupMoved() to update group adjacent edges
    endProposeNodeDrop();
}

void    TableGroupItem::ungroupNodeItem(qan::NodeItem* nodeItem, bool transform)
{
    if (nodeItem == nullptr)   // A container must have configured in concrete QML group component
        return;
    if (getGraph() &&
        getGraph()->getContainerItem() != nullptr) {
        const QPointF nodeGlobalPos = mapToItem(getGraph()->getContainerItem(), nodeItem->position());
        nodeItem->setParentItem(getGraph()->getContainerItem());
        if (transform)
            nodeItem->setPosition(nodeGlobalPos);
        nodeItem->setZ(z()+1.);
        nodeItem->setDraggable(true);
        nodeItem->setDroppable(true);
    }
}

void    TableGroupItem::mouseDoubleClickEvent(QMouseEvent* event)
{
    qan::NodeItem::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton &&
        (getNode() != nullptr &&
         !getNode()->getLocked()))
        emit groupDoubleClicked(this, event->localPos());
}

void    TableGroupItem::mousePressEvent(QMouseEvent* event)
{
    qan::NodeItem::mousePressEvent(event);

    if (event->button() == Qt::LeftButton &&    // Selection management
         getGroup() &&
         isSelectable() &&
         !getCollapsed() &&         // Locked/Collapsed group is not selectable
         !getNode()->getLocked()) {
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
