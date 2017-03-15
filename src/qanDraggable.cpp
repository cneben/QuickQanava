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
// \file	qanDraggable.cpp
// \author	benoit@destrat.io
// \date	2016 03 15
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "./qanDraggable.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
Draggable::Draggable() { /* Nil */ }
Draggable::~Draggable() { /* Nil */ }

void    Draggable::configure(QQuickItem* target )
{
    _target = target;
}
//-----------------------------------------------------------------------------

/* Draggable Management *///---------------------------------------------------
void    Draggable::setDraggable( bool draggable ) noexcept
{
    if ( draggable != _draggable ) {
        _draggable = draggable;
        if ( !draggable ) {
            setDragged(false);
            // FIXME QAN3
            //endDragMove();
        }
        emitDraggableChanged();
    }
}

void    Draggable::setDragged( bool dragged ) noexcept
{
    if ( dragged != _dragged ) {
        _dragged = dragged;
        emitDraggedChanged( );
    }
}

void    Draggable::setDroppable( bool droppable ) noexcept
{
    if ( droppable != _droppable ) {
        _droppable = droppable;
        emitDroppableChanged();
    }
}

void    Draggable::setAcceptDrops( bool acceptDrops ) noexcept
{
    if ( acceptDrops != _acceptDrops ) {
        _acceptDrops = acceptDrops;
        if ( acceptDrops &&
             _target &&
             !(_target->flags().testFlag(QQuickItem::ItemAcceptsDrops)) )
            _target->setFlag( QQuickItem::ItemAcceptsDrops, acceptDrops );
        emitAcceptDropsChanged();
    }
}
//-----------------------------------------------------------------------------

} // ::qan
