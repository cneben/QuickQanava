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
// \file	qglSGLineNode.cpp
// \author	benoit@qanava.org
// \date	2015 11 10
//-----------------------------------------------------------------------------

// Qt headers
#include <QSGGeometryNode>
#include <QSGEngine>

// QuickGeoGL headers
#include "./qglSGLineNode.h"
#include "./qglSGLineAAMaterial.h"

namespace qgl { // ::qgl

/* SGLineNode Scene Graph Interface *///---------------------------------------
const QSGGeometry::AttributeSet& SGLineNode::geometryAttributes()
{
    static QSGGeometry::Attribute attr[] = {
        QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true )
    };
    static QSGGeometry::AttributeSet set = { 1, 2 * sizeof( float ), attr };
    return set;
}

SGLineNode::SGLineNode() noexcept :
    QSGGeometryNode{ }
{
    setFlag( QSGNode::OwnedByParent, true );
    try {
        _material = new qgl::SGLineAAMaterial{};
        setFlag( QSGNode::OwnsMaterial );
        setMaterial( _material );
        _geometry = new QSGGeometry{ geometryAttributes( ), 2 };
        _geometry->setDrawingMode( GL_LINE_STRIP );
        setFlag( QSGNode::OwnsGeometry );
        setGeometry( _geometry );
    } catch (...) { }
    if ( geometry() != nullptr ) {
        _gadget = reinterpret_cast< LineGadget* >( geometry()->vertexData( ) );
        _gadget->p1.x = _gadget->p1.y = _gadget->p2.x = _gadget->p2.y = 0.;
    }
    markDirty( QSGNode::DirtyGeometry );
    markDirty( QSGNode::DirtyMaterial );
}

auto    SGLineNode::updateGeometry( const QPointF& p1, const QPointF& p2 ) noexcept -> void
{
    if ( _gadget != nullptr ) {
        _gadget->p1.x = static_cast<float>( p1.x() );
        _gadget->p1.y = static_cast<float>( p1.y() );
        _gadget->p2.x = static_cast<float>( p2.x() );
        _gadget->p2.y = static_cast<float>( p2.y() );
        markDirty( QSGNode::DirtyGeometry );
    }
}
//-----------------------------------------------------------------------------

} // ::qgl
