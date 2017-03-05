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
#include "./qanEdge.h"
#include "./qanEdgeItem.h"
#include "./qanGroup.h"
#include "./qanNode.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Edge Object Management *///-------------------------------------------------
Edge::Edge() :
    gtpo::GenEdge< qan::GraphConfig >{},
    _defaultStyle{ new qan::EdgeStyle{ "", "qan::Edge" } }
{
    setStyle( _defaultStyle.data() );
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

/* Drag'nDrop Management *///--------------------------------------------------
// FIXME QAN3
/*
bool    Edge::contains( const QPointF& point ) const
{
    qreal d = distanceFromLine( point, QLineF{_p1, _p2} );
    return ( d > 0. && d < 5. );
}

void    Edge::dragEnterEvent( QDragEnterEvent* event )
{
    // Note 20160218: contains() is used so enter event is necessary generated "on the edge line"
    if ( _acceptDrops ) {
        if ( event->source() == nullptr ) {
            event->accept(); // This is propably a drag initated with type=Drag.Internal, for exemple a connector drop node trying to create an hyper edge, accept by default...
            QQuickItem::dragEnterEvent( event );
            return;
        }

        if ( event->source() != nullptr ) { // Get the source item from the quick drag attached object received
            QVariant source = event->source()->property( "source" );
            if ( source.isValid() ) {
                QQuickItem* sourceItem = source.value< QQuickItem* >( );
                QVariant draggedStyle = sourceItem->property( "draggedEdgeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
                if ( draggedStyle.isValid() ) {
                    event->accept();
                    return;
                }
            }
        }
        event->ignore();
        QQuickItem::dragEnterEvent( event );
    }
    QQuickItem::dragEnterEvent( event );
}

void	Edge::dragMoveEvent( QDragMoveEvent* event )
{
    if ( getAcceptDrops() ) {
        qreal d = distanceFromLine( event->posF( ), QLineF{_p1, _p2} );
        if ( d > 0. && d < 5. )
            event->accept();
        else event->ignore();
    }
    QQuickItem::dragMoveEvent( event );
}

void	Edge::dragLeaveEvent( QDragLeaveEvent* event )
{
    QQuickItem::dragLeaveEvent( event );
    if ( getAcceptDrops() )
        event->ignore();
}

void    Edge::dropEvent( QDropEvent* event )
{
    if ( getAcceptDrops() && event->source() != nullptr ) { // Get the source item from the quick drag attached object received
        QVariant source = event->source()->property( "source" );
        if ( source.isValid() ) {
            QQuickItem* sourceItem = source.value< QQuickItem* >( );
            QVariant draggedStyle = sourceItem->property( "draggedEdgeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
            if ( draggedStyle.isValid() ) {
                qan::EdgeStyle* draggedEdgeStyle = draggedStyle.value< qan::EdgeStyle* >( );
                if ( draggedEdgeStyle != nullptr ) {
                    setStyle( draggedEdgeStyle );
                    event->accept();
                }
            }
        }
    }
    QQuickItem::dropEvent( event );
}
*/
//-----------------------------------------------------------------------------

} // ::qan
