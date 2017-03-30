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
// \file	qanDraggableCtrl.h
// \author	benoit@destrat.io
// \date	2017 03 15
//-----------------------------------------------------------------------------

#ifndef qanDraggableCtrl_h
#define qanDraggableCtrl_h

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>
#include <QDrag>
#include <QPointer>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanStyle.h"         // Used in handleDropEvent()
#include "./qanGroup.h"

namespace qan { // ::qan

/*! \brief Generic logic for dragging either qan::Node or qan::Group visual items.
 *
 * \nosubgrouping
 */
template < class Node_t, class NodeItem_t >
class DraggableCtrl
{
    /*! \name Node Object Management *///--------------------------------------
    //@{
public:
    //! DraggableCtrl constructor.
    explicit DraggableCtrl();
    virtual ~DraggableCtrl();
    DraggableCtrl( const DraggableCtrl& ) = delete;

public:
    inline auto getTarget() noexcept -> Node_t* { return _target.data(); }
    inline auto getTarget() const noexcept -> const Node_t* { return _target.data(); }
    inline auto setTarget(Node_t* target) noexcept { _target = target; }
private:
    QPointer<Node_t>    _target{nullptr};

public:
    inline auto getTargetItem() noexcept -> NodeItem_t* { return _targetItem.data(); }
    inline auto getTargetItem() const noexcept -> const NodeItem_t* { return _targetItem.data(); }
    inline auto setTargetItem(NodeItem_t* targetItem) noexcept { _targetItem = targetItem; }
private:
    QPointer<NodeItem_t>    _targetItem{nullptr};

protected:
    inline qan::Graph*  getGraph() noexcept { return _target ? _target->getGraph() : nullptr; }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node DnD Management *///-----------------------------------------
    //@{
public:
    bool    handleDragEnterEvent( QDragEnterEvent* event );
    void    handleDragMoveEvent( QDragMoveEvent* event );
    void    handleDragLeaveEvent( QDragLeaveEvent* event );
    void    handleDropEvent( QDropEvent* event );

    void    handleMouseDoubleClickEvent( QMouseEvent* event );
    bool    handleMouseMoveEvent( QMouseEvent* event );
    void    handleMousePressEvent( QMouseEvent* event );
    void    handleMouseReleaseEvent( QMouseEvent* event );

public:
    //! \c dragInitialMousePos in window coordinate system.
    auto    beginDragMove( const QPointF& dragInitialMousePos, bool dragSelection = true ) -> void;
    //! \c delta in scene coordinate system.
    auto    dragMove( const QPointF& dragInitialMousePos, const QPointF& delta, bool dragSelection = true ) -> void;
    auto    endDragMove( bool dragSelection = true ) -> void;

private:
    //! Initial global mouse position at the beginning of a node drag operation.
    QPointF                 _dragInitialMousePos{ 0., 0. };
    //! Node position at the beginning of a node drag.
    QPointF                 _dragInitialPos{ 0., 0. };
    //! Last group hovered during a node drag (cached to generate a dragLeave signal on qan::Group).
    QPointer<qan::Group>    _lastProposedGroup{ nullptr };
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

#include "./qanDraggableCtrl.hpp"

#endif // qanDraggableCtrl_h
