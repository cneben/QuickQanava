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
// \file	qglPolyLine.cpp
// \author	benoit@destrat.io
// \date	2016 09 30
//-----------------------------------------------------------------------------

// Qt headers
#include <QQuickItem>
#include <QSGGeometryNode>

// QuickGeoGL headers
#include "./qglPolyLine.h"
#include "./qglSGPolyLineNode.h"
#include "./qglSGPolyLineAAMaterial.h"

namespace qgl { // ::qgl

/* PolyLine Object Management *///-------------------------------------------------
PolyLine::PolyLine( QQuickItem* parent ) :
    QQuickItem{ parent }
{
    setFlag( ItemHasContents, true );
}

void    PolyLine::setPoints( const QVariantList& points )
{
    _points.resize(points.size());
    int p{0};
    for ( const auto& point: points )
        _points[p++] = point.toPointF();

    if ( _closed &&
         _points.size() >= 3 ) {
        int initialSize{points.size()};
        _points.resize( initialSize + 1 );
        _points[initialSize] = _points[0];
    }
    setDirty(GeometryDirty);
    setDirty(ClosedDirty);
    update();
}

auto    PolyLine::setPoints( const QVector<QPointF>& points ) noexcept -> void
{
    _points = points;
    applyClosing(_closed);
    setDirty(GeometryDirty);
    setDirty(ClosedDirty);
    update();
}

auto    PolyLine::setClosed( bool closed ) noexcept -> void
{
    if ( closed != _closed ) { // Binding loop protection
        _closed = closed;
        applyClosing(closed);
        emit closedChanged();
        update();
    }
}

auto    PolyLine::applyClosing(bool closed) -> void
{
    if ( closed &&                  // Try to close a potentially unclosed poly line
         _points.size() >= 3 ) {    // An open (unclosed) polyline might have 3 vertices or more
        if ( _points[0] != _points[_points.size()-1] ) {
            int initialSize = _points.size();
            _points.resize(initialSize + 1);        // Note 20161001: there is bug when calling reserve() to avoid last point default construction
            if ( _points.size() > initialSize ) {   // reserve() suceed, last point is unitialized
                _points[ _points.size() - 1 ] = _points[ 0 ];
                setDirty(GeometryDirty);
                setDirty(ClosedDirty);
            }
        }   // Else: closing already applied
    }
    if ( !closed &&                 // Try to unclose a potentially closed poly line
         _points.size() > 3 ) {     // A potentially closed poly line has always at least four vertices
        // If we have a potentially closed polyline, and first vertex equals last
        // vertex (ie the path is closed), unclose it by removing last vertice
        if ( _points[0] == _points[_points.size()-1] ) {
            _points.resize( _points.size()-1 );
            setDirty(GeometryDirty);
            setDirty(ClosedDirty);
        }   // Else: polyline already unclosed
    }
}
//-----------------------------------------------------------------------------

/* Qt QML Scene Graph Interface *///-------------------------------------------
QSGNode* PolyLine::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* )
{
    qgl::SGPolyLineNode* polyLineNode = reinterpret_cast< qgl::SGPolyLineNode* >( oldNode );
    if ( polyLineNode == nullptr ) {
        polyLineNode = new qgl::SGPolyLineNode{_points };
        setDirty(Dirty);
        _node = polyLineNode;
    }
    if ( _node != nullptr ) {
        if ( isDirty( PolyLine::GeometryDirty ) )
            _node->updateGeometry(_points);
        if ( isDirty( PolyLine::ClosedDirty ) )
            _node->updateClosedGeometry(_closed);
        if ( isDirty( PolyLine::WidthDirty ) ) {
            // FIXME: move that to SG node
            auto material = static_cast<qgl::SGPolyLineAAMaterial*>(_node->material());
            if ( material != nullptr ) {
                material->setWidth( _lineWidth );
                _node->markDirty(QSGNode::DirtyMaterial);
            }
        }
        if ( isDirty( PolyLine::ColorDirty ) ) {
            // FIXME: move that to SG node
            auto material = static_cast<qgl::SGPolyLineAAMaterial*>(_node->material());
            if ( material != nullptr ) {
                material->setColor( _color );
                _node->markDirty(QSGNode::DirtyMaterial);
            }
        }
        cleanDirtyFlags();
    }
    return polyLineNode;
}
//-----------------------------------------------------------------------------

} // ::qgl
