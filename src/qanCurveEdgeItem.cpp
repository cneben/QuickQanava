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
// \file	qanCurveEdgeItem.cpp
// \author	benoit@destrat.io
// \date	2017 09 05
//-----------------------------------------------------------------------------

// Std headers
#include <cmath>    // std::abs (c++11)
#include <cstdlib>  // std::abs (c++17)

// Qt headers
#include <QBrush>
#include <QPainter>

// QuickQanava headers
#include "./qanCurveEdgeItem.h"
#include "./qanNodeItem.h"          // Resolve forward declaration
#include "./qanGroupItem.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Edge Object Management *///-------------------------------------------------
CurveEdgeItem::CurveEdgeItem(QQuickItem* parent) :
    qan::EdgeItem(parent)
{
    setObjectName( QStringLiteral("qan::CurveEdgeItem") );
}

CurveEdgeItem::~CurveEdgeItem() { /* Nil */ }
//-----------------------------------------------------------------------------

/* Curve Control Points Management *///----------------------------------------
void    CurveEdgeItem::updateItem() noexcept
{
    qan::EdgeItem::updateItem();

    { // Generate cubic curve control points
        const auto xDistance = _p2.x() - _p1.x();
        const auto defaultOffset = 200.0;
        const auto minimum = std::min(defaultOffset, std::abs(xDistance));
        // FIXME: floating point =, horrible....
        const auto ratio = std::abs(xDistance) <= 0 ? 1.0 : 0.5;

        _c1.rx() = _p1.x() + (xDistance * ratio);
        _c1.ry() = _p1.y();
        _c2.rx() = _p2.x() - (xDistance * ratio);
        _c2.ry() = _p2.y();
    }

    // Generate arrow angle
    QLineF line{getP1(), getP2()};

    { // Generate arrow geometry
        const auto arrowSize = getStyle() != nullptr ? getStyle()->getArrowSize() : 4.0;
        const auto arrowLength = arrowSize * 2.;
        const auto base = QPointF{-arrowLength, 0. };

        _dstA1 = QPointF{ base.x(),                 -arrowSize  };
        _dstA2 = QPointF{ base.x() + arrowLength,   base.y()    };
        _dstA3 = QPointF{ base.x(),                 arrowSize   };
        emit dstA1Changed(); emit dstA2Changed(); emit dstA3Changed();
    }

    _dstAngle = lineAngle(line); emit dstAngleChanged();

    emit c1Changed();
    emit c2Changed();
}

qreal   CurveEdgeItem::cubicCurveAngleAt(qreal pos, const QPointF& start, const QPointF& end, const QPointF& c1, const QPointF& c2 ) const noexcept
{
    // http://www.paulwrightapps.com/blog/2014/9/4/finding-the-position-and-angle-of-points-along-a-bezier-curve-on-ios
    // https://www.codeproject.com/Articles/199645/Bezier-curve-angle-calculation-in-Silverlight

    return -1;
}

qreal   CurveEdgeItem::lineAngle(const QLineF& line) const noexcept
{
    static constexpr    qreal Pi = 3.141592653;
    static constexpr    qreal TwoPi = 2. * Pi;
    static constexpr    qreal MinLength = 0.00001;

    const qreal lineLength = line.length();
    if ( lineLength < MinLength )
        return -1.;
    double angle = std::acos( line.dx( ) / lineLength );
    if ( line.dy( ) <= 0 )
        angle = 2.0 * Pi - angle;
    return angle * ( 360. / TwoPi );
}
//-----------------------------------------------------------------------------

} // ::qan
