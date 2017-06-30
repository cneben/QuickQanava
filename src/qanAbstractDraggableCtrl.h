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
// \file	qanAbstractDraggableCtrl.h
// \author	benoit@destrat.io
// \date	2017 06 30
//-----------------------------------------------------------------------------

#ifndef qanAbstractDraggableCtrl_h
#define qanAbstractDraggableCtrl_h

// Qt headers
#include <QPointF>

// QuickQanava headers
// Nil

namespace qan { // ::qan

/*! \brief Generic logic for dragging either qan::Node or qan::Group visual items.
 *
 * \nosubgrouping
 */
class AbstractDraggableCtrl
{
public:
    //! \c dragInitialMousePos in window coordinate system.
    virtual void    beginDragMove( const QPointF& dragInitialMousePos, bool dragSelection = true ) = 0;
    //! \c delta in scene coordinate system.
    virtual void    dragMove( const QPointF& dragInitialMousePos, const QPointF& delta, bool dragSelection = true ) = 0;
    virtual void    endDragMove( bool dragSelection = true ) = 0;
};

} // ::qan

#endif // qanAbstractDraggableCtrl_h
