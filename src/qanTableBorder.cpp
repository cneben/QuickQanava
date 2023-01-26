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

// Qt headers
#include <QCursor>
#include <QMouseEvent>

// QuickQanava headers
#include "./qanTableBorder.h"
#include "./qanTableCell.h"

namespace qan {  // ::qan

/* TableBorder Object Management *///-----------------------------------
TableBorder::TableBorder(QQuickItem* parent) :
    QQuickItem{parent}
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptHoverEvents(true);
}

TableBorder::~TableBorder() { /* prevCells and nextCells are not owned */ }
//-----------------------------------------------------------------------------


/* Border Management *///-----------------------------------------------------
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

void    TableBorder::addPrevCell(qan::TableCell* prevCell)
{
    std::vector<qan::TableCell*>    _prevCells;
    std::vector<qan::TableCell*>    _nextCells;
}
void    TableBorder::addNextCell(qan::TableCell* nextCell)
{

}

//-----------------------------------------------------------------------------

/* Border Events Management *///-----------------------------------------------
void    TableBorder::hoverEnterEvent(QHoverEvent* event)
{
    if (isVisible()) {
        setCursor(Qt::SizeFDiagCursor);
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
        !_dragInitialPos.isNull()) {
        const QPointF startLocalPos = parentItem() != nullptr ? parentItem()->mapFromScene(_dragInitialPos) :
                                                                QPointF{.0, 0.};
        const QPointF curLocalPos = parentItem() != nullptr ? parentItem()->mapFromScene(mePos) :
                                                              QPointF{0., 0.};
        const QPointF delta{curLocalPos - startLocalPos};

        setPosition(position() + delta);  // FIXME: vraiment pas souhaitable

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
    _dragInitialPos = mePos;
    //emit resizeStart(_target ? QSizeF{_target->width(), _target->height()} :
    //                           QSizeF{});
    event->setAccepted(true);
}

void    TableBorder::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    _dragInitialPos = {0., 0.};       // Invalid all cached coordinates when button is released
}
//-------------------------------------------------------------------------

} // ::qan
