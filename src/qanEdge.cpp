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
// This file is a part of the QuickQanava software library.
//
// \file	qanEdge.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>

// Qanava headers
#include "./qanEdge.h"
#include "./qanGroup.h"
#include "./qanNode.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Edge Object Management *///-------------------------------------------------
Edge::Edge( QQuickItem* parent ) :
    gtpo::GenEdge< qan::Config >{},
    _defaultStyle{ new qan::EdgeStyle{ "", "qan::Edge" } }
{
    setStyle( _defaultStyle.data() );
    setParentItem( parent );
    setAntialiasing( true );
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::RightButton | Qt::LeftButton );
    setAcceptDrops( true );
}
//-----------------------------------------------------------------------------

/* Edge Topology Management *///-----------------------------------------------
auto    Edge::setSourceItem( qan::Node* source ) -> void
{
    if ( source == nullptr )
        return;

    // Connect dst x and y monitored properties change notify signal to slot updateEdge()
    QMetaMethod updateItemSlot = metaObject()->method( metaObject()->indexOfSlot( "updateItemSlot()" ) );
    if ( updateItemSlot.isValid() ) {  // Connect src and dst x and y monitored properties change notify signal to slot updateItemSlot()
        auto srcMetaObj = source->metaObject();
        QMetaProperty srcX      = srcMetaObj->property( srcMetaObj->indexOfProperty( "x" ) );
        QMetaProperty srcY      = srcMetaObj->property( srcMetaObj->indexOfProperty( "y" ) );
        QMetaProperty srcZ      = srcMetaObj->property( srcMetaObj->indexOfProperty( "z" ) );
        QMetaProperty srcWidth  = srcMetaObj->property( srcMetaObj->indexOfProperty( "width" ) );
        QMetaProperty srcHeight = srcMetaObj->property( srcMetaObj->indexOfProperty( "height" ) );
        if ( !srcX.isValid() || !srcX.hasNotifySignal() ) {
            qDebug() << "qan::Edge::setSourceItem(): Error: can't access source x property.";
            return;
        }
        if (  !srcY.isValid() || !srcY.hasNotifySignal() ) {
            qDebug() << "qan::Edge::setSourceItem(): Error: can't access source y property.";
            return;
        }
        if (  !srcWidth.isValid() || !srcWidth.hasNotifySignal() ) {
            qDebug() << "qan::Edge::setSourceItem(): Error: can't access source width property.";
            return;
        }
        if (  !srcHeight.isValid() || !srcHeight.hasNotifySignal() ) {
            qDebug() << "qan::Edge::setSourceItem(): Error: can't access source height property.";
            return;
        }
        connect( source, srcX.notifySignal(),       this, updateItemSlot );
        connect( source, srcY.notifySignal(),       this, updateItemSlot );
        connect( source, srcZ.notifySignal(),       this, updateItemSlot );
        connect( source, srcWidth.notifySignal(),   this, updateItemSlot );
        connect( source, srcHeight.notifySignal(),  this, updateItemSlot );
        emit sourceItemChanged();
        if ( source->z() < z() )
            setZ( source->z() );
        updateItem();
    }
}

auto    Edge::setDestinationItem( qan::Node* destination ) -> void
{
    if ( destination == nullptr )
        return;

    // Connect dst x and y monitored properties change notify signal to slot updateItemSlot()
    QMetaMethod updateItemSlot = metaObject( )->method( metaObject( )->indexOfSlot( "updateItemSlot()" ) );
    if ( !updateItemSlot.isValid( ) ) {
        qDebug() << "qan::Edge::setDestinationItem(): Error: no access to edge updateItem slot.";
        return;
    }
    auto dstMetaObj = destination->metaObject( );
    QMetaProperty dstX      = dstMetaObj->property( dstMetaObj->indexOfProperty( "x" ) );
    QMetaProperty dstY      = dstMetaObj->property( dstMetaObj->indexOfProperty( "y" ) );
    QMetaProperty dstZ      = dstMetaObj->property( dstMetaObj->indexOfProperty( "z" ) );
    QMetaProperty dstWidth  = dstMetaObj->property( dstMetaObj->indexOfProperty( "width" ) );
    QMetaProperty dstHeight = dstMetaObj->property( dstMetaObj->indexOfProperty( "height" ) );
    if ( !dstX.isValid() || !dstX.hasNotifySignal() ) {
        qDebug() << "qan::Edge::setDestinationItem(): Error: can't access source x property.";
        return;
    }
    if (  !dstY.isValid() || !dstY.hasNotifySignal() ) {
        qDebug() << "qan::Edge::setDestinationItem(): Error: can't access source y property.";
        return;
    }
    if (  !dstWidth.isValid() || !dstWidth.hasNotifySignal() ) {
        qDebug() << "qan::Edge::setDestinationItem(): Error: can't access source width property.";
        return;
    }
    if (  !dstHeight.isValid() || !dstHeight.hasNotifySignal() ) {
        qDebug() << "qan::Edge::setDestinationItem(): Error: can't access source height property.";
        return;
    }
    connect( destination, dstX.notifySignal(),       this, updateItemSlot );
    connect( destination, dstY.notifySignal(),       this, updateItemSlot );
    connect( destination, dstZ.notifySignal(),       this, updateItemSlot );
    connect( destination, dstWidth.notifySignal(),   this, updateItemSlot );
    connect( destination, dstHeight.notifySignal(),  this, updateItemSlot );
    emit destinationItemChanged();
    if ( destination->z() < z() )
        setZ( destination->z() );
    updateItem( );
}
//-----------------------------------------------------------------------------

/* Edge Drawing Management *///------------------------------------------------
void    Edge::updateItem( )
{
    SharedNode ownedSource = getSrc().lock();
    SharedNode ownedDestination = getDst().lock();
    if ( ownedSource == nullptr ||
         ownedDestination == nullptr ) {
        update(); // Note 20160218: Force a simple update, since for example in edge style preview edge, there is no src and dst.
        return;
    }
    qan::Node* sourceItem = static_cast< qan::Node* >( ownedSource.get() );
    qan::Node* destinationItem = static_cast< qan::Node* >( ownedDestination.get() );
    if ( sourceItem == nullptr ||
         destinationItem == nullptr )
        return;

    // Update edge z to source or destination maximum x
    qreal srcZ = sourceItem->getQanGroup() != nullptr ? sourceItem->getQanGroup()->z() + sourceItem->z() :
                                                        sourceItem->z();
    qreal dstZ = destinationItem->getQanGroup() != nullptr ? destinationItem->getQanGroup()->z() + destinationItem->z() :
                                                             destinationItem->z();
    qreal edgeZ = qMax( srcZ, dstZ );
    setZ( edgeZ );

    // Compute a global bounding boxe according to the actual src and dst
    qan::Graph* qanGraph = static_cast< qan::Graph* >( getGraph() );
    QPointF srcPos = ownedSource->mapToItem( qanGraph->getContainerItem(), QPointF( 0, 0 ) );
    QPointF dstPos = destinationItem->mapToItem( qanGraph->getContainerItem(), QPointF( 0, 0 ) );

    QRectF srcBr{ srcPos, QSizeF( ownedSource->width( ), ownedSource->height( ) ) };
    QRectF dstBr{ dstPos, QSizeF( ownedDestination->width( ), ownedDestination->height( ) ) };

    QRectF br = srcBr.united( dstBr );
    setPosition( br.topLeft( ) );
    setSize( br.size( ) );

    // Mapping src shape polygon to this CCS
    QPolygonF srcBoundingShape{ownedSource->getBoundingShape().size()};
    int p = 0;
    for ( const auto& point: ownedSource->getBoundingShape() )
        srcBoundingShape[p++] = mapFromItem( ownedSource.get(), point );

    QPolygonF dstBoundingShape{ownedDestination->getBoundingShape().size()};
    p = 0;
    for ( const auto& point: ownedDestination->getBoundingShape() )
        dstBoundingShape[p++] = mapFromItem( ownedDestination.get(), point );

    // Works, but uncommented is probably faster. Uncomment for debugging
    //QPointF src = mapFromItem( getSrc( ), getSrc( )->boundingRect( ).center( ) );
    //QPointF dst = mapFromItem( getDst( ), getDst( )->boundingRect( ).center( ) );
    QPointF src = srcBoundingShape.boundingRect( ).center( );
    QPointF dst = dstBoundingShape.boundingRect( ).center( );
    QLineF line = getPolyLineIntersection( src, dst, srcBoundingShape, dstBoundingShape );
    _p1 = line.p1();
    emit p1Changed();
    _p2 = line.pointAt( 1 - 2 / line.length() );    // Note 20161001: Hack to take into account arrow border of 2px
    emit p2Changed();
    setLabelPos( line.pointAt( 0.5 ) + QPointF{10., 10.} );
}

void    Edge::setLine( QPoint src, QPoint dst )
{
    _p1 = src; emit p1Changed();
    _p2 = dst; emit p2Changed();
}

QLineF  Edge::getPolyLineIntersection( const QPointF& p1, const QPointF& p2,
                                       const QPolygonF& srcBp, const QPolygonF& dstBp ) const
{
    QLineF line{p1, p2};
    QPointF source{p1};
    QPointF intersection;
    for ( int p = 0; p < srcBp.length( ) - 1 ; p++ ) {
        QLineF polyLine( srcBp[ p ], srcBp[ p + 1 ] );
        if ( line.intersect( polyLine, &intersection ) == QLineF::BoundedIntersection ) {
            source = intersection;
            break;
        }
    }
    QPointF destination{p2};
    for ( int p = 0; p < dstBp.length( ) - 1 ; p++ ) {
        QLineF polyLine( dstBp[ p ], dstBp[ p + 1 ] );
        if ( line.intersect( polyLine, &intersection ) == QLineF::BoundedIntersection ) {
            destination = intersection;
            break;
        }
    }
    return QLineF{source, destination};
}
//-----------------------------------------------------------------------------

/* Mouse Management *///-------------------------------------------------------
void    Edge::mouseDoubleClickEvent( QMouseEvent* event )
{
    qreal d = distanceFromLine( event->localPos( ), QLineF{_p1, _p2} );
    if ( d >= 0. && d < 5. && event->button( ) == Qt::LeftButton ) {
        emit edgeDoubleClicked( QVariant::fromValue< qan::Edge* >( this ), QVariant( event->localPos( ) ) );
        event->accept( );
    }
    else
        event->ignore( );
    QQuickItem::mouseDoubleClickEvent( event );
}

void    Edge::mousePressEvent( QMouseEvent* event )
{
    qreal d = distanceFromLine( event->localPos( ), QLineF{_p1, _p2} );
    if ( d >= 0. && d < 5. ) {
        if ( event->button( ) == Qt::LeftButton ) {
            emit edgeClicked( QVariant::fromValue< qan::Edge* >( this ), QVariant( event->localPos( ) ) );
            event->accept( );
        }
        else if ( event->button( ) == Qt::RightButton ) {
            emit edgeRightClicked( QVariant::fromValue< qan::Edge* >( this ), QVariant( event->localPos( ) ) );
            event->accept( );
            return;
        }
    }
    else
        event->ignore( );
}

qreal   Edge::distanceFromLine( const QPointF& p, const QLineF& line ) const
{
    // Inspired by DistancePointLine Unit Test, Copyright (c) 2002, All rights reserved
    // Damian Coventry  Tuesday, 16 July 2002
    qreal lLenght = line.length( );
    qreal u  = ( ( ( p.x( ) - line.x1( ) ) * ( line.x2( ) - line.x1( ) ) ) +
                 ( ( p.y( ) - line.y1( ) ) * ( line.y2( ) - line.y1( ) ) ) ) /
                    ( lLenght * lLenght );

    if ( u < 0. || u > 1. )
        return -1;
    QPointF i;
    i.rx( ) = line.x1( ) + u * ( line.x2( ) - line.x1( ) );
    i.ry( ) = line.y1( ) + u * ( line.y2( ) - line.y1( ) );
    QLineF pi( p, i );
    return pi.length( );
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
    connect( _style, &qan::EdgeStyle::styleModified, this, &Edge::updateItem );
    emit styleChanged( );
}

void    Edge::styleDestroyed( QObject* style )
{
    if ( style != nullptr && style != _defaultStyle.data() ) {
        QObject::disconnect( style, 0, this, 0 );
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
    }
}
//-----------------------------------------------------------------------------

/* Drag'nDrop Management *///--------------------------------------------------
bool    Edge::contains( const QPointF& point ) const
{
    qreal d = distanceFromLine( point, QLineF{_p1, _p2} );
    return ( d > 0. && d < 5. );
}

void    Edge::dragEnterEvent( QDragEnterEvent* event )
{
    // Note 20160218: contains() is used so enter event is necessary generated "on the edge line"
    if ( _acceptDrops ) {
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
//-----------------------------------------------------------------------------

} // ::qan
