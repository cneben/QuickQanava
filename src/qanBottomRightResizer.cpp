/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// \file	qanBottomRightResizer.cpp
// \author	benoit@destrat.io
// \date	2016 07 08
//-----------------------------------------------------------------------------

// Std headers
#include <sstream>

// Qt headers
#include <QCursor>
#include <QMouseEvent>

// QuickQanava headers
#include "./qanBottomRightResizer.h"

namespace qan {  // ::qan

/* BottomRightResizer Object Management *///-----------------------------------
BottomRightResizer::BottomRightResizer( QQuickItem* parent ) :
    QQuickItem{ parent }
{
}

BottomRightResizer::~BottomRightResizer( )
{
    if ( _handler )
        _handler->deleteLater();
}
//-----------------------------------------------------------------------------

/* Resizer Management *///-----------------------------------------------------
void    BottomRightResizer::setHandler( QQuickItem* handler ) noexcept
{
    if ( handler != _handler.data() ) {
        if ( _handler ) {
            if ( QQmlEngine::objectOwnership(_handler.data()) == QQmlEngine::CppOwnership )
                _handler.data()->deleteLater();
        }
        _handler = handler;
        if ( _handler )
            _handler->installEventFilter(this);
        emit handlerChanged();
    }
    if ( _target )      // Force target reconfiguration for new handler
        configureTarget(*_target);
}

QQuickItem* BottomRightResizer::getHandler( ) const noexcept
{
    return ( _handler ? _handler.data() : nullptr );
}

void    BottomRightResizer::setTarget( QQuickItem* target )
{
    if ( target == nullptr ) {  // Set a null target = disable the control
        if ( _target )
            disconnect( _target.data(), nullptr,
                        this,           nullptr );  // Disconnect old target width/height monitoring
        _target = nullptr;
        return;
    }

    if ( !_handler ) {  // Eventually, create the handler component
        QQmlEngine* engine = qmlEngine( this );
        if ( engine != nullptr ) {
            QQmlComponent defaultHandlerComponent{engine};
            QString handlerQml{ QStringLiteral("import QtQuick 2.7\n  Rectangle {") +
                        QStringLiteral("width:")    + QString::number(_handlerSize.width()) + QStringLiteral(";") +
                        QStringLiteral("height:")   + QString::number(_handlerSize.height()) + QStringLiteral(";") +
                        QStringLiteral("border.width:4;radius:3;") +
                        QStringLiteral("border.color:\"") + _handlerColor.name() + QStringLiteral("\";") +
                        QStringLiteral("color:Qt.lighter(border.color); }") };
            defaultHandlerComponent.setData( handlerQml.toUtf8(), QUrl{} );
            if ( defaultHandlerComponent.isReady() ) {
                _handler = qobject_cast<QQuickItem*>(defaultHandlerComponent.create());
                if ( _handler ) {
                    engine->setObjectOwnership( _handler.data(), QQmlEngine::CppOwnership );
                    _handler->setParentItem(this);
                    _handler->installEventFilter(this);
                }
                else {
                    qWarning() << "qan::BottomRightResizer::setTarget(): Error: Can't create resize handler QML component:";
                    qWarning() << "QML Component status=" << defaultHandlerComponent.status();
                }
            }
        }

        forceHandlerWidth(_handlerWidth);       // Force taking into account handler
        forceHandlerRadius(_handlerRadius);     // setting if they have been modified before
        forceHandlerSize(_handlerSize);         // _handler item has been created
    }

    // Configure handler on given target
    if ( _handler )
        configureHandler(*_handler);

    _target = target;
    if ( _target )
        configureTarget(*_target);
    emit targetChanged();
}

void    BottomRightResizer::configureHandler(QQuickItem& handler) noexcept
{
    handler.setOpacity( _autoHideHandler ? 0. : 1. );
    handler.setSize( _handlerSize );
    handler.setZ( z() + 1. );
    QObject* handlerBorder = handler.property( "border" ).value<QObject*>();
    if ( handlerBorder != nullptr )
        handlerBorder->setProperty( "color", _handlerColor );
    handler.setVisible( true );
    handler.setParentItem( this );
    handler.setAcceptedMouseButtons( Qt::LeftButton );
    handler.setAcceptHoverEvents( true );
}

void    BottomRightResizer::configureTarget(QQuickItem& target) noexcept
{
    if ( !_minimumTargetSize.isEmpty() ) { // Check that target size is not bellow resizer target minimum size
        if (target.width() < _minimumTargetSize.width())
            target.setWidth(_minimumTargetSize.width());
        if ( target.height() < _minimumTargetSize.height())
            target.setHeight(_minimumTargetSize.height());
    }

    if ( &target != parentItem() ) { // Resizer is not in target sibling (ie is not a child of target)
        connect( &target,   &QQuickItem::xChanged,
                 this,      &BottomRightResizer::onTargetXChanged );
        connect( &target,   &QQuickItem::yChanged,
                 this,      &BottomRightResizer::onTargetYChanged );
        setX( target.x() );
        setY( target.y() );
    }

    connect( &target,   &QQuickItem::widthChanged,
             this,      &BottomRightResizer::onTargetWidthChanged );
    connect( &target,   &QQuickItem::heightChanged,
             this,      &BottomRightResizer::onTargetHeightChanged );

    onTargetWidthChanged();
    onTargetHeightChanged();
}

void    BottomRightResizer::onTargetXChanged()
{
    if ( _target &&
         _target != parentItem() )
        setX( _target->x() );
}

void    BottomRightResizer::onTargetYChanged()
{
    if ( _target &&
         _target != parentItem() )
        setY( _target->y() );
}

void    BottomRightResizer::onTargetWidthChanged()
{
    if ( _target &&
         _handler ) {
        const qreal targetWidth = _target->width();
        const qreal handlerWidth2 = _handlerSize.width() / 2.;
        _handler->setX( targetWidth - handlerWidth2 );
    }
}

void    BottomRightResizer::onTargetHeightChanged()
{
    if ( _target &&
         _handler ) {
        const qreal targetHeight = _target->height();
        const qreal handlerHeight2 = _handlerSize.height() / 2.;
        _handler->setY( targetHeight - handlerHeight2 );
    }
}

void    BottomRightResizer::setHandlerSize( const QSizeF& handlerSize )
{
    if ( handlerSize.isEmpty() )
        return;
    if ( handlerSize == _handlerSize )  // Binding loop protection
        return;

    forceHandlerSize(handlerSize);
}

void    BottomRightResizer::forceHandlerSize( const QSizeF& handlerSize )
{
    _handlerSize = handlerSize;
    if ( _handler ) {
        onTargetWidthChanged();     // Force resize handler position change
        onTargetHeightChanged();    // to take new handler size

        _handler->setSize( handlerSize );
    }

    emit handlerSizeChanged();
}

void    BottomRightResizer::setHandlerColor( QColor handlerColor )
{
    if ( !handlerColor.isValid() )
        return;
    if ( handlerColor == _handlerColor )    // Binding loop protection
        return;
    if ( _handler ) {
        QObject* handlerBorder = _handler->property( "border" ).value<QObject*>();
        if ( handlerBorder ) {
            handlerBorder->setProperty( "color", handlerColor );
        }
    }
    _handlerColor = handlerColor;
    emit handlerColorChanged();
}

void    BottomRightResizer::setHandlerRadius( qreal handlerRadius )
{
    if ( qFuzzyCompare( 1.0 + handlerRadius, 1.0 + _handlerRadius ) )    // Binding loop protection
        return;
    forceHandlerRadius(handlerRadius);
}

void    BottomRightResizer::forceHandlerRadius( qreal handlerRadius )
{
    if ( _handler )
        _handler->setProperty( "radius", handlerRadius );
    _handlerRadius = handlerRadius;
    emit handlerRadiusChanged();
}

void    BottomRightResizer::setHandlerWidth( qreal handlerWidth )
{
    if ( qFuzzyCompare( 1.0 + handlerWidth, 1.0 + _handlerWidth ) )    // Binding loop protection
        return;
    forceHandlerWidth(handlerWidth);
}

void    BottomRightResizer::forceHandlerWidth( qreal handlerWidth )
{
    if ( _handler ) {
        QObject* handlerBorder = _handler->property( "border" ).value<QObject*>();
        if ( handlerBorder != nullptr ) {
            handlerBorder->setProperty( "width", handlerWidth );
        }
    }
    _handlerWidth = handlerWidth;
    emit handlerWidthChanged();
}

void    BottomRightResizer::setMinimumTargetSize(QSizeF minimumTargetSize)
{
    if (minimumTargetSize.isEmpty())
        return;
    if (minimumTargetSize != _minimumTargetSize) {
        _minimumTargetSize = minimumTargetSize;
        if (_target) { // Eventually, resize target if its actual size is below minimum
            if (_target->width() < minimumTargetSize.width())
                _target->setWidth(minimumTargetSize.width());
            if (_target->height() < minimumTargetSize.height())
                _target->setHeight(minimumTargetSize.height());
        }
        emit minimumTargetSizeChanged();
    }
}

void    BottomRightResizer::setAutoHideHandler( bool autoHideHandler )
{
    if ( autoHideHandler == _autoHideHandler )    // Binding loop protection
        return;
    if ( _handler  &&
         autoHideHandler &&
         _handler->isVisible() )    // If autoHide is set to false and the handler is visible, hide it
        _handler->setVisible( false );
    _autoHideHandler = autoHideHandler;
    emit autoHideHandlerChanged();
}

void    BottomRightResizer::setPreserveRatio(bool preserveRatio) noexcept
{
    if (preserveRatio != _preserveRatio) {
        _preserveRatio = preserveRatio;
        emit preserveRatioChanged();
    }
}

void    BottomRightResizer::setRatio(qreal ratio) noexcept
{
    if (!qFuzzyCompare(2.0 + _ratio, 2.0 + ratio)) { // Using 2.0 because -1.0 is a valid input (disable ratio...)
        _ratio = ratio;
        emit ratioChanged();
    }
}
//-----------------------------------------------------------------------------

/* Resizer Management *///-----------------------------------------------------
bool   BottomRightResizer::eventFilter(QObject *item, QEvent *event)
{
    if ( item != _handler )
        return QObject::eventFilter(item, event);
    bool accepted{ false };
    if ( _handler &&
         item == _handler.data() ) {
        switch ( event->type() ) {
        case QEvent::HoverEnter:
        {
            _handler->setCursor( Qt::SizeFDiagCursor );
            _handler->setOpacity( 1.0 );   // Handler is always visible when hovered
            QMouseEvent* me = static_cast<QMouseEvent*>( event );
            me->setAccepted(true);
            accepted = true;
        }
            break;
        case QEvent::HoverLeave:
        {
            _handler->setCursor( Qt::ArrowCursor );
            _handler->setOpacity( getAutoHideHandler() ? 0. : 1.0 );
            QMouseEvent* me = static_cast<QMouseEvent*>( event );
            me->setAccepted(true);
            accepted = true;
            break;
        }
        case QEvent::MouseMove: {
            QMouseEvent* me = static_cast<QMouseEvent*>( event );
            if ( me->buttons() |  Qt::LeftButton &&
                 !_dragInitialPos.isNull() &&
                 !_targetInitialSize.isEmpty() ) {
                // Inspired by void QQuickMouseArea::mouseMoveEvent(QMouseEvent *event)
                // https://code.woboq.org/qt5/qtdeclarative/src/quick/items/qquickmousearea.cpp.html#47curLocalPos
                // Coordinate mapping in qt quick is even more a nightmare than with graphics view...
                // BTW, this code is probably buggy for deep quick item hierarchy.
                QPointF startLocalPos;
                QPointF curLocalPos;
                if ( parentItem() != nullptr ) {
                    startLocalPos = parentItem()->mapFromScene( _dragInitialPos );
                    curLocalPos = parentItem()->mapFromScene( me->windowPos() );
                } else {
                    startLocalPos = _dragInitialPos;
                    curLocalPos = me->windowPos();
                }
                const QPointF delta{curLocalPos - startLocalPos};
                if (_target) {
                    // Do not resize below minimumSize
                    const qreal targetWidth = _targetInitialSize.width() + delta.x();
                    if (targetWidth > _minimumTargetSize.width())
                            _target->setWidth(targetWidth);
                    if (_preserveRatio) {
                        const qreal finalTargetWidth = targetWidth > _minimumTargetSize.width() ? targetWidth :
                                                                                                  _minimumTargetSize.width();
                        const qreal targetHeight = finalTargetWidth * getRatio();
                        if (targetHeight > _minimumTargetSize.height())
                            _target->setHeight(targetHeight);
                    } else {
                        const qreal targetHeight = _targetInitialSize.height() + delta.y();
                        if (targetHeight > _minimumTargetSize.height())
                            _target->setHeight(targetHeight);
                    }
                    me->setAccepted(true);
                    accepted = true;
                }
            }
        }
            break;
        case QEvent::MouseButtonPress: {
            QMouseEvent* me = static_cast<QMouseEvent*>( event );
            if ( _target ) {
                _dragInitialPos = me->windowPos();
                _targetInitialSize = { _target->width(), _target->height() };
                emit resizeStart( _target ? QSizeF{ _target->width(), _target->height() } :
                                            QSizeF{} );
                if ( getFlickable() != nullptr )
                    getFlickable()->setProperty( "interactive", QVariant{false} );
                me->setAccepted(true);
                accepted = true;
            }
        }
            break;
        case QEvent::MouseButtonRelease: {
            _dragInitialPos = { 0., 0. };       // Invalid all cached coordinates when button is released
            _targetInitialSize = { 0., 0. };
            emit resizeEnd( _target ? QSizeF{ _target->width(), _target->height() } :
                                      QSizeF{} );
            if ( getFlickable() != nullptr )
                getFlickable()->setProperty( "interactive", QVariant{true} );
        }
            break;
        default:
            accepted = false;
        }
    }
    return accepted ? true : QObject::eventFilter(item, event);
}
//-------------------------------------------------------------------------

} // ::qan
