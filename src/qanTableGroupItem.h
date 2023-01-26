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

namespace qan { // ::qan

class Graph;

class TableCell : public QQuickItem
{
    /*! \name TableCell Object Management *///---------------------------------
    //@{
    Q_OBJECT
public:
    explicit TableCell(QQuickItem* parent = nullptr);
    virtual ~TableCell() override = default;
    TableCell(const TableCell&) = delete;
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief FIXME
 *
 * \nosubgrouping
 */
class TableGroupItem : public qan::GroupItem
{
    /*! \name TableGroupItem Object Management *///----------------------------
    //@{
    Q_OBJECT
public:
    explicit TableGroupItem(QQuickItem* parent = nullptr);
    virtual ~TableGroupItem() override = default;
    TableGroupItem(const TableGroupItem&) = delete;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Cells Management *///--------------------------------------------
    //@{
public:

    //! Layout current cell after a table geometry change.
    void        layoutCells();
private:
    std::vector<TableCell*> _cells;
    //@}
    //-------------------------------------------------------------------------


    /*! \name Dragging Support Management *///---------------------------------
    //@{
public:
    /*! \brief Configure \c nodeItem in this group item (modify target item parenthcip, but keep same visual position).
     */
    virtual void    groupNodeItem(qan::NodeItem* nodeItem, bool transform = true) override;

    //! Configure \c nodeItem outside this group item (modify parentship, keep same visual position).
    virtual void    ungroupNodeItem(qan::NodeItem* nodeItem, bool transform = true) override;

    //! Call at the beginning of another group or node hover operation on this group (usually trigger a visual change to notify user that insertion is possible trought DND).
    //void            proposeNodeDrop() noexcept { emit nodeDragEnter( ); }

    //! End an operation started with proposeNodeDrop().
    //void            endProposeNodeDrop() noexcept { emit nodeDragLeave( ); }

protected:
    virtual void    mouseDoubleClickEvent(QMouseEvent* event ) override;
    virtual void    mousePressEvent(QMouseEvent* event ) override;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE(qan::TableGroupItem)
