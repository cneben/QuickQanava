/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    QQuickItem{}
{
    setParentItem( parent );
    setAntialiasing( true );
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::RightButton | Qt::LeftButton );
    setAcceptDrops( true );
    setVisible(false);  // Invisible until there is a valid src/dst

    setStyle( qan::Edge::style() );
    setObjectName( QStringLiteral("qan::EdgeItem") );
}

EdgeItem::~EdgeItem() { /* Nil */ }

auto    EdgeItem::getEdge() noexcept -> qan::Edge* { return _edge.data(); }
auto    EdgeItem::getEdge() const noexcept -> const qan::Edge* { return _edge.data(); }
auto    EdgeItem::setEdge(qan::Edge* edge) noexcept -> void
{
    _edge = edge;
    if ( edge != nullptr&&
         edge->getItem() != this )
        edge->setItem(this);
}

auto    EdgeItem::getGraph() const noexcept -> const qan::Graph* {
    if ( _graph )
        return _graph;
    return _edge ? _edge->getGraph() : nullptr;
}
auto    EdgeItem::getGraph() noexcept -> qan::Graph* {
    if ( _graph )
        return _graph;
    return _edge ? _edge->getGraph() : nullptr;
}
auto    EdgeItem::setGraph(qan::Graph* graph) noexcept -> void {
    _graph = graph; emit graphChanged();
}
//-----------------------------------------------------------------------------

/* Edge Topology Management *///-----------------------------------------------
bool    EdgeItem::isHyperEdge() const noexcept { return ( _edge ? _edge->getHDst().lock() != nullptr : false ); }

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
    _destinationEdge = destination;
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
        qWarning() << "qan::EdgeItem::setDestinationItem(): Error: no access to edge updateItem slot.";
        return;
    }
    auto dstMetaObj = item->metaObject( );
    QMetaProperty dstX      = dstMetaObj->property( dstMetaObj->indexOfProperty( "x" ) );
    QMetaProperty dstY      = dstMetaObj->property( dstMetaObj->indexOfProperty( "y" ) );
    QMetaProperty dstZ      = dstMetaObj->property( dstMetaObj->indexOfProperty( "z" ) );
    QMetaProperty dstWidth  = dstMetaObj->property( dstMetaObj->indexOfProperty( "width" ) );
    QMetaProperty dstHeight = dstMetaObj->property( dstMetaObj->indexOfProperty( "height" ) );
    if ( !dstX.isValid() || !dstX.hasNotifySignal() ) {
        qWarning() << "qan::EdgeItem::setDestinationItem(): Error: can't access source x property.";
        return;
    }
    if (  !dstY.isValid() || !dstY.hasNotifySignal() ) {
        qWarning() << "qan::EdgeItem::setDestinationItem(): Error: can't access source y property.";
        return;
    }
    if (  !dstWidth.isValid() || !dstWidth.hasNotifySignal() ) {
        qWarning() << "qan::EdgeItem::setDestinationItem(): Error: can't access source width property.";
        return;
    }
    if (  !dstHeight.isValid() || !dstHeight.hasNotifySignal() ) {
        qWarning() << "qan::EdgeItem::setDestinationItem(): Error: can't access source height property.";
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
void    EdgeItem::setHidden(bool hidden) noexcept
{
    if ( hidden != _hidden ) {
        _hidden = hidden;
        emit hiddenChanged();
    }
}

void    EdgeItem::updateItem()
{
    qan::NodeItem*  srcItem = _sourceItem.data();
    qan::GroupItem* srcGroupItem = nullptr;
    qan::GroupItem* dstGroupItem = nullptr;

    qan::NodeItem*  dstNodeItem = _destinationItem.data();
    qan::EdgeItem*  dstEdgeItem = _destinationEdge.data();

    qan::Graph*     graph = getGraph();
    QQuickItem*     graphContainerItem = ( graph != nullptr ? graph->getContainerItem() : this );

    if ( srcGroupItem == nullptr &&
         _sourceItem ) {
        qan::Node*  srcNode = static_cast< qan::Node* >( _sourceItem->getNode() );
        if ( srcNode != nullptr ) {
            auto srcNodeGroup = qobject_cast<qan::Group*>( srcNode != nullptr ? srcNode->getGroup().lock().get() : nullptr );
            if ( srcNodeGroup != nullptr )
                srcGroupItem = srcNodeGroup->getItem();
        }
    }
    if ( dstNodeItem == nullptr &&
         _edge ) {
        qan::Node*  dstNode = static_cast< qan::Node* >( _edge->getDst().lock().get() );
        if ( dstNode != nullptr )
            dstNodeItem = dstNode->getItem();
    }
    if ( dstGroupItem == nullptr &&
         dstNodeItem != nullptr &&
         dstNodeItem->getNode() != nullptr ) {
        auto dstNodeGroup = qobject_cast<qan::Group*>( dstNodeItem->getNode()->getGroup().lock().get() );
        if ( dstNodeGroup )
            dstGroupItem = dstNodeGroup->getItem();
    }
    if ( dstEdgeItem == nullptr &&
         _edge ) {
        qan::Edge*  dstEdge = static_cast< qan::Edge* >( _edge->getHDst().lock().get() );
        if ( dstEdge != nullptr )
            dstEdgeItem = dstEdge->getItem();
    }
    QQuickItem* dstItem = ( dstNodeItem != nullptr ? qobject_cast<QQuickItem*>(dstNodeItem) :
                                                     qobject_cast<QQuickItem*>(dstEdgeItem) );

    if ( graphContainerItem == nullptr ||
         srcItem == nullptr ||
         dstItem == nullptr )
        return;

    qreal srcZ = srcGroupItem != nullptr ? srcGroupItem->z() + srcItem->z() : srcItem->z();
    QPointF srcPos = srcItem->mapToItem( graphContainerItem, QPointF{ 0, 0 } );
    QRectF srcBr{ srcPos, QSizeF{ srcItem->width(), srcItem->height() } };

    // Compute a global bounding boxe according to the actual src and dst
    QPointF dstPos = dstItem->mapToItem( graphContainerItem, QPointF{ 0, 0 } );
    QRectF dstBr{ dstPos, QSizeF{ dstItem->width(), dstItem->height() } };


    // Mapping src shape polygon to this CCS
    QPolygonF srcBoundingShape{_sourceItem->getBoundingShape().size()};
    int p = 0;
    for ( const auto& point: srcItem->getBoundingShape() )
        srcBoundingShape[p++] = _sourceItem->mapToItem( graphContainerItem, point );
    QPointF sourceBrCenter = srcBoundingShape.boundingRect().center();

    if ( dstNodeItem != nullptr ) {        // Regular Node -> Node edge
        // Update edge z to source or destination maximum x
        qreal dstZ = dstGroupItem != nullptr ? dstGroupItem->z() + dstNodeItem->z() : dstNodeItem->z();
        setZ( qMax( srcZ, qMax( srcZ, dstZ ) ) + 0.1 );
        p = 0;
        QPolygonF dstBoundingShape{dstNodeItem->getBoundingShape().size()};
        for ( const auto& point: dstNodeItem->getBoundingShape() )
            dstBoundingShape[p++] = dstNodeItem->mapToItem( graphContainerItem, point );
        QLineF line = getLineIntersection( sourceBrCenter,
                                           dstBoundingShape.boundingRect().center( ),
                                           srcBoundingShape, dstBoundingShape );
        QRectF br = QRectF{line.p1(), line.p2()}.normalized();  // Generate a Br with intersection points
        const bool edgeHidden = ( srcBoundingShape.boundingRect().contains(br) ||
                                  dstBoundingShape.boundingRect().contains(br) );
        if (!edgeHidden) {
            setPosition( br.topLeft() );    // Warning: it should be done before mapFromItem...
            setSize( br.size() );
            const auto p1 = mapFromItem(graphContainerItem, line.p1());
            const auto p2 = mapFromItem(graphContainerItem, line.pointAt( 1 - (2/line.length()) ) );    // Note 20161001: Hack to take into account arrow border of 2px
            if ( QLineF{p1, p2}.length() > 2.0 ) {    // FIXME: arrow size has to be taken into account here...
                setHidden(false);
                _p1 = p1;
                _p2 = p2;
                emit p1Changed();
                emit p2Changed();
                setLabelPos( line.pointAt( 0.5 ) + QPointF{10., 10.} );
            } else
                setHidden(true);
        } else
            setHidden(true);
    }
    else if ( dstEdgeItem != nullptr ) {            // Node -> Edge restricted hyper edge
        setZ( qMax( srcZ, dstEdgeItem->z() ) );     // Update edge z to source or destination maximum x
        QLineF destinationEdgeLine{ dstEdgeItem->mapToItem(graphContainerItem, dstEdgeItem->getP1() ),
                                    dstEdgeItem->mapToItem(graphContainerItem, dstEdgeItem->getP2() ) };
        if ( destinationEdgeLine.length() > 0.001 ) {
            QLineF line{ sourceBrCenter, destinationEdgeLine.pointAt( 0.5 ) };
            line.setP1(getLineIntersection( sourceBrCenter, line.p2(), srcBoundingShape ));

            QRectF br = QRectF{line.p1(), line.p2()}.normalized();  // Generate a Br with intersection points
            setPosition( br.topLeft() );
            setSize( br.size() );

            _p1 = mapFromItem(graphContainerItem, line.p1() );
            _p2 = mapFromItem(graphContainerItem, line.pointAt( 1 - 2/line.length()) );
            emit p1Changed();
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

QPointF  EdgeItem::getLineIntersection( const QPointF& p1, const QPointF& p2,
                                        const QPolygonF& polygon ) const
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
    qreal d = distanceFromLine( event->localPos(), QLineF{_p1, _p2} );
    if ( d >= 0. && d < 5. && event->button() == Qt::LeftButton ) {
        emit edgeDoubleClicked( this, event->localPos() );
        event->accept();
    }
    else
        event->ignore();
    QQuickItem::mouseDoubleClickEvent( event );
}

void    EdgeItem::mousePressEvent( QMouseEvent* event )
{
    qreal d = distanceFromLine( event->localPos( ), QLineF{_p1, _p2} );
    if ( d >= 0. && d < 5. ) {
        if ( event->button() == Qt::LeftButton ) {
            emit edgeClicked( this, event->localPos() );
            event->accept();
        }
        else if ( event->button() == Qt::RightButton ) {
            emit edgeRightClicked( this, event->localPos() );
            event->accept();
            return;
        }
    }
    else
        event->ignore();
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
void    EdgeItem::setStyle( EdgeStyle* style ) noexcept
{
    if ( style != _style ) {
        if ( _style != nullptr )  // Every style that is non default is disconnect from this node
            QObject::disconnect( _style, 0, this, 0 );
        _style = style;
        if ( _style ) {
            connect( _style,    &QObject::destroyed,    // Monitor eventual style destruction
                     this,      &EdgeItem::styleDestroyed );
            //connect( _style,    &qan::EdgeStyle::styleModified,
            //         this,      &EdgeItem::updateItem );
        }
        emit styleChanged( );
    }
}

void    EdgeItem::styleDestroyed( QObject* style )
{
    if ( style != nullptr )
        setStyle( nullptr );
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
