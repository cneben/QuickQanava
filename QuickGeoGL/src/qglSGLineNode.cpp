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
// \file	qglSGLineNode.cpp
// \author	benoit@destrat.io
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
