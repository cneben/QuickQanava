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
// \file	qanStyle.cpp
// \author	benoit@destrat.io
// \date	2015 06 05
//-----------------------------------------------------------------------------

// Qt headers
#include <QFont>

// QuickQanava headers
#include "./qanStyle.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Style Object Management *///------------------------------------------------
Style::Style( QString name, QObject* parent ) :
    QObject{ parent },
    _name{ name }
{

}
//-----------------------------------------------------------------------------

/* Node Style Object Management *///-------------------------------------------
NodeStyle::NodeStyle( QString name, QObject* parent ) :
    qan::Style( name, parent )
{
}
//-----------------------------------------------------------------------------

/* Node Style Properties *///--------------------------------------------------
void    NodeStyle::setBackOpacity( qreal backOpacity ) noexcept
{
    if ( !qFuzzyCompare( 1. + _backOpacity, 1. + backOpacity ) ) {
        _backOpacity = backOpacity;
        emit backOpacityChanged();
    }
}

void    NodeStyle::setBackColor( const QColor& backColor ) noexcept
{
    if ( _backColor != backColor ) {
        _backColor = backColor;
        emit backColorChanged();
    }
}

void    NodeStyle::setBorderColor( const QColor& borderColor ) noexcept
{
    if ( _borderColor != borderColor ) {
        _borderColor = borderColor;
        emit borderColorChanged();
    }
}

void    NodeStyle::setBorderWidth( qreal borderWidth ) noexcept
{
    if ( !qFuzzyCompare( 1. + _borderWidth, 1. + borderWidth ) ) {
        _borderWidth = borderWidth;
        emit borderWidthChanged();
    }
}

void    NodeStyle::setHasShadow( bool hasShadow ) noexcept
{
    if ( _hasShadow != hasShadow ) {
        _hasShadow = hasShadow;
        emit hasShadowChanged();
    }
}

void    NodeStyle::setShadowColor( QColor shadowColor ) noexcept
{
    if ( _shadowColor != shadowColor ) {
        _shadowColor = shadowColor;
        emit shadowColorChanged();
    }
}

void    NodeStyle::setShadowRadius( qreal shadowRadius ) noexcept
{
    if ( !qFuzzyCompare( 1. + _shadowRadius, 1. + shadowRadius ) ) {
        _shadowRadius = shadowRadius;
        emit shadowRadiusChanged();
    }
}
//-----------------------------------------------------------------------------


/* Edge Style Object Management *///-------------------------------------------
EdgeStyle::EdgeStyle( QString name, QObject* parent ) :
    qan::Style( name, parent )
{
}

void    EdgeStyle::setLineType( LineType lineType ) noexcept
{
    if ( lineType != _lineType ) {
        _lineType = lineType;
        emit lineTypeChanged();
        emit styleModified();
    }
}

void    EdgeStyle::setLineColor( const QColor& lineColor ) noexcept
{
    if ( lineColor != _lineColor ) {
        _lineColor = lineColor;
        emit lineColorChanged();
        emit styleModified();
    }
}

void    EdgeStyle::setLineWidth( qreal lineWidth ) noexcept
{
    if ( !qFuzzyCompare( 1.0 + lineWidth, 1.0 + _lineWidth ) ) {
        _lineWidth = lineWidth;
        emit lineWidthChanged();
        emit styleModified();
    }
}

void    EdgeStyle::setArrowSize( qreal arrowSize ) noexcept
{
    if ( !qFuzzyCompare(1. + arrowSize, 1. + _arrowSize ) ) {
        _arrowSize = arrowSize;
        emit arrowSizeChanged();
        emit styleModified();
    }
}

void    EdgeStyle::setDashed( bool dashed ) noexcept
{
    if ( dashed != _dashed ) {
        _dashed = dashed;
        emit dashedChanged();
    }
}

void    EdgeStyle::setDashPattern( const QVector<qreal>& dashPattern ) noexcept
{
    _dashPattern = dashPattern;
    emit dashPatternChanged();
}

const QVector<qreal>& EdgeStyle::getDashPattern() const noexcept { return _dashPattern; }
//-----------------------------------------------------------------------------

} // ::qan

