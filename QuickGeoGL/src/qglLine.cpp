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
