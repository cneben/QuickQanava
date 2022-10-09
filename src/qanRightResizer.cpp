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
#include "./qanGroupItem.h"

namespace qan {  // ::qan

/* RightResizer Object Management *///-----------------------------------------
RightResizer::RightResizer(QQuickItem* parent) :
    QQuickItem{parent}
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptHoverEvents(true);
}
//-----------------------------------------------------------------------------

/* Resizer Management *///-----------------------------------------------------
QQuickItem* RightResizer::getHandler() const
{
    return (_handler ? _handler.data() : nullptr);
}

void    RightResizer::setTarget(QQuickItem* target)
{
    if (_target != target) {
        if (_target)
            disconnect(_target.data(),  nullptr,
                       this,            nullptr );  // Disconnect old target width/height monitoring
        _target = target;
        emit targetChanged();
    }

    if (!_handler) {  // Eventually, create the handler component
        QQmlEngine* engine = qmlEngine(this);
        if (engine != nullptr) {
            QQmlComponent defaultHandlerComponent{engine};
            const QString handlerQml{ QStringLiteral("import QtQuick 2.7\n  Rectangle {") +
                        QStringLiteral("width: 5;") +
                        QStringLiteral("height: 10; border.width: 1; border.color: 'red';") +
                        QStringLiteral("color:Qt.rgba(0,0,0,0); }") };
            defaultHandlerComponent.setData(handlerQml.toUtf8(), QUrl{});
            if (defaultHandlerComponent.isReady()) {
                _handler = qobject_cast<QQuickItem*>(defaultHandlerComponent.create());
                if (_handler) {
                    engine->setObjectOwnership(_handler.data(), QQmlEngine::CppOwnership);
                    _handler->setParentItem(this);
                    // FIXME #169 NO !
                    //_handler->installEventFilter(this);
                } else {
                    qWarning() << "qan::RightResizer::setTarget(): Error: Can't create resize handler QML component:";
                    qWarning() << "QML Component status=" << defaultHandlerComponent.status();
                }
            }
        }
    }
    if (_target) {
        if (!_minimumTargetSize.isEmpty()) { // Check that target size is not bellow resizer target minimum size
            if (_target->width() < _minimumTargetSize.width())
                _target->setWidth(_minimumTargetSize.width());
            if (_target->height() < _minimumTargetSize.height())
                _target->setHeight(_minimumTargetSize.height());
        }
        connect(_target,    &QQuickItem::xChanged,
                this,       &RightResizer::onTargetXChanged);
        connect(_target,    &QQuickItem::yChanged,
                this,       &RightResizer::onTargetYChanged);

        connect(_target,    &QQuickItem::widthChanged,
                this,       &RightResizer::onTargetWidthChanged);
        connect(_target,    &QQuickItem::heightChanged,
                this,       &RightResizer::onTargetHeightChanged);
        onTargetXChanged();
        onTargetYChanged();
        onTargetWidthChanged();
        onTargetHeightChanged();
        if (_handler)
            _handler->setVisible(true);
    }
    if (!_target && _handler)
        _handler->setVisible(false);
}

void    RightResizer::onTargetXChanged()
{
    if (_target)
        setX(_target->x() + _target->width() - 2.5);  // FIXME #169 grip width...
    // FIXME #169 map to global x...
}

void    RightResizer::onTargetYChanged()
{
    if (_target && _handler)
        setY(_target->y());
    // FIXME #169 map to global y...
}

void    RightResizer::onTargetWidthChanged()
{
    if (_target) {
        setX(_target->x() + _target->width() - 2.5);  // FIXME #169 grip width...
        setWidth(5);
    }
}

void    RightResizer::onTargetHeightChanged()
{
    if (_target && _handler) {
        _handler->setHeight(_target->height());
        setHeight(_target->height());
    }
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
void    RightResizer::hoverEnterEvent(QHoverEvent *event)
{
    qWarning() << "qan::RightResizer::hoverEnter()";
    setCursor(Qt::SplitHCursor);
    event->setAccepted(true);
}
void    RightResizer::hoverLeaveEvent(QHoverEvent *event)
{
    qWarning() << "qan::RightResizer::hoverLeave()";
    setCursor(Qt::ArrowCursor);
    event->setAccepted(true);
}
void    RightResizer::mouseMoveEvent(QMouseEvent* event)
{
    qWarning() << "qan::RightResizer::mouseMove()";
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const auto mePos = event->windowPos();
#else
    const auto mePos = event->scenePosition();
#endif
    if (event->buttons() |  Qt::LeftButton &&
            !_dragInitialPos.isNull() &&
            !_targetInitialSize.isEmpty()) {
        const auto groupTarget = qobject_cast<qan::GroupItem*>(_target.data());
        const auto target = groupTarget != nullptr ? groupTarget->getContainer() :
                                                     _target.data();
        QPointF startLocalPos;
        QPointF curLocalPos;
        if (parentItem() != nullptr) {
            startLocalPos = parentItem()->mapFromScene(_dragInitialPos);
            curLocalPos = parentItem()->mapFromScene(mePos);
        } else
            qWarning() << "qan::RightResizer::mouseMoveEvent(): Internal error, no parent item.";
        const QPointF delta{curLocalPos - startLocalPos};
        if (target) {
            // Do not resize below minimumSize
            const qreal targetWidth = _targetInitialSize.width() + delta.x();
            if (targetWidth > _minimumTargetSize.width())
                target->setWidth(targetWidth);
            if (_preserveRatio) {
                const qreal finalTargetWidth = targetWidth > _minimumTargetSize.width() ? targetWidth :
                                                                                          _minimumTargetSize.width();
                const qreal targetHeight = finalTargetWidth * getRatio();
                if (targetHeight > _minimumTargetSize.height())
                    target->setHeight(targetHeight);
            }
            event->setAccepted(true);
        }
    }
}
void    RightResizer::mousePressEvent(QMouseEvent* event)
{
    qWarning() << "qan::RightResizer::mousePress()";
    const auto groupTarget = qobject_cast<qan::GroupItem*>(_target.data());
    const auto target = groupTarget != nullptr ? groupTarget->getContainer() :
                                                 _target.data();
    if (target) {
        _dragInitialPos = event->windowPos();
        _targetInitialSize = {target->width(), target->height()};
        emit resizeStart(_target ? QSizeF{target->width(), target->height()} :
                                   QSizeF{});
        if (getFlickable() != nullptr)
            getFlickable()->setProperty("interactive", QVariant{false});
        event->setAccepted(true);
    }
}
void    RightResizer::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    _dragInitialPos = {0., 0.};       // Invalid all cached coordinates when button is released
    _targetInitialSize = {0., 0.};
    emit resizeEnd(_target ? QSizeF{_target->width(), _target->height()} :
                             QSizeF{});
    if (getFlickable() != nullptr)
        getFlickable()->setProperty("interactive", QVariant{true});
}
//-------------------------------------------------------------------------

} // ::qan
