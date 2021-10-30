/*
 Copyright (c) 2008-2021, Benoit AUTHEMAN All rights reserved.

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
// \file	qanEdgeDraggableCtrl.cpp
// \author	benoit@destrat.io
// \date	2021 10 29
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "./qanEdgeDraggableCtrl.h"
#include "./qanEdgeItem.h"
#include "./qanGraph.h"

namespace qan { // ::qan

qan::Graph*  EdgeDraggableCtrl::getGraph() noexcept
{
    return _targetItem ? _targetItem->getGraph() : nullptr;
}

/* Drag'nDrop Management *///--------------------------------------------------
bool    EdgeDraggableCtrl::handleDragEnterEvent(QDragEnterEvent* event) { Q_UNUSED(event); return false; }

void	EdgeDraggableCtrl::handleDragMoveEvent(QDragMoveEvent* event) { Q_UNUSED(event) }

void	EdgeDraggableCtrl::handleDragLeaveEvent(QDragLeaveEvent* event) { Q_UNUSED(event) }

void    EdgeDraggableCtrl::handleDropEvent(QDropEvent* event) { Q_UNUSED(event) }

void    EdgeDraggableCtrl::handleMouseDoubleClickEvent(QMouseEvent* event) { Q_UNUSED(event) }

bool    EdgeDraggableCtrl::handleMouseMoveEvent(QMouseEvent* event)
{
    // PRECONDITIONS:
        // graph must be non nullptr (configured).
        // _targetItem can't be nullptr
    if (!_targetItem)
        return false;
    const auto graph = _targetItem->getGraph();
    if (graph == nullptr)
        return false;

    // Early exits
    if (event->buttons().testFlag(Qt::NoButton))
        return false;
    if (!_targetItem->getDraggable())
        return false;

    const auto rootItem = graph->getContainerItem();
    if (rootItem != nullptr &&      // Root item exist, left button is pressed and the target item
        event->buttons().testFlag(Qt::LeftButton)) {    // is draggable and not collapsed
        const auto globalPos = rootItem->mapFromGlobal(event->globalPos());
        if (!_targetItem->getDragged()) {
            beginDragMove(globalPos, false);  // false = no selection (no edge selection support)
            return true;
        } else {
            const auto delta = globalPos - _dragLastPos;
            _dragLastPos = globalPos;
            dragMove(delta, false);  // false = no selection (no edge selection support)
            return true;
        }
    }
    return false;
}

void    EdgeDraggableCtrl::handleMousePressEvent(QMouseEvent* event) { Q_UNUSED(event) }

void    EdgeDraggableCtrl::handleMouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    if (_targetItem &&
        _targetItem->getDragged())
        endDragMove();
}

void    EdgeDraggableCtrl::beginDragMove(const QPointF& dragInitialMousePos, bool dragSelection)
{
    Q_UNUSED(dragSelection)
    if (_targetItem == nullptr)
        return;
    _targetItem->setDragged(true);
    _dragLastPos = dragInitialMousePos;

    // Get target edge adjacent nodes
    auto src = _targetItem->getSourceItem()->getNode()->getItem();
    auto dst = _targetItem->getDestinationItem()->getNode()->getItem();
    src->draggableCtrl().beginDragMove(dragInitialMousePos, false);
    dst->draggableCtrl().beginDragMove(dragInitialMousePos, false);
}

void    EdgeDraggableCtrl::dragMove(const QPointF& delta, bool dragSelection)
{
    Q_UNUSED(dragSelection)
    // PRECONDITIONS:
        // _targetItem must be configured
    if (!_targetItem)
        return;
    // Get target edge adjacent nodes
    auto src = _targetItem->getSourceItem()->getNode()->getItem();
    auto dst = _targetItem->getDestinationItem()->getNode()->getItem();
    src->draggableCtrl().dragMove(delta, false);
    dst->draggableCtrl().dragMove(delta, false);
}

void    EdgeDraggableCtrl::endDragMove(bool dragSelection)
{
    Q_UNUSED(dragSelection)
    if (!_targetItem)
        return;

    // FIXME #141 add an edgeMoved() signal...
    //emit graph->nodeMoved(_target);
    _targetItem->setDragged(false);

    // Get target edge adjacent nodes
    auto src = _targetItem->getSourceItem()->getNode()->getItem();
    auto dst = _targetItem->getDestinationItem()->getNode()->getItem();
    src->draggableCtrl().endDragMove(false);
    dst->draggableCtrl().endDragMove(false);
}
//-----------------------------------------------------------------------------

} // ::qan
