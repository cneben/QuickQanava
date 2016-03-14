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
// \file	qanNavigable.cpp
// \author	benoit@qanava.org
// \date	2015 07 19
//-----------------------------------------------------------------------------

// QT headers
// Nil

// Qanava headers
#include "./qanNavigable.h"

namespace qan { // ::qan

Navigable::Navigable( QQuickItem* parent ) :
    QQuickItem( parent ),
    _containerItem( nullptr ),
    _leftButtonPressed( false ),
    _autoFitMode( NoAutoFit ),
    _panModified( false ),
    _zoomModified( false ),
    _zoomIncrement( 0.05 ),
    _zoom( 1.0 ),
    _zoomOrigin( QQuickItem::Center ),
    _zoomMax( -1.0 ),
    _zoomMin( 0.1 ),
    _lastPan( )
{
    _containerItem = new QQuickItem( this );
    setAcceptedMouseButtons( Qt::RightButton | Qt::LeftButton );
    setTransformOrigin( TransformOrigin::TopLeft );
}

void    Navigable::fitInView( )
{
    //qDebug( ) << "qan::Navigable::fitInView():";
    //qDebug( ) << "\tcontainer pos=" << _containerItem->x( ) << " " << _containerItem->y();
    //qDebug( ) << "\tcontainer br=" << _containerItem->childrenRect( );

    QRectF content = _containerItem->childrenRect();
    if ( !content.isEmpty() ) { // Protect against div/0, can't fit if there is no content...
        qreal viewWidth = width( );
        qreal viewHeight = height( );

        qreal fitWidthZoom = 1.0;
        qreal fitHeightZoom = 1.0;
        fitWidthZoom = viewWidth / content.width();
        fitHeightZoom = viewHeight / content.height( );
        //qDebug( ) << "\tviewWidth=" << viewWidth << "  viewHeight=" << viewHeight;
        //qDebug( ) << "\tfitWidthZoom=" << fitWidthZoom << "  fitHeightZoom=" << fitHeightZoom;

        qreal fitZoom = fitWidthZoom;
        if ( content.height() * fitWidthZoom > viewHeight )
            fitZoom = fitHeightZoom;
        //qDebug( ) << "\tfitZoom=" << fitZoom;

        QPointF contentPos( 0., 0. );
        if ( content.width() * fitZoom < viewWidth ) {  // Center zoomed content horizontally
            contentPos.rx() = ( viewWidth - ( content.width() * fitZoom ) ) / 2.;
        }
        if ( content.height() * fitZoom < viewHeight ) {   // Center zoomed content horizontally
            contentPos.ry() = ( viewHeight - ( content.height() * fitZoom ) ) / 2.;
        }
        _containerItem->setPosition( contentPos );
        _panModified = false;
        _zoomModified = false;

        // Don't use setZoom() because we force a TopLeft scale
        if ( isValidZoom( fitZoom ) ) {
            _zoom = fitZoom;
            _containerItem->setScale( _zoom );
            emit zoomChanged();
            emit containerItemModified( );
        }
    }
}

void    Navigable::setAutoFitMode( AutoFitMode autoFitMode )
{
    if ( _autoFitMode != AutoFit && autoFitMode == AutoFit )
        fitInView();    // When going to auto fit mode from another mode, force fitInView()

    _autoFitMode = autoFitMode;
    emit autoFitModeChanged();
}

void    Navigable::setZoom( qreal zoom )
{
    if ( isValidZoom( zoom ) ) {
        switch ( _zoomOrigin ) {
        case QQuickItem::Center: {
            zoomOn( QPointF( width( ) / 2., height() / 2. ),
                    zoom );
        }
            break;
        case QQuickItem::TopLeft:
        default:
            _zoom = zoom;
            _containerItem->setScale( _zoom );
            _zoomModified = true;
            emit zoomChanged();
            emit containerItemModified( );
        }
    }
}

void    Navigable::zoomOn( QPointF center, qreal zoom )
{
    // Get center coordinates in container CS, it is our
    // zoom application point
    qreal containerCenterX = center.x() - _containerItem->x();
    qreal containerCenterY = center.y() - _containerItem->y();

    qreal lastZoom = _zoom;

    // Don't apply modification if new zoom is not valid (probably because it is not in zoomMin, zoomMax range)
    if ( isValidZoom( zoom ) ) {
        // Get center coordinate in container CS with the new zoom
        qreal oldZoomX = containerCenterX * ( zoom / lastZoom );
        qreal oldZoomY = containerCenterY * ( zoom / lastZoom );

        // Compute a container position correction to have a fixed "zoom
        // application point"
        qreal zoomCorrectionX = containerCenterX - oldZoomX;
        qreal zoomCorrectionY = containerCenterY - oldZoomY;

        // Correct container position and set the appropriate scaling
        _containerItem->setX( _containerItem->x() + zoomCorrectionX );
        _containerItem->setY( _containerItem->y() + zoomCorrectionY );
        _containerItem->setScale( zoom );
        _zoom = zoom;
        _zoomModified = true;
        _panModified = true;
        emit zoomChanged();
        emit containerItemModified();
    }
}

bool    Navigable::isValidZoom( qreal zoom ) const
{
    if ( qFuzzyCompare( 1. + zoom - _zoom, 1.0 ) )
        return false;
    if ( ( zoom > _zoomMin ) &&    // Don't zoom less than zoomMin
         ( _zoomMax < 0. ||   // Don't zoom more than zoomMax except if zoomMax is infinite
           zoom < _zoomMax ) )
            return true;
    return false;
}

void    Navigable::setZoomOrigin( QQuickItem::TransformOrigin zoomOrigin )
{
    if ( zoomOrigin != _zoomOrigin ) {
        _zoomOrigin = zoomOrigin;
        emit zoomOriginChanged();
    }
}

void    Navigable::setZoomMax( qreal zoomMax )
{
    if ( qFuzzyCompare( 1. + zoomMax - _zoomMax, 1.0 ) )
        return;
    _zoomMax = zoomMax;
    emit zoomMaxChanged();
}

void    Navigable::setZoomMin( qreal zoomMin )
{
    if ( qFuzzyCompare( 1. + zoomMin - _zoomMin, 1.0 ) )
        return;
    if ( zoomMin < 0.01 )
        return;
    _zoomMin = zoomMin;
    emit zoomMinChanged();
}

void    Navigable::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry )
{
    QQuickItem::geometryChanged( newGeometry, oldGeometry );

    // Apply fitInView if auto fitting is set to true and the user has not applyed a custom zoom or pan
    if ( _autoFitMode == AutoFit &&
        ( !_panModified || !_zoomModified ) ) {
        fitInView();
    }
    // In AutoFit mode, try centering the content when the view is resized and the content
    // size is less than the view size (it is no fitting but centering...)
    if ( _autoFitMode == AutoFit ) {
        bool centerWidth = false;
        bool centerHeight = false;
        // Container item children Br mapped in root CS.
        QRectF contentBr = mapRectFromItem( _containerItem, _containerItem->childrenRect( ) );
        if ( newGeometry.contains( contentBr ) ) {
            centerWidth = true;
            centerHeight = true;
        } else {
            if ( contentBr.top( ) >= newGeometry.top( ) &&
                 contentBr.bottom() <= newGeometry.bottom( ) )
                centerHeight = true;
            if ( contentBr.left( ) >= newGeometry.left( ) &&
                 contentBr.right() <= newGeometry.right( ) )
                centerWidth = true;
        }
        if ( centerWidth ) {
            qreal cx = ( newGeometry.width() - contentBr.width() ) / 2.;
            _containerItem->setX( cx );
        }
        if ( centerHeight ) {
            qreal cy = ( newGeometry.height() - contentBr.height() ) / 2.;
            _containerItem->setY( cy );
        }
    }

    // In AutoFit mode, try anchoring the content to the visible border when the content has
    // custom user zoom
    if ( _autoFitMode == AutoFit &&
         ( _panModified || _zoomModified ) ) {
        bool anchorRight = false;
        bool anchorLeft = false;

        // Container item children Br mapped in root CS.
        QRectF contentBr = mapRectFromItem( _containerItem, _containerItem->childrenRect( ) );
        if ( contentBr.width( ) > newGeometry.width( ) &&
             contentBr.right( ) < newGeometry.right( ) ) {
            anchorRight = true;
        }
        if ( contentBr.width( ) > newGeometry.width( ) &&
             contentBr.left( ) > newGeometry.left( ) ) {
            anchorLeft = true;
        }
        if ( anchorRight ) {
            qreal xd = newGeometry.right( ) - contentBr.right( );
            _containerItem->setX( _containerItem->x( ) + xd );
        } else if ( anchorLeft ) {  // Right anchoring has priority over left anchoring...
            qreal xd = newGeometry.left( ) - contentBr.left( );
            _containerItem->setX( _containerItem->x( ) + xd );
        }
    }
}

void    Navigable::mouseMoveEvent( QMouseEvent* event )
{
    if ( _leftButtonPressed && !_lastPan.isNull( ))
    {
        QPointF delta = _lastPan - event->localPos( );
        QPointF p( QPointF( _containerItem->x( ), _containerItem->y( ) ) - delta );
        _containerItem->setX( p.x( ) );
        _containerItem->setY( p.y( ) );
        emit containerItemModified( );
        _panModified = true;
        _lastPan = event->localPos( );
    }
    QQuickItem::mouseMoveEvent( event );
}

void    Navigable::mousePressEvent( QMouseEvent* event )
{
    if ( event->button( ) == Qt::LeftButton )
    {
        _leftButtonPressed = true;
        _lastPan = event->localPos( );
        event->accept( );
        return;
    }
    if ( event->button( ) == Qt::RightButton )
    {
        event->accept( );
        return;
    }
    event->ignore( );
}

void    Navigable::mouseReleaseEvent( QMouseEvent* event )
{
    if ( event->button( ) == Qt::RightButton )
    {
        emit rightClicked( event->localPos( ) );
    }
    _leftButtonPressed = false;
    QQuickItem::mouseReleaseEvent( event );
}

void    Navigable::wheelEvent( QWheelEvent* event )
{
    qreal zoomFactor = ( event->angleDelta( ).y( ) > 0. ? _zoomIncrement : -_zoomIncrement );
    zoomOn( QPointF( event->x( ), event->y( ) ),
            getZoom( ) + zoomFactor );
    // Note 20160117: NavigableArea is opaque for wheel events
    //QQuickItem::wheelEvent( event );
}

} // ::qan
