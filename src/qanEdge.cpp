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
<<<<<<< HEAD
#include "./qanEdge.h"
#include "./qanGroup.h"
#include "./qanNode.h"
#include "./qanGraph.h"
=======
#include "./qanGraph.h"
#include "./qanNode.h"
#include "./qanGroup.h"
#include "./qanEdge.h"
#include "./qanEdgeItem.h"
>>>>>>> dev

namespace qan { // ::qan

/* Edge Object Management *///-------------------------------------------------
<<<<<<< HEAD
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
    configureDestinationItem( destination );
    emit destinationItemChanged();
    updateItem();
}

void    Edge::setDestinationEdge( qan::Edge* destination )
{
    configureDestinationItem( destination );
    emit destinationEdgeChanged();
    updateItem();
}

void    Edge::configureDestinationItem( QQuickItem* item )
{
    if ( item == nullptr )
        return;

    // Connect dst x and y monitored properties change notify signal to slot updateItemSlot()
    QMetaMethod updateItemSlot = metaObject( )->method( metaObject( )->indexOfSlot( "updateItemSlot()" ) );
    if ( !updateItemSlot.isValid( ) ) {
        qDebug() << "qan::Edge::setDestinationItem(): Error: no access to edge updateItem slot.";
        return;
    }
    auto dstMetaObj = item->metaObject( );
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
void    Edge::updateItem( )
{
    auto source = getSrc().lock();
    auto destination = getDst().lock();
    auto hDestination = getHDst().lock();
    if ( source == nullptr ||
         ( destination == nullptr && hDestination == nullptr ) ) {
        update(); // Note 20160218: Force a simple update, since for example in edge style preview edge, there is no src and dst.
        return;
    }
    qan::Graph* graph = static_cast< qan::Graph* >( getGraph() );
    qan::Node*  sourceNode = static_cast< qan::Node* >( source.get() );
    qan::Node*  destinationNode = static_cast< qan::Node* >( destination.get() );
    qan::Edge*  destinationEdge = static_cast< qan::Edge* >( hDestination.get() );
    QQuickItem* destinationItem = ( destinationNode != nullptr ? static_cast<QQuickItem*>(destinationNode) :
                                                                 static_cast<QQuickItem*>(destinationEdge) );
    if ( graph != nullptr &&
         sourceNode != nullptr &&
         destinationItem != nullptr )  {
        qreal sourceZ = sourceNode->getQanGroup() != nullptr ? sourceNode->getQanGroup()->z() + sourceNode->z() :
                                                               sourceNode->z();
        QPointF srcPos = sourceNode->mapToItem( graph->getContainerItem(), QPointF{ 0, 0 } );
        QRectF srcBr{ srcPos, QSizeF{ sourceNode->width(), sourceNode->height() } };

        // Compute a global bounding boxe according to the actual src and dst
        QPointF dstPos = destinationItem->mapToItem( graph->getContainerItem(), QPointF{ 0, 0 } );
        QRectF dstBr{ dstPos, QSizeF{ destinationItem->width(), destinationItem->height() } };
        QRectF br = srcBr.united( dstBr );
        setPosition( br.topLeft() );
        setSize( br.size() );

        // Mapping src shape polygon to this CCS
        QPolygonF srcBoundingShape{sourceNode->getBoundingShape().size()};
        int p = 0;
        for ( const auto& point: sourceNode->getBoundingShape() )
            srcBoundingShape[p++] = mapFromItem( sourceNode, point );
        QPointF sourceBrCenter = srcBoundingShape.boundingRect( ).center( );

        if ( destinationNode != nullptr ) {        // Regular Node -> Node edge
            // Update edge z to source or destination maximum x
            qreal dstZ = destinationNode->getQanGroup() != nullptr ? destinationNode->getQanGroup()->z() + destinationNode->z() :
                                                                     destinationNode->z();
            setZ( qMax( sourceZ, dstZ ) );
            p = 0;
            QPolygonF dstBoundingShape{destinationNode->getBoundingShape().size()};
            for ( const auto& point: destinationNode->getBoundingShape() )
                dstBoundingShape[p++] = mapFromItem( destinationNode, point );
            QLineF line = getLineIntersection( sourceBrCenter,
                                               dstBoundingShape.boundingRect( ).center( ),
                                               srcBoundingShape, dstBoundingShape );
            _p1 = line.p1();
            emit p1Changed();
            _p2 = line.pointAt( 1 - 2 / line.length() );    // Note 20161001: Hack to take into account arrow border of 2px
            emit p2Changed();
            setLabelPos( line.pointAt( 0.5 ) + QPointF{10., 10.} );
        }
        else if ( destinationEdge != nullptr )     // Node -> Edge restricted hyper edge
        {
            setZ( qMax( sourceZ, destinationEdge->z() ) ); // Update edge z to source or destination maximum x
            QLineF destinationEdgeLine{ mapFromItem( destinationEdge, destinationEdge->getP1() ),
                                        mapFromItem( destinationEdge, destinationEdge->getP2() ) };
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
}

void    Edge::setLine( QPoint src, QPoint dst )
{
    _p1 = src; emit p1Changed();
    _p2 = dst; emit p2Changed();
}

QPointF  Edge::getLineIntersection( const QPointF& p1, const QPointF& p2, const QPolygonF& polygon ) const
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

QLineF  Edge::getLineIntersection( const QPointF& p1, const QPointF& p2,
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

=======
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
>>>>>>> dev
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

<<<<<<< HEAD
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
//-----------------------------------------------------------------------------

=======
>>>>>>> dev
} // ::qan
