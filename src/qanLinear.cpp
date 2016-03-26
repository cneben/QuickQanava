/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

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
// This file is a part of the Qanava software.
//
// \file	qanLinear.cpp
// \author	benoit@qanava.org
// \date	2016 03 23
//-----------------------------------------------------------------------------

// Qanava headers
#include "./qanLinear.h"

namespace qan { // ::qan

/* Linear Layout Management *///-----------------------------------------------
Linear::Linear( QObject* parent ) :
    Layout( parent ),
    _spacing( 25 )
{
    connect( this, SIGNAL( orientationChanged( ) ), this, SLOT( layout( ) ) );
}

void    Linear::layout( )
{
    if ( !isEnabled( ) )
        return;
    if ( getOrientation( ) == Qt::Horizontal ) {
        qreal x = 0.;
        /*foreach ( Node* node, *_nodes ) {
            node->setPosition( QPointF( x, 0 ) );
            x += node->boundingRect( ).width( ) + _spacing;
        }*/
    }
    else if ( getOrientation( ) == Qt::Vertical ) {
        qreal y = 0.;
        /*foreach ( Node* node, *_nodes ) {
            node->setPosition( QPointF( 0, y ) );
            y += node->boundingRect( ).height( ) + _spacing;
        }*/
    }
}

void    Linear::proposeNodeDrop( QQuickItem* layoutItem, qan::Node* node, qan::Node* shadowNode )
{
    // Find previous and next nodes
    QPointF nodePos = node->mapToItem( layoutItem, QPointF( 0, 0 ) );
    QPointF previousPos;
    if ( getOrientation( ) == Qt::Horizontal )
        previousPos = QPointF( nodePos.x() - 1.0, nodePos.y( ) );
    else if ( getOrientation( ) == Qt::Vertical )
        previousPos = QPointF( );
    QQuickItem* previousItem = layoutItem->childAt( previousPos.x( ), previousPos.y( ) );

    QPointF nextPos;
    if ( getOrientation( ) == Qt::Horizontal )
        nextPos = QPointF( nodePos.x() - node->width( ) + 1., nodePos.y( ) );
    else if ( getOrientation( ) == Qt::Vertical )
        nextPos = QPointF( );
    QQuickItem* nextItem = layoutItem->childAt( nextPos.x( ), nextPos.y( ) );
    Q_UNUSED( nextItem );
    //qDebug( ) << "layoutItem->childAt( " << previousPos.x( ) << ", " << previousPos.y( ) << " ) = " << previousItem;
    //qDebug( ) << "layoutItem->childAt( " << nextPos.x( ) << ", " << nextPos.y( ) << " ) = " << nextItem;

    if ( previousItem != nullptr && previousItem->inherits( "qan::Node" ) )
    {
        qan::Node* previousNode = qobject_cast< qan::Node* >( previousItem );
        if ( previousNode != nullptr )
        {
            //qDebug( ) << "Previous node=" << previousNode->getLabel( );
            // FIXME: There is need for deep modification of GTPO here
            // because insert is not actually supported for generic container
            // access...
                // It is however not a problem to implement it...
            //_nodes->move( _nodes->indexOf( shadowNode ), _nodes->indexOf( previousNode ) );
            layout( );
        }
    }
}
//-----------------------------------------------------------------------------

} // ::qan
