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
