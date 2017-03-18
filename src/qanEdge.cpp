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
    gtpo::GenEdge< qan::GraphConfig >{}
{
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

/* Node Static Factories *///--------------------------------------------------
static std::unique_ptr<QQmlComponent>   qan_Edge_delegate;
static std::unique_ptr<qan::EdgeStyle>  qan_Edge_style;

QQmlComponent*  Edge::delegate(QObject* caller) noexcept
{
    if ( !qan_Edge_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            qan_Edge_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/QuickQanava/Edge.qml");
    }
    return qan_Edge_delegate.get();
}

qan::EdgeStyle* Edge::style() noexcept
{
    if ( !qan_Edge_style )
        qan_Edge_style = std::make_unique<qan::EdgeStyle>();
    return qan_Edge_style.get();
}
//-----------------------------------------------------------------------------

/* Edge Properties Management *///---------------------------------------------
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
