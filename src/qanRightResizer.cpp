/*
 Copyright (c) 2008-2022, Benoit AUTHEMAN All rights reserved.

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
// \file	qanRightResizer.cpp
// \author	benoit@destrat.io
// \date	2022 10 09
//-----------------------------------------------------------------------------

// Qt headers
#include <QCursor>
#include <QMouseEvent>

// QuickQanava headers
#include "./qanRightResizer.h"

namespace qan {  // ::qan

/* RightResizer Object Management *///-----------------------------------------
RightResizer::RightResizer(QQuickItem* parent) :
    QQuickItem{parent}
{
}
//-----------------------------------------------------------------------------

/* Resizer Management *///-----------------------------------------------------
QQuickItem* RightResizer::getHandler() const
{
    return (_handler ? _handler.data() : nullptr);
}

void    RightResizer::setTarget(QQuickItem* target)
{
    if (target == nullptr) {  // Set a null target = disable the control
        if (_target)
            disconnect(_target.data(),  nullptr,
                       this,            nullptr );  // Disconnect old target width/height monitoring
        _target = nullptr;
        return;
    }

    if (!_handler) {  // Eventually, create the handler component
        QQmlEngine* engine = qmlEngine(this);
        if (engine != nullptr) {
            QQmlComponent defaultHandlerComponent{engine};
            const QString handlerQml{ QStringLiteral("import QtQuick 2.7\n  Rectangle {") +
                        QStringLiteral("width: 5;") +
                        QStringLiteral("height: 10;") +
                        QStringLiteral("color:Qt.rgba(0,0,0,0); }") };
            defaultHandlerComponent.setData(handlerQml.toUtf8(), QUrl{});
            if (defaultHandlerComponent.isReady()) {
                _handler = qobject_cast<QQuickItem*>(defaultHandlerComponent.create());
                if (_handler) {
                    engine->setObjectOwnership(_handler.data(), QQmlEngine::CppOwnership);
                    _handler->setParentItem(this);
                    _handler->installEventFilter(this);
                } else {
                    qWarning() << "qan::RightResizer::setTarget(): Error: Can't create resize handler QML component:";
                    qWarning() << "QML Component status=" << defaultHandlerComponent.status();
                }
            }
        }
    }

    _target = target;
    if (_target)
        configureTarget(*_target);
    emit targetChanged();
}

void    RightResizer::configureTarget(QQuickItem& target)
{
    if (!_minimumTargetSize.isEmpty()) { // Check that target size is not bellow resizer target minimum size
        if (target.width() < _minimumTargetSize.width())
            target.setWidth(_minimumTargetSize.width());
        if (target.height() < _minimumTargetSize.height())
            target.setHeight(_minimumTargetSize.height());
    }

    connect(&target,   &QQuickItem::widthChanged,
            this,      &RightResizer::onTargetWidthChanged);
    connect(&target,   &QQuickItem::heightChanged,
            this,      &RightResizer::onTargetHeightChanged);

    onTargetWidthChanged();
    onTargetHeightChanged();
}

void    RightResizer::onTargetWidthChanged()
{
    if (_target && _handler) {
        const qreal handlerWidth2 = 5 / 2.;
        _handler->setX(_target->width() - handlerWidth2);
    }
}

void    RightResizer::onTargetHeightChanged()
{
    if (_target && _handler)
        _handler->setHeight(_target->height());
}

void    RightResizer::setMinimumTargetSize(QSizeF minimumTargetSize)
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

void    RightResizer::setPreserveRatio(bool preserveRatio) noexcept
{
    if (preserveRatio != _preserveRatio) {
        _preserveRatio = preserveRatio;
        emit preserveRatioChanged();
    }
}

void    RightResizer::setRatio(qreal ratio) noexcept
{
    if (!qFuzzyCompare(2.0 + _ratio, 2.0 + ratio)) { // Using 2.0 because -1.0 is a valid input (disable ratio...)
        _ratio = ratio;
        emit ratioChanged();
    }
}
//-----------------------------------------------------------------------------

/* Resizer Management *///-----------------------------------------------------
bool   RightResizer::eventFilter(QObject* item, QEvent* event)
{
    bool accepted = false;
    if (_handler &&
        item == _handler.data()) {
        switch (event->type()) {
        case QEvent::HoverEnter:
        {
            _handler->setCursor(Qt::SplitHCursor);
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            me->setAccepted(true);
            accepted = true;
        }
            break;
        case QEvent::HoverLeave:
        {
            _handler->setCursor(Qt::ArrowCursor);
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            me->setAccepted(true);
            accepted = true;
            break;
        }
        case QEvent::MouseMove: {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            const auto mePos = me->windowPos();
#else
            const auto mePos = me->scenePosition();
#endif
            if (me->buttons() |  Qt::LeftButton &&
                !_dragInitialPos.isNull() &&
                !_targetInitialSize.isEmpty()) {
                // Inspired by void QQuickMouseArea::mouseMoveEvent(QMouseEvent *event)
                // https://code.woboq.org/qt5/qtdeclarative/src/quick/items/qquickmousearea.cpp.html#47curLocalPos
                // Coordinate mapping in qt quick is even more a nightmare than with graphics view...
                // BTW, this code is probably buggy for deep quick item hierarchy.
                QPointF startLocalPos;
                QPointF curLocalPos;
                if (parentItem() != nullptr) {
                    startLocalPos = parentItem()->mapFromScene(_dragInitialPos);
                    curLocalPos = parentItem()->mapFromScene(mePos);
                } else {
                    startLocalPos = _dragInitialPos;
                    curLocalPos = mePos;
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
                    }
                    me->setAccepted(true);
                    accepted = true;
                }
            }
        }
            break;
        case QEvent::MouseButtonPress: {
            QMouseEvent* me = static_cast<QMouseEvent*>( event );
            if (_target) {
                _dragInitialPos = me->windowPos();
                _targetInitialSize = {_target->width(), _target->height()};
                emit resizeStart(_target ? QSizeF{_target->width(), _target->height()} :
                                           QSizeF{});
                if (getFlickable() != nullptr)
                    getFlickable()->setProperty("interactive", QVariant{false});
                me->setAccepted(true);
                accepted = true;
            }
        }
            break;
        case QEvent::MouseButtonRelease: {
            _dragInitialPos = {0., 0.};       // Invalid all cached coordinates when button is released
            _targetInitialSize = {0., 0.};
            emit resizeEnd(_target ? QSizeF{_target->width(), _target->height()} :
                                     QSizeF{});
            if (getFlickable() != nullptr)
                getFlickable()->setProperty("interactive", QVariant{true});
        }
            break;
        default:
            accepted = false;
        }
    }
    return accepted ? true :
                      QObject::eventFilter(item, event);
}
//-------------------------------------------------------------------------

} // ::qan
