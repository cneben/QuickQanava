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


/* Edge Style Object Management *///-------------------------------------------
EdgeStyle::EdgeStyle( QString name, QObject* parent ) :
    qan::Style( name, parent )
{
}

void            EdgeStyle::setLineType( LineType lineType ) noexcept
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
//-----------------------------------------------------------------------------

} // ::qan

