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
// \file	qanEdgeItem.cpp
// \author	benoit@destrat.io
// \date	2017 03 02
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>

// QuickQanava headers
#include "./qanEdgeItem.h"
#include "./qanNodeItem.h"
#include "./qanGroupItem.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Edge Object Management *///-------------------------------------------------
EdgeItem::EdgeItem( QQuickItem* parent ) :
    QQuickItem{},
    _defaultStyle{ new qan::EdgeStyle{ "", "qan::Edge" } }
{
    setStyle( _defaultStyle.data() );
    setParentItem( parent );
    setAntialiasing( true );
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::RightButton | Qt::LeftButton );
    setAcceptDrops( true );
}

auto EdgeItem::getEdge() noexcept -> qan::Edge* { return _edge.data(); }
auto EdgeItem::getEdge() const noexcept -> const qan::Edge* { return _edge.data(); }
//-----------------------------------------------------------------------------

/* Edge Topology Management *///-----------------------------------------------
auto    EdgeItem::setSourceItem( qan::NodeItem* source ) -> void
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
            qWarning() << "qan::EdgeItem::setSourceItem(): Error: can't access source x property.";
            return;
        }
        if (  !srcY.isValid() || !srcY.hasNotifySignal() ) {
            qWarning() << "qan::EdgeItem::setSourceItem(): Error: can't access source y property.";
            return;
        }
        if (  !srcWidth.isValid() || !srcWidth.hasNotifySignal() ) {
            qWarning() << "qan::EdgeItem::setSourceItem(): Error: can't access source width property.";
            return;
        }
        if (  !srcHeight.isValid() || !srcHeight.hasNotifySignal() ) {
            qWarning() << "qan::EdgeItem::setSourceItem(): Error: can't access source height property.";
            return;
        }
        connect( source, srcX.notifySignal(),       this, updateItemSlot );
        connect( source, srcY.notifySignal(),       this, updateItemSlot );
        connect( source, srcZ.notifySignal(),       this, updateItemSlot );
        connect( source, srcWidth.notifySignal(),   this, updateItemSlot );
        connect( source, srcHeight.notifySignal(),  this, updateItemSlot );
        _sourceItem = source;
        emit sourceItemChanged();
        if ( source->z() < z() )
            setZ( source->z() );
        updateItem();
    }
}

auto    EdgeItem::setDestinationItem( qan::NodeItem* destination ) -> void
{
    configureDestinationItem( destination );
    _destinationItem = destination;
    emit destinationItemChanged();
    updateItem();
}

void    EdgeItem::setDestinationEdge( qan::EdgeItem* destination )
{
    configureDestinationItem( destination );
    //FIXME QAN3
    //_destinationItem = destination;
    emit destinationEdgeChanged();
    updateItem();
}

void    EdgeItem::configureDestinationItem( QQuickItem* item )
{
    if ( item == nullptr )
        return;

    // Connect dst x and y monitored properties change notify signal to slot updateItemSlot()
    QMetaMethod updateItemSlot = metaObject( )->method( metaObject( )->indexOfSlot( "updateItemSlot()" ) );
    if ( !updateItemSlot.isValid( ) ) {
        qDebug() << "qan::EdgeItem::setDestinationItem(): Error: no access to edge updateItem slot.";
        return;
    }
    auto dstMetaObj = item->metaObject( );
    QMetaProperty dstX      = dstMetaObj->property( dstMetaObj->indexOfProperty( "x" ) );
    QMetaProperty dstY      = dstMetaObj->property( dstMetaObj->indexOfProperty( "y" ) );
    QMetaProperty dstZ      = dstMetaObj->property( dstMetaObj->indexOfProperty( "z" ) );
    QMetaProperty dstWidth  = dstMetaObj->property( dstMetaObj->indexOfProperty( "width" ) );
    QMetaProperty dstHeight = dstMetaObj->property( dstMetaObj->indexOfProperty( "height" ) );
    if ( !dstX.isValid() || !dstX.hasNotifySignal() ) {
        qDebug() << "qan::EdgeItem::setDestinationItem(): Error: can't access source x property.";
        return;
    }
    if (  !dstY.isValid() || !dstY.hasNotifySignal() ) {
        qDebug() << "qan::EdgeItem::setDestinationItem(): Error: can't access source y property.";
        return;
    }
    if (  !dstWidth.isValid() || !dstWidth.hasNotifySignal() ) {
        qDebug() << "qan::EdgeItem::setDestinationItem(): Error: can't access source width property.";
        return;
    }
    if (  !dstHeight.isValid() || !dstHeight.hasNotifySignal() ) {
        qDebug() << "qan::EdgeItem::setDestinationItem(): Error: can't access source height property.";
        return;
    }
    connect( item, dstX.notifySignal(),       this, updateItemSlot );
    connect( item, dstY.notifySignal(),       this, updateItemSlot );
    connect( item, dstZ.notifySignal(),       this, updateItemSlot );
    connect( item, dstWidth.notifySignal(),   this, updateItemSlot );
    connect( item, dstHeight.notifySignal(),  this, updateItemSlot );
    if ( item->z() < z() )
        setZ( item->z() );
}
//-----------------------------------------------------------------------------

/* Edge Drawing Management *///------------------------------------------------
void    EdgeItem::updateItem()
{
    if ( !_edge )
        return;
    /*auto source = getSrc().lock();
    auto destination = getDst().lock();
    auto hDestination = getHDst().lock();
    if ( source == nullptr ||
         ( destination == nullptr && hDestination == nullptr ) ) {
        update(); // Note 20160218: Force a simple update, since for example in edge style preview edge, there is no src and dst.
        return;
    }*/
    qan::Graph* graph = static_cast< qan::Graph* >( _edge->getGraph() );
    if ( graph == nullptr )
        return;

    qan::NodeItem* srcItem = _sourceItem.data();
    if ( srcItem == nullptr )
        return;

    qan::Node*  srcNode = static_cast< qan::Node* >( _edge->getSrc().lock().get() );
    qan::GroupItem* srcGroupItem = nullptr;
    auto srcNodeGroup = srcNode->getGroup().lock();
    if ( srcNodeGroup )
        srcGroupItem = srcNodeGroup->getItem();

    qan::Node*  dstNode = static_cast< qan::Node* >( _edge->getDst().lock().get() );
    qan::NodeItem* dstNodeItem = nullptr;
    if ( dstNode != nullptr )
        dstNodeItem = dstNode->getItem();

    qan::EdgeItem* dstEdgeItem = nullptr;
    qan::Edge*  dstEdge = static_cast< qan::Edge* >( _edge->getHDst().lock().get() );
    if ( dstNodeItem == nullptr &&
         dstEdge != nullptr )
        dstEdgeItem = dstEdge->getItem();
    QQuickItem* dstItem = ( dstNodeItem != nullptr ? qobject_cast<QQuickItem*>(dstNodeItem) :
                                                     qobject_cast<QQuickItem*>(dstEdgeItem) );

    QQuickItem* graphContainerItem = graph->getContainerItem();

    if ( graphContainerItem == nullptr ||
         srcItem == nullptr ||
         dstItem == nullptr ||
         ( dstNodeItem == nullptr && dstEdgeItem == nullptr ) )
        return;

    qreal sourceZ = srcGroupItem != nullptr ? srcGroupItem->z() + srcItem->z() : srcItem->z();
    QPointF srcPos = srcItem->mapToItem( graphContainerItem, QPointF{ 0, 0 } );
    QRectF srcBr{ srcPos, QSizeF{ srcItem->width(), srcItem->height() } };

    // Compute a global bounding boxe according to the actual src and dst
    QPointF dstPos = dstItem->mapToItem( graphContainerItem, QPointF{ 0, 0 } );
    QRectF dstBr{ dstPos, QSizeF{ dstItem->width(), dstItem->height() } };
    QRectF br = srcBr.united( dstBr );
    setPosition( br.topLeft() );
    setSize( br.size() );

    // Mapping src shape polygon to this CCS
    QPolygonF srcBoundingShape{_sourceItem->getBoundingShape().size()};
    int p = 0;
    for ( const auto& point: srcItem->getBoundingShape() )
        srcBoundingShape[p++] = mapFromItem( srcItem, point );
    QPointF sourceBrCenter = srcBoundingShape.boundingRect().center();

    if ( dstNodeItem != nullptr ) {        // Regular Node -> Node edge
        // Update edge z to source or destination maximum x
        qreal dstZ = srcGroupItem != nullptr ? srcGroupItem->z() + dstNodeItem->z() : dstNodeItem->z();
        setZ( qMax( sourceZ, dstZ ) );
        p = 0;
        QPolygonF dstBoundingShape{dstNodeItem->getBoundingShape().size()};
        for ( const auto& point: dstNodeItem->getBoundingShape() )
            dstBoundingShape[p++] = mapFromItem( dstNodeItem, point );
        QLineF line = getLineIntersection( sourceBrCenter,
                                           dstBoundingShape.boundingRect().center( ),
                                           srcBoundingShape, dstBoundingShape );
        _p1 = line.p1();
        emit p1Changed();
        _p2 = line.pointAt( 1 - 2 / line.length() );    // Note 20161001: Hack to take into account arrow border of 2px
        emit p2Changed();
        setLabelPos( line.pointAt( 0.5 ) + QPointF{10., 10.} );
    }
    else if ( dstEdgeItem != nullptr ) {            // Node -> Edge restricted hyper edge
        setZ( qMax( sourceZ, dstEdgeItem->z() ) );  // Update edge z to source or destination maximum x
        QLineF destinationEdgeLine{ mapFromItem( dstEdgeItem, dstEdgeItem->getP1() ),
                    mapFromItem( dstEdgeItem, dstEdgeItem->getP2() ) };
        if ( destinationEdgeLine.length() > 0.001 ) {
            QLineF line{ sourceBrCenter, destinationEdgeLine.pointAt( 0.5 ) };
            _p1 = getLineIntersection( line.p1(), line.p2(), srcBoundingShape );
            emit p1Changed();
            _p2 = line.pointAt( 1 - 2 / line.length() );    // Note 20161001: Hack to take into account arrow border of 2px
            emit p2Changed();
            setLabelPos( line.pointAt( 0.5 ) + QPointF{10., 10.} );
        }
    }
}

void    EdgeItem::setLine( QPoint src, QPoint dst )
{
    _p1 = src; emit p1Changed();
    _p2 = dst; emit p2Changed();
}

QPointF  EdgeItem::getLineIntersection( const QPointF& p1, const QPointF& p2, const QPolygonF& polygon ) const
{
    QLineF line{p1, p2};
    QPointF source{p1};
    QPointF intersection;
    for ( int p = 0; p < polygon.length( ) - 1 ; p++ ) {
        QLineF polyLine( polygon[ p ], polygon[ p + 1 ] );
        if ( line.intersect( polyLine, &intersection ) == QLineF::BoundedIntersection ) {
            source = intersection;
            break;
        }
    }
    return source;
}

QLineF  EdgeItem::getLineIntersection( const QPointF& p1, const QPointF& p2,
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
void    EdgeItem::mouseDoubleClickEvent( QMouseEvent* event )
{
    qreal d = distanceFromLine( event->localPos( ), QLineF{_p1, _p2} );
    if ( d >= 0. && d < 5. && event->button( ) == Qt::LeftButton ) {
        emit edgeDoubleClicked( QVariant::fromValue< qan::EdgeItem* >( this ), QVariant( event->localPos( ) ) );
        event->accept( );
    }
    else
        event->ignore( );
    QQuickItem::mouseDoubleClickEvent( event );
}

void    EdgeItem::mousePressEvent( QMouseEvent* event )
{
    qreal d = distanceFromLine( event->localPos( ), QLineF{_p1, _p2} );
    if ( d >= 0. && d < 5. ) {
        if ( event->button( ) == Qt::LeftButton ) {
            emit edgeClicked( QVariant::fromValue< qan::EdgeItem* >( this ), QVariant( event->localPos( ) ) );
            event->accept( );
        }
        else if ( event->button( ) == Qt::RightButton ) {
            emit edgeRightClicked( QVariant::fromValue< qan::EdgeItem* >( this ), QVariant( event->localPos( ) ) );
            event->accept( );
            return;
        }
    }
    else
        event->ignore( );
}

qreal   EdgeItem::distanceFromLine( const QPointF& p, const QLineF& line ) const
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
void    EdgeItem::setStyle( EdgeStyle* style )
{
    if ( style == _style )
        return;
    if ( style == nullptr )
        style = _defaultStyle.data();
    if ( _style != nullptr && _style != _defaultStyle.data() )  // Every style that is non default is disconnect from this node
        QObject::disconnect( _style, 0, this, 0 );
    _style = style;
    connect( _style, &QObject::destroyed, this, &EdgeItem::styleDestroyed );    // Monitor eventual style destruction
    connect( _style, &qan::EdgeStyle::styleModified, this, &EdgeItem::updateItem );
    emit styleChanged( );
}

void    EdgeItem::styleDestroyed( QObject* style )
{
    if ( style != nullptr && style != _defaultStyle.data() ) {
        QObject::disconnect( style, 0, this, 0 );
        setStyle( _defaultStyle.data() );   // Set default style when current style is destroyed
    }
}

void    EdgeItem::setLabel( const QString& label )
{
    if ( label != _label ) {
        _label = label;
        emit labelChanged( );
    }
}

void     EdgeItem::setWeight( qreal weight )
{
    if ( !qFuzzyCompare( 1.0 + weight, 1.0 + _weight ) ) {
        _weight = weight;
        emit weightChanged();
    }
}
//-----------------------------------------------------------------------------

/* Drag'nDrop Management *///--------------------------------------------------
bool    EdgeItem::contains( const QPointF& point ) const
{
    qreal d = distanceFromLine( point, QLineF{_p1, _p2} );
    return ( d > 0. && d < 5. );
}

void    EdgeItem::dragEnterEvent( QDragEnterEvent* event )
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

void	EdgeItem::dragMoveEvent( QDragMoveEvent* event )
{
    if ( getAcceptDrops() ) {
        qreal d = distanceFromLine( event->posF( ), QLineF{_p1, _p2} );
        if ( d > 0. && d < 5. )
            event->accept();
        else event->ignore();
    }
    QQuickItem::dragMoveEvent( event );
}

void	EdgeItem::dragLeaveEvent( QDragLeaveEvent* event )
{
    QQuickItem::dragLeaveEvent( event );
    if ( getAcceptDrops() )
        event->ignore();
}

void    EdgeItem::dropEvent( QDropEvent* event )
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
