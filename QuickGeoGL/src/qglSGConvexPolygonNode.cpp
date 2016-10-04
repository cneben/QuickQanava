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
// \file	qglSGConvexPolygonNode.cpp
// \author	benoit@destrat.io
// \date	2016 09 24
//-----------------------------------------------------------------------------

// Qt headers
#include <QSGSimpleMaterialShader>
#include <QSGGeometryNode>
#include <QPolygonF>
#include <QOpenGLFunctions>
#include <QSGFlatColorMaterial>

// QuickGeoGL headers
#include "./qglSGConvexPolygonNode.h"

namespace qgl { // ::qgl

/* SGLineNodeGS Scene Graph Interface *///---------------------------------------
const QSGGeometry::AttributeSet& SGConvexPolygonNode::geometryAttributes()
{
    static QSGGeometry::Attribute attr[] = {
        QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true )/*,
        QSGGeometry::Attribute::create(1, 2, GL_FLOAT ),
        QSGGeometry::Attribute::create(2, 2, GL_FLOAT )*/
    };
    //static QSGGeometry::AttributeSet set = { 3, 6 * sizeof( GL_FLOAT ), attr };
    static QSGGeometry::AttributeSet set = { 3, 2 * sizeof( GL_FLOAT ), attr };
    return set;
}

SGConvexPolygonNode::SGConvexPolygonNode() noexcept :
    QSGGeometryNode{}
{
    setFlag( QSGNode::OwnedByParent, true );
    try {
        _material = std::make_unique<QSGFlatColorMaterial>();
        setFlag( QSGNode::OwnsMaterial, false );
        setMaterial( _material.get() );
        markDirty( QSGNode::DirtyMaterial );
        createGeometry(QPolygonF{});    // Create empty geometry
    } catch (...) {}
}

auto    SGConvexPolygonNode::createGeometry( const QPolygonF& polygon ) noexcept -> void
{
    if ( _gadget.pointCount != polygon.size() ||
         _gadget.points == nullptr ) {
        try {
            _geometry = std::make_unique<QSGGeometry>( geometryAttributes(), polygon.size() );
            // FIXME GL_TRIANGLE_STRIP
            _geometry->setDrawingMode( GL_TRIANGLE_FAN );
            setGeometry( _geometry.get() );
            setFlag( QSGNode::OwnsGeometry, false );    // Geometry memory management is done with unique_ptr
            _gadget.pointCount = polygon.size();
            _gadget.points = static_cast< PolygonPoint* >( _geometry->vertexData( ) );
            markDirty( QSGNode::DirtyGeometry );
        } catch ( const std::bad_alloc& ) {
            _gadget.pointCount = 0;
            _gadget.points = nullptr;
        }
    }
}

auto    SGConvexPolygonNode::updateGeometry( const QPolygonF& polygon ) noexcept -> void
{
    createGeometry(polygon);   // Eventually, recreate geometry
    if ( _gadget.pointCount == polygon.size() &&
         _gadget.points != nullptr &&
         polygon.size() >= 3 ) {         // Do not loose time feeding geometry for less than thress vertices
        int i = 0;
        for ( const auto& p : polygon ) {
            _gadget.points[ i ].x = p.x();
            _gadget.points[ i ].y = p.y();
            ++i;
        }
        markDirty( QSGNode::DirtyGeometry );
    }
}
//-----------------------------------------------------------------------------

} // ::qgl
