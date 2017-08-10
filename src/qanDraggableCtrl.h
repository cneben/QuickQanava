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
#include "./qanAbstractDraggableCtrl.h"
#include "./qanStyle.h"         // Used in handleDropEvent()
#include "./qanGroup.h"

namespace qan { // ::qan

/*! \brief Generic logic for dragging either qan::Node or qan::Group visual items.
 *
 * \nosubgrouping
 */
template < class Node_t, class NodeItem_t >
class DraggableCtrl : public qan::AbstractDraggableCtrl
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
    virtual void    beginDragMove( const QPointF& dragInitialMousePos, bool dragSelection = true ) override;
    //! \c delta in scene coordinate system.
    virtual void    dragMove( const QPointF& dragInitialMousePos, const QPointF& delta, bool dragSelection = true ) override;
    virtual void    endDragMove( bool dragSelection = true ) override;

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
