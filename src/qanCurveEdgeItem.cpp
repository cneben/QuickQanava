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
    qan::EdgeItem::updateItem();    // Generate _p1 and _p2

    QLineF line{getP1(), getP2()};
    const auto lineLength = line.length();
    if ( lineLength < 0.0001 ) {    // Protected against 0 length edges
        setHidden( false );
        return;
    }

    { // Generate cubic curve control points
        const auto xDelta = _p2.x() - _p1.x();
        const auto xDeltaAbs = std::abs(xDelta);
        const auto yDelta = _p2.y() - _p1.y();
        const auto yDeltaAbs = std::abs(yDelta);

        const auto controlPointDistance = std::max( 0.001, std::min( xDeltaAbs / 2.,      // Control point should be on a line for horizontal/vertical line orientation
                                                                     yDeltaAbs / 2. ) );
        // FIXME: control point distance should also take line length into account...
            // Smaller when distance is low
            // Larger wehn distance is large   (with log(distance)???)

        const QPointF center{ ( _p1.x() + _p2.x() ) / 2.,           // (P1,P2) Line center
                              ( _p1.y() + _p2.y() ) / 2. };
        // Invert if:
            // Top left quarter:     do not invert (xDelta < 0 && yDelta < 0)
            // Top right quarter:    xDelta > 0 && yDelta < 0
            // Bottom rigth quarter: do not invert (xDelta > 0 && yDelta >0)
            // Bottom left quarter:  xDelta < 0 && yDelta > 0
        const auto invert =  ( xDelta > 0 && yDelta < 0 ) ||
                             ( xDelta < 0 && yDelta > 0 ) ? -1. : 1.;
        const QPointF normal = QPointF{ -line.dy(), line.dx() } / ( lineLength * invert );

        _c1 = center + ( normal * controlPointDistance );
        _c2 = center - ( normal * controlPointDistance );
        emit c1Changed();
        emit c2Changed();

        /*qDebug() << "_p1=" << _p1 << "\t_p2=" << _p2;
        qDebug() << "center=" << center << "\tnormal=" << normal;
        qDebug() << "controlPointDistance=" << controlPointDistance;
        qDebug() << "normal * cpd=" << ( normal * controlPointDistance );
        qDebug() << "_c1=" << _c1;
        qDebug() << "_c2=" << _c2;*/
    }

    { // Generate arrow geometry
        const auto arrowSize = getStyle() != nullptr ? getStyle()->getArrowSize() : 4.0;
        const auto arrowLength = arrowSize * 3.;
        const auto base = QPointF{-arrowLength, 0. };

        _dstA1 = QPointF{ base.x(),                 -arrowSize  };
        _dstA2 = QPointF{ base.x() + arrowLength,   base.y()    };
        _dstA3 = QPointF{ base.x(),                 arrowSize   };
        emit dstA1Changed(); emit dstA2Changed(); emit dstA3Changed();
    }

    //_dstAngle = lineAngle(line);
    _dstAngle = cubicCurveAngleAt(0.99, _p1, _p2, _c1, _c2);
    emit dstAngleChanged();
}

qreal   CurveEdgeItem::cubicCurveAngleAt(qreal pos, const QPointF& start, const QPointF& end, const QPointF& c1, const QPointF& c2 ) const noexcept
{
    // Preconditions:
        // pos in [0., 1.0]
    if ( pos < 0. || pos > 1. )
        return -1;

    // This code is (C) 2014 Paul W.
    // Description:
        // http://www.paulwrightapps.com/blog/2014/9/4/finding-the-position-and-angle-of-points-along-a-bezier-curve-on-ios

    // Compute cubic polynomial coefficients
    QPointF coeff3{end.x() - (3. * c2.x()) + (3. * c1.x()) - start.x(),
                   end.y() - (3. * c2.y()) + (3. * c1.y()) - start.y()};

    QPointF coeff2{(3. * c2.x()) - (6. * c1.x()) + (3. * start.x()),
                   (3. * c2.y()) - (6. * c1.y()) + (3. * start.y())};

    QPointF coeff1{(3. * c1.x()) - (3. * start.x()),
                   (3. * c1.y()) - (3. * start.y())};

    const auto pos2 = pos * pos;
    const auto dxdt = (3.0*coeff3.x()*pos2) + (2.0*coeff2.x()*pos) + coeff1.x();
    const auto dydt = (3.0*coeff3.y()*pos2) + (2.0*coeff2.y()*pos) + coeff1.y();

    static constexpr    qreal Pi = 3.141592653;
    static constexpr    qreal TwoPi = 2. * Pi;
    auto angle = std::atan2(dxdt, dydt);

    angle = (2. * Pi) - angle;
    angle = angle * ( 360. / TwoPi );
    angle += 90.;
    return angle;
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
    if ( line.dy( ) < 0. )
        angle = (2.0 * Pi) - angle;
    return angle * ( 360. / TwoPi );
}
//-----------------------------------------------------------------------------

} // ::qan
