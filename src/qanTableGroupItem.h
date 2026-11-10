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
// \file    qanTableGroupItem.h
// \author  benoit@destrat.io
// \date    2023 01 26
//-----------------------------------------------------------------------------

#pragma once

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanNodeItem.h"
#include "./qanGroupItem.h"
#include "./qanTableBorder.h"

namespace qan { // ::qan

class Graph;
class TableGroup;
class TableCell;

/*! \brief Visual delegate for qan::TableGroup, manage table layout, borders and cells.
 *
 * \nosubgrouping
 */
class TableGroupItem : public qan::GroupItem
{
    /*! \name TableGroupItem Object Management *///----------------------------
    //@{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TableGroupItem(QQuickItem* parent = nullptr);
    virtual ~TableGroupItem() override;
    TableGroupItem(const TableGroupItem&) = delete;

public:
    //! Override qan::GroupItem::setContainer()
    virtual bool    setContainer(QQuickItem* container) noexcept override;

signals:
    //! Emitted when the table geometry has change (border modified for example).
    void            modified();
    //@}
    //-------------------------------------------------------------------------


    /*! \name Borders and Cells Management *///--------------------------------
    //@{
public:
    //! Clear table borders and cells (cells nodes are reparented to group).
    void        clearLayout();

    //! Initialize a table withe default cells and borders.
    void        initialize(int cols, int rows);
public:
    void        initializeCells(int cellsCount);
    void        initializeBorders(int verticalBordersCount, int horizontalBordersCount);
protected:
    //! Initialize the links between borders and cells, existing links are cleared.
    void        initializeCellsLinks();

protected:
    //! Factory of TableBorder.qml components, return nullptr on error, border is returned partially configured.
    qan::TableBorder*   createBorder();
private:
    // TableBorder.qml component cache.
    inline static QQmlComponent* _borderComponent = nullptr;
protected:
    //! Factory of TableCell.qml components, return nullptr on error, cell is returned partially configured.
    qan::TableCell*     createCell();
private:
    // TableCell.qml component cache.
    inline static QQmlComponent* _cellComponent = nullptr;

public:
    /*! \brief Return true when cells and borders match table columns and rows count.
     *
     * A valid layout has `cols * rows` cells, `cols - 1` vertical borders and `rows - 1`
     * horizontal borders, all non nullptr. Columns and rows can't be inserted or removed
     * in an invalid layout.
     */
    bool        hasValidLayout() const;

    /*! \brief Insert (append) a column, it takes half of the last column space.
     *
     * qan::Graph::tableModified() is not emitted.
     *
     * \return false when nothing was inserted: table is locked or protected, has an invalid layout, or
     * a cell or border could not be created.
     */
    bool        insertColumn();
    //! Insert (append) a row, it takes half of the last row space, see insertColumn().
    bool        insertRow();

    /*! \brief Remove the last column, the previous column takes its space.
     *
     * Removing a column just inserted with insertColumn() gives back the previous layout.
     *
     * Nodes still in the removed cells, including nodes under the cell item (see qan::TableCell::getNodes()),
     * are ungrouped and moved just right of the table, they are not removed from the graph.
     * As for insertColumn(), qan::Graph::tableModified() is not emitted, except by
     * qan::Graph::ungroupNode() for each ungrouped node, once the table has its new layout.
     *
     * \return false when nothing was removed: table has a single column, is locked or protected, or
     * has an invalid layout.
     */
    bool        removeColumn();
    //! Remove the last row, the previous row takes its space, see removeColumn() (ungrouped nodes go below the table).
    bool        removeRow();

protected:
    auto        createFromComponent(QQmlComponent& component) -> QQuickItem*;

public:
    void        initializeTableLayout();
public slots:
    //! Layout current cell after a table geometry change.
    void        layoutTable();

    //! Layout table cells, triggered when table style change.
    void        layoutCells();

public:
    virtual bool            setGroup(qan::Group* group) noexcept override;
protected:
    const qan::TableGroup*  getTableGroup() const;
    qan::TableGroup*        getTableGroup();

public:
    using Cells_t = std::vector<qan::TableCell*>;
    using Borders_t = std::vector<qan::TableBorder*>;

    const Cells_t&      getCells() const  { return _cells; };
    const Borders_t&    getVerticalBorders() const  { return _verticalBorders; };
    const Borders_t&    getHorizontalBorders() const  { return _horizontalBorders; };
private:
    //! True when columns and rows can be inserted or removed: table is neither locked nor protected, with a valid layout.
    bool        isLayoutEditable() const;
    //! Create \c count cells, return an empty vector (and destroy the cells already created) if one can't be created.
    auto        createCells(int count) -> Cells_t;
    /*! \brief Ungroup the nodes displayed in non nullptr \c cells, then hide and destroy them.
     *
     * \c cells must be out of the table layout. Ungrouped nodes are moved right of the table for
     * Qt::Vertical \c orientation (a column was removed), below it for Qt::Horizontal (a row).
     */
    void        releaseCells(const Cells_t& cells, Qt::Orientation orientation);
    //! Hide and destroy non nullptr \c cells, their content is not ungrouped.
    void        destroyCells(const Cells_t& cells);
    //! Hide and destroy last border in non empty \c borders, previous border becomes the last one.
    void        removeLastBorder(Borders_t& borders);

    Cells_t     _cells;
    Borders_t   _verticalBorders;
    Borders_t   _horizontalBorders;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Dragging Support Management *///---------------------------------
    //@{
public:
    /*! \brief Return the cell whose rectangle contains \c containerPos (in container CS).
     *
     * \return nullptr when \c containerPos is in no cell: table padding, spacing between cells or outside the table.
     */
    qan::TableCell* cellAt(const QPointF& containerPos) const;

    /*! \brief Return true when \c nodeItem can be grouped in this table without an explicit cell.
     *
     * The cell under \c nodeItem top-left corner must be empty or already display \c nodeItem: a cell displays
     * one node. qan::Graph::groupNode() refuses the grouping otherwise.
     */
    bool            canGroupNodeItem(const qan::NodeItem* nodeItem) const;

    /*! \brief Configure \c nodeItem in this group item (modify target item parenthcip, but keep same visual position).
     *
     * Without \c groupCell, \c nodeItem goes in the cell under its top-left corner, see cellAt() and canGroupNodeItem().
     */
    virtual void    groupNodeItem(qan::NodeItem* nodeItem, qan::TableCell* groupCell = nullptr, bool transform = true) override;

    //! Configure \c nodeItem outside this group item (modify parentship, keep same visual position).
    virtual void    ungroupNodeItem(qan::NodeItem* nodeItem, bool transform = true) override;

protected:
    virtual void    mouseDoubleClickEvent(QMouseEvent* event ) override;
    virtual void    mousePressEvent(QMouseEvent* event ) override;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE(qan::TableGroupItem)
