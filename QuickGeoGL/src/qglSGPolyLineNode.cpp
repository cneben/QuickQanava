/*
    This file is part of QuickGeoGL library.

    Copyright (C) 2016 Benoit AUTHEMAN

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
// This file is a part of the QuickGeoGL software. Copyright (C) 2016 Benoit Autheman.
//
// \file	qglSGPolyLineNode.cpp
// \author	benoit@qanava.org
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
    static QSGGeometry::AttributeSet set = { 3, 6 * sizeof( GL_FLOAT ), attr };
    return set;
}

SGPolyLineNode::SGPolyLineNode( const QVector<QPointF>& points ) noexcept :
    QSGGeometryNode{}
{
    setFlag( QSGNode::OwnedByParent, true );
    try {
        _material = std::make_unique<qgl::SGPolyLineAAMaterial>();
        setMaterial( _material.get() );
        setFlag( QSGNode::OwnsMaterial, false );
        markDirty( QSGNode::DirtyMaterial );
    } catch (...) { }
    createGeometry(points);
}

void    SGPolyLineNode::createGeometry( const QVector<QPointF>& points )
{
    if ( _gadget.pointCount != points.size() ||
         _gadget.points == nullptr ) {
        try {
            _geometry = std::make_unique<QSGGeometry>( geometryAttributes( ), points.size() );
            _geometry->setDrawingMode( GL_LINE_STRIP );
            _geometry->setLineWidth( 2);
            setGeometry( _geometry.get() );
            setFlag( QSGNode::OwnsGeometry, false );
            _gadget.pointCount = points.size();
            _gadget.points = reinterpret_cast< PolyLinePoint* >( _geometry->vertexData( ) );
            markDirty( QSGNode::DirtyGeometry );
        }  catch ( const std::bad_alloc& ) {
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
        markDirty( QSGNode::DirtyGeometry );
    }
}

void    SGPolyLineNode::updateClosedGeometry( bool closed )
{
    if ( _gadget.pointCount >= 4 &&     // A polyline with less than 4 vertices can't be closed: minimal closed polyline=[P0 - P1 - P2 - P3=P0]
         _gadget.points != nullptr ) {
        int p{ _gadget.pointCount - 1 };
        // If path is closed, point 0 previous is last point, otherwise point 0 previous is point 0
        _gadget.points[ 0 ].ppX = _gadget.points[ closed ? p-1 : 0 ].x;
        _gadget.points[ 0 ].ppY = _gadget.points[ closed ? p-1 : 0 ].y;
        _gadget.points[ p ].npX = _gadget.points[ closed ? 1 : p ].x;
        _gadget.points[ p ].npY = _gadget.points[ closed ? 1 : p ].y;
        markDirty( QSGNode::DirtyGeometry );
    }
}
//-----------------------------------------------------------------------------

} // ::qgl
