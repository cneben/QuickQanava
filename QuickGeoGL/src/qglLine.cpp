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

// QuickGeoGL headers
#include "./qglLine.h"
#include "./qglSGLineNode.h"
#include "./qglSGLineAAMaterial.h"

namespace qgl { // ::qgl

/* Line Object Management *///-------------------------------------------------
Line::Line( QQuickItem* parent ) :
    QQuickItem{ parent }
{
    setFlag( ItemHasContents, true );
}
//-----------------------------------------------------------------------------

/* Qt QML Scene Graph Interface *///-------------------------------------------
QSGNode* Line::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* )
{
    qgl::SGLineNode* lineNode = reinterpret_cast< qgl::SGLineNode* >( oldNode );
    if ( lineNode == nullptr ) {
        lineNode = new qgl::SGLineNode{};
        setDirty(Dirty);
        _node = lineNode;
    }

    if ( _node != nullptr ) {
        if ( isDirty( Line::P1Dirty ) ||
             isDirty( Line::P2Dirty ) ) {
            _node->updateGeometry(_p1, _p2);
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
    return lineNode;
}
//-----------------------------------------------------------------------------

} // ::qgl
