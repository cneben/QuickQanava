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
// This file is a part of the QuickGeoGL software. Copyright (C) 2016 Benoit Autheman.
//
// \file	qglSGPolyLineNode.cpp
// \author	benoit@destrat.io
// \date	2015 11 11
//-----------------------------------------------------------------------------

// Qt headers
#include <QSGSimpleMaterialShader>
#include <QSGGeometryNode>
#include <QOpenGLFunctions>
#include <QSGEngine>

// QuickGeoGL headers
#include "./qglSGPolyLineNode.h"
#include "./qglSGPolyLineAAMaterial.h"

namespace qgl { // ::qgl

/* SGLineNodeGS Scene Graph Interface *///---------------------------------------
const QSGGeometry::AttributeSet& SGPolyLineNode::geometryAttributes()
{
    static QSGGeometry::Attribute attr[] = {
        QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true ),
        QSGGeometry::Attribute::create(1, 2, GL_FLOAT ),
        QSGGeometry::Attribute::create(2, 2, GL_FLOAT )
    };
    static QSGGeometry::AttributeSet set = { 3, 6 * sizeof( float ), attr };
    return set;
}

SGPolyLineNode::SGPolyLineNode( const QVector<QPointF>& points ) noexcept :
    QSGGeometryNode{}
{
    setFlag( QSGNode::OwnedByParent, true );
    try {
        _material = new qgl::SGPolyLineAAMaterial{};
        setFlag( QSGNode::OwnsMaterial );
        setMaterial( _material );
        markDirty( QSGNode::DirtyMaterial );
    } catch (...) { }
    createGeometry(points);
}

void    SGPolyLineNode::createGeometry( const QVector<QPointF>& points )
{
    if ( _gadget.pointCount != points.size() ||
         _gadget.points == nullptr ) {
        try {
            _geometry = new QSGGeometry{ geometryAttributes( ), points.size() };
            _geometry->setDrawingMode( GL_LINE_STRIP );
            setFlag( QSGNode::OwnsGeometry );
            setGeometry( _geometry );
            _gadget.pointCount = points.size();
            _gadget.points = reinterpret_cast< PolyLinePoint* >( _geometry->vertexData( ) );
            markDirty( QSGNode::DirtyGeometry );
        }  catch ( const std::bad_alloc& ) {
            _geometry = nullptr;
            _gadget.pointCount = 0;
            _gadget.points = nullptr;
        }
    }
}

void    SGPolyLineNode::updateGeometry( const QVector<QPointF>& points )
{
    createGeometry(points);   // Eventually, create geometry or adapt to current point count
    if ( _gadget.pointCount >= 3 &&             // Can't draw polyline with less than three vertices...
         _gadget.pointCount == points.size() &&
         _gadget.points != nullptr ) {
        _gadget.points[ 0 ].x = static_cast<float>( points[0].x() );
        _gadget.points[ 0 ].y = static_cast<float>( points[0].y() );
        _gadget.points[ 1 ].x = static_cast<float>( points[1].x() );
        _gadget.points[ 1 ].y = static_cast<float>( points[1].y() );
        _gadget.points[ 0 ].ppX = _gadget.points[ 0 ].x;
        _gadget.points[ 0 ].ppY = _gadget.points[ 0 ].y;
        _gadget.points[ 0 ].npX = _gadget.points[ 1 ].x;
        _gadget.points[ 0 ].npY = _gadget.points[ 1 ].y;
        int p = 1;
        for ( ; p < _gadget.pointCount - 1; p++ ) {
            _gadget.points[ p ].x = static_cast<float>( points[ p ].x() );
            _gadget.points[ p ].y = static_cast<float>( points[ p ].y() );
            _gadget.points[ p + 1 ].x = static_cast<float>( points[ p + 1 ].x() );
            _gadget.points[ p + 1 ].y = static_cast<float>( points[ p + 1 ].y() );

            _gadget.points[ p ].ppX = _gadget.points[ p - 1 ].x;
            _gadget.points[ p ].ppY = _gadget.points[ p - 1 ].y;
            _gadget.points[ p ].npX = _gadget.points[ p + 1 ].x;
            _gadget.points[ p ].npY = _gadget.points[ p + 1 ].y;
        }
        _gadget.points[ p ].x = static_cast<float>( points[ p ].x() );
        _gadget.points[ p ].y = static_cast<float>( points[ p ].y() );
        _gadget.points[ p ].ppX = _gadget.points[ p - 1 ].x;
        _gadget.points[ p ].ppY = _gadget.points[ p - 1 ].y;
        _gadget.points[ p ].npX = _gadget.points[ p ].x;
        _gadget.points[ p ].npY = _gadget.points[ p ].y;

        if ( _gadget.pointCount >= 3 &&
             _gadget.points != nullptr ) {  // Test if closing is applyied
            const auto& first = _gadget.points[0];
            const auto& last = _gadget.points[_gadget.pointCount-1];
            if ( qFuzzyCompare( 1.f + first.x, 1.f + last.x ) &&
                 qFuzzyCompare( 1.f + first.y, 1.f + last.y ) ) {
                int p{ _gadget.pointCount - 1 };
                // If path is closed, point 0 previous is last point, otherwise point 0 previous is point 0
                _gadget.points[ 0 ].ppX = _gadget.points[ p-1 ].x;
                _gadget.points[ 0 ].ppY = _gadget.points[ p-1 ].y;
                _gadget.points[ p ].npX = _gadget.points[ 1 ].x;
                _gadget.points[ p ].npY = _gadget.points[ 1 ].y;
            }
        }

        markDirty( QSGNode::DirtyGeometry );
    }
}
//-----------------------------------------------------------------------------

} // ::qgl
