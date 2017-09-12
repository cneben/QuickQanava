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
// \file	qglLine.cpp
// \author	benoit@destrat.io
// \date	2016 09 29
//-----------------------------------------------------------------------------

// Qt headers
#include <QQuickItem>
#include <QSGGeometryNode>
#include <QLineF>

// QuickGeoGL headers
#include "./qglLine.h"
#include "./qglSGLineNode.h"
#include "./qglSGLineAAMaterial.h"

namespace qgl { // ::qgl

/* Line Object Management *///-------------------------------------------------
Line::Line( QQuickItem* parent ) :
    QQuickItem{ parent }
{
    QQuickItem::setFlag( ItemHasContents, true );
}

auto    Line::setP1( QPointF p1 ) noexcept -> void
{
    _line.setP1( p1 );
    setLineFlag( P1Valid );
    setVisible( getLineFlag( P2Valid ) &&
                _line.length() > MinLength );   // Visible only if p2 has already been set and line length is not null
    setDirty( P1Dirty );
    emit p1Changed();
    update();
}

auto    Line::setP2( QPointF p2 ) noexcept -> void
{
    _line.setP2( p2 );
    setLineFlag( P2Valid );
    setVisible( getLineFlag( P1Valid ) &&
                _line.length() > MinLength );   // Visible only if p1 has already been set and line length is not null
    setDirty( P2Dirty );
    emit p2Changed();
    update();
}
//-----------------------------------------------------------------------------

/* Qt QML Scene Graph Interface *///-------------------------------------------
QSGNode* Line::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* )
{
    //qDebug() << "qgl::Line::updatePaintNode(): this=" << this << "\toldNode=" << oldNode << "\twidth=" << width() << "\theight=" << height();
    qgl::SGLineNode* lineNode = reinterpret_cast< qgl::SGLineNode* >( oldNode );
    if ( lineNode == nullptr ) {
        lineNode = new qgl::SGLineNode{};
        setDirty(Dirty);
        _node = lineNode;
    }
    if ( _node != nullptr ) {
        if ( isDirty( Line::P1Dirty ) ||
             isDirty( Line::P2Dirty ) ) {
            if ( getLineFlag(P1Valid) &&
                 getLineFlag(P2Valid) &&
                 _line.length() > MinLength ) {
                _node->updateGeometry( _line.p1(), _line.p2() );
            }
        }
        if ( isDirty( Line::WidthDirty ) ) {
            // FIXME: move that to SG node
            auto material = static_cast<qgl::SGLineAAMaterial*>(_node->material());
            if ( material != nullptr ) {
                material->setWidth( _lineWidth );
                _node->markDirty(QSGNode::DirtyMaterial);
            }
        }
        if ( isDirty( Line::ColorDirty ) ) {
            // FIXME: move that to SG node
            auto material = static_cast<qgl::SGLineAAMaterial*>(_node->material());
            if ( material != nullptr ) {
                material->setColor( _color );
                _node->markDirty(QSGNode::DirtyMaterial);
            }
        }
        cleanDirtyFlags();
    }
    return _node;
}
//-----------------------------------------------------------------------------

} // ::qgl
