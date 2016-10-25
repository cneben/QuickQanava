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
/*const QSGGeometry::AttributeSet& SGConvexPolygonNode::geometryAttributes()
{
    static QSGGeometry::Attribute attr[] = {
        QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true )
    };
    static QSGGeometry::AttributeSet set = { 1, 2 * sizeof( GL_FLOAT ), attr };
    return set;
}*/

SGConvexPolygonNode::SGConvexPolygonNode() noexcept :
    QSGGeometryNode{}
{
    setFlag( QSGNode::OwnedByParent, true );
    try {
        _material = new QSGFlatColorMaterial{};
        setFlag( QSGNode::OwnsMaterial );
        setMaterial( _material );
        markDirty( QSGNode::DirtyMaterial );
        createGeometry( QPolygonF{} );    // Create empty geometry
    } catch (...) {}
}

auto    SGConvexPolygonNode::createGeometry( const QPolygonF& polygon ) noexcept -> void
{
    if ( _gadget.pointCount != polygon.size() ||
         _gadget.points == nullptr ) {
        try {
            _geometry = new QSGGeometry{ QSGGeometry::defaultAttributes_Point2D(), polygon.size() };
            _geometry->setDrawingMode( GL_TRIANGLE_FAN );
            setFlag( QSGNode::OwnsGeometry );
            setGeometry( _geometry );
            _gadget.pointCount = polygon.size();
            _gadget.points = static_cast< QSGGeometry::Point2D* >( geometry()->vertexData( ) );
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
         polygon.size() >= 3 ) {         // Do not loose time feeding geometry for less than three vertices
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
