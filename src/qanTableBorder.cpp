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
// \file	qanTableBorder.cpp
// \author	benoit@destrat.io
// \date	2023 01 26
//-----------------------------------------------------------------------------

// Std headers
#include <sstream>
#include <limits>
#include <algorithm>

// Qt headers
#include <QCursor>
#include <QMouseEvent>

// QuickQanava headers
#include "./qanTableBorder.h"
#include "./qanTableCell.h"
#include "./qanTableGroupItem.h"

namespace qan {  // ::qan

/* TableBorder Object Management *///-----------------------------------
TableBorder::TableBorder(QQuickItem* parent) :
    QQuickItem{parent}
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptHoverEvents(true);

    connect(this,   &QQuickItem::xChanged,
            this,   &TableBorder::onHorizontalMove);
    connect(this,   &QQuickItem::yChanged,
            this,   &TableBorder::onVerticalMove);
}

TableBorder::~TableBorder() { /* prevCells and nextCells are not owned */ }
//-----------------------------------------------------------------------------

/* Border Management *///------------------------------------------------------
void    TableBorder::setTableGroup(const qan::TableGroup* tableGroup)
{
    _tableGroup = tableGroup;
}

bool    TableBorder::setOrientation(Qt::Orientation orientation)
{
    if (orientation != _orientation) {
        _orientation = orientation;
        emit orientationChanged();
        return true;
    }
    return false;
}

void    TableBorder::setBorderColor(QColor borderColor)
{
    if (!borderColor.isValid())
        return;
    if (borderColor != _borderColor) {
        _borderColor = borderColor;
        emit borderColorChanged();
    }
}

void    TableBorder::setBorderWidth(qreal borderWidth)
{
    if (!qFuzzyCompare(1.0 + borderWidth, 1.0 + _borderWidth)) {
        _borderWidth = borderWidth;
        emit borderWidthChanged();
    }
}

void    TableBorder::setPrevBorder(qan::TableBorder* prevBorder)
{
    _prevBorder = prevBorder;
}
void    TableBorder::setNextBorder(qan::TableBorder* nextBorder)
{
    _nextBorder = nextBorder;
}

void    TableBorder::addPrevCell(qan::TableCell* prevCell)
{
    if (prevCell != nullptr)
        _prevCells.push_back(prevCell);
}
void    TableBorder::addNextCell(qan::TableCell* nextCell)
{
    if (nextCell != nullptr)
        _nextCells.push_back(nextCell);
}

void    TableBorder::onHorizontalMove()
{
    const auto tableGroupItem = _tableGroup ? qobject_cast<const qan::TableGroupItem*>(_tableGroup->getGroupItem()) :
                                              nullptr;
    const auto spacing = 10.;  // FIXME #190 maintain a link to table item...
    const auto spacing2 = spacing / 2.;
    if (getOrientation() == Qt::Vertical) {
        const auto verticalCenter = x() + (width() / 2.);

        // Layout prev/next cells position and size
        for (auto prevCell: _prevCells) {
            prevCell->setWidth(verticalCenter -
                               prevCell->x() - spacing2);
            if (!_prevBorder &&     // For first column, set cell x too (not prev border will do it).
                _tableGroup) {
                prevCell->setX(_tableGroup->getTablePadding());
            }
        }
        for (auto nextCell: _nextCells) {
            nextCell->setX(verticalCenter + spacing2);
            if (!_nextBorder &&     // For last column, set cell witdh too (no next border will do it).
                _tableGroup && tableGroupItem != nullptr) {
                const auto padding = _tableGroup->getTablePadding();
                nextCell->setWidth(tableGroupItem->width() - verticalCenter - padding - spacing2);
            }
        }
    }
}

void    TableBorder::onVerticalMove()
{
    if (getOrientation() == Qt::Horizontal) {

    }
}
//-----------------------------------------------------------------------------

/* Border Events Management *///-----------------------------------------------
void    TableBorder::hoverEnterEvent(QHoverEvent* event)
{
    if (isVisible()) {
        if (getOrientation() == Qt::Vertical)
            setCursor(Qt::SplitHCursor);
        else if (getOrientation() == Qt::Horizontal)
                      setCursor(Qt::SplitVCursor);
        event->setAccepted(true);
    }
}

void    TableBorder::hoverLeaveEvent(QHoverEvent* event)
{
    setCursor(Qt::ArrowCursor);
    event->setAccepted(true);
}

void    TableBorder::mouseMoveEvent(QMouseEvent* event)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const auto mePos = event->windowPos();
#else
    const auto mePos = event->scenePosition();
#endif
    if (event->buttons() |  Qt::LeftButton &&
        !_dragInitialMousePos.isNull()) {

        const QPointF startLocalPos = parentItem() != nullptr ? parentItem()->mapFromScene(_dragInitialMousePos) :
                                                                QPointF{.0, 0.};
        const QPointF curLocalPos = parentItem() != nullptr ? parentItem()->mapFromScene(mePos) :
                                                              QPointF{0., 0.};
        const QPointF delta{curLocalPos - startLocalPos};

        // Bound to ] prevCells.left(), nextCells.right() [
        const auto spacing = 10;                // FIXME #190: Add link to table...
        const auto spacing2 = spacing / 2.;

        auto minX = std::numeric_limits<qreal>::max();
        for (auto prevCell: _prevCells)
            minX = std::min(minX, prevCell->x());
        minX += spacing2;

        auto maxX = std::numeric_limits<qreal>::min();
        for (auto nextCell: _nextCells)
            maxX = std::max(maxX, nextCell->x() + nextCell->width());
        maxX -= spacing2;

        qreal minY = 0;         // FIXME #190 Add horizontal border support...
        qreal maxY = 100000;

        const auto position = _dragInitialPos + delta;
        if (getOrientation() == Qt::Vertical)
            setX(qBound(minX, position.x(), maxX));
        else if (getOrientation() == Qt::Horizontal)
            setY(qBound(minY, position.y(), maxY));
        event->setAccepted(true);
    }
}

void    TableBorder::mousePressEvent(QMouseEvent* event)
{
    if (!isVisible())
        return;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const auto mePos = event->windowPos();
#else
    const auto mePos = event->scenePosition();
#endif
    _dragInitialMousePos = mePos;
    _dragInitialPos = position();
    event->setAccepted(true);
}

void    TableBorder::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    _dragInitialMousePos = {0., 0.};       // Invalid all cached coordinates when button is released
    _dragInitialPos = {0., 0.};
}
//-------------------------------------------------------------------------

} // ::qan
