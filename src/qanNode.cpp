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
    gtpo::GenNode< qan::GraphConfig >{},
    _defaultStyle{ new qan::NodeStyle{ "", QStringLiteral("qan::Node") } },
    _style{ nullptr }
{
    Q_UNUSED(parent)
    setStyle( _defaultStyle.data() );
    //setResizable( true );
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

/* Behaviours Management *///--------------------------------------------------
void    Node::installBehaviour( std::unique_ptr<qan::NodeBehaviour> behaviour )
{
    addBehaviour( std::move( behaviour ) );
}
//-----------------------------------------------------------------------------

/* Appearance Management *///--------------------------------------------------
void    Node::setStyle( NodeStyle* style )
{
    if ( style == _style )
        return;
    if ( style == nullptr )
        style = _defaultStyle.data();
    if ( _style != nullptr &&
         _style != _defaultStyle.data() )  // Every style that is non default is disconnect from this node
        QObject::disconnect( _style, 0, this, 0 );
    _style = style;
    connect( _style, &QObject::destroyed, this, &Node::styleDestroyed );    // Monitor eventual style destruction
    emit styleChanged( );
}

void    Node::styleDestroyed( QObject* style )
{
    if ( style != nullptr &&
         style != _defaultStyle.data() )
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
}
//-----------------------------------------------------------------------------

/* Node Group Management *///--------------------------------------------------
// FIXME QAN3
/*void    Node::ungroup( )
{
    if ( getQanGroup() != nullptr &&
         getGraph()->hasGroup( getQanGroup() ) )
        getQanGroup()->removeNode( this );
}

qan::Group* Node::getQanGroup( )
{
    WeakGroup weakGroup = gtpo::GenNode<qan::GraphConfig>::getGroup();
    if ( weakGroup.expired() )
        return nullptr;
    return weakGroup.lock().get();
}
*/
//-----------------------------------------------------------------------------

} // ::qan
