/*
 Copyright (c) 2008-2024, Benoit AUTHEMAN All rights reserved.

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
// \file    qanTableGroupItem.cpp
// \author	benoit@destrat.io
// \date    2023 01 26
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanGroupItem.h"
#include "./qanTableGroupItem.h"
#include "./qanTableCell.h"

namespace qan { // ::qan

/* TableGroupItem Object Management *///---------------------------------------
TableGroupItem::TableGroupItem(QQuickItem* parent) :
    qan::GroupItem{parent}
{
    setObjectName(QStringLiteral("qan::TableGroupItem"));
    setItemStyle(qan::TableGroup::style(parent));
    setStrictDrop(false);  // Top left corner of a node is enought to allow a drop
}

TableGroupItem::~TableGroupItem()
{
    clearLayout();
}

bool    TableGroupItem::setContainer(QQuickItem* container) noexcept
{
    //qWarning() << "qan::TableGroupItem::setContainer(): container=" << container;
    if (qan::GroupItem::setContainer(container)) {
        // Note: Force reparenting all borders and cell to container, it might be nullptr
        // at initialization.
        for (const auto verticalBorder: _verticalBorders)
            if (verticalBorder != nullptr)
                verticalBorder->setParentItem(container);
        for (const auto horizontalBorder: _horizontalBorders)
            if (horizontalBorder != nullptr)
                horizontalBorder->setParentItem(container);
        for (const auto cell: _cells)
            if (cell != nullptr)
                cell->setParentItem(container);

        // Note 20240830: React to size modifications, usually table size
        // is fully initialized at this point, to prevent spurious reaction
        // set setEnabled(false).
        const auto container = getContainer();
        if (container != nullptr) {
            connect(container,  &QQuickItem::widthChanged,
                    this,       &TableGroupItem::layoutTable);
            connect(container,  &QQuickItem::heightChanged,
                    this,       &TableGroupItem::layoutTable);
        }

        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------


/* Borders and Cells Management *///-------------------------------------------
void    TableGroupItem::clearLayout()
{
    //Warning() << "qan::TableGroupItem::clearLayout()";
    for (const auto verticalBorder: _verticalBorders)
        if (verticalBorder != nullptr)
            verticalBorder->deleteLater();
    _verticalBorders.clear();
    for (const auto horizontalBorder: _horizontalBorders)
        if (horizontalBorder != nullptr)
            horizontalBorder->deleteLater();
    _horizontalBorders.clear();

    for (const auto cell: _cells)
        if (cell != nullptr)
            cell->deleteLater();
    _cells.clear();
}

void    TableGroupItem::initialize(int cols, int rows)
{
    //qWarning() << "qan::TableGroupItem::initialize(): rows=" << rows << "  cols=" << cols;
    //qWarning() << "  tablegroup rows=" << getTableGroup()->getRows() << "  cols=" << getTableGroup()->getCols();
    //qWarning() << "  container=" << getContainer();
    if (rows <= 0 || cols <= 0) {
        qWarning() << "TableGroupItem::initialize(): Error, invalid rows or cols count.";
        return;
    }

    auto engine = qmlEngine(this);
    if (engine == nullptr) {
        qWarning() << "qan::TableGroupItem::initialize(): Error, no QML engine.";
        return;
    }
    clearLayout();
    initializeCells(rows * cols);  // Create cells

    // Notes:
    // - There is no "exterior" borders:
    //    - So there is  cols-1 vertical borders
    //    - And there is rows-1 horizontal borders
    //    - For exemple 6 cells == 4 borders
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

    initializeBorders(cols - 1, rows - 1);
    initializeCellsLinks();  // Link borders and cells

    // Note 20240830: Do not call initializeTableLayout(),
    // it is up to the user to do that, it might not be
    // desirable in vertain serialization use cases.
}

void    TableGroupItem::initializeCells(int cellsCount)
{
    if (cellsCount <= 0) {
        qWarning() << "TableGroupItem::initializeCells(): Error, invalid rows or cols count.";
        return;
    }
    if (cellsCount == static_cast<int>(_cells.size()))
        return;
    auto engine = qmlEngine(this);
    if (engine == nullptr) {
        qWarning() << "qan::TableGroupItem::initializeCells(): Error, no QML engine.";
        return;
    }
    // Create cells
    for (auto c = 0; c < cellsCount; c++) {
        auto cell = createCell();
        if (cell != nullptr)
            _cells.push_back(cell);
    }
}

void    TableGroupItem::initializeBorders(int verticalBordersCount, int horizontalBordersCount)
{
    //qWarning() << "qan::TableGroupItem::initializeBorders(): verticalBordersCount=" << verticalBordersCount << "  horizontalBordersCount=" << horizontalBordersCount;
    if (verticalBordersCount < 0 ||     // Might be 0 for 1x1 tables
        horizontalBordersCount < 0) {
        qWarning() << "TableGroupItem::initializeBorders(): Error, invalid vertical or horizontal borders count.";
        return;
    }

    qan::TableBorder* prevBorder = nullptr;
    if (verticalBordersCount != static_cast<int>(_verticalBorders.size())) {
        for (auto v = 0; v < verticalBordersCount; v++) {
            auto border = createBorder();
            if (border != nullptr) {
                border->setOrientation(Qt::Vertical);
                border->setPrevBorder(prevBorder);
                _verticalBorders.push_back(border);
                if (prevBorder != nullptr)  // Audacious initialization of prevBorder nextBorder
                    prevBorder->setNextBorder(border);  // with this border
                prevBorder = border;
            }
        }
    }
    prevBorder = nullptr;
    if (horizontalBordersCount != static_cast<int>(_horizontalBorders.size())) {
        for (auto h = 0; h < horizontalBordersCount; h++) {
            auto border = createBorder();
            if (border != nullptr) {
                border->setOrientation(Qt::Horizontal);
                border->setPrevBorder(prevBorder);
                _horizontalBorders.push_back(border);
                if (prevBorder != nullptr)  // Audacious initialization of prevBorder nextBorder
                    prevBorder->setNextBorder(border);  // with this border
                prevBorder = border;
            }
        }
    }
}

void    TableGroupItem::initializeCellsLinks()
{
    // Note: Links are cleared first: a border must never keep a link to a cell that is no longer in _cells, it
    // might be destroyed. They are rebuilt only for a valid layout, borders count must match cells indexes.
    for (const auto verticalBorder: _verticalBorders)
        if (verticalBorder != nullptr)
            verticalBorder->clearCells();
    for (const auto horizontalBorder: _horizontalBorders)
        if (horizontalBorder != nullptr)
            horizontalBorder->clearCells();
    if (!hasValidLayout())
        return;
    const auto tableGroup = getTableGroup();
    const std::size_t rows = tableGroup->getRows();
    const std::size_t cols = tableGroup->getCols();
    std::size_t c = 1;
    for (const auto verticalBorder: _verticalBorders) {
        for (std::size_t r = 0; r < rows; r++) {
            verticalBorder->addPrevCell(_cells[(r * cols) + c - 1]);
            verticalBorder->addNextCell(_cells[(r * cols) + c]);
        }
        c++;
    }
    std::size_t r = 1;
    for (const auto horizontalBorder: _horizontalBorders) {
        for (std::size_t col = 0; col < cols; col++) {
            horizontalBorder->addPrevCell(_cells[((r-1) * cols) + col]);
            horizontalBorder->addNextCell(_cells[(r * cols)     + col]);
        }
        r++;
    }
}

qan::TableBorder*   TableGroupItem::createBorder()
{
    if (TableGroupItem::_borderComponent == nullptr) {
        auto engine = qmlEngine(this);
        if (engine == nullptr) {
            qWarning() << "qan::TableGroupItem::initializeBorders(): Error, no QML engine.";
            return nullptr;
        }
        // Component is parented to graph, will be destroyed when graph is destroyed
        TableGroupItem::_borderComponent = new QQmlComponent(engine, "qrc:/QuickQanava/TableBorder.qml",
                                                             QQmlComponent::PreferSynchronous, getGraph());
    }
    if (!TableGroupItem::_borderComponent)
        return nullptr;
    auto border = qobject_cast<qan::TableBorder*>(createFromComponent(*TableGroupItem::_borderComponent));
    if (border != nullptr) {
        border->setTableGroup(getTableGroup());
        border->setParentItem(getContainer() != nullptr ? getContainer() : this);
        border->setVisible(true);
        connect(border, &qan::TableBorder::modified,
                this,   [this]() {
                    const auto graph = this->getGraph();
                    const auto tableGroup = this->getTableGroup();
                    if (graph != nullptr &&
                        tableGroup != nullptr)
                        emit graph->tableModified(tableGroup);
                });
    }
    return border;
}


qan::TableCell*   TableGroupItem::createCell()
{
    if (TableGroupItem::_cellComponent == nullptr) {
        auto engine = qmlEngine(this);
        if (engine == nullptr) {
            qWarning() << "qan::TableGroupItem::initializeBorders(): Error, no QML engine.";
            return nullptr;
        }
        // Component is parented to graph, will be destroyed when graph is destroyed
        TableGroupItem::_cellComponent = new QQmlComponent(engine, "qrc:/QuickQanava/TableCell.qml",
                                                           QQmlComponent::PreferSynchronous, nullptr);
    }
    if (!TableGroupItem::_cellComponent)
        return nullptr;
    auto cell = qobject_cast<qan::TableCell*>(createFromComponent(*TableGroupItem::_cellComponent));
    if (cell != nullptr) {
        cell->setParentItem(getContainer() != nullptr ? getContainer() : this);
        cell->setVisible(true);
        cell->setTable(getTableGroup());
    }
    return cell;
}

bool    TableGroupItem::hasValidLayout() const
{
    const auto tableGroup = getTableGroup();
    if (tableGroup == nullptr ||
        tableGroup->getRows() <= 0 ||
        tableGroup->getCols() <= 0)
        return false;
    const std::size_t rows = tableGroup->getRows();
    const std::size_t cols = tableGroup->getCols();
    return _cells.size() == (rows * cols) &&
           _verticalBorders.size() == (cols - 1) &&
           _horizontalBorders.size() == (rows - 1) &&
           std::find(_cells.cbegin(), _cells.cend(), nullptr) == _cells.cend() &&
           std::find(_verticalBorders.cbegin(), _verticalBorders.cend(), nullptr) == _verticalBorders.cend() &&
           std::find(_horizontalBorders.cbegin(), _horizontalBorders.cend(), nullptr) == _horizontalBorders.cend();
}

bool    TableGroupItem::isLayoutEditable() const
{
    const auto tableGroup = getTableGroup();
    return getContainer() != nullptr &&
           tableGroup != nullptr &&
           !tableGroup->getLocked() &&
           !tableGroup->getIsProtected() &&
           hasValidLayout();
}

bool    TableGroupItem::insertColumn()
{
    // Algorithm:
    // 1. Create the new column cells and its vertical border: nothing is modified if one can't be created.
    // 2. The new border splits the last column in two. A single column table has no border, the new border
    //    then splits the whole table.
    // 3. Update the _cells array, be very carefull there is a mapping from old table layout to new layout:
    //    copy old cells to the new layout, then add the new cells in the new column.
    // 4. Rebuild links between borders and cells, then layout cells with valid size.
    if (!isLayoutEditable())
        return false;
    const auto container = getContainer();
    auto tableGroup = getTableGroup();
    const auto oldCols = tableGroup->getCols();
    const auto rows = tableGroup->getRows();
    const auto addedCells = createCells(rows);  // 1.
    if (addedCells.empty())
        return false;
    const auto border = createBorder();
    if (border == nullptr) {
        destroyCells(addedCells);
        return false;
    }

    qan::TableBorder* prevBorder = _verticalBorders.empty() ? nullptr : _verticalBorders.back();  // 2.
    border->setOrientation(Qt::Vertical);
    border->setPrevBorder(prevBorder);
    _verticalBorders.push_back(border);
    if (prevBorder != nullptr)
        prevBorder->setNextBorder(border);
    const auto width = container->width();
    const auto prevX = prevBorder != nullptr ? prevBorder->x() : 0.;
    const auto x = prevX + ((width - prevX) / 2.0);
    border->setX(x);
    border->setSx(x / width);
    border->setHeight(container->height());
    border->setWidth(3);

    const auto newCols = oldCols + 1;           // 3.
    Cells_t newCells(newCols * rows);
    for (auto r = 0; r < rows; r++) {
        for (auto c = 0; c < oldCols; c++)
            newCells[(r * newCols) + c] = _cells[(r * oldCols) + c];
        newCells[(r * newCols) + oldCols] = addedCells[r];
    }
    _cells = newCells;
    tableGroup->setCols(newCols);
    initializeCellsLinks();                     // 4.
    layoutCells();
    return true;
}

bool    TableGroupItem::insertRow()
{
    // Algorithm: See insertColumn(), there is no mapping since the new row cells are appended.
    if (!isLayoutEditable())
        return false;
    const auto container = getContainer();
    auto tableGroup = getTableGroup();
    const auto cols = tableGroup->getCols();
    const auto oldRows = tableGroup->getRows();
    const auto addedCells = createCells(cols);
    if (addedCells.empty())
        return false;
    const auto border = createBorder();
    if (border == nullptr) {
        destroyCells(addedCells);
        return false;
    }

    qan::TableBorder* prevBorder = _horizontalBorders.empty() ? nullptr : _horizontalBorders.back();
    border->setOrientation(Qt::Horizontal);
    border->setPrevBorder(prevBorder);
    _horizontalBorders.push_back(border);
    if (prevBorder != nullptr)
        prevBorder->setNextBorder(border);
    const auto height = container->height();
    const auto prevY = prevBorder != nullptr ? prevBorder->y() : 0.;
    const auto y = prevY + ((height - prevY) / 2.0);
    border->setY(y);
    border->setSy(y / height);
    border->setWidth(container->width());
    border->setHeight(3);

    _cells.insert(_cells.end(), addedCells.cbegin(), addedCells.cend());
    tableGroup->setRows(oldRows + 1);
    initializeCellsLinks();
    layoutCells();
    return true;
}

bool    TableGroupItem::removeColumn()
{
    // Algorithm:
    // 1. Remove the last vertical border: the previous column takes its space.
    // 2. Update the _cells array, there is a mapping from old table layout to new layout.
    // 3. Rebuild links between borders and cells, then layout cells.
    // 4. Release the removed cells: ungroup their content, then hide and destroy them. Done last, since
    //    ungrouping emits signals: their receivers see the table with its final layout.
    if (!isLayoutEditable())
        return false;
    auto tableGroup = getTableGroup();
    const auto oldCols = tableGroup->getCols();
    const auto rows = tableGroup->getRows();
    if (oldCols <= 1)
        return false;
    const auto newCols = oldCols - 1;
    Cells_t newCells;
    Cells_t removedCells;
    for (auto r = 0; r < rows; r++)
        for (auto c = 0; c < oldCols; c++)
            (c < newCols ? newCells : removedCells).push_back(_cells[(r * oldCols) + c]);
    removeLastBorder(_verticalBorders);         // 1.
    _cells = newCells;                          // 2.
    tableGroup->setCols(newCols);
    initializeCellsLinks();                     // 3.
    layoutCells();
    releaseCells(removedCells, Qt::Vertical);   // 4.
    return true;
}

bool    TableGroupItem::removeRow()
{
    // Algorithm: See removeColumn(), there is no mapping since the last row cells are the last cells.
    if (!isLayoutEditable())
        return false;
    auto tableGroup = getTableGroup();
    const auto cols = tableGroup->getCols();
    const auto oldRows = tableGroup->getRows();
    if (oldRows <= 1)
        return false;
    const auto newRows = oldRows - 1;
    const Cells_t removedCells(_cells.cbegin() + (newRows * cols), _cells.cend());
    removeLastBorder(_horizontalBorders);
    _cells.resize(newRows * cols);
    tableGroup->setRows(newRows);
    initializeCellsLinks();
    layoutCells();
    releaseCells(removedCells, Qt::Horizontal);
    return true;
}

auto    TableGroupItem::createCells(int count) -> Cells_t
{
    Cells_t cells;
    for (auto c = 0; c < count; c++) {
        const auto cell = createCell();
        if (cell == nullptr) {
            destroyCells(cells);
            return {};
        }
        cells.push_back(cell);
    }
    return cells;
}

void    TableGroupItem::releaseCells(const Cells_t& cells, Qt::Orientation orientation)
{
    // Algorithm:
    // 1. Ungroup every node still displayed in the cells, not only their getItem() (see qan::TableCell::getNodes()).
    //    A node whose cell link is stale gets this cell first: ungroupNodeItem() only takes a node out of its own cell.
    // 2. Move ungrouped nodes just outside the table, right of it for a column, below it for a row: the table keeps
    //    its size, they would otherwise lie over the previous column (row) cells.
    // 3. Hide and destroy the cells.
    const auto graph = getGraph();
    const auto tableGroup = getTableGroup();
    const auto graphContainer = graph != nullptr ? graph->getContainerItem() : nullptr;
    const auto tableRect = graphContainer != nullptr ? mapRectToItem(graphContainer, boundingRect()) :
                                                       QRectF{};
    for (const auto cell: cells) {
        std::vector<QPointer<qan::Node>> nodes;     // Note: An ungroupNode() signal receiver might destroy a node
        for (const auto node: cell->getNodes())
            nodes.push_back(node);
        for (const auto& node: nodes) {
            if (!node ||
                graph == nullptr ||
                node->get_group() != tableGroup)
                continue;
            if (node->getCell() != cell)            // 1.
                node->setCell(cell);
            if (!graph->ungroupNode(node, tableGroup))
                continue;
            const auto nodeItem = node ? node->getItem() : nullptr;     // 2.
            if (nodeItem != nullptr &&
                nodeItem->parentItem() == graphContainer)
                nodeItem->setPosition(orientation == Qt::Vertical ? QPointF{tableRect.right() + 10., nodeItem->y()} :
                                                                   QPointF{nodeItem->x(), tableRect.bottom() + 10.});
        }
    }
    destroyCells(cells);                            // 3.
}

void    TableGroupItem::destroyCells(const Cells_t& cells)
{
    for (const auto cell: cells) {
        if (cell == nullptr)
            continue;
        cell->setItem(nullptr);
        cell->setVisible(false);
        cell->deleteLater();
    }
}

void    TableGroupItem::removeLastBorder(Borders_t& borders)
{
    const auto border = borders.back();
    borders.pop_back();
    if (!borders.empty())   // Note: Do not rely on QPointer, it is reset only when border is destroyed
        borders.back()->setNextBorder(nullptr);
    border->setVisible(false);
    border->deleteLater();
}

auto TableGroupItem::createFromComponent(QQmlComponent& component) -> QQuickItem*
{
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

void    TableGroupItem::initializeTableLayout()
{
    const auto tableGroup = getTableGroup();
    if (tableGroup == nullptr)
        return;
    const auto tableContainer = getContainer();
    if (tableContainer == nullptr)
        return;
    const auto tableWidth = tableContainer->width();
    const auto tableHeight = tableContainer->height();
    //const auto tableSize = tableContainer->size();
    //qWarning() << "qan::TableGroupItem::initializeTableLayout(): tableSize=" << tableSize;
    if (qRound(tableWidth) <= 0 || qRound(tableHeight) <= 0)
        return;

    const int cols = tableGroup->getCols();
    const int rows = tableGroup->getRows();
    const auto spacing = tableGroup != nullptr ? tableGroup->getCellSpacing() :
                             5.;
    const auto padding = tableGroup != nullptr ? tableGroup->getTablePadding() :
                             2.;

    if (cols <= 0 || rows <= 0) {
        qWarning() << "qan::TableGroupItem::initializeTableLayout(): Error, rows and columns count can't be <= 0.";
        return;
    }
    if (spacing < 0 || padding < 0) {
        qWarning() << "qan::TableGroupItem::initializeTableLayout(): Error, padding and spacing can't be < 0.";
        return;
    }

    const auto cellWidth = tableWidth > 0. ? (tableWidth
                                              - (2 * padding)
                                              - ((cols - 1) * spacing)) / cols :
                               0.;
    const auto cellHeight = tableHeight > 0. ? (tableHeight
                                                - (2 * padding)
                                                - ((rows - 1) * spacing)) / rows :
                                0.;

    //qWarning() << "  cellWidth=" << cellWidth << " cellHeight=" << cellHeight;

    if (cellWidth < 0. || cellHeight < 0.) {
        qWarning() << "qan::TableGroupItem::initializeTableLayout(): Error, invalid cell width/height.";
        return;
    }
    // Note: cells are laid out by their borders, do not set their geometry
    // Layout in space

    // Layout vertical borders:
    // |             cell         |         cell         |         cell             |
    // | padding |   cell   |   border  |   cell   |   border  |   cell   | padding |
    //                       <-spacing->            <-spacing->
    if (static_cast<int>(_verticalBorders.size()) == cols - 1) {
        const auto borderWidth = 3.;    // All easy mouse resize handling
        const auto borderWidth2 = borderWidth / 2.;
        for (auto c = 1; c <= (cols - 1); c++) {
            auto* verticalBorder = _verticalBorders[c-1];
            const auto x = padding +
                           ((c - 1) * spacing) +
                           (c * cellWidth) +
                           (spacing / 2.);
            const auto borderX = x - borderWidth2;
            verticalBorder->setSx(borderX / tableWidth);
            verticalBorder->setY(0.);
            verticalBorder->setWidth(borderWidth);
            verticalBorder->setHeight(tableHeight);
        }
    } else
        qWarning() << "qan::TableGoupItem::initializeTableLayout(): Invalid vertical border count.";

    // Layout horizontal borders
    if (static_cast<int>(_horizontalBorders.size()) == rows - 1) {
        const auto borderHeight = 3.;    // All easy mouse resize handling
        const auto borderHeight2 = borderHeight / 2.;
        for (auto r = 1; r <= (rows - 1); r++) {
            auto* horizontalBorder = _horizontalBorders[r-1];
            const auto y = padding +
                           ((r - 1) * spacing) +
                           (r * cellHeight) +
                           (spacing / 2.);
            horizontalBorder->setX(0.);
            const auto borderY = y - borderHeight2;
            horizontalBorder->setSy(borderY / tableHeight);
            horizontalBorder->setWidth(tableWidth);
            horizontalBorder->setHeight(borderHeight);
        }
    } else
        qWarning() << "qan::TableGoupItem::initializeTableLayout(): Invalid horizontal border count.";

    // Note: There is no need to manually call borders layoutCells() method
    // it will be called automatically when border are moved.
    // Note 20230406: In fact calling layout cell is necessary for rows==1, cols==1
    // table that need special handling to dimension cells since there is no horiz/vert borders.
    layoutTable();
}

void    TableGroupItem::layoutTable()
{
    if (!isEnabled())
        return; // Note 20240830: prevent spurious layouts during serialization (see hlg::TableGroup::serializeFromJson()).
    const auto tableContainer = getContainer();
    if (tableContainer == nullptr)
        return;

    const auto tableSize = tableContainer->size();
    const auto tableWidth = tableContainer->width();
    const auto tableHeight = tableContainer->height();

    // During initial polish loop and since we are binded directly on width/
    // height change, table container size might be empty.
    if (tableSize.isEmpty() || tableSize.isNull())
        return;

    // qWarning() << "TableGroupItem::layoutTable(): " << getGroup()->getLabel() <<
    //     "  tableWidth=" << tableWidth << "tableHeight=" << tableHeight;

    // Project normalized sx/sy coordinates to table item container CS.
    for (const auto verticalBorder: _verticalBorders) {
        if (verticalBorder == nullptr)
            continue;
        verticalBorder->setX(verticalBorder->getSx() * tableWidth);
        verticalBorder->setY(0.);
        verticalBorder->setHeight(tableHeight);
    }

    for (const auto horizontalBorder: _horizontalBorders) {
        if (horizontalBorder == nullptr)
            continue;
        horizontalBorder->setX(0.);
        horizontalBorder->setY(horizontalBorder->getSy() * tableHeight);
        horizontalBorder->setWidth(tableWidth);
    }

    layoutCells();
}

void    TableGroupItem::layoutCells()
{
    //qWarning() << "qan::TableGroupItem::layoutCells()";
    for (const auto verticalBorder: _verticalBorders) {
        if (verticalBorder != nullptr)
            verticalBorder->layoutCells();
    }
    for (const auto horizontalBorder: _horizontalBorders) {
        if (horizontalBorder != nullptr)
            horizontalBorder->layoutCells();
    }

    const auto tableContainer = getContainer();
    if (tableContainer == nullptr)
        return;
    const auto tableWidth = tableContainer->width();
    const auto tableHeight = tableContainer->height();

    // Special handling for 1 row or 1 column table: since there is
    // no "moveable" border, calls to layoutCells() do not set either width/height
    // of cells. Manually set correct width / height:
    const auto tableGroup = getTableGroup();
    if (tableGroup == nullptr)
        return;
    //qWarning() << "  tableGroup->getRows()=" << tableGroup->getRows();
    //qWarning() << "  tableGroup->getCols()=" << tableGroup->getCols();
    const auto padding = getTableGroup() != nullptr ? getTableGroup()->getTablePadding() : 2.;
    const auto padding2 = padding * 2.;
    if (tableGroup->getCols() == 1) {
        for (const auto cell: _cells)
            if (cell != nullptr) {
                cell->setX(padding);
                cell->setWidth(tableWidth - padding2);
            }
    }
    if (tableGroup->getRows() == 1) {
        for (const auto cell: _cells)
            if (cell != nullptr) {
                cell->setHeight(tableHeight - padding2);
                cell->setY(padding);
            }
    }
}

bool    TableGroupItem::setGroup(qan::Group* group) noexcept
{
    if (qan::GroupItem::setGroup(group)) {
        auto tableGroup = qobject_cast<qan::TableGroup*>(group);
        if (tableGroup != nullptr) {
            initialize(tableGroup->getCols(),
                       tableGroup->getRows());

            // Set borders reference to group
            for (auto border: _horizontalBorders)
                if (border)
                    border->setTableGroup(tableGroup);
            for (auto border: _verticalBorders)
                if (border)
                    border->setTableGroup(tableGroup);
            connect(tableGroup, &qan::TableGroup::cellSpacingChanged,
                    this,       &qan::TableGroupItem::layoutCells);
            connect(tableGroup, &qan::TableGroup::cellMinimumSizeChanged,
                    this,       &qan::TableGroupItem::layoutCells);
            connect(tableGroup, &qan::TableGroup::tablePaddingChanged,
                    this,       &qan::TableGroupItem::layoutCells);

            // Set cell reference to group
            for (auto cell: _cells)
                if (cell != nullptr)
                    cell->setTable(tableGroup);

            // Note 20240831: Do not layout it is up to the user to call qan::TableGroup::initializeLayout(),
            // for example you might want to use custom serialization code
            return true;
        }
    }
    return false;
}

const qan::TableGroup*  TableGroupItem::getTableGroup() const { return qobject_cast<const qan::TableGroup*>(getGroup()); }
qan::TableGroup*        TableGroupItem::getTableGroup() { return qobject_cast<qan::TableGroup*>(getGroup()); };
//-----------------------------------------------------------------------------


/* TableGroupItem DnD Management *///------------------------------------------
void    TableGroupItem::groupNodeItem(qan::NodeItem* nodeItem, qan::TableCell* groupCell, bool transform)
{
    //qWarning() << "qan::TableGroupItem::groupNodeItem(): nodeItem=" << nodeItem << "  groupCell=" << groupCell;
    // PRECONDITIONS:
    // nodeItem can't be nullptr
    // A 'container' must have been configured
    Q_UNUSED(groupCell)
    Q_UNUSED(transform)
    if (nodeItem == nullptr ||
        getContainer() == nullptr)   // A container must have configured in concrete QML group component
        return;

    // Note: no need for the container to be visible or open.
    const auto globalPos = nodeItem->mapToGlobal(QPointF{0., 0.});
    const auto groupPos = getContainer()->mapFromGlobal(globalPos);
    if (groupCell != nullptr) {
        if (std::find(_cells.cbegin(), _cells.cend(), groupCell) != _cells.cend()) {
            groupCell->setItem(nodeItem);
            nodeItem->getNode()->setCell(groupCell);
        } else
            qWarning() << "qan::TableGroupItem::groupNodeItem(): Internal error, groupCell could not be found in internal cells.";
    } else {
        // Find cell at groupPos and attach node to cell
        for (const auto& cell: _cells) {
            if (cell == nullptr)
                continue;
            const auto cellBr = cell->boundingRect().translated(cell->position());
            if (cellBr.contains(groupPos)) {
                cell->setItem(nodeItem);
                nodeItem->getNode()->setCell(cell);
                break;
            }
        }
    }

    groupMoved();           // Force call to groupMoved() to update group adjacent edges

    // Notify that the table has changed
    const auto graph = this->getGraph();
    const auto tableGroup = this->getTableGroup();
    if (graph != nullptr &&
        tableGroup != nullptr)
        emit graph->tableModified(tableGroup);

    endProposeNodeDrop();
}

void    TableGroupItem::ungroupNodeItem(qan::NodeItem* nodeItem, bool transform)
{
    if (nodeItem == nullptr)   // A container must have configured in concrete QML group component
        return;
    if (getGraph() != nullptr &&
        getGraph()->getContainerItem() != nullptr) {
        auto nodeCell = nodeItem->getNode()->getCell();
        if (nodeCell != nullptr) {
            const QPointF nodeGlobalPos = nodeItem->mapToItem(getGraph()->getContainerItem(),
                                                              QPointF{0., 0.});

            nodeCell->restoreCache(nodeItem);
            nodeCell->setItem(nullptr);
            nodeCell->setUserProp(QVariant{});
            nodeItem->setParentItem(getGraph()->getContainerItem());
            if (transform)
                nodeItem->setPosition(nodeGlobalPos + QPointF{10., 10.});  // A delta to visualize ungroup
            nodeItem->setZ(z() + 1.);
            nodeItem->getNode()->setCell(nullptr);
        }
    }
}

void    TableGroupItem::mouseDoubleClickEvent(QMouseEvent* event)
{
    qan::NodeItem::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton &&
        (getNode() != nullptr &&
         !getNode()->getLocked()))
        emit groupDoubleClicked(this, event->position());
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
        emit groupClicked(this, event->position());
    else if (event->button() == Qt::RightButton)
        emit groupRightClicked(this, event->position());
}
//-----------------------------------------------------------------------------

} // ::qan
