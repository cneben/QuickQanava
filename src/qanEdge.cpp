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
// \file	qanEdge.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanNode.h"
#include "./qanGroup.h"
#include "./qanEdge.h"
#include "./qanEdgeItem.h"

namespace qan { // ::qan

/* Edge Object Management *///-------------------------------------------------
Edge::Edge() :
    gtpo::GenEdge< qan::GraphConfig >{},
    _defaultStyle{ new qan::EdgeStyle{ "", "qan::Edge" } }
{
    setStyle( _defaultStyle.data() );
}

qan::Graph* Edge::getGraph() noexcept {
    return qobject_cast< qan::Graph* >( gtpo::GenEdge< qan::GraphConfig >::getGraph() );
}

const qan::Graph* Edge::getGraph() const noexcept {
    return qobject_cast< const qan::Graph* >( gtpo::GenEdge< qan::GraphConfig >::getGraph() );
}

void    Edge::setItem(qan::EdgeItem* edgeItem) noexcept
{
    if ( edgeItem != nullptr )
        _item = edgeItem;
}
//-----------------------------------------------------------------------------

/* Style and Properties Management *///----------------------------------------
void    Edge::setStyle( EdgeStyle* style )
{
    if ( style == _style )
        return;
    if ( style == nullptr )
        style = _defaultStyle.data();
    if ( _style != nullptr && _style != _defaultStyle.data() )  // Every style that is non default is disconnect from this node
        QObject::disconnect( _style, 0, this, 0 );
    _style = style;
    connect( _style, &QObject::destroyed, this, &Edge::styleDestroyed );    // Monitor eventual style destruction
    // FIXME QAN3
    //connect( _style, &qan::EdgeStyle::styleModified, this, &Edge::updateItem );
    emit styleChanged( );
}

void    Edge::styleDestroyed( QObject* style )
{
    if ( style != nullptr && style != _defaultStyle.data() ) {
        QObject::disconnect( style, 0, this, 0 );
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
    }
}

void    Edge::setLabel( const QString& label )
{
    if ( label != _label ) {
        _label = label;
        emit labelChanged( );
    }
}

void     Edge::setWeight( qreal weight )
{
    if ( !qFuzzyCompare( 1.0 + weight, 1.0 + _weight ) ) {
        _weight = weight;
        emit weightChanged();
    }
}
//-----------------------------------------------------------------------------

} // ::qan
