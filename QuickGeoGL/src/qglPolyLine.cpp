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
    applyClosing(_closed);
    setDirty(GeometryDirty);
    update();
}

auto    PolyLine::setPoints( const QVector<QPointF>& points ) noexcept -> void
{
    _points = points;
    applyClosing(_closed);
    setDirty(GeometryDirty);
    update();
}

auto    PolyLine::setClosed( bool closed ) noexcept -> void
{
    if ( closed != _closed ) { // Binding loop protection
        _closed = closed;
        applyClosing(closed);   // Eventually regenerate geomtery
        emit closedChanged();
        update();
    }
}

auto    PolyLine::applyClosing(bool closed) -> void
{
    if ( _points.size() <= 0 )
        return; // 1 point at least is necessary to apply closing

    bool isClosed = _points.size() >= 2 &&
                    fuzzyComparePoints( _points[0], _points[_points.size()-1] );
    if ( closed && !isClosed ) {
        int initialSize = _points.size();
        _points.resize(initialSize + 1);        // Note 20161001: there is bug when calling reserve() to avoid last point default construction
        if ( _points.size() > initialSize ) {   // reserve() suceed, last point is unitialized
            _points[ _points.size() - 1 ] = _points[ 0 ];
            setDirty(GeometryDirty);
        }
    }
    else if ( !closed && isClosed ) {
        _points.resize( _points.size()-1 );
        setDirty(GeometryDirty);
    }
}

bool    PolyLine::fuzzyComparePoints( const QPointF& p1, const QPointF& p2 )
{
    return  qFuzzyCompare( 1.0 + p1.x(), 1.0 + p2.x() ) &&
            qFuzzyCompare( 1.0 + p1.y(), 1.0 + p2.y() );
}
//-----------------------------------------------------------------------------

/* Qt QML Scene Graph Interface *///-------------------------------------------
QSGNode* PolyLine::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* )
{
    qgl::SGPolyLineNode* polyLineNode = reinterpret_cast< qgl::SGPolyLineNode* >( oldNode );
    if ( polyLineNode == nullptr ) {
        polyLineNode = new qgl::SGPolyLineNode{ _points };
        setDirty(Dirty);
    }
    if ( polyLineNode != nullptr ) {
        if ( isDirty( PolyLine::GeometryDirty ) )
            polyLineNode->updateGeometry(_points);
        if ( isDirty( PolyLine::WidthDirty ) ) {
            // FIXME: move that to SG node
            auto material = static_cast<qgl::SGPolyLineAAMaterial*>(polyLineNode->material());
            if ( material != nullptr ) {
                material->setWidth( _lineWidth );
                polyLineNode->markDirty(QSGNode::DirtyMaterial);
            }
        }
        if ( isDirty( PolyLine::ColorDirty ) ) {
            // FIXME: move that to SG node
            auto material = static_cast<qgl::SGPolyLineAAMaterial*>(polyLineNode->material());
            if ( material != nullptr ) {
                material->setColor( _color );
                polyLineNode->markDirty(QSGNode::DirtyMaterial);
            }
        }
        cleanDirtyFlags();
    }
    return polyLineNode;
}
//-----------------------------------------------------------------------------

} // ::qgl
