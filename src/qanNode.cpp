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
// \file	qanNode.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QPainter>
#include <QPainterPath>

// QuickQanava headers
#include "./qanNode.h"
#include "./qanNodeItem.h"
#include "./qanGroup.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
Node::Node(QObject* parent) :
    gtpo::GenNode< qan::GraphConfig >{}
{
    Q_UNUSED(parent)
}

Node::~Node()
{
    if ( _item )
        _item->deleteLater();
}

qan::Graph* Node::getGraph() noexcept {
    return qobject_cast< qan::Graph* >( gtpo::GenNode< qan::GraphConfig >::getGraph() );
}

const qan::Graph* Node::getGraph() const noexcept {
    return qobject_cast< const qan::Graph* >( gtpo::GenNode< qan::GraphConfig >::getGraph() );
}

bool    Node::operator==( const qan::Node& right ) const
{
    return getLabel() == right.getLabel();
}

void    Node::setItem(qan::NodeItem* nodeItem) noexcept
{
    if ( nodeItem != nullptr ) {
        _item = nodeItem;
        if ( nodeItem->getNode() != this )
            nodeItem->setNode(this);
    }
}
//-----------------------------------------------------------------------------

/* Node Static Factories *///--------------------------------------------------
static std::unique_ptr<QQmlComponent>   qan_Node_delegate;
static std::unique_ptr<qan::NodeStyle>  qan_Node_style;

QQmlComponent*  Node::delegate(QObject* caller) noexcept
{
    if ( !qan_Node_delegate &&
         caller != nullptr ) {
        const auto engine = qmlEngine(caller);
        if ( engine != nullptr )
            qan_Node_delegate = std::make_unique<QQmlComponent>(engine, "qrc:/QuickQanava/Node.qml");
    }
    return qan_Node_delegate.get();
}

qan::NodeStyle* Node::style() noexcept
{
    if ( !qan_Node_style )
        qan_Node_style = std::make_unique<qan::NodeStyle>();
    return qan_Node_style.get();
}
//-----------------------------------------------------------------------------

/* Behaviours Management *///--------------------------------------------------
void    Node::installBehaviour( std::unique_ptr<qan::NodeBehaviour> behaviour )
{
    addBehaviour( std::move( behaviour ) );
}
//-----------------------------------------------------------------------------

} // ::qan
