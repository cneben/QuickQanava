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
// \file	qanEdge.cpp
// \author	benoit@qanava.org
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QBrush>
#include <QPainter>

// Qanava headers
#include "./qanEdge.h"
#include "./qanNode.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Edge Object Management *///-------------------------------------------------
Edge::Edge( QQuickItem* parent ) :
    gtpo::GenEdge< qan::Config >( ),
    _line( ),
    _defaultStyle{ new qan::EdgeStyle{ "", "qan::Edge" } },
    _style{ nullptr }
{
    setStyle( _defaultStyle.data() );
    setParentItem( parent );
    setAntialiasing( true );
    setPerformanceHint( QQuickPaintedItem::FastFBOResizing, true );    // Optimize item size modification but consume more graphics memory
    setFlag( QQuickItem::ItemHasContents );
    setAcceptedMouseButtons( Qt::RightButton | Qt::LeftButton );
    setAcceptDrops( true );
    update( );
}
//-----------------------------------------------------------------------------

/* Edge Topology Management *///-----------------------------------------------
auto    Edge::setSourceItem( qan::Node* source ) -> void
{
    if ( source == nullptr )
        return;

    // Connect dst x and y monitored properties change notify signal to slot updateEdge()
    QMetaMethod updateItemSlot = metaObject()->method( metaObject()->indexOfSlot( "updateItem()" ) );
    Q_ASSERT( updateItemSlot.isValid() );  // Connect src and dst x and y monitored properties change notify signal to slot updateItem()

    auto srcMetaObj = source->metaObject();
    QMetaProperty srcPropertyX = srcMetaObj->property( srcMetaObj->indexOfProperty( "x" ) );
    QMetaProperty srcPropertyY = srcMetaObj->property( srcMetaObj->indexOfProperty( "y" ) );
    Q_ASSERT( srcPropertyX.isValid() && srcPropertyY.isValid() );
    Q_ASSERT( srcPropertyX.hasNotifySignal() && srcPropertyY.hasNotifySignal() );
    QMetaMethod srcNotifyX = srcPropertyX.notifySignal();
    QMetaMethod srcNotifyY = srcPropertyY.notifySignal();
    connect( source, srcNotifyX, this, updateItemSlot );
    connect( source, srcNotifyY, this, updateItemSlot );
    if ( source->z() < z() )
        setZ( source->z() );
    emit sourceItemChanged();
    updateItem();
}

auto    Edge::setDestinationItem( qan::Node* destination ) -> void
{
    if ( destination == nullptr )
        return;

    // Connect dst x and y monitored properties change notify signal to slot updateItem()
    QMetaMethod updateItemSlot = metaObject( )->method( metaObject( )->indexOfSlot( "updateItem()" ) );
    Q_ASSERT( updateItemSlot.isValid( ) );

    auto dstMetaObj = destination->metaObject( );
    QMetaProperty dstPropertyX = dstMetaObj->property( dstMetaObj->indexOfProperty( "x" ) );
    QMetaProperty dstPropertyY = dstMetaObj->property( dstMetaObj->indexOfProperty( "y" ) );
    Q_ASSERT( dstPropertyX.isValid( ) && dstPropertyY.isValid( ) );
    Q_ASSERT( dstPropertyX.hasNotifySignal( ) && dstPropertyY.hasNotifySignal( ) );
    QMetaMethod dstNotifyX = dstPropertyX.notifySignal( );
    QMetaMethod dstNotifyY = dstPropertyY.notifySignal( );
    connect( destination, dstNotifyX, this, updateItemSlot );
    connect( destination, dstNotifyY, this, updateItemSlot );
    emit destinationItemChanged();
    if ( destination->z() < z() )
        setZ( destination->z() );
    updateItem( );
}
//-----------------------------------------------------------------------------

/* Edge Drawing Management *///------------------------------------------------
void Edge::paint( QPainter* painter )
{
    // Paint the line between src and dst
    QPen arrowPen( getStyle()->getLineColor(), getStyle()->getLineWidth(),
                   Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
    if ( _line.length( ) >= getStyle()->getArrowSize() + 1. ) {
        painter->setRenderHint( QPainter::Antialiasing );
        painter->setBrush( Qt::NoBrush);
        painter->setPen( arrowPen );
        drawArrow( painter, _line,
                   getStyle()->getLineColor(),
                   getStyle()->getArrowSize() );
    }
}

void    Edge::updateItem( )
{
    SharedNode ownedSource = getSrc().lock();
    SharedNode ownedDestination = getDst().lock();
    if ( ownedSource == nullptr ||
         ownedDestination == nullptr )
    {
        update(); // Note 20160218: Force a simple update, since for example in edge style preview edge, there is no src and dst.
        return;
    }
    qan::Node* sourceItem = static_cast< qan::Node* >( ownedSource.get() );
    qan::Node* destinationItem = static_cast< qan::Node* >( ownedDestination.get() );
    if ( sourceItem == nullptr || destinationItem == nullptr )
        return;

    // Update edge z to source or destination maximum x
    qreal maxZ = qMax( sourceItem->z(), destinationItem->z() );
    if ( z() < maxZ )
        setZ( maxZ );

    // Compute a global bounding boxe according to the actual src and dst
    // FIXME 20150824: Following code should works, but don't !
    //QPointF srcPos = getSrc( )->mapToScene( getSrc( )->position( ) );
    //QPointF dstPos = getDst( )->mapToScene( getDst( )->position( ) );
    QPointF srcPos = ownedSource->mapToItem( getGraph(), QPointF( 0, 0 ) );
    QPointF dstPos = destinationItem->mapToItem( getGraph(), QPointF( 0, 0 ) );

    QRectF srcBr{ srcPos, QSizeF( ownedSource->width( ), ownedSource->height( ) ) };
    QRectF dstBr{ dstPos, QSizeF( ownedDestination->width( ), ownedDestination->height( ) ) };

    QRectF br = srcBr.united( dstBr );
    setPosition( br.topLeft( ) );
    setSize( br.size( ) );

    // Mapping src shape polygon to this CCS
    QPolygonF srcBoundingShape;
    foreach ( QPointF p, ownedSource->getBoundingShape( ) )
        srcBoundingShape.append( mapFromItem( ownedSource.get(), p ) );
    QPolygonF dstBoundingShape;
    foreach ( QPointF p, ownedDestination->getBoundingShape( ) )
        dstBoundingShape.append( mapFromItem( ownedDestination.get(), p ) );

    // Works, but uncommented is probably faster. Uncomment for debugging
    //QPointF src = mapFromItem( getSrc( ), getSrc( )->boundingRect( ).center( ) );
    //QPointF dst = mapFromItem( getDst( ), getDst( )->boundingRect( ).center( ) );
    QPointF src = srcBoundingShape.boundingRect( ).center( );
    QPointF dst = dstBoundingShape.boundingRect( ).center( );

    _line = getPolyLineIntersection( QLineF( src, dst ),
                                     srcBoundingShape,
                                     dstBoundingShape );
    setLabelPos( _line.pointAt( 0.5 ) + QPointF( 10., 10. ) );
    update( );
}

void    Edge::setLine( QPoint src, QPoint dst )
{
    _line.setP1( src );
    _line.setP2( dst );
}

QLineF  Edge::getPolyLineIntersection( const QLineF line, const QPolygonF srcBp, const QPolygonF dstBp ) const
{
    QPointF source = line.p1( );
    for ( int p = 0; p < srcBp.length( ) - 1 ; p++ ) {
        QLineF polyLine( srcBp[ p ], srcBp[ p + 1 ] );
        QPointF intersection;
        if ( line.intersect( polyLine, &intersection ) == QLineF::BoundedIntersection ) {
            source = intersection;
            break;
        }
    }
    QPointF destination = line.p2( );
    for ( int p = 0; p < dstBp.length( ) - 1 ; p++ ) {
        QLineF polyLine( dstBp[ p ], dstBp[ p + 1 ] );
        QPointF intersection;
        if ( line.intersect( polyLine, &intersection ) == QLineF::BoundedIntersection ) {
            destination = intersection;
            break;
        }
    }
    return QLineF( source, destination );
}

void	Edge::drawArrow( QPainter* painter, QLineF line, QColor color, float arrowSize ) const
{
    // Don't draw a null line
    if ( line.isNull( ) )
        return;
    qreal lineLength = line.length( );
    if ( lineLength < 0.0001 )
        return;

    const double Pi = 3.141592653;
    double TwoPi = 2.0 * Pi;
    double angle = ::acos( line.dx( ) / lineLength );
    if ( line.dy( ) <= 0 )
        angle = TwoPi - angle;

    // Note 20160218: Correct line length to avoid drawing under the line arrow since it generate
    // drawing artifacts when line width is > to arrowSize
    QLineF correctedLine{ line.p1(), line.pointAt( 1.0 - ( arrowSize / lineLength ) ) };
    painter->drawLine( correctedLine );

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( QPen( color, 1., Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
    painter->setBrush( QBrush( color ) );

    painter->save( );
    painter->translate( line.p2( ) );
    painter->rotate( angle * 180. / Pi );

    double arrowLength = arrowSize * 2.;
    QPointF dst = QPointF( -arrowLength /*- 1*/, 0. );
    QPolygonF poly;
    poly	<< QPointF( dst.x( ), dst.y( ) - arrowSize )
            << QPointF( dst.x( ) + arrowLength, dst.y( ) )
            << QPointF( dst.x( ), dst.y( ) + arrowSize ) << QPointF( dst.x( ), dst.y( ) - arrowSize );
    painter->drawPolygon( poly );
    painter->restore( );
}
//-----------------------------------------------------------------------------


/* Mouse Management *///-------------------------------------------------------
void    Edge::mouseDoubleClickEvent( QMouseEvent* event )
{
    qDebug( ) << "Edge::mouseDoubleClickEvent()";
    qreal d = distanceFromLine( event->localPos( ), _line );
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
    qreal d = distanceFromLine( event->localPos( ), _line );
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
    //QQuickItem::mousePressEvent( event );
}

/*void    Edge::hoverMoveEvent( QHoverEvent* event )
{
    QQuickItem::hoverMoveEvent( event );
    qreal d = distanceFromLine( event->posF( ), _line );
    if (  !_hoverPopupEntered && d > 0. && d < 5. )
    {
        emit edgeHoverPopupEvent( event->posF( ) );
        _hoverPopupEntered = true;
    }
}
void    Edge::hoverLeaveEvent( QHoverEvent* event )
{
    QQuickItem::hoverLeaveEvent( event );
    _hoverPopupEntered = false;
    emit edgeHoverPopupLeaveEvent( event->posF( ) );
}*/

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
    update();
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
    qreal d = distanceFromLine( point, _line );
    return ( d > 0. && d < 5. );
}

void    Edge::dragEnterEvent( QDragEnterEvent* event )
{
    // Note 20160218: contains() is used so enter event is necessary generated "on the edge line"
    if ( _acceptDrops ) {
        if ( event->source() != nullptr ) { // Get the source item from the quick drag attached object received
            QVariant source = event->source()->property( "source" );
            qDebug() << "source=" << source;
            if ( source.isValid() ) {
                QQuickItem* sourceItem = source.value< QQuickItem* >( );
                qDebug() << "sourceItem=" << sourceItem;
                QVariant draggedStyle = sourceItem->property( "draggedEdgeStyle" ); // The source item (usually a style node or edge delegate must expose a draggedStyle property.
                qDebug() << "draggedStyle=" << draggedStyle;
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
        qreal d = distanceFromLine( event->posF( ), _line );
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
