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
// \file	qanGroup.cpp
// \author	benoit@destrat.io
// \date	2016 03 22
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanGroup.h"
#include "./qanGroupItem.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Group Object Management *///------------------------------------------------
Group::Group( QObject* parent ) :
    gtpo::GenGroup< qan::GraphConfig >{}
{
}

Group::~Group() { /* Nil */ }

qan::Graph*         Group::getGraph() noexcept {
    return qobject_cast< qan::Graph* >( gtpo::GenGroup< qan::GraphConfig >::getGraph() );
}

const qan::Graph*   Group::getGraph() const noexcept {
    return qobject_cast< const qan::Graph* >( gtpo::GenGroup< qan::GraphConfig >::getGraph() );
}

void    Group::setItem(qan::GroupItem* item) noexcept
{
    if ( item != nullptr ) {
        _item = item;
        if ( item->getGroup() != this )
            item->setGroup(this);
    }
}
//-----------------------------------------------------------------------------

/* Group Nodes Management *///-------------------------------------------------
bool    Group::hasNode( qan::Node* node ) const
{
    if ( node == nullptr )
        return false;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return false; }
    return gtpo::GenGroup< qan::GraphConfig >::hasNode( weakNode );
}
//-----------------------------------------------------------------------------

/* Group Appearance Management *///--------------------------------------------
void    Group::setCollapsed( bool collapsed )
{
    if ( collapsed != _collapsed ) {
        _collapsed = collapsed;
        for ( auto weakEdge : getAdjacentEdges() ) {    // When a group is collapsed, all adjacent edges shouldbe hidden/shown...
            auto edge = weakEdge.lock().get();
            // FIXME QAN3
            //if ( edge &&
                 //static_pointer_cast<>(edge)->getItem() )
            //    edge->getItem()->setVisible( !collapsed );
        }
        emit collapsedChanged();
    }
}
//-----------------------------------------------------------------------------

} // ::qan
